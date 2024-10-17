#ifndef BROADCASTER_H
#define BROADCASTER_H

#include <QObject>
#include <QUdpSocket>
#include <QTimer>
#include <QNetworkInterface>
#include <QProcess>
#include <QRegularExpression>

class Broadcaster : public QObject
{
    Q_OBJECT
public:
    explicit Broadcaster(QObject *parent = nullptr);
    Broadcaster(QHostAddress serverAddress, quint16 serverPort);

public slots:
    void start();

signals:

private slots:
    void readyRead();
private:
    std::unique_ptr<QUdpSocket> _clientSocket;
    QHostAddress _serverAddress;
    quint16 _serverPort;
    QVector<quint16> _ports = { 13105, 13106, 13107, 13108, 13109 };

    void sendBroadcast();
    QList<QHostAddress> getAddresses();
};

#endif // BROADCASTER_H
