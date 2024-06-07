QT -= gui
QT += core network sql

CONFIG += c++17 console
CONFIG -= app_bundle

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

COMMON = $${PWD}/../common

INCLUDEPATH += \
    $${COMMON}/ \

SOURCES += \
    main.cpp \
    requesthandler.cpp \
    httpserver/httpserver.cpp \
    httpserver/httpsockethandler.cpp \
    httpserver/httprequest.cpp \
    httpserver/httpresponse.cpp \
    db/dbmanager.cpp \
    db/dbpostgres/dbmanagerpostgres.cpp \
    db/dbpostgres/dbpostgresquery.cpp \
    $${COMMON}/employee.cpp \
    $${COMMON}/task.cpp \

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

HEADERS += \
    requesthandler.h \
    httpserver/httpserver.h \
    httpserver/httpsockethandler.h \
    httpserver/httprequest.h \
    httpserver/httpresponse.h \
    db/dbmanager.h \
    db/dbexception.h \
    db/dbopener.h \
    db/dbauthorization.h \
    db/dbpostgres/dbmanagerpostgres.h \
    db/dbpostgres/dbpostgresquery.h \
    $${COMMON}/employee.h   \
    $${COMMON}/task.h \

