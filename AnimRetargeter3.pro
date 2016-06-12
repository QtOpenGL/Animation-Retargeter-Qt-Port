HEADERS       = ui/glwidget.h \
                ui/window.h \
                ui/mainwindow.h \
    engine/engine.h \
    engine/logo.h \
    ui/loaddialog.h \
    utils/procedureresult.h \
    engine/camera.h \
    engine/entity.h \
    engine/model.h \
    engine/md5/structs.h \
    utils/quaternion.h \
    utils/md5exception.h \
    engine/md5/animloader.h \
    engine/md5/animretargeter.h \
    engine/md5/modelloader.h \
    engine/glslprogram.h
SOURCES       = ui/glwidget.cpp \
                main.cpp \
                ui/window.cpp \
                ui/mainwindow.cpp \
    engine/engine.cpp \
    engine/logo.cpp \
    ui/loaddialog.cpp \
    utils/procedureresult.cpp \
    engine/camera.cpp \
    engine/entity.cpp \
    engine/model.cpp \
    engine/md5/structs.cpp \
    utils/quaternion.cpp \
    engine/md5/animloader.cpp \
    engine/md5/animretargeter.cpp \
    engine/md5/modelloader.cpp \
    engine/glslprogram.cpp

QT           += widgets

# install
target.path = $$[QT_INSTALL_EXAMPLES]/retargeter3
INCLUDEPATH += /Users/Alex/Library/Frameworks/glm
INCLUDEPATH += /Users/Alex/Library/Frameworks/glm/gtx
INCLUDEPATH += /Users/Alex/Library/Frameworks/glm/gtc
INCLUDEPATH += /Users/Alex/Library/Frameworks/glm/detail

CONFIG += c++11

INSTALLS += target
