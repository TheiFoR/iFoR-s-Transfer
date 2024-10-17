#include "clienthandler.h"
#include "../defines.h"

#define NAME "[ClientHandler]"

ClientHandler::ClientHandler(QObject *parent)
    : QObject{parent}
{}

ClientHandler::ClientHandler(QTcpSocket *socket)
{
    _socket = socket;
}

void ClientHandler::start()
{
    QObject::connect(_socket, &QTcpSocket::readyRead, this, &ClientHandler::readData);
}

void ClientHandler::sendFiles(const QList<QString> files)
{
    qInfo() << QThread::currentThreadId() << files;
}

void ClientHandler::readData()
{
    QByteArray data = _socket->readAll();

    DEBUG("Received data from the server:" << data);

    if(data == BROADCAST_MESSAGE){
        _socket->write(BROADCAST_MESSAGE);
    }
}
