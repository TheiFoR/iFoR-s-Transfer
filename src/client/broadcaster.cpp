#include "broadcaster.h"
#include "../defines.h"

#define NAME "[Client][Broadcast]"

Broadcaster::Broadcaster(QObject *parent)
    : QObject{parent}
{}

Broadcaster::Broadcaster(QHostAddress serverAddress, quint16 serverPort)
{
    _serverAddress = serverAddress;
    _serverPort = serverPort;
}

void Broadcaster::start()
{
    QTimer timer;

    QList<QHostAddress> allAddresses = getAddresses();

    _clientSocket = std::make_unique<QUdpSocket>();

    DEBUG("IPv4 addresses in the local network:");
    for (const QHostAddress &address : allAddresses) {
        DEBUG(address.toString());
    }

    // Подключение сигнала таймера для отправки запросов каждые 5 секунд
    QObject::connect(&timer, &QTimer::timeout, this, &Broadcaster::sendBroadcast);

    // Обработка получения ответа от сервера
    QObject::connect(_clientSocket.get(), &QUdpSocket::readyRead, this, &Broadcaster::readyRead);

    timer.start(10000);
    sendBroadcast();
}

void Broadcaster::sendBroadcast()
{
    QList<QHostAddress> allAddresses = getAddresses();
    for(const QHostAddress &address : allAddresses){
        for(const quint16 &port : _ports){
            if(port == _serverPort && address == _serverAddress){
                continue;
            }

            // Попытка соединения с сервером (здесь можно указать конкретные IP и порты)
            _clientSocket->connectToHost(address, port);
            DEBUG(QString("Connection to " + address.toString() + ":" + QString::number(port)).toStdString().c_str());

            // Ждём подключения
            if (_clientSocket->waitForConnected(100)) {
                // Отправляем запрос серверу
                _clientSocket->write(BROADCAST_MESSAGE);
                _clientSocket->flush();
                // I don't know how it works.
                if(!_clientSocket->waitForReadyRead(100)){
                    _clientSocket->close();
                }
            }
        }
    }
}

void Broadcaster::readyRead()
{
    while (_clientSocket->hasPendingDatagrams()) {
        QByteArray datagram;
        datagram.resize(_clientSocket->pendingDatagramSize());

        QHostAddress sender;
        quint16 senderPort;

        _clientSocket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);

        DEBUG("Received response from" << sender.toString() << ":" << senderPort);
        DEBUG("Message:" << datagram);
    }
}

QList<QHostAddress> Broadcaster::getAddresses()
{
    QList<QHostAddress> allAddressesNotValidation = QNetworkInterface::allAddresses();
    QList<QHostAddress> allAddresses;

    QProcess process;
    process.start("arp", QStringList() << "-a");
    process.waitForFinished();

    QTextStream stream(process.readAllStandardOutput());
    while (!stream.atEnd()) {
        QString line = stream.readLine();
        if (line.contains("dynamic")) {
            QRegularExpression re("\\s+");
            QStringList parts = line.split(re);
            if (parts.size() >= 3) {
                QString physicalAddress = parts[1];
                if (QHostAddress(physicalAddress).protocol() == QAbstractSocket::IPv4Protocol && !QHostAddress(physicalAddress).isLoopback() && QHostAddress(physicalAddress) != QHostAddress("192.168.0.1") && QHostAddress("192.168.1.1") != QHostAddress(physicalAddress)) {
                    allAddresses.append(QHostAddress(physicalAddress));
                }
            }
        }
    }
    return allAddresses;
}
