QT       += core gui sql printsupport

#LIBS += -L/usr/local/Cellar/libpq/15.2/lib -lpq #Conexion macOS


greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    order.cpp \
    orderdetails.cpp \
    productoscrud.cpp \
    products.cpp \
    reportes.cpp

HEADERS += \
    mainwindow.h \
    order.h \
    orderdetails.h \
    productoscrud.h \
    products.h \
    reportes.h

FORMS += \
    mainwindow.ui \
    productoscrud.ui \
    reportes.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
