#include "voicerecognition.h"
#include <QDebug>
#include <QMediaDevices>
#include <QAudioDevice>
#include <QFile>
#include <QProcess>
#include <QProcessEnvironment>
#include <QCoreApplication>
#include <QDir>
#include <QTextStream>
#include <QMessageBox>

VoiceRecognition::VoiceRecognition(QObject *parent)
    : QObject(parent)
    , m_audioSource(nullptr)
    , m_audioDevice(nullptr)
    , m_isRecording(false)
    , m_recordingTimer(new QTimer(this))
{
    connect(m_recordingTimer, &QTimer::timeout, this, &VoiceRecognition::stopRecording);
    m_recordingTimer->setSingleShot(true);

    // Print available audio devices for debugging
    listAudioDevices();

    setupAudio();
}

VoiceRecognition::~VoiceRecognition()
{
    if (m_isRecording) {
        stopRecording();
    }

    if (m_audioSource) {
        delete m_audioSource;
    }
}

void VoiceRecognition::listAudioDevices()
{
    qDebug() << "=== PÉRIPHÉRIQUES AUDIO DISPONIBLES ===";

    QList<QAudioDevice> devices = QMediaDevices::audioInputs();

    if (devices.isEmpty()) {
        qDebug() << "❌ AUCUN MICROPHONE DÉTECTÉ!";
        return;
    }

    for (int i = 0; i < devices.size(); i++) {
        const QAudioDevice &device = devices.at(i);
        qDebug() << "Microphone" << i << ":";
        qDebug() << "  - Nom:" << device.description();
        qDebug() << "  - ID:" << device.id();
        qDebug() << "  - Défaut:" << (device == QMediaDevices::defaultAudioInput() ? "OUI" : "Non");
    }

    QAudioDevice defaultDevice = QMediaDevices::defaultAudioInput();
    qDebug() << "\n🎤 Microphone par défaut:" << defaultDevice.description();
    qDebug() << "======================================\n";
}

