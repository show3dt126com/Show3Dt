#-------------------------------------------------
#
# Project created by QtCreator 2019-05-06T09:15:53
#
#-------------------------------------------------

QT       += core gui network

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = DFShow3D
TEMPLATE = app

INCLUDEPATH += .\FreeType
LIBS += -L$$PWD\FreeType -lfreetype
DESTDIR = D:\DFShow3DRun

#INCLUDEPATH += -I  "C:/Program Files/Python37/include"
#LIBS += -L"C:/Program Files/Python37/libs" -lpython37
#INCLUDEPATH += -I "D:\Python3.7\include"
#LIBS += -L"D:\Python3.7\libs" -lpython37


#QMAKE_CXXFLAGS -= -Wunused-parameter -Wconversion -Wunused-variable
QMAKE_CXXFLAGS += -Wno-unused-parameter -Wno-unused-variable -Wno-return-type -Wno-switch -Wno-sign-compare

# The following define makes your compiler emit warnings if you use
# any feature of Qt which has been marked as deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if you use deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

#CONFIG += c++11

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    BBS.cpp \
    Camera.cpp \
    Center.cpp \
    Cube.cpp \
    Global.cpp \
    Shader.cpp \
    ToolBox.cpp \
    Sky.cpp \
    Dem.cpp \
    Texter.cpp \
    WinBox.cpp \
    ColorFrame.cpp \
    ColorMapSet.cpp \
    Gate.cpp \
    Defines.cpp \
    Scheme.cpp \
    WebMercator.cpp \
    BSIMap.cpp \
    LookDownControl.cpp \
    LookIso3DCamera.cpp \
    LookIso3DControl.cpp \
    LookIso3DValue.cpp \
    LookSideControl.cpp \
    CubeModel.cpp \
    Test.cpp \
    Spirit.cpp \
    Texture2D.cpp \
    EagleEye.cpp \
    SideEye.cpp \
    Scene.cpp \
    SchemeSaveDlg.cpp \
    SceneWin.cpp \
    ScrollBarV.cpp \
    PythonIf.cpp \
    BubbleTip.cpp \
    ShapeButton.cpp \
    RotatePanel.cpp \
    CustomStyleDlg.cpp \
    MapMan.cpp

HEADERS += \
    MainWindow.h \
    Scene.h \
    BBS.h \
    Camera.h \
    Center.h \
    Cube.h \
    Global.h \
    Shader.h \
    ToolBox.h \
    Defines.h \
    Sky.h \
    Dem.h \
    Texter.h \
    WinBox.h \
    ColorFrame.h \
    ColorMapSet.h \
    Gate.h \
    Scheme.h \
    WebMercator.h \
    BSIMap.h \
    LookDownControl.h \
    LookIso3DCamera.h \
    LookIso3DControl.h \
    LookIso3DValue.h \
    LookSideControl.h \
    CubeModel.h \
    Test.h \
    Spirit.h \
    Texture2D.h \
    BaseModel.h \
    EagleEye.h \
    SideEye.h \
    SchemeSaveDlg.h \
    SceneWin.h \
    ScrollBarV.h \
    PythonIf.h \
    BubbleTip.h \
    RotatePanel.h \
    CustomStyleDlg.h \
    ShapeButton.h \
    MapMan.h
FORMS += \
        MainWindow.ui \
    ColorMapSet.ui \
    LookDownControl.ui \
    LookIso3DCamera.ui \
    LookIso3DControl.ui \
    LookIso3DValue.ui \
    LookSideControl.ui \
    SchemeSaveDlg.ui \
    CustomStyleDlg.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Shader.qrc

DISTFILES += \
    ../../ShuiJingZhu-GoogleMap/GoogleMap-05/World-L05-000-000.txt \
    Design.txt \
    Python/kde.py


win32:CONFIG(release, debug|release): LIBS += -L$$PWD/../Python3.7/libs/ -lpython37
else:win32:CONFIG(debug, debug|release): LIBS += -L$$PWD/../Python3.7/libs/ -lpython37d

INCLUDEPATH += $$PWD/../Python3.7/include
DEPENDPATH += $$PWD/../Python3.7/include
