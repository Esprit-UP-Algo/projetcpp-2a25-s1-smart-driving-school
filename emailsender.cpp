#include "emailsender.h"
#include <QDebug>
#include <QDateTime>
#include <QEventLoop>
#include <QTimer>

EmailSender::EmailSender(QObject *parent)
    : QObject(parent)
    , m_socket(new QSslSocket(this))
    , m_smtpPort(587)
    , m_state(Init)
{
    connect(m_socket, &QSslSocket::readyRead, this, &EmailSender::socketReadyRead);
    connect(m_socket, &QSslSocket::connected, this, &EmailSender::socketConnected);
    connect(m_socket, &QSslSocket::encrypted, this, &EmailSender::socketEncrypted);
    connect(m_socket, QOverload<QAbstractSocket::SocketError>::of(&QAbstractSocket::errorOccurred),
            this, &EmailSender::socketError);
    connect(m_socket, &QSslSocket::sslErrors, this, &EmailSender::sslErrors);
}

EmailSender::~EmailSender()
{
    if (m_socket->state() == QAbstractSocket::ConnectedState) {
        m_socket->close();
    }
}

void EmailSender::setSmtpServer(const QString &server, int port)
{
    m_smtpServer = server;
    m_smtpPort = port;
}

void EmailSender::setCredentials(const QString &username, const QString &password)
{
    m_username = username;
    m_password = password;
}

void EmailSender::setFromAddress(const QString &email, const QString &name)
{
    m_fromEmail = email;
    m_fromName = name;
}

bool EmailSender::sendEmail(const QString &toEmail, const QString &toName,
                            const QString &subject, const QString &body)
{
    m_toEmail = toEmail;
    m_toName = toName;
    m_subject = subject;
    m_body = body;
    m_state = Init;
    m_responseBuffer.clear();
    m_lastError.clear();

    qDebug() << "Connexion au serveur SMTP:" << m_smtpServer << ":" << m_smtpPort;

    m_socket->connectToHost(m_smtpServer, m_smtpPort);

    // Attendre la connexion (avec timeout)
    QEventLoop loop;
    QTimer timer;
    timer.setSingleShot(true);

    connect(this, &EmailSender::emailSent, &loop, &QEventLoop::quit);
    connect(this, &EmailSender::emailFailed, &loop, &QEventLoop::quit);
    connect(&timer, &QTimer::timeout, &loop, &QEventLoop::quit);

    timer.start(30000); // 30 secondes timeout
    loop.exec();

    if (timer.isActive()) {
        timer.stop();
        return m_lastError.isEmpty();
    } else {
        m_lastError = "Timeout de connexion";
        return false;
    }
}

void EmailSender::socketConnected()
{
    qDebug() << "Connecté au serveur SMTP";
}

void EmailSender::socketEncrypted()
{
    qDebug() << "Connexion sécurisée établie (TLS)";
}

void EmailSender::sslErrors(const QList<QSslError> &errors)
{
    qDebug() << "Erreurs SSL détectées:";
    for (const QSslError &error : errors) {
        qDebug() << "  -" << error.errorString();
    }

    // Ignorer les erreurs SSL (ATTENTION: seulement pour le développement!)
    // En production, vous devriez vérifier les certificats correctement
    m_socket->ignoreSslErrors();
}

void EmailSender::socketReadyRead()
{
    QString response = QString::fromUtf8(m_socket->readAll());
    m_responseBuffer += response;

    qDebug() << "SMTP <<" << response.trimmed();

    if (!m_responseBuffer.endsWith("\r\n")) {
        return; // Message incomplet
    }

    QStringList lines = m_responseBuffer.split("\r\n", Qt::SkipEmptyParts);

    for (const QString &line : lines) {
        if (line.length() < 3) continue;

        QString code = line.left(3);
        bool isError = code.startsWith("4") || code.startsWith("5");

        if (isError) {
            m_lastError = "Erreur SMTP: " + line;
            qDebug() << m_lastError;
            emit emailFailed(m_lastError);
            m_socket->close();
            return;
        }

        switch (m_state) {
        case Init:
            if (code == "220") {
                m_state = HandShake;
                sendCommand("EHLO localhost");
            }
            break;

        case HandShake:
            if (code == "250") {
                // Chercher STARTTLS dans la réponse
                if (line.contains("STARTTLS", Qt::CaseInsensitive) || m_responseBuffer.contains("STARTTLS", Qt::CaseInsensitive)) {
                    m_state = StartTLS;
                    sendCommand("STARTTLS");
                } else {
                    // Pas de STARTTLS disponible, passer directement à AUTH
                    m_state = Auth;
                    sendCommand("AUTH LOGIN");
                }
            }
            break;

        case StartTLS:
            if (code == "220") {
                qDebug() << "Démarrage de la négociation TLS...";
                m_socket->startClientEncryption();
                m_state = Auth;

                // Attendre que la connexion soit chiffrée avant de continuer
                QTimer::singleShot(500, this, [this]() {
                    sendCommand("EHLO localhost");
                    QTimer::singleShot(500, this, [this]() {
                        sendCommand("AUTH LOGIN");
                    });
                });
            }
            break;

        case Auth:
            if (code == "334") {
                m_state = User;
                sendCommand(encodeBase64(m_username));
            } else if (code == "250") {
                // Réponse EHLO après STARTTLS
                // Attendre la prochaine commande
            }
            break;

        case User:
            if (code == "334") {
                m_state = Pass;
                sendCommand(encodeBase64(m_password));
            }
            break;

        case Pass:
            if (code == "235") {
                m_state = Mail;
                sendCommand("MAIL FROM:<" + m_fromEmail + ">");
            }
            break;

        case Mail:
            if (code == "250") {
                m_state = Rcpt;
                sendCommand("RCPT TO:<" + m_toEmail + ">");
            }
            break;

        case Rcpt:
            if (code == "250") {
                m_state = Data;
                sendCommand("DATA");
            }
            break;

        case Data:
            if (code == "354") {
                m_state = Body;

                QString message;
                message += "From: " + m_fromName + " <" + m_fromEmail + ">\r\n";
                message += "To: " + m_toName + " <" + m_toEmail + ">\r\n";
                message += "Subject: " + m_subject + "\r\n";
                message += "Content-Type: text/plain; charset=UTF-8\r\n";
                message += "Date: " + QDateTime::currentDateTime().toString(Qt::RFC2822Date) + "\r\n";
                message += "\r\n";
                message += m_body;
                message += "\r\n.\r\n";

                sendCommand(message);
            }
            break;

        case Body:
            if (code == "250") {
                m_state = Quit;
                sendCommand("QUIT");
                qDebug() << "Email envoyé avec succès!";
                emit emailSent();
            }
            break;

        case Quit:
            if (code == "221") {
                m_state = Close;
                m_socket->close();
            }
            break;

        default:
            break;
        }
    }

    m_responseBuffer.clear();
}

void EmailSender::socketError(QAbstractSocket::SocketError error)
{
    m_lastError = "Erreur de connexion: " + m_socket->errorString();
    qDebug() << m_lastError;
    emit emailFailed(m_lastError);
}

void EmailSender::sendCommand(const QString &command)
{
    qDebug() << "SMTP >>" << command.left(100).trimmed();
    m_socket->write(command.toUtf8() + (command.endsWith("\r\n") ? "" : "\r\n"));
}

QString EmailSender::encodeBase64(const QString &text)
{
    return QString::fromUtf8(text.toUtf8().toBase64());
}
