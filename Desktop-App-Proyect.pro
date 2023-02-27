QT       += core gui sql
<<<<<<< HEAD
##LIBS += -L/usr/local/Cellar/libpq/15.2/lib -lpq #Conexion macOS
=======
#LIBS += -L/usr/local/Cellar/libpq/15.2/lib -lpq #Conexion macOS
>>>>>>> 963e26ad032c775d083ec431bb3a16f0ab8aa3f4

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp

HEADERS += \
    mainwindow.h

FORMS += \
    mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
