QT += quick svg concurrent

CONFIG *= c++1z

SOURCES += \
        main.cpp \
        src/client/broadcaster.cpp \
        src/client/client.cpp \
        src/core.cpp \
        src/server/clienthandler.cpp \
        src/server/server.cpp \
        src/server/serverbroadcaster.cpp

RESOURCES += \
    assets.qrc \
    qml.qrc

# Additional import path used to resolve QML modules in Qt Creator's code model
QML_IMPORT_PATH =

# Additional import path used to resolve QML modules just for Qt Quick Designer
QML_DESIGNER_IMPORT_PATH =

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    src/client/broadcaster.h \
    src/client/client.h \
    src/core.h \
    src/defines.h \
    src/server/clienthandler.h \
    src/server/server.h \
    src/server/serverbroadcaster.h
