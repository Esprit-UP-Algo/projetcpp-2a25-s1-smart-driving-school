#include "arduinoVH.h"
#include <QtSerialPort>
#include <QDateTime>

ArduinoVH::ArduinoVH(QObject *parent)
    : QObject(parent)
    , m_serialPort(nullptr)
    , m_connected(false)
    , m_presenceTimer(nullptr)
{
    m_serialPort = new QSerialPort(this);

    // Setup timer to check for expired presence every 1 second
    m_presenceTimer = new QTimer(this);
    connect(m_presenceTimer, &QTimer::timeout, this, &ArduinoVH::checkExpiredCards);
    m_presenceTimer->start(1000); // Check every second
}

ArduinoVH::~ArduinoVH()
{
    if (m_connected) {
        disconnect();
    }
}

QStringList ArduinoVH::getAvailablePorts()
{
    QStringList ports;
    const auto portInfos = QSerialPortInfo::availablePorts();

    for (const QSerialPortInfo &portInfo : portInfos) {
        QString portDescription = portInfo.portName() + " - " + portInfo.description();
        ports << portDescription;
    }

    return ports;
}

QString ArduinoVH::findArduinoPort()
{
    const auto portInfos = QSerialPortInfo::availablePorts();

    for (const QSerialPortInfo &portInfo : portInfos) {
        if (portInfo.description().contains("Arduino", Qt::CaseInsensitive) ||
            portInfo.manufacturer().contains("Arduino", Qt::CaseInsensitive) ||
            portInfo.portName().contains("COM") ||
            portInfo.portName().contains("ttyUSB") ||
            portInfo.portName().contains("ttyACM")) {

            qDebug() << "Arduino trouvé sur:" << portInfo.portName();
            return portInfo.portName();
        }
    }

    return QString();
}

bool ArduinoVH::connectToArduino()
{
    if (m_connected) {
        qDebug() << "Déjà connecté à Arduino";
        return true;
    }

    QString portName = findArduinoPort();

    if (portName.isEmpty()) {
        QString error = "Aucun Arduino détecté. Vérifiez la connexion USB.";
        qDebug() << error;
        emit errorOccurred(error);
        return false;
    }

    m_serialPort->setPortName(portName);
    m_serialPort->setBaudRate(QSerialPort::Baud9600);
    m_serialPort->setDataBits(QSerialPort::Data8);
    m_serialPort->setParity(QSerialPort::NoParity);
    m_serialPort->setStopBits(QSerialPort::OneStop);
    m_serialPort->setFlowControl(QSerialPort::NoFlowControl);

    if (m_serialPort->open(QIODevice::ReadWrite)) {
        m_connected = true;

        connect(m_serialPort, &QSerialPort::readyRead,
                this, &ArduinoVH::readSerialData);
        connect(m_serialPort, &QSerialPort::errorOccurred,
                this, &ArduinoVH::handleSerialError);

        qDebug() << "Connecté à Arduino sur" << portName;
        emit connectionStatusChanged(true);
        return true;
    } else {
        QString error = "Impossible d'ouvrir le port: " + m_serialPort->errorString();
        qDebug() << error;
        emit errorOccurred(error);
        return false;
    }
}

void ArduinoVH::disconnect()
{
    if (m_connected && m_serialPort->isOpen()) {
        m_serialPort->close();
        m_connected = false;
        emit connectionStatusChanged(false);
        qDebug() << "Déconnecté d'Arduino";
    }
}

void ArduinoVH::readSerialData()
{
    QByteArray data = m_serialPort->readAll();
    m_buffer += QString::fromUtf8(data);

    while (m_buffer.contains('\n')) {
        int index = m_buffer.indexOf('\n');
        QString line = m_buffer.left(index).trimmed();
        m_buffer = m_buffer.mid(index + 1);

        if (line.startsWith("CARD:")) {
            QString cardCode = line.mid(5).trimmed();
            qDebug() << "Carte RFID détectée:" << cardCode;

            // Check if card exists in database
            bool exists = cardExists(cardCode);

            if (exists) {
                // Mark card as present (true for 10 seconds)
                if (markCardPresent(cardCode)) {
                    emit cardScanned(cardCode, true);
                    qDebug() << "✅ Carte existante scannée - Présence marquée";
                }
            } else {
                // New card - save it to database
                if (saveToDatabase(cardCode)) {
                    // Mark as present immediately after saving
                    markCardPresent(cardCode);
                    emit cardDetected(cardCode);
                    emit cardScanned(cardCode, false);
                }
            }
        }
    }

    if (m_buffer.size() > 1000) {
        m_buffer.clear();
    }
}

