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

# ------------------------------------------------------------------------------
INCLUDEPATH += \
    $${COMMON}/ \

# ------------------------------------------------------------------------------
SOURCES +=                              \
    dialogs/settingsdialog.cpp \
    models/tasksmodel.cpp               \
    models/objectsmodel.cpp             \
    models/employeesmodel.cpp           \
    models/expiredtasksproxymodel.cpp   \
    delegates/expiredtasksdelegate.cpp  \
    delegates/statedelegate.cpp         \
    models/viewerproxymodel.cpp         \
    widgets/tasksviewer.cpp             \
    widgets/employeesviewer.cpp         \
    dialogs/taskdialog.cpp              \
    dialogs/employeedialog.cpp          \
    dialogs/connectdialog.cpp           \
    db/dbremotemanager.cpp              \
    httpclient/httpclient.cpp           \
    $${COMMON}/employee.cpp             \
    $${COMMON}/task.cpp                 \
    main.cpp                            \
    mainwindow.cpp                      \
    widgets/viewer.cpp                  \
    settings.cpp                        \

# ------------------------------------------------------------------------------
HEADERS +=                              \
    dialogs/settingsdialog.h \
    models/tasksmodel.h                 \
    models/objectsmodel.h               \
    models/employeesmodel.h             \
    models/expiredtasksproxymodel.h     \
    delegates/expiredtasksdelegate.h    \
    delegates/statedelegate.h           \
    models/viewerproxymodel.h           \
    widgets/tasksviewer.h               \
    widgets/employeesviewer.h           \
    dialogs/taskdialog.h                \
    dialogs/employeedialog.h            \
    dialogs/connectdialog.h             \
    db/dbremotemanager.h                \
    httpclient/httpclient.h             \
    $${COMMON}/employee.h               \
    $${COMMON}/task.h                   \
    mainwindow.h                        \
    widgets/viewer.h                    \
    settings.h                          \

# ------------------------------------------------------------------------------
FORMS +=                                \
    dialogs/settingsdialog.ui \
    dialogs/taskdialog.ui               \
    dialogs/employeedialog.ui           \
    dialogs/connectdialog.ui            \
    widgets/viewer.ui                   \
    mainwindow.ui                       \

# ------------------------------------------------------------------------------
TRANSLATIONS += ru.ts

CODECFORSRC = UTF-8

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
