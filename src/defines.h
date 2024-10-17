#ifndef DEFINES_H
#define DEFINES_H

#define BROADCAST_MESSAGE "DISCOVER_SERVER"

#define DEBUG(message) qDebug() << NAME << message
#define INFO(message) qInfo() << NAME << message
#define WARNING(message) qWarning() << NAME << message
#define CRITICAL(message) qCritical() << NAME << message
#define ERROR(message) qError() << NAME << message

#ifdef NAME
#undef NAME
#endif

#endif // DEFINES_H
