#ifndef SERVERBROADCASTER_H
#define SERVERBROADCASTER_H

#include <QObject>
#include <QUdpSocket>

class ServerBroadcaster : public QObject
{
    Q_OBJECT
public:
    explicit ServerBroadcaster(QObject *parent = nullptr);

    void start();

private slots:
    void readyRead();

private:
    std::unique_ptr<QUdpSocket> _udpSocket;
    quint16 _port = 13105;
    quint16 _maxPort = 13109;
};

#endif // SERVERBROADCASTER_H