void ArduinoVH::handleSerialError(QSerialPort::SerialPortError error)
{
    if (error == QSerialPort::ResourceError) {
        QString errorMsg = "Arduino déconnecté: " + m_serialPort->errorString();
        qDebug() << errorMsg;
        m_connected = false;
        emit connectionStatusChanged(false);
        emit errorOccurred(errorMsg);
        disconnect();
    }
}

bool ArduinoVH::saveToDatabase(const QString &cardCode)
{
    QSqlQuery query;

    // Check if card already exists
    query.prepare("SELECT COUNT(*) FROM ARDUINO WHERE CODE_AR = :code");
    query.bindValue(":code", cardCode);

    if (query.exec() && query.next()) {
        int count = query.value(0).toInt();

        if (count > 0) {
            qDebug() << "Carte déjà enregistrée:" << cardCode;
            emit errorOccurred("Cette carte est déjà enregistrée!");
            return false;
        }
    }

    // Generate new ID manually
    int newId = 1;
    if (query.exec("SELECT NVL(MAX(ID_ARDUINO), 0) + 1 FROM ARDUINO")) {
        if (query.next()) {
            newId = query.value(0).toInt();
        }
    }

    qDebug() << "Génération ID:" << newId << "pour carte:" << cardCode;

    // Insert new card with generated ID and set IS_PRESENT to 1
    query.prepare("INSERT INTO ARDUINO (ID_ARDUINO, CODE_AR, IS_PRESENT, LAST_SCAN_TIME) "
                  "VALUES (:id, :code, 1, SYSTIMESTAMP)");
    query.bindValue(":id", newId);
    query.bindValue(":code", cardCode);

    if (query.exec()) {
        qDebug() << "✅ Carte sauvegardée! ID:" << newId << "Code:" << cardCode;
        return true;
    } else {
        qDebug() << "❌ Erreur sauvegarde:" << query.lastError().text();
        emit errorOccurred("Erreur: " + query.lastError().text());
        return false;
    }
}

bool ArduinoVH::markCardPresent(const QString &cardCode)
{
    QSqlQuery query;

    // Update the card to mark it as present with current timestamp
    query.prepare("UPDATE ARDUINO SET IS_PRESENT = 1, LAST_SCAN_TIME = SYSTIMESTAMP "
                  "WHERE CODE_AR = :code");
    query.bindValue(":code", cardCode);

    if (query.exec()) {
        qDebug() << "✅ Carte marquée présente:" << cardCode;
        return true;
    } else {
        qDebug() << "❌ Erreur marquage présence:" << query.lastError().text();
        return false;
    }
}

void ArduinoVH::checkExpiredCards()
{
    QSqlQuery query;

    // Set IS_PRESENT to 0 for cards where more than 10 seconds have passed
    // Using SYSTIMESTAMP - LAST_SCAN_TIME > INTERVAL '10' SECOND
    query.prepare("UPDATE ARDUINO SET IS_PRESENT = 0 "
                  "WHERE IS_PRESENT = 1 AND "
                  "(SYSTIMESTAMP - LAST_SCAN_TIME) > INTERVAL '10' SECOND");

    if (query.exec()) {
        int rowsAffected = query.numRowsAffected();
        if (rowsAffected > 0) {
            qDebug() << "⏰" << rowsAffected << "carte(s) marquée(s) comme absente(s)";
        }
    } else {
        qDebug() << "❌ Erreur vérification expiration:" << query.lastError().text();
    }
}

bool ArduinoVH::cardExists(const QString &cardCode)
{
    QSqlQuery query;
    query.prepare("SELECT COUNT(*) FROM ARDUINO WHERE CODE_AR = :code");
    query.bindValue(":code", cardCode);

    if (query.exec() && query.next()) {
        return query.value(0).toInt() > 0;
    }

    return false;
}

int ArduinoVH::getTotalCards()
{
    QSqlQuery query;
    query.exec("SELECT COUNT(*) FROM ARDUINO");

    if (query.next()) {
        return query.value(0).toInt();
    }

    return 0;
}