void VoiceRecognition::setupAudio()
{
    QAudioFormat format;
    format.setSampleRate(16000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    QAudioDevice info = QMediaDevices::defaultAudioInput();

    if (info.isNull()) {
        qWarning() << "❌ Aucun périphérique audio d'entrée trouvé!";
        QMessageBox::critical(nullptr, "Erreur Microphone",
                              "Aucun microphone détecté!\n\n"
                              "Vérifiez que votre microphone est:\n"
                              "1. Branché correctement\n"
                              "2. Activé dans les paramètres Windows\n"
                              "3. Défini comme périphérique par défaut");
        return;
    }

    qDebug() << "✅ Microphone sélectionné:" << info.description();

    if (!info.isFormatSupported(format)) {
        qWarning() << "⚠️ Format 16000Hz non supporté, utilisation du format préféré";
        format = info.preferredFormat();
        qDebug() << "   Format utilisé:" << format.sampleRate() << "Hz,"
                 << format.channelCount() << "canaux,"
                 << format.sampleFormat();
    }

    m_audioSource = new QAudioSource(info, format, this);
    m_sampleRate = format.sampleRate();

    // Set a larger buffer and lower volume (more sensitive)
    m_audioSource->setBufferSize(65536);
    m_audioSource->setVolume(1.0);  // Maximum volume

    connect(m_audioSource, &QAudioSource::stateChanged,
            this, &VoiceRecognition::handleStateChanged);
}

void VoiceRecognition::startRecording()
{
    if (m_isRecording) {
        qDebug() << "Enregistrement déjà en cours";
        return;
    }

    if (!m_audioSource) {
        emit error("Aucun périphérique audio disponible!\n\nVérifiez votre microphone dans les paramètres Windows.");
        return;
    }

    m_audioData.clear();
    m_audioBuffer.close();
    m_audioBuffer.setBuffer(&m_audioData);
    m_audioBuffer.open(QIODevice::WriteOnly);

    qDebug() << "🎤 Démarrage de l'enregistrement...";
    qDebug() << "   Buffer size:" << m_audioSource->bufferSize();
    qDebug() << "   Volume:" << m_audioSource->volume();
    qDebug() << "   Format:" << m_audioSource->format().sampleRate() << "Hz";

    m_audioDevice = m_audioSource->start();

    if (m_audioDevice) {
        connect(m_audioDevice, &QIODevice::readyRead, this, [this]() {
            QByteArray data = m_audioDevice->readAll();

            if (!data.isEmpty()) {
                m_audioData.append(data);

                // Calculate audio level for debugging
                const qint16* samples = reinterpret_cast<const qint16*>(data.constData());
                int numSamples = data.size() / 2;
                qint16 maxAmplitude = 0;

                for (int i = 0; i < numSamples; i++) {
                    qint16 amp = qAbs(samples[i]);
                    if (amp > maxAmplitude) maxAmplitude = amp;
                }

                qDebug() << "📊 Audio:" << data.size() << "octets, Amplitude max:" << maxAmplitude
                         << "(Total:" << m_audioData.size() << "octets)";
            }
        });

        m_isRecording = true;
        m_recordingTimer->start(MAX_RECORDING_TIME);
        emit recordingStarted();
        qDebug() << "✅ Enregistrement démarré - PARLEZ MAINTENANT!";
    } else {
        emit error("Impossible de démarrer l'enregistrement audio");
    }
}

void VoiceRecognition::stopRecording()
{
    if (!m_isRecording) {
        return;
    }

    m_recordingTimer->stop();
    m_audioSource->stop();
    m_audioBuffer.close();
    m_isRecording = false;

    emit recordingStopped();
    qDebug() << "⏹️ Enregistrement arrêté. Taille totale:" << m_audioData.size() << "octets";

    if (m_audioData.size() < 1000) {
        qDebug() << "❌ Audio trop court:" << m_audioData.size() << "octets";
        emit error("Audio trop court!\n\nEnregistrement: " + QString::number(m_audioData.size()) + " octets\n\nVérifiez:\n- Microphone activé dans Windows\n- Volume du micro augmenté\n- Permissions de l'application");
        return;
    }

    // Detailed audio analysis
    const qint16* samples = reinterpret_cast<const qint16*>(m_audioData.constData());
    int numSamples = m_audioData.size() / 2;

    qint16 maxAmplitude = 0;
    qint64 totalAmplitude = 0;
    int nonZeroSamples = 0;

    for (int i = 0; i < numSamples; i++) {
        qint16 amp = qAbs(samples[i]);
        if (amp > maxAmplitude) maxAmplitude = amp;
        totalAmplitude += amp;
        if (amp > 10) nonZeroSamples++;
    }

    double avgAmplitude = numSamples > 0 ? (double)totalAmplitude / numSamples : 0;

    qDebug() << "📈 ANALYSE AUDIO:";
    qDebug() << "   Échantillons:" << numSamples;
    qDebug() << "   Amplitude max:" << maxAmplitude << "/ 32767";
    qDebug() << "   Amplitude moyenne:" << avgAmplitude;
    qDebug() << "   Échantillons non-silencieux:" << nonZeroSamples;

    // Much more lenient threshold
    if (maxAmplitude < 50) {  // Changed from 100 to 50
        qDebug() << "❌ AMPLITUDE TROP FAIBLE!";
        emit error(QString("Aucun son détecté!\n\nAmplitude max: %1 / 32767\n\n"
                           "Solutions:\n"
                           "1. Augmentez le volume du microphone dans Windows\n"
                           "2. Parlez TRÈS près du micro\n"
                           "3. Vérifiez les permissions de l'application\n"
                           "4. Testez avec un autre microphone")
                       .arg(maxAmplitude));
        return;
    }

    if (nonZeroSamples < (numSamples / 10)) {
        qDebug() << "⚠️ Trop de silence détecté";
        emit error(QString("Audio majoritairement silencieux!\n\n"
                           "Échantillons valides: %1 / %2\n\n"
                           "Parlez plus fort et plus longtemps")
                       .arg(nonZeroSamples).arg(numSamples));
        return;
    }

    qDebug() << "✅ Audio valide, conversion en WAV...";

    // Save audio as WAV
    QByteArray wavData = convertToWav(m_audioData, m_sampleRate);

    QString wavFilePath = QCoreApplication::applicationDirPath() + "/temp_voice.wav";
    QFile wavFile(wavFilePath);

    if (wavFile.open(QIODevice::WriteOnly)) {
        wavFile.write(wavData);
        wavFile.close();
        qDebug() << "💾 Audio sauvegardé:" << wavFilePath;
        qDebug() << "📊 Taille fichier WAV:" << wavData.size() << "octets";

        // Process with Python script
        processWithPython(wavFilePath);
    } else {
        emit error("Impossible de sauvegarder le fichier audio");
    }
}

void VoiceRecognition::handleStateChanged(QAudio::State state)
{
    switch (state) {
    case QAudio::StoppedState:
        if (m_audioSource && m_audioSource->error() != QAudio::NoError) {
            qDebug() << "❌ Erreur audio:" << m_audioSource->error();

            QString errorMsg;
            switch (m_audioSource->error()) {
            case QAudio::OpenError:
                errorMsg = "Impossible d'ouvrir le microphone";
                break;
            case QAudio::IOError:
                errorMsg = "Erreur d'entrée/sortie audio";
                break;
            case QAudio::UnderrunError:
                errorMsg = "Buffer audio insuffisant";
                break;
            case QAudio::FatalError:
                errorMsg = "Erreur fatale du périphérique audio";
                break;
            default:
                errorMsg = "Erreur audio inconnue";
            }

            emit error(errorMsg + "\n\nVérifiez votre microphone dans Windows");
        }
        break;
    case QAudio::ActiveState:
        qDebug() << "✅ Audio ACTIF - Parlez maintenant!";
        break;
    case QAudio::IdleState:
        qDebug() << "⏸️ Audio en veille";
        break;
    default:
        break;
    }
}

void VoiceRecognition::processWithPython(const QString &wavFilePath)
{
    qDebug() << "🐍 Lancement de la reconnaissance vocale Python...";

    QString scriptPath = QCoreApplication::applicationDirPath() + "/speech_recognizer.py";
    createPythonScript(scriptPath);

    QProcess *process = new QProcess(this);
    process->setProcessChannelMode(QProcess::MergedChannels);

    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {

                QString output = QString::fromUtf8(process->readAllStandardOutput()).trimmed();
                QString errorOutput = QString::fromUtf8(process->readAllStandardError()).trimmed();

                qDebug() << "=== RÉSULTAT PYTHON ===";
                qDebug() << "Code de sortie:" << exitCode;
                qDebug() << "Sortie:" << output;

                if (!errorOutput.isEmpty()) {
                    qDebug() << "Erreurs:" << errorOutput;
                }

                if (output.contains("No module named 'speech_recognition'") ||
                    errorOutput.contains("No module named 'speech_recognition'")) {
                    emit error("Module Python manquant!\n\nOuvrez un terminal et tapez:\npip install SpeechRecognition");
                    process->deleteLater();
                    return;
                }

                if (exitCode == 0 && !output.isEmpty()) {
                    if (output.contains("Error:", Qt::CaseInsensitive) ||
                        output.contains("Exception:", Qt::CaseInsensitive)) {
                        emit error("Erreur: " + output);
                    } else {
                        qDebug() << "✅ TEXTE RECONNU:" << output;
                        emit textRecognized(output);
                    }
                } else if (output.contains("Could not understand")) {
                    emit error("Audio incompréhensible\n\nConseils:\n- Parlez plus clairement\n- Rapprochez-vous du micro\n- Réduisez le bruit ambiant");
                } else if (output.contains("request error") || output.contains("RequestError")) {
                    emit error("Erreur réseau\n\nGoogle Speech Recognition nécessite:\n- Connexion internet active");
                } else {
                    QString detailedError = "Reconnaissance échouée\n\n";
                    detailedError += "Code de sortie: " + QString::number(exitCode) + "\n";
                    detailedError += "Sortie: " + (output.isEmpty() ? "(vide)" : output) + "\n";
                    detailedError += "Erreurs: " + (errorOutput.isEmpty() ? "(aucune)" : errorOutput);
                    emit error(detailedError);
                }

                process->deleteLater();
            });

    connect(process, &QProcess::errorOccurred, this, [this, process](QProcess::ProcessError error) {
        if (error == QProcess::FailedToStart) {
            emit this->error("Python non trouvé!\n\nInstallez Python 3:\nhttps://python.org\n\nPuis:\npip install SpeechRecognition");
        } else {
            emit this->error("Erreur Python");
        }
        process->deleteLater();
    });

    QStringList pythonCommands = {"python", "python3", "py"};
    QString foundPython;

    for (const QString &pythonCmd : pythonCommands) {
        QProcess testProcess;
        testProcess.start(pythonCmd, QStringList() << "--version");
        if (testProcess.waitForFinished(2000) && testProcess.exitCode() == 0) {
            foundPython = pythonCmd;
            qDebug() << "🐍 Python trouvé:" << pythonCmd;
            break;
        }
    }

    if (foundPython.isEmpty()) {
        emit error("Python non installé!\n\nhttps://www.python.org/downloads/");
        return;
    }

    // CRITICAL FIX: Clear Qt's MinGW environment variables that conflict with Python
    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.remove("PYTHONHOME");
    env.remove("PYTHONPATH");
    process->setProcessEnvironment(env);

    qDebug() << "Lancement Python avec environnement nettoyé";
    process->start(foundPython, QStringList() << scriptPath << wavFilePath);
}

