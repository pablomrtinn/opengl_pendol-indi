TEMPLATE    = app
QT         += opengl 

INCLUDEPATH +=  /home/pablo/glm-master

FORMS += MyForm.ui

HEADERS += MyForm.h MyGLWidget.h

SOURCES += main.cpp \
        MyForm.cpp MyGLWidget.cpp

DISTFILES += \
    shaders/basicShader.frag \
    shaders/basicShader.vert
