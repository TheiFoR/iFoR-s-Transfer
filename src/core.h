#ifndef CORE_H
#define CORE_H

#include "src/server/server.h"
#include "src/client/client.h"
#include <QObject>
#include <QQmlApplicationEngine>
#include <QQmlContext>
#include <QColor>
#include <QThread>

class Core : public QObject
{
    Q_OBJECT

    Q_PROPERTY(quint8 currentPage READ currentPage WRITE setCurrentPage NOTIFY currentPageChanged FINAL)
    Q_PROPERTY(QString message READ message WRITE setMessage NOTIFY messageChanged FINAL)
    Q_PROPERTY(QColor messageColor READ messageColor WRITE setMessageColor NOTIFY messageColorChanged FINAL)
    Q_PROPERTY(quint8 currentInfoType READ currentInfoType WRITE setCurrentInfoType NOTIFY currentInfoTypeChanged FINAL)
    Q_PROPERTY(QString downloadPath READ downloadPath WRITE setDownloadPath NOTIFY downloadPathChanged FINAL)
    Q_PROPERTY(QString filesPathStr READ filesPathStr WRITE setFilesPathStr NOTIFY filesPathStrChanged FINAL)
    Q_PROPERTY(QString log READ log WRITE setLog NOTIFY logChanged FINAL)
public:
    explicit Core(QObject *parent = nullptr);

    void registerContext(QQmlApplicationEngine& engine);

    void start();

    quint8 currentPage() const;
    void setCurrentPage(quint8 newCurrentPage);

    QString message() const;
    void setMessage(const QString &newMessage);

    QColor messageColor() const;
    void setMessageColor(const QColor &newMessageColor);

    quint8 currentInfoType() const;
    void setCurrentInfoType(quint8 newCurrentInfoType);

    QString downloadPath() const;
    void setDownloadPath(const QString &newDownloadPath);

    QString filesPathStr() const;
    void setFilesPathStr(const QString &newFilesPathStr);

    QString log() const;
    void setLog(const QString &newLog);

signals:
    void currentPageChanged();
    void messageChanged();
    void messageColorChanged();
    void currentInfoTypeChanged();
    void downloadPathChanged();
    void filesPathStrChanged();
    void logChanged();

    void sendFiles(const QList<QString> files);

private:
    quint8 _currentPage = 0;
    QString _message = "Text";
    QColor _messageColor = QColor(0x40, 0x40, 0x40);
    quint8 _currentInfoType = 0;
    QString _downloadPath;

    std::shared_ptr<Server> _server;
    std::unique_ptr<QThread> _serverThread;

    std::unique_ptr<Client> _client;
    std::unique_ptr<QThread> _clientThread;
    QString _filesPathStr;
    QStringList _filesPath;
    QString _log;
};

#endif // CORE_H
