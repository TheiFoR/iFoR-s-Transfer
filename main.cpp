#include <QGuiApplication>
#include <QQmlApplicationEngine>
#include <QFile>
#include "src/core.h"
#include <QJsonObject>
#include <QJsonArray>
#include <QJsonDocument>

//const QString logFilePath = "logs/" + QDateTime::currentDateTime().toString("MM_dd_yyyy___hh_mm_ss_zzz_ap") + ".log";
const QString logFilePath = "logs/" + QDateTime::currentDateTime().toString("log") + ".json";
Core core;

void myMessageOutput(QtMsgType type, const QMessageLogContext &context, const QString &msg)
{
    QHash<QtMsgType, QString> msgLevelHash({{QtDebugMsg, "Debug"}, {QtInfoMsg, "Information"}, {QtWarningMsg, "Warning"}, {QtCriticalMsg, "Error"}, {QtFatalMsg, "Fatal"}});
    QByteArray localMsg = msg.toLocal8Bit();
    QDateTime time = QDateTime::currentDateTime();
    QString formattedTime = time.toString("hh:mm:ss.zzz");
    QByteArray formattedTimeMsg = formattedTime.toLocal8Bit();
    QString logLevelName = msgLevelHash[type];
    QByteArray logLevelMsg = logLevelName.toLocal8Bit();

    QString message = QString(localMsg.constData());
    QString messagePath = QString("%1:%2, %3").arg(context.file).arg(context.line).arg(context.function);
    fprintf(stderr, (formattedTime + " " + logLevelMsg + ": " + message + " (" + messagePath + ")\n").toStdString().c_str());

    QJsonObject jsonObject
    {
        {"@t", time.toOffsetFromUtc(QDateTime::currentDateTime().offsetFromUtc()).toString(Qt::ISODate) },
        {"@m", message },
        {"@l", logLevelName },
        {"@i", messagePath },
    };
    QJsonDocument jsonDocument;
    jsonDocument.setObject(jsonObject);

    QFile outFile(logFilePath);
    outFile.open(QIODevice::WriteOnly | QIODevice::Append);
    QTextStream ts(&outFile);
    ts << jsonDocument.toJson(QJsonDocument::Compact) << Qt::endl;
    outFile.close();

    core.setLog(message);

    fflush(stderr);
}


int main(int argc, char *argv[])
{
    qInstallMessageHandler(myMessageOutput);
    QGuiApplication app(argc, argv);

    QQmlApplicationEngine engine;

    core.registerContext(engine);

    core.start();

    const QUrl url(QStringLiteral("qrc:/qml/main.qml"));
    QObject::connect(
        &engine,
        &QQmlApplicationEngine::objectCreated,
        &app,
        [url](QObject *obj, const QUrl &objUrl) {
            if (!obj && url == objUrl)
                QCoreApplication::exit(-1);
        },
        Qt::QueuedConnection);
    engine.load(url);

    return app.exec();
}
