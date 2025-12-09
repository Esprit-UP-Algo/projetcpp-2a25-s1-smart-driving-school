
#ifndef ARDUINOVH_H
#define ARDUINOVH_H

#include <QObject>
#include <QSerialPort>
#include <QSerialPortInfo>
#include <QString>
#include <QDebug>
#include <QSqlQuery>
#include <QSqlError>
#include <QTimer>

    class ArduinoVH : public QObject
{
    Q_OBJECT

public:
    explicit ArduinoVH(QObject *parent = nullptr);
    ~ArduinoVH();

    // Connection management
    bool connectToArduino();
    bool isConnected() const { return m_connected; }
    void disconnect();

    // Get available ports
    QStringList getAvailablePorts();

    // Database operations
    bool saveToDatabase(const QString &cardCode);
    bool cardExists(const QString &cardCode);
    int getTotalCards();

    // New: Mark card as present
    bool markCardPresent(const QString &cardCode);

signals:
    void cardDetected(const QString &cardCode);
    void cardScanned(const QString &cardCode, bool exists); // New signal
    void connectionStatusChanged(bool connected);
    void errorOccurred(const QString &error);

private slots:
    void readSerialData();
    void handleSerialError(QSerialPort::SerialPortError error);
    void checkExpiredCards(); // New: Check for expired presence

private:
    QSerialPort *m_serialPort;
    QString m_buffer;
    bool m_connected;
    QTimer *m_presenceTimer; // New: Timer to check expired cards

    QString findArduinoPort();
};

#endif // ARDUINOVH_H
