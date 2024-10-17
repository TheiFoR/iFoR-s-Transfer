#include "serverbroadcaster.h"
#include "../defines.h"

#define NAME "[Server][Broadcast]"

ServerBroadcaster::ServerBroadcaster(QObject *parent)
    : QObject{parent}
{}

void ServerBroadcaster::start()
{
    _udpSocket = std::make_unique<QUdpSocket>(this);

    while (!_udpSocket->bind(QHostAddress::AnyIPv4, _port)) {
        WARNING("Server startup error:" << _udpSocket->errorString());
        ++_port;
        if(_port > _maxPort){
            CRITICAL("SUPER BIG ULTRA CRITICAL ERROR!!! ALARM!!!!");
            std::terminate();
        }
    }

    DEBUG("Broadcast server started:" << _udpSocket->peerAddress().toString() << _udpSocket->peerPort());

    connect(_udpSocket.get(), &QUdpSocket::readyRead, this, &ServerBroadcaster::readyRead);
}

void ServerBroadcaster::readyRead()
{
    while (_udpSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(_udpSocket->pendingDatagramSize());

        QHostAddress sender;
        quint16 senderPort;

        _udpSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        DEBUG("Received datagram from" << sender.toString() << ":" << senderPort);
        DEBUG("Message:" << datagram);
    }
}
