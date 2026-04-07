//#include <GL/glew.h>
#include "MyGLWidget.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include <QTimer>
#include "MyForm.h"

#include <iostream>

MyGLWidget::MyGLWidget (QWidget* parent) : QOpenGLWidget(parent), program(NULL)
{
  setFocusPolicy(Qt::StrongFocus);  // per rebre events de teclat
}

MyGLWidget::~MyGLWidget ()
{
  if (program != NULL)
    delete program;
}

void MyGLWidget::tick(){

    t+=MyForm::CLOCK_PERIOD_MILIS/1000.0f;
    emit timeUpdate(t);
    update(); // actualitza la interfície gràfica (repinta)
}

void MyGLWidget::resetTime(){
    t=0;
    emit timeUpdate(t);
}

float MyGLWidget::getTime(){
    return t;
}

void MyGLWidget::initializeGL ()
{
  // Cal inicialitzar l'ús de les funcions d'OpenGL
  initializeOpenGLFunctions();
  
  angleMan1 = angleMan2 = 0;

  glClearColor (200/255.0, 220/255.0, 255/255.0, 1.0); // defineix color de fons (d'esborrat)
  carregaShaders();
  creaBuffers();

  // Inicialització de variables d'estat
 }

void MyGLWidget::paintGL ()
{
// Aquest codi és necessari únicament per a MACs amb pantalla retna.
#ifdef __APPLE__
  GLint vp[4];
  glGetIntegerv (GL_VIEWPORT, vp);
  ample = vp[2];
  alt = vp[3];
#endif

    //================================================================
    // NOTA: Modifiqueu amb total llibertat aquest mètode, el codi que
    // es suministra és simplement a tall d'exemple, com a esquema
    // per orientar-vos.
    //================================================================

    glClear (GL_COLOR_BUFFER_BIT);  // Esborrem el frame-buffer
    pintaTerra();

    float angle1 = 0, angle2 = 0;

    if(esModeManual()) {
        angle1 = angleMan1;
        angle2 = angleMan2;
    } else {
        // Mode automàtic, posició per temps. Useu
        // this->getTime() per obtenir el temps (en segons) i
        // a partir d'aquí calcular els angles

        float temps = this->getTime();
        float amplitud = 40.0f;   // 40 graus
        float periode = 2.0f;
        float frequencia = 2*M_PI/periode; // Freqüència angular, 1 segon la anada -> 2π/periode

        // θ(t)=A⋅sin(ωt+ϕ)
        // Per començar a l'angle -40: -amplitud i sumem 90 (l'angle màxim)
        float angle = -amplitud * sin(frequencia * temps + 90);

        if(angle <= 0) angle1 = angle;
        else if(angle >= 0) angle2 = angle;
    }

    pintaSuport(-0.375f, angle1);
    pintaSuport(-0.125f, 0);
    pintaSuport(0.125f, 0);
    pintaSuport(0.375f, angle2);

    // Desactivem el VAO actiu
    glBindVertexArray(0);
}

void MyGLWidget::resizeGL (int w, int h)
{
  ample = w;
  alt = h;
}


void MyGLWidget::pintaBola(float d, float angle){

    transformacioBola(d,angle);
    glBindVertexArray(VAO_BOLA);
    glDrawArrays(GL_TRIANGLE_FAN, 0, CIRCLE_SAMPLES+1);
    pintaEstrella();
}

void MyGLWidget::pintaEstrella(){
    glBindVertexArray(VAO_ESTRELLA);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 12);
    // Completeu amb el que considereu necessari

}

void MyGLWidget::pintaSuport(float d, float angle){
    transformacioSuport(d,angle);
    glBindVertexArray(VAO_SUPORT);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 4);
    pintaBola(d, angle);
}

void MyGLWidget::pintaTerra(){
    transformacioTerra();
    glBindVertexArray(VAO_TERRA);
    glUniform1i(vimetLoc, true);
    glDrawArrays(GL_TRIANGLE_STRIP, 0, 6);
    glUniform1i(vimetLoc, false);

}

