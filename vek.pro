QT += widgets
#QT += concurrent
CONFIG += c++17
CONFIG += static
CONFIG += release
#DEFINES += QT_DEPRECATED_WARNINGS\
#           QT_NO_WARNING_OUTPUT
#          QT_NO_DEBUG_OUTPUT
VERSION = 1.0.2.1
DEFINES +=APP_VERSION=\\\"$$VERSION\\\"
# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
  main.cpp \
    src/obj/objectAppAT.cpp \
    src/obj/objectAppMT.cpp \
    src/obj/objectExtend.cpp \
    src/obj/objectGetCurl.cpp \
    src/obj/objectGitWine.cpp \
    src/obj/objectJson.cpp \
    src/obj/objectNet.cpp \
    src/obj/objectPackage.cpp \
    src/obj/objectProcManage.cpp \
    src/obj/objectSource.cpp \
    src/obj/objectTray.cpp \
    src/obj/pObject.cpp \
    src/vek.cpp \
    src/vekAbout.cpp \
    src/vekAppAddAT.cpp \
    src/vekAppAddMT.cpp \
    src/vekAppAddMulti.cpp \
    src/vekExportJson.cpp \
    src/vekPackage.cpp \
    src/vekSourceEdit.cpp \
    src/vekExtendDebug.cpp \
    src/vekWineOption.cpp \
    src/view/aview/vekAppData.cpp \
    src/view/aview/vekAppListView.cpp \
    src/view/aview/vekAppPanel.cpp \
    src/view/jview/vekModelAT.cpp \
    src/view/jview/vekPanelAT.cpp \
    src/view/jview/vekViewAT.cpp

HEADERS += \
    src/obj/baseData.h \
    src/obj/baseRes.h \
    src/obj/common.h \
    src/obj/objectAppAT.h \
    src/obj/objectAppMT.h \
    src/obj/objectExtend.h \
    src/obj/objectGetCurl.h \
    src/obj/objectGitWine.h \
    src/obj/objectJson.h \
    src/obj/objectNet.h \
    src/obj/objectPackage.h \
    src/obj/objectProcManage.h \
    src/obj/objectSource.h \
    src/obj/objectTray.h \
    src/obj/pObject.h \
    src/ui_common.h \
    src/vek.h \
    src/vekAbout.h \
    src/vekAppAddAT.h \
    src/vekAppAddMT.h \
    src/vekAppAddMulti.h \
    src/vekExportJson.h \
    src/vekExtendDebug.h \
    src/vekPackage.h \
    src/vekSourceEdit.h \
    src/vekWineOption.h \
    src/view/aview/vekAppData.h \
    src/view/aview/vekAppListView.h \
    src/view/aview/vekAppPanel.h \
    src/view/jview/vekModelAT.h \
    src/view/jview/vekPanelAT.h \
    src/view/jview/vekViewAT.h

FORMS += \
    ui/vekAppAddAT.ui \
    ui/vekAppAddMT.ui \
    ui/vekAppAddMulti.ui \
    ui/vekExportJson.ui \
    ui/vekSourceEdit.ui \
    ui/vek.ui \
    ui/vekAbout.ui \
    ui/vekExtendDebug.ui \
    ui/vekPackage.ui \
    ui/vekWineOption.ui

#DEBUG
#QMAKE_CXXFLAGS_RELEASE = $$QMAKE_CFLAGS_RELEASE_WITH_DEBUGINFO
#QMAKE_LFLAGS_RELEASE = $$QMAKE_LFLAGS_RELEASE_WITH_DEBUGINFO

#UPX
#Release:QMAKE_POST_LINK += ./upx -9 $(DESTDIR_TARGET)
QMAKE_CXXFLAGS_RELEASE += -O3
# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    vek_res.qrc

RC_ICONS = res\img\vek.ico

DISTFILES += \
    vek_us_EN.ts
######curl##########
DEFINES += CURL_STATICLIB
LIBS += -L$$PWD/3rdparty/curl/lib/ -lcurl -lssl -lcrypto -lz

INCLUDEPATH += $$PWD/3rdparty/curl/include
DEPENDPATH += $$PWD/3rdparty/curl/include

#########rapidjson########
INCLUDEPATH += $$PWD/3rdparty/json
DEPENDPATH += $$PWD/3rdparty/json
##########libgit2#########
LIBS += -L$$PWD/3rdparty/libgit2/lib/ -lgit2 -lpcre -lssl -lcrypto
INCLUDEPATH += $$PWD/3rdparty/libgit2/include
DEPENDPATH += $$PWD/3rdparty/libgit2/include
