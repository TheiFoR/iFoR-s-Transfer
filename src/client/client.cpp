#include "client.h"
#include "../defines.h"

#define NAME "[Client]"

Client::Client(QObject *parent)
{

}

void Client::start()
{
    _clientSocket = std::make_shared<QTcpSocket>();
    _broadcaster = std::make_unique<Broadcaster>(_serverAddress, _serverPort);
    _broadcastThread = std::make_unique<QThread>();

    QObject::connect(_broadcastThread.get(), &QThread::started, _broadcaster.get(), &Broadcaster::start);

    _broadcaster->moveToThread(_broadcastThread.get());
    _broadcastThread->start();

    QEventLoop loop;
    loop.exec();
}

void Client::readyRead()
{
    QByteArray response = _clientSocket->readAll();
    DEBUG("Received a response from the server:" << response);

    // Проверяем, что ответ именно от нашего сервера
    if (response.contains(BROADCAST_MESSAGE)) {
        DEBUG("Connection to the server is successful." << _clientSocket->peerAddress() << _clientSocket->peerPort());
        emit connected(_clientSocket->peerAddress().toString() + ":" + QString::number(_clientSocket->peerPort()));
    } else {
        DEBUG("The answer is not from our server.");
    }
}

void Client::setServerAddress(const QHostAddress address, quint16 port)
{
    _serverAddress = address;
    _serverPort = port;
}

void Client::sendBroadcast()
{
    QList<QHostAddress> allAddresses = getAddresses();
    for(const QHostAddress &address : allAddresses){
        for(const quint16 &port : _ports){
            if(port == _serverPort){
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
                if(_clientSocket->waitForReadyRead(100)){
                    _clientSocket->close();
                }
            }
        }
    }
}

QList<QHostAddress> Client::getAddresses()
{
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