void MyGLWidget::keyPressEvent(QKeyEvent* event)
{
    makeCurrent();
        switch (event->key()) {

        case Qt::Key_A: {
            if(angleMan1 > -40) angleMan1 -= 5;
            break;
        }
        case Qt::Key_D: {
            if(angleMan1 < 0) angleMan1 += 5;
            break;
        }
        case Qt::Key_Q: {
            if(angleMan2 > 0) angleMan2 -= 5;
            break;
        }
        case Qt::Key_E: {
            if(angleMan2 < 40) angleMan2 += 5;
            break;
        }

        default: event->ignore(); break;
        }
    update();

}

void MyGLWidget::transformacioTerra(){
    glm::mat4 transform (1.0f);
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::transformacioBola(float d, float angle){
    glm::mat4 transform (1.0f);
    transform = glm::translate(transform, glm::vec3(d,0.5f,0.0f));
    transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0,0,1));
    transform = glm::translate(transform, glm::vec3(0, -0.625f, 0.0f));
    transform = glm::scale(transform, glm::vec3(0.25f));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::transformacioSuport(float d, float angle){
    glm::mat4 transform (1.0f);
    transform = glm::translate(transform, glm::vec3(d,0.5f,0.0f));
    transform = glm::rotate(transform, glm::radians(angle), glm::vec3(0,0,1));
    transform = glm::translate(transform, glm::vec3(0,-0.25f,0.0f));
    transform = glm::scale(transform, glm::vec3(0.5f));
    glUniformMatrix4fv(transLoc, 1, GL_FALSE, &transform[0][0]);
}

void MyGLWidget::creaBuffers (){
    creaBufferBola();
    creaBufferSuport();
    creaBufferTerra();
    creaBufferEstrella();
    // Desactivem el VAO
    glBindVertexArray(0);
}

