#ifndef VOICERECOGNITION_H
#define VOICERECOGNITION_H

#include <QObject>
#include <QAudioSource>
#include <QAudioFormat>
#include <QBuffer>
#include <QTimer>

class VoiceRecognition : public QObject
{
    Q_OBJECT

public:
    explicit VoiceRecognition(QObject *parent = nullptr);
    ~VoiceRecognition();

    void startRecording();
    void stopRecording();
    bool isRecording() const { return m_isRecording; }

signals:
    void textRecognized(const QString &text);
    void error(const QString &errorMessage);
    void recordingStarted();
    void recordingStopped();

private slots:
    void handleStateChanged(QAudio::State state);

private:
    void setupAudio();
    void listAudioDevices();  // ADDED - This was missing!
    QByteArray convertToWav(const QByteArray &rawAudio, int sampleRate);
    void processWithPython(const QString &wavFilePath);
    void createPythonScript(const QString &scriptPath);

    QAudioSource *m_audioSource;
    QIODevice *m_audioDevice;
    QBuffer m_audioBuffer;
    QByteArray m_audioData;

    bool m_isRecording;
    int m_sampleRate;
    QTimer *m_recordingTimer;
    static const int MAX_RECORDING_TIME = 10000; // 10 secondes max
};

#endif // VOICERECOGNITION_H
