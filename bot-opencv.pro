QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

DEFINES += QT_DEPRECATED_WARNINGS


CONFIG += console
CONFIG += c+14

#QMAKE_CXXFLAGS_DEBUG += Od
#QMAKE_CXXFLAGS_DEBUG += -Zi

RC_FILE     = resource.rc
TARGET = bot-opencv
QMAKE_CXXFLAGS_DEBUG += -O0


unix: !macx {
    INCLUDEPATH += /usr/local/include/opencv4
    INCLUDEPATH += /usr/include/tesseract
    LIBS += -L/usr/bin/tesseract \
        -ltesseract
    LIBS += -lX11   -lopencv_core
    LIBS += -L/usr/local/lib \
    #    -lopencv_superres\
    #    -lopencv_shape\
    #    -lopencv_videostab\
        -lopencv_calib3d \
        -lopencv_photo \
        -lopencv_core\
        -lopencv_features2d\
        -lopencv_stitching\
        -lopencv_flann \
        -lopencv_highgui\
        -lopencv_videoio\
        -lopencv_imgcodecs\
        -lopencv_video\
        -lopencv_imgproc\
        -lopencv_ml\
        -lopencv_objdetect
}


win32-msvc* {

    VERSION_BUILD = 320
#    PATH_VERSION = 64      # 64 version path
    PATH_VERSION = 32      # 32 version path
    VERSION_VS = 2017
    DISK = E

    temp = "$${DISK}:\projects\opencv\opencv\build$${VERSION_VS}-$${PATH_VERSION}\install\include"
    message($$temp)



    INCLUDEPATH += "$${DISK}:\projects\opencv\opencv\build$${VERSION_VS}-$${PATH_VERSION}\install\include"
    INCLUDEPATH += "$${DISK}:\projects\vcpkg\installed\x86-windows\include"

#    debug:LIBS += "$${DISK}:\projects\vcpkg\installed\x86-windows\debug\lib\tesseract41d.lib"
#    else:LIBS += "$${DISK}:\projects\vcpkg\installed\x86-windows\lib\tesseract41.lib"

    CONFIG(debug, debug|release) {
            LIBS += "$${DISK}:\projects\vcpkg\installed\x86-windows\debug\lib\tesseract41d.lib"
            LIBS += "$${DISK}:\projects\vcpkg\installed\x86-windows\debug\lib\leptonica-1.78.0d.lib"

            LIBS += "$${DISK}:\projects\opencv\opencv\build$${VERSION_VS}-$${PATH_VERSION}\install\lib\opencv_core$${VERSION_BUILD}d.lib"
            LIBS += "$${DISK}:\projects\opencv\opencv\build$${VERSION_VS}-$${PATH_VERSION}\install\lib\opencv_highgui$${VERSION_BUILD}d.lib"
            LIBS += "$${DISK}:\projects\opencv\opencv\build$${VERSION_VS}-$${PATH_VERSION}\install\lib\opencv_imgcodecs$${VERSION_BUILD}d.lib"
            LIBS += "$${DISK}:\projects\opencv\opencv\build$${VERSION_VS}-$${PATH_VERSION}\install\lib\opencv_imgproc$${VERSION_BUILD}d.lib"
            LIBS += "$${DISK}:\projects\opencv\opencv\build$${VERSION_VS}-$${PATH_VERSION}\install\lib\opencv_features2d$${VERSION_BUILD}d.lib"
            LIBS += "$${DISK}:\projects\opencv\opencv\build$${VERSION_VS}-$${PATH_VERSION}\install\lib\opencv_calib3d$${VERSION_BUILD}d.lib"
            LIBS += "$${DISK}:\projects\opencv\opencv\build$${VERSION_VS}-$${PATH_VERSION}\install\lib\opencv_videoio$${VERSION_BUILD}d.lib"
    }
    CONFIG(release, debug|release) {
            LIBS += "$${DISK}:\projects\vcpkg\installed\x86-windows\lib\tesseract41.lib"
            LIBS += "$${DISK}:\projects\vcpkg\installed\x86-windows\lib\leptonica-1.78.0.lib"

            LIBS += "$${DISK}:\projects\opencv\opencv\build$${VERSION_VS}-$${PATH_VERSION}\install\lib\opencv_core$${VERSION_BUILD}.lib"
            LIBS += "$${DISK}:\projects\opencv\opencv\build$${VERSION_VS}-$${PATH_VERSION}\install\lib\opencv_highgui$${VERSION_BUILD}.lib"
            LIBS += "$${DISK}:\projects\opencv\opencv\build$${VERSION_VS}-$${PATH_VERSION}\install\lib\opencv_imgcodecs$${VERSION_BUILD}.lib"
            LIBS += "$${DISK}:\projects\opencv\opencv\build$${VERSION_VS}-$${PATH_VERSION}\install\lib\opencv_imgproc$${VERSION_BUILD}.lib"
            LIBS += "$${DISK}:\projects\opencv\opencv\build$${VERSION_VS}-$${PATH_VERSION}\install\lib\opencv_features2d$${VERSION_BUILD}.lib"
            LIBS += "$${DISK}:\projects\opencv\opencv\build$${VERSION_VS}-$${PATH_VERSION}\install\lib\opencv_calib3d$${VERSION_BUILD}.lib"
            LIBS += "$${DISK}:\projects\opencv\opencv\build$${VERSION_VS}-$${PATH_VERSION}\install\lib\opencv_videoio$${VERSION_BUILD}.lib"

    }

}



SOURCES += \
    actions/actiondebug.cpp \
    actions/actionwait.cpp \
    actions/clickeroftime.cpp \
    actions/fishing.cpp \
    actions/getstrstaticfield.cpp \
    actions/gettextstaticfield.cpp \
    actions/imageexpected.cpp \
    actions/imageexpectedclose.cpp \
    actions/manufacturing.cpp \
    actions/marker.cpp \
    actions/panelinventory.cpp \
    actions/restorgame.cpp \
    actions/sendeventcontrol.cpp \
    actions/simlegendkult.cpp \
    actions/tests.cpp \
    aicontrol.cpp \
    baseaction.cpp \
    capturewindow.cpp \
    controlpanel.cpp \
    displays.cpp \
    enginescript.cpp \
    guiinfo.cpp \
    iodata.cpp \
    lowlvlenginescript.cpp \
    main.cpp \
    socketio.cpp

HEADERS += \
    actions/actiondebug.h \
    actions/actionwait.h \
    actions/clickeroftime.h \
    actions/fishing.h \
    actions/getstrstaticfield.h \
    actions/gettextstaticfield.h \
    actions/imageexpected.h \
    actions/imageexpectedclose.h \
    actions/manufacturing.h \
    actions/marker.h \
    actions/panelinventory.h \
    actions/restorgame.h \
    actions/sendeventcontrol.h \
    actions/simlegendkult.h \
    actions/tests.h \
    aicontrol.h \
    baseaction.h \
    capturewindow.h \
    controlpanel.h \
    displays.h \
    enginescript.h \
    global.h \
    guiinfo.h \
    iodata.h \
    lowlvlenginescript.h \
    main.h \
    resource.rc \
    socketio.h
#    ui_controlpanel.h



FORMS += \
    controlpanel.ui
