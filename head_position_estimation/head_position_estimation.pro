QT       += core gui

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
    camconfig.cpp \
    dnnfacedetect.cpp \
    gethist.cpp \
    getseg.cpp \
    main.cpp \
    mainwindow.cpp \
    mycombobox.cpp \
    mycombopage.cpp \
    mytablemodel.cpp \
    opencam.cpp \
    segwidget.cpp

HEADERS += \
    camconfig.h \
    dnnfacedetect.h \
    gethist.h \
    getseg.h \
    mainwindow.h \
    mycombobox.h \
    mycombopage.h \
    mytablemodel.h \
    opencam.h \
    segwidget.h

FORMS += \
    camconfig.ui \
    mainwindow.ui \
    mycombobox.ui \
    mycombopage.ui \
    segwidget.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    qrsc.qrc

INCLUDEPATH += /usr/local/include \
                /usr/local/include/opencv
                /usr/local/include/opencv2

LIBS += /usr/local/lib/libopencv_*

DISTFILES += \
    opencv_face_detector.pbtxt \
    opencv_face_detector_uint8.pb


