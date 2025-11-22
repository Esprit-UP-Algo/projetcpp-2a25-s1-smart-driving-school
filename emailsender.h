#ifndef EMAILSENDER_H
#define EMAILSENDER_H

#include <QObject>
#include <QString>
#include <QSslSocket>

class EmailSender : public QObject
{
    Q_OBJECT

public:
    explicit EmailSender(QObject *parent = nullptr);
    ~EmailSender();

    // Configuration
    void setSmtpServer(const QString &server, int port);
    void setCredentials(const QString &username, const QString &password);
    void setFromAddress(const QString &email, const QString &name);

    // Envoyer un email
    bool sendEmail(const QString &toEmail,
                   const QString &toName,
                   const QString &subject,
                   const QString &body);

    // Obtenir le dernier message d'erreur
    QString lastError() const { return m_lastError; }

signals:
    void emailSent();
    void emailFailed(const QString &error);

private slots:
    void socketReadyRead();
    void socketError(QAbstractSocket::SocketError error);
    void socketConnected();
    void socketEncrypted();
    void sslErrors(const QList<QSslError> &errors);

private:
    void sendCommand(const QString &command);
    QString encodeBase64(const QString &text);

    QSslSocket *m_socket;

    QString m_smtpServer;
    int m_smtpPort;
    QString m_username;
    QString m_password;
    QString m_fromEmail;
    QString m_fromName;

    QString m_toEmail;
    QString m_toName;
    QString m_subject;
    QString m_body;

    QString m_lastError;
    QString m_responseBuffer;

    enum State {
        Init,
        HandShake,
        StartTLS,
        Auth,
        User,
        Pass,
        Mail,
        Rcpt,
        Data,
        Body,
        Quit,
        Close
    };

    State m_state;
};

#endif // EMAILSENDER_H
