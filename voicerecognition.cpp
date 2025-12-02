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

void VoiceRecognition::setupAudio()
{
    QAudioFormat format;
    format.setSampleRate(16000);
    format.setChannelCount(1);
    format.setSampleFormat(QAudioFormat::Int16);

    QAudioDevice info = QMediaDevices::defaultAudioInput();

    if (info.isNull()) {
        QMessageBox::critical(nullptr, "Erreur Microphone",
                              "Aucun microphone détecté!\n\n"
                              "Vérifiez que votre microphone est:\n"
                              "1. Branché correctement\n"
                              "2. Activé dans les paramètres Windows\n"
                              "3. Défini comme périphérique par défaut");
        return;
    }

    if (!info.isFormatSupported(format)) {
        format = info.preferredFormat();
    }

    m_audioSource = new QAudioSource(info, format, this);
    m_sampleRate = format.sampleRate();

    m_audioSource->setBufferSize(65536);
    m_audioSource->setVolume(1.0);

    connect(m_audioSource, &QAudioSource::stateChanged,
            this, &VoiceRecognition::handleStateChanged);
}

void VoiceRecognition::startRecording()
{
    if (m_isRecording) {
        return;
    }

    if (!m_audioSource) {
        emit error("Aucun périphérique audio disponible!");
        return;
    }

    m_audioData.clear();
    m_audioBuffer.close();
    m_audioBuffer.setBuffer(&m_audioData);
    m_audioBuffer.open(QIODevice::WriteOnly);

    m_audioDevice = m_audioSource->start();

    if (m_audioDevice) {
        connect(m_audioDevice, &QIODevice::readyRead, this, [this]() {
            QByteArray data = m_audioDevice->readAll();
            if (!data.isEmpty()) {
                m_audioData.append(data);
            }
        });

        m_isRecording = true;
        m_recordingTimer->start(MAX_RECORDING_TIME);
        emit recordingStarted();
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

    if (m_audioData.size() < 1000) {
        emit error("Audio trop court! Parlez plus longtemps.");
        return;
    }


    const qint16* samples = reinterpret_cast<const qint16*>(m_audioData.constData());
    int numSamples = m_audioData.size() / 2;

    qint16 maxAmplitude = 0;
    int nonZeroSamples = 0;

    for (int i = 0; i < numSamples; i++) {
        qint16 amp = qAbs(samples[i]);
        if (amp > maxAmplitude) maxAmplitude = amp;
        if (amp > 10) nonZeroSamples++;
    }

    if (maxAmplitude < 50) {
        emit error("Aucun son détecté! Augmentez le volume du microphone.");
        return;
    }

    if (nonZeroSamples < (numSamples / 10)) {
        emit error("Audio majoritairement silencieux! Parlez plus fort.");
        return;
    }


    QByteArray wavData = convertToWav(m_audioData, m_sampleRate);
    QString wavFilePath = QCoreApplication::applicationDirPath() + "/temp_voice.wav";
    QFile wavFile(wavFilePath);

    if (wavFile.open(QIODevice::WriteOnly)) {
        wavFile.write(wavData);
        wavFile.close();
        processWithPython(wavFilePath);
    } else {
        emit error("Impossible de sauvegarder le fichier audio");
    }
}

void VoiceRecognition::handleStateChanged(QAudio::State state)
{
    if (state == QAudio::StoppedState) {
        if (m_audioSource && m_audioSource->error() != QAudio::NoError) {
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
            emit error(errorMsg);
        }
    }
}

void VoiceRecognition::processWithPython(const QString &wavFilePath)
{
    QString scriptPath = QCoreApplication::applicationDirPath() + "/speech_recognizer.py";
    createPythonScript(scriptPath);

    QProcess *process = new QProcess(this);
    process->setProcessChannelMode(QProcess::MergedChannels);

    connect(process, QOverload<int, QProcess::ExitStatus>::of(&QProcess::finished),
            this, [this, process](int exitCode, QProcess::ExitStatus exitStatus) {

                QString output = QString::fromUtf8(process->readAllStandardOutput()).trimmed();
                QString errorOutput = QString::fromUtf8(process->readAllStandardError()).trimmed();

                if (output.contains("No module named 'speech_recognition'")) {
                    emit error("Module Python manquant!\n\nOuvrez un terminal:\npip install SpeechRecognition");
                    process->deleteLater();
                    return;
                }

                if (exitCode == 0 && !output.isEmpty()) {
                    if (output.contains("Error:", Qt::CaseInsensitive)) {
                        emit error("Erreur: " + output);
                    } else {
                        emit textRecognized(output);
                    }
                } else if (output.contains("Could not understand")) {
                    emit error("Audio incompréhensible. Parlez plus clairement.");
                } else if (output.contains("request error")) {
                    emit error("Erreur réseau. Vérifiez votre connexion internet.");
                } else {
                    emit error("Reconnaissance échouée");
                }

                process->deleteLater();
            });

    connect(process, &QProcess::errorOccurred, this, [this, process](QProcess::ProcessError error) {
        if (error == QProcess::FailedToStart) {
            emit this->error("Python non trouvé!\n\nInstallez Python 3 depuis:\nhttps://python.org");
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
            break;
        }
    }

    if (foundPython.isEmpty()) {
        emit error("Python non installé!");
        return;
    }


    QProcessEnvironment env = QProcessEnvironment::systemEnvironment();
    env.remove("PYTHONHOME");
    env.remove("PYTHONPATH");
    process->setProcessEnvironment(env);

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
