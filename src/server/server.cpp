#include "server.h"
#include "qthread.h"
#include "../defines.h"

#define NAME "[Server]"

Server::Server(QObject *parent)
{

}

void Server::start()
{
    _server = std::make_unique<QTcpServer>();
    _serverBroadcaster = std::make_unique<ServerBroadcaster>();
    _serverBroadcasterThread = std::make_unique<QThread>();

    QObject::connect(_serverBroadcasterThread.get(), &QThread::started, _serverBroadcaster.get(), &ServerBroadcaster::start);

    _serverBroadcaster->moveToThread(_serverBroadcasterThread.get());
    _serverBroadcasterThread->start();

    QHostAddress serverAddress;
    foreach (const QNetworkInterface &interface, QNetworkInterface::allInterfaces()) {
        // Перечисляем все IP-адреса, связанные с данным интерфейсом
        foreach (const QHostAddress &address, interface.allAddresses()) {
            // Проверяем, является ли адрес IPv4 и не является ли это loopback (127.0.0.1)
            if (address.protocol() == QAbstractSocket::IPv4Protocol && !address.isLoopback()) {
                DEBUG("IP Address:" << address.toString());
                serverAddress = address;
                break;
            }
        }
        if(!serverAddress.isNull())
            break;
    }

    while (!_server->listen(serverAddress, _port)) {
        WARNING("Server startup error:" << _server->errorString());
        ++_port;
        if(_port > _maxPort){
            CRITICAL("SUPER BIG ULTRA CRITICAL ERROR!!! ALARM!!!!");
            std::terminate();
        }
    }

    emit started(_server->serverAddress(), _server->serverPort());

    // Получение IP и порта
    QHostAddress ip = _server->serverAddress();
    quint16 port = _server->serverPort();

    // Вывод IP и порта сервера
    INFO("The server is running on IP:" << ip.toString() << "and the port:" << port);

    // Подключаемся к сигналу newConnection, чтобы обрабатывать новые подключения
    QObject::connect(_server.get(), &QTcpServer::newConnection, this, [&]() {
        // Получение клиентского сокета
        QTcpSocket *clientSocket = _server->nextPendingConnection();
        QThread *clientThread = new QThread();
        ClientHandler* clientHandler = new ClientHandler(clientSocket);

        QObject::connect(this, &Server::signalSendFiles, clientHandler, &ClientHandler::sendFiles, Qt::QueuedConnection);

        clientHandler->moveToThread(clientThread);
        _clients.append(QPair<ClientHandler*, QThread*>(clientHandler, clientThread));
        clientThread->start();
        // Вывод информации о клиенте
        INFO("The client is connected with an IP:" << clientSocket->peerAddress().toString() << "and the port:" << clientSocket->peerPort());

        // Обработка разрыва соединения
        QObject::connect(clientSocket, &QTcpSocket::disconnected, this, [&clientSocket, &clientThread]() {
            INFO("Client is disconnected." << clientSocket->peerAddress() << clientSocket->peerPort());
            clientThread->exit();
        });
    });
}

void Server::sendFiles(const QList<QString> files)
{
    emit signalSendFiles(files);
}