void MyGLWidget::createVBOs(int size,  glm::vec3 Colors[], glm::vec3 Vertices[] ){

    // Creació del buffer amb les dades dels vèrtexs
    GLuint VBO[2];
    glGenBuffers(2, VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO[0]);
    glBufferData(GL_ARRAY_BUFFER, size, Vertices, GL_STATIC_DRAW);
    // Activem l'atribut que farem servir per vèrtex (només el 0 en aquest cas)
    glVertexAttribPointer(vertexLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(vertexLoc);

    // Creació del buffer de colors
    glBindBuffer(GL_ARRAY_BUFFER, VBO[1]);
    glBufferData(GL_ARRAY_BUFFER, size, Colors, GL_STATIC_DRAW);
    glVertexAttribPointer(colorLoc, 3, GL_FLOAT, GL_FALSE, 0, 0);
    glEnableVertexAttribArray(colorLoc);

    // Desactivem el VAO
    glBindVertexArray(0);
}


void MyGLWidget::creaBufferTerra(){

    glm::vec3 Vertices[6];  // Tres vèrtexs amb X, Y i Z
    Vertices[0] = glm::vec3(-1.00,  0.75, 0.0);
    Vertices[1] = glm::vec3(-0.50,  0.5, 0.0);
    Vertices[2] = glm::vec3(-1.00,  1.0, 0.0);
    Vertices[3] = glm::vec3( 0.5,  0.5, 0.0);
    Vertices[4] = glm::vec3( 1.00,  1.0, 0.0);
    Vertices[5] = glm::vec3( 1.00,  0.75, 0.0);

    // Creació del Vertex Array Object (VAO) que usarem per pintar
    glGenVertexArrays(1, &VAO_TERRA);
    glBindVertexArray(VAO_TERRA);

    glm::vec3 Colors[6];
    for(int i=0;i<6;i++) {
        Colors[i] = COLOR_MARRO;
    }

    createVBOs( sizeof(Vertices), Colors, Vertices );
}

void MyGLWidget::creaBufferSuport(){

    glm::vec3 Vertices[4];  // Tres vèrtexs amb X, Y i Z

    Vertices[0] = glm::vec3(+0.04,  -0.5,  0.0);
    Vertices[1] = glm::vec3(-0.04,  -0.5,  0.0);
    Vertices[2] = glm::vec3(+0.04,  +0.5,  0.0);
    Vertices[3] = glm::vec3(-0.04,  +0.5,  0.0);

    glm::vec3 Colors[4];
    for(int i=0;i<4;i++) {
        Colors[i] = COLOR_BLAU;//
    }


    // Creació del Vertex Array Object (VAO) que usarem per pintar
    glGenVertexArrays(1, &VAO_SUPORT);
    glBindVertexArray(VAO_SUPORT);

    createVBOs( sizeof(Vertices), Colors, Vertices );

}

void MyGLWidget::creaBufferBola()
{
    glm::vec3   Colors[CIRCLE_SAMPLES+1];
    glm::vec3 Vertices[CIRCLE_SAMPLES+1];
    Vertices[0] = glm::vec3(0.0f , 0.0f, 0.0f);
    Colors[0]   = glm::vec3( 1.0,1.0,1.0);

    QColor q(244,247, 17);
    qreal h,s,v,a;
    q.getHsvF(&h,&s,&v,&a); // Treballem amb coordenades HSV de color.

    float radi      = 0.5f;
    float alfa      = 0;
    float deltaAlfa = 2.0*M_PI / (CIRCLE_SAMPLES-1);
    float deltaV    = 1.0 / (CIRCLE_SAMPLES-1);

    for(int k=1;k<=CIRCLE_SAMPLES; k++, alfa+=deltaAlfa){
        // càlcul de les coordenades del vèrtex
        Vertices[k] = glm::vec3( radi * cos(alfa), radi * sin(alfa), 0.0);
        // càlcul del color pel gradient.
        q.setHsvF(h,s,v,a);
        Colors[k] = glm::vec3(q.red()/255.0, q.green()/255.0, q.blue()/255.0);
        if(k<CIRCLE_SAMPLES/2) {v-=deltaV;} else {v+=deltaV;}
    }

    // Creació del Vertex Array Object (VAO) que usarem per pintar
    glGenVertexArrays(1, &VAO_BOLA);
    glBindVertexArray(VAO_BOLA);

    createVBOs(sizeof(Vertices),  Colors, Vertices );
}

void MyGLWidget::creaBufferEstrella()
{
    // Crear VAO_ESTRELLA
    // Creeu el buffer de vèrtex i colors
    // Després, caldrà que programeu i crideu
    // adequadament al mètode pintaEstrella()

    glm::vec3 Vertices[12];
    Vertices[0] = glm::vec3(0.0f , 0.0f, 0.0f);

    // Càlcul de l'estrella.
    float radiPetit = 0.125f, radiGran = 0.25f;
    float angle = 0;
    for(int i = 1; i < 12; ++i){
        if(i%2 != 0) Vertices[i] = glm::vec3(radiGran*sin(glm::radians(angle)) , radiGran*cos(glm::radians(angle)), 0.0f);
        else Vertices[i] = glm::vec3(radiPetit*sin(glm::radians(angle)) , radiPetit*cos(glm::radians(angle)), 0.0f);
        angle+=36;
    }

    glm::vec3 Colors[12];
    for(int i=0;i<12;i++) {
        Colors[i] = glm::vec3(236.0/255, 107.0/255, 70.0/255);
    }

    // Creació del Vertex Array Object (VAO) que usarem per pintar
    glGenVertexArrays(1, &VAO_ESTRELLA);
    glBindVertexArray(VAO_ESTRELLA);

    createVBOs(sizeof(Vertices), Colors, Vertices);
}


void MyGLWidget::carregaShaders()
{
  // Creem els shaders per al fragment shader i el vertex shader
  QOpenGLShader fs (QOpenGLShader::Fragment, this);
  QOpenGLShader vs (QOpenGLShader::Vertex, this);
  // Carreguem el codi dels fitxers i els compilem
  fs.compileSourceFile("shaders/basicShader.frag");
  vs.compileSourceFile("shaders/basicShader.vert");
  // Creem el program
  program = new QOpenGLShaderProgram(this);
  // Li afegim els shaders corresponents
  program->addShader(&fs);
  program->addShader(&vs);
  // Linkem el program
  program->link();
  // Indiquem que aquest és el program que volem usar
  program->bind();

  // Obtenim identificador per a l'atribut “vertex” del vertex shader
  vertexLoc = glGetAttribLocation (program->programId(), "vertex");
  colorLoc = glGetAttribLocation (program->programId(), "color");
  transLoc = glGetUniformLocation(program->programId(), "TG");
  vimetLoc = glGetUniformLocation(program->programId(), "aplicaVimet");
}
