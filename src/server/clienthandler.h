#ifndef CLIENTHANDLER_H
#define CLIENTHANDLER_H

#include <QTcpSocket>
#include <QObject>
#include <QThread>

class ClientHandler : public QObject
{
    Q_OBJECT
public:
    explicit ClientHandler(QObject *parent = nullptr);
    ClientHandler(QTcpSocket* socket);

signals:

public slots:
    void start();
    void sendFiles(const QList<QString> files);

private slots:
    void readData();
private:
    QTcpSocket* _socket;
};

#endif // CLIENTHANDLER_H
