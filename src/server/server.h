#ifndef SERVER_H
#define SERVER_H

#include <QObject>
#include <QTimer>
#include <QTcpServer>
#include <QTcpSocket>
#include <QNetworkInterface>
#include <QHostAddress>
#include <QHostInfo>

#include "src/server/clienthandler.h"
#include "src/server/serverbroadcaster.h"

class Server : public QObject
{
    Q_OBJECT
public:
    explicit Server(QObject *parent = nullptr);

public slots:
    void start();
    void sendFiles(const QList<QString> files);

signals:
    void signalSendFiles(const QList<QString> files);
    void started(const QHostAddress address, quint16 port);

private:
    std::unique_ptr<QTcpServer> _server;
    std::unique_ptr<ServerBroadcaster> _serverBroadcaster;
    std::unique_ptr<QThread> _serverBroadcasterThread;

    QList<QPair<ClientHandler*, QThread*>> _clients;

    const quint16 _maxPort = 13104;
    quint16 _port = 13100;
};

#endif // SERVER_H
