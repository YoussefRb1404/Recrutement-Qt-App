QT       += core gui sql
QT       += printsupport
QT       += sql
QT += multimedia
QT += multimediawidgets

QT += charts

QT       += widgets
QT       += core gui sql network
QT       += serialport





#LIBS += -L$$PWD / -lssleay32
#LIBS += -L$$PWD / -llibeay32



greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    camera.cpp \
    candidat.cpp \
    connexion.cpp \
    exportexcelobject.cpp \
    gescond.cpp \
    login.cpp \
    loginc.cpp \
    mailling.cpp \
    main.cpp \
    gesoff.cpp \
    offre.cpp \
    options.cpp \
    smtp.cpp

HEADERS += \
    camera.h \
    candidat.h \
    connexion.h \
    exportexcelobject.h \
    gescond.h \
    gesoff.h \
    login.h \
    loginc.h \
    mailling.h \
    offre.h \
    options.h \
    smtp.h

FORMS += \
    camera.ui \
    gescond.ui \
    gesoff.ui \
    login.ui \
    mailling.ui \
    options.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    youssef.qrc

DISTFILES +=
