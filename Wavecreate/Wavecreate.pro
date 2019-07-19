#-------------------------------------------------
#
# Project created by QtCreator 2019-07-12T14:01:03
#
#-------------------------------------------------

QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = Wavecreate
TEMPLATE = app

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

CONFIG += c++11

INCLUDEPATH += C:\OpenCV\opencv\release\install\include

LIBS += C:\OpenCV\opencv\release\bin\libopencv_calib3d410.dll
LIBS += C:\OpenCV\opencv\release\bin\libopencv_core410.dll
LIBS += C:\OpenCV\opencv\release\bin\libopencv_dnn410.dll
LIBS += C:\OpenCV\opencv\release\bin\libopencv_features2d410.dll
LIBS += C:\OpenCV\opencv\release\bin\libopencv_flann410.dll
LIBS += C:\OpenCV\opencv\release\bin\libopencv_gapi410.dll
LIBS += C:\OpenCV\opencv\release\bin\libopencv_highgui410.dll
LIBS += C:\OpenCV\opencv\release\bin\libopencv_imgcodecs410.dll
LIBS += C:\OpenCV\opencv\release\bin\libopencv_imgproc410.dll
LIBS += C:\OpenCV\opencv\release\bin\libopencv_ml410.dll
LIBS += C:\OpenCV\opencv\release\bin\libopencv_objdetect410.dll
LIBS += C:\OpenCV\opencv\release\bin\libopencv_photo410.dll
LIBS += C:\OpenCV\opencv\release\bin\libopencv_stitching410.dll
LIBS += C:\OpenCV\opencv\release\bin\libopencv_test_gapi.dll.a
LIBS += C:\OpenCV\opencv\release\bin\libopencv_video410.dll
LIBS += C:\OpenCV\opencv\release\bin\libopencv_videoio410.dll
LIBS += C:\OpenCV\opencv\release\bin\opencv_ffmpeg410_64.dll



SOURCES += \
        main.cpp \
        mainwindow.cpp \
        simulationwave.cpp \
        wave.cpp

HEADERS += \
        mainwindow.h \
        simulationwave.h \
        wave.h

FORMS += \
        mainwindow.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../OpenCV/opencv/build/lib/ -lade

#else:unix: LIBS += -L$$PWD/../../OpenCV/opencv/build/lib/ -lade

#INCLUDEPATH += $$PWD/../../OpenCV/opencv/build/include
#DEPENDPATH += $$PWD/../../OpenCV/opencv/build/include

#win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../../OpenCV/opencv/build/lib/ -llibopencv_core410.dll
#else:unix: LIBS += -L$$PWD/../../OpenCV/opencv/build/lib/ -llibopencv_core410.dll
#unix|win32: LIBS += -L$$PWD/../../OpenCV/opencv/build/lib/ -llibopencv_dnn410.dll
#unix|win32: LIBS += -L$$PWD/../../OpenCV/opencv/build/lib/ -llibopencv_features2d410.dll
#unix|win32: LIBS += -L$$PWD/../../OpenCV/opencv/build/bin/ -llibopencv_test_gapi.dll
#win32: LIBS += -L$$PWD/../../OpenCV/opencv/build/lib/ -llibopencv_gapi410.dll
#win32: LIBS += -L$$PWD/../../OpenCV/opencv/build/lib/ -llibopencv_highgui410.dll
#win32: LIBS += -L$$PWD/../../OpenCV/opencv/build/lib/ -llibopencv_imgcodecs410.dll
#win32: LIBS += -L$$PWD/../../OpenCV/opencv/build/lib/ -llibopencv_imgproc410.dll
#win32: LIBS += -L$$PWD/../../OpenCV/opencv/build/lib/ -llibopencv_ml410.dll
#win32: LIBS += -L$$PWD/../../OpenCV/opencv/build/lib/ -llibopencv_photo410.dll
#win32: LIBS += -L$$PWD/../../OpenCV/opencv/build/lib/ -lopencv_ts410
#win32: LIBS += -L$$PWD/../../OpenCV/opencv/build/lib/ -llibopencv_video410.dll
#win32: LIBS += -L$$PWD/../../OpenCV/opencv/build/lib/ -llibopencv_videoio410.dll
#win32: LIBS += -L$$PWD/../../OpenCV/opencv/build/lib/ -llibopencv_flann410.dll