void VoiceRecognition::createPythonScript(const QString &scriptPath)
{
    QFile scriptFile(scriptPath);
    if (scriptFile.exists()) {
        scriptFile.remove();
    }

    if (scriptFile.open(QIODevice::WriteOnly | QIODevice::Text)) {
        QTextStream out(&scriptFile);
        out << "#!/usr/bin/env python3\n";
        out << "# -*- coding: utf-8 -*-\n";
        out << "import sys\n";
        out << "import os\n";
        out << "\n";
        out << "try:\n";
        out << "    import speech_recognition as sr\n";
        out << "except ImportError:\n";
        out << "    print('Error: Module speech_recognition non trouvé')\n";
        out << "    sys.exit(1)\n";
        out << "\n";
        out << "def recognize_speech(audio_file):\n";
        out << "    if not os.path.exists(audio_file):\n";
        out << "        print(f'Error: Fichier introuvable: {audio_file}')\n";
        out << "        return 1\n";
        out << "    \n";
        out << "    recognizer = sr.Recognizer()\n";
        out << "    recognizer.energy_threshold = 100\n";
        out << "    recognizer.dynamic_energy_threshold = True\n";
        out << "    \n";
        out << "    try:\n";
        out << "        with sr.AudioFile(audio_file) as source:\n";
        out << "            recognizer.adjust_for_ambient_noise(source, duration=0.3)\n";
        out << "            audio_data = recognizer.record(source)\n";
        out << "            \n";
        out << "        text = recognizer.recognize_google(audio_data, language='fr-FR')\n";
        out << "        print(text)\n";
        out << "        return 0\n";
        out << "    except sr.UnknownValueError:\n";
        out << "        print('Could not understand audio')\n";
        out << "        return 1\n";
        out << "    except sr.RequestError as e:\n";
        out << "        print(f'RequestError: {e}')\n";
        out << "        return 2\n";
        out << "    except Exception as e:\n";
        out << "        print(f'Error: {e}')\n";
        out << "        return 3\n";
        out << "\n";
        out << "if __name__ == '__main__':\n";
        out << "    if len(sys.argv) < 2:\n";
        out << "        print('Usage: python speech_recognizer.py <audio.wav>')\n";
        out << "        sys.exit(1)\n";
        out << "    sys.exit(recognize_speech(sys.argv[1]))\n";

        scriptFile.close();
    }
}

QByteArray VoiceRecognition::convertToWav(const QByteArray &rawAudio, int sampleRate)
{
    QByteArray wav;
    QDataStream stream(&wav, QIODevice::WriteOnly);
    stream.setByteOrder(QDataStream::LittleEndian);

    qint32 dataSize = rawAudio.size();
    qint32 fileSize = 36 + dataSize;
    qint16 numChannels = 1;
    qint16 bitsPerSample = 16;
    qint32 byteRate = sampleRate * numChannels * (bitsPerSample / 8);
    qint16 blockAlign = numChannels * (bitsPerSample / 8);

    stream.writeRawData("RIFF", 4);
    stream << fileSize;
    stream.writeRawData("WAVE", 4);
    stream.writeRawData("fmt ", 4);
    qint32 fmtSize = 16;
    stream << fmtSize;
    qint16 audioFormat = 1;
    stream << audioFormat;
    stream << numChannels;
    stream << (qint32)sampleRate;
    stream << byteRate;
    stream << blockAlign;
    stream << bitsPerSample;
    stream.writeRawData("data", 4);
    stream << dataSize;
    stream.writeRawData(rawAudio.data(), rawAudio.size());

    return wav;
}
