#include "core.h"
#include "qeventloop.h"

Core::Core(QObject *parent)
    : QObject{parent}
{

}

void Core::registerContext(QQmlApplicationEngine &engine)
{
    engine.rootContext()->setContextProperty("core", this);
    setMessage("SS");
}

void Core::start()
{
    _server = std::make_shared<Server>();
    _serverThread = std::make_unique<QThread>();
    _client = std::make_unique<Client>();
    _clientThread = std::make_unique<QThread>();


    QObject::connect(_serverThread.get(), &QThread::started, _server.get(), &Server::start);
    QObject::connect(_clientThread.get(), &QThread::started, _client.get(), &Client::start);

    QObject::connect(_client.get(), &Client::connected, this, &Core::setMessage);

    QObject::connect(this, &Core::sendFiles, _server.get(), &Server::sendFiles);

    _server->moveToThread(_serverThread.get());
    _serverThread->start();

    QEventLoop loop;
    QObject::connect(_server.get(), &Server::started, this, [this, &loop](const QHostAddress address, quint16 port){
        _client->setServerAddress(address, port);
        loop.quit();
    });
    loop.exec();

    _client->moveToThread(_clientThread.get());
    _clientThread->start();
}


quint8 Core::currentPage() const
{
    return _currentPage;
}
void Core::setCurrentPage(quint8 newCurrentPage)
{
    if (_currentPage == newCurrentPage)
        return;
    _currentPage = newCurrentPage;
    emit currentPageChanged();
}

QString Core::message() const
{
    return _message;
}
void Core::setMessage(const QString &newMessage)
{
    if (_message == newMessage)
        return;
    _message = newMessage;
    emit messageChanged();
}

QColor Core::messageColor() const
{
    return _messageColor;
}
void Core::setMessageColor(const QColor &newMessageColor)
{
    if (_messageColor == newMessageColor)
        return;
    _messageColor = newMessageColor;
    emit messageColorChanged();
}

quint8 Core::currentInfoType() const
{
    return _currentInfoType;
}
void Core::setCurrentInfoType(quint8 newCurrentInfoType)
{
    if (_currentInfoType == newCurrentInfoType)
        return;
    _currentInfoType = newCurrentInfoType;
    emit currentInfoTypeChanged();
}

QString Core::downloadPath() const
{
    return _downloadPath;
}
void Core::setDownloadPath(const QString &newDownloadPath)
{
    if (_downloadPath == newDownloadPath)
        return;
    _downloadPath = newDownloadPath;
    emit downloadPathChanged();
}

QString Core::filesPathStr() const
{
    return _filesPathStr;
}

void Core::setFilesPathStr(const QString &newFilesPathStr)
{
    if (_filesPathStr == newFilesPathStr)
        return;
    _filesPathStr = newFilesPathStr;
    _filesPath = _filesPathStr.split('\n', Qt::SkipEmptyParts);
    qInfo() << _filesPath;
    emit sendFiles(_filesPath);
    emit filesPathStrChanged();
}

QString Core::log() const
{
    return _log;
}

void Core::setLog(const QString &newLog)
{
    // _log += newLog + '\n';
    // emit logChanged();
}
