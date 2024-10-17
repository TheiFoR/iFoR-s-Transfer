#ifndef CLIENT_H
#define CLIENT_H

#include "src/client/broadcaster.h"
#include <QObject>
#include <QTimer>
#include <QTcpSocket>
#include <QProcess>
#include <QRegularExpression>
#include <QNetworkInterface>
#include <QEventLoop>
#include <QtConcurrent>

class Client : public QObject
{
    Q_OBJECT
public:
    explicit Client(QObject *parent = nullptr);

    void setServerAddress(const QHostAddress address, quint16 port);

public slots:
    void start();

signals:
    void connected(QString str);

private slots:
    void readyRead();

private:
    QVector<quint16> _ports = { 13100, 13101, 13102, 13103, 13104 };

    QHostAddress _serverAddress;
    quint16 _serverPort;

    QList<QHostAddress> getAddresses();
    void sendBroadcast();

    std::shared_ptr<QTcpSocket> _clientSocket;
    std::unique_ptr<Broadcaster> _broadcaster;
    std::unique_ptr<QThread> _broadcastThread;
};

#endif // CLIENT_H
