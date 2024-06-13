QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

COMMON = $${PWD}/../common

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0


INCLUDEPATH += \
    $${COMMON}/ \

SOURCES += \
    dialogs/dialogedittask/dialogedittask.cpp \
    models/employeesmodel.cpp \
    models/tasksmodel.cpp \
    models/employeeobject.cpp \
    models/taskobject.cpp \
    views/employeesview.cpp \
    views/tasksview.cpp \
    views/taskstatedelegate.cpp \
    dialogs/dialogeditemployee/dialogeditemployee.cpp \
    dialogs/dialogopendb/dialogopendb.cpp \
    httpclient/httpclient.cpp \
    $${COMMON}/employee.cpp \
    $${COMMON}/task.cpp \
    main.cpp \
    mainwindow.cpp \

HEADERS += \
    dialogs/dialogedittask/dialogedittask.h \
    models/employeesmodel.h \
    models/employeeobject.h \
    models/tasksmodel.h \
    models/taskobject.h \
    views/tasksview.h \
    views/taskstatedelegate.h \
    views/employeesview.h \
    dialogs/dialogeditemployee/dialogeditemployee.h \
    dialogs/dialogopendb/dialogopendb.h \
    $${COMMON}/employee.h \
    $${COMMON}/task.h \
    httpclient/httpclient.h \
    mainwindow.h

FORMS += \
    dialogs/dialogeditemployee/dialogeditemployee.ui \
    dialogs/dialogedittask/dialogedittask.ui \
    dialogs/dialogopendb/dialogopendb.ui \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
