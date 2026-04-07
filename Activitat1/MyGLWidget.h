#include <QOpenGLFunctions_3_3_Core>
#include <QOpenGLWidget>
#include <QOpenGLShader>
#include <QOpenGLShaderProgram>
#include <QKeyEvent>
#include "glm/glm.hpp"


#ifndef __MyGLWidget__
#define __MyGLWidget__
class MyGLWidget : public QOpenGLWidget, protected QOpenGLFunctions_3_3_Core
{
  Q_OBJECT

  public:
    MyGLWidget (QWidget *parent=0);
    ~MyGLWidget ();
    
    void resetTime();

    float getTime();

    bool esModeManual(){
        return modeManual;
    }

  private:

    /// Temps (no tocar, es modifica sol en mode automàtic.
    float t = 0;

    /// Indica si el mode és manual (per tecles) o automàtic (per temps)
    bool modeManual=true;

  public slots:

    void tick();
    void setModeManual(bool esManual){
        modeManual=esManual;
    }


  signals:
    void timeUpdate(double time);

  protected:
    // initializeGL - Aqui incluim les inicialitzacions del contexte grafic.
    virtual void initializeGL ();

    // paintGL - Mètode cridat cada cop que cal refrescar la finestra.
    // Tot el que es dibuixa es dibuixa aqui.
    virtual void paintGL ();
 
    // resize - Es cridat quan canvia la mida del widget
    virtual void resizeGL (int width, int height);  

    // keyPressEvent - Es cridat quan es prem una tecla
     virtual void keyPressEvent (QKeyEvent *event);

  private:

    //Variables d'estat =======================
    float rotation1 = 0, rotation2 = 0;

    const int CIRCLE_SAMPLES = 40;


    //-------------------------------------------------
    // Funcions generals de pintat per objectes

    void pintaTerra();
    void pintaSuport(float d, float angle);
    void pintaBola(float d, float angle);

    void pintaEstrella();
    //-------------------------------------------------
    //  Creació de buffers
    void creaBuffers();
    void creaBufferBola();
    void creaBufferTerra();
    void creaBufferSuport();
    void creaBufferEstrella();

    //-------------------------------------------------
    // Funcions de transformació pels objectes
    void transformacioBola(float d, float angle);
    void transformacioSuport(float d, float angle);
    void transformacioTerra();

    //-------------------------------------------------
    void carregaShaders ();
    void createVBOs( int size, glm::vec3 Colors[], glm::vec3 Vertices[] );
    //-------------------------------------------------
    // attribute locations
    GLuint vertexLoc, colorLoc;
    // uniforms
    GLuint transLoc, vimetLoc;

    
    // Program
    QOpenGLShaderProgram *program;

    GLuint VAO_TERRA, VAO_SUPORT, VAO_BOLA, VAO_ESTRELLA;
    GLint ample, alt;
    glm::vec3 COLOR_BLANC=glm::vec3(1.0,1.0,1.0);
    glm::vec3 COLOR_VERMELL=glm::vec3(1.0,0.5,0.3);
    glm::vec3 COLOR_BLAU=glm::vec3(48.0/255,58.0/255,158.0/255);
    glm::vec3 COLOR_BLAU_FLUIX=glm::vec3(109.0/255,117.0/255,201.0/255);
    glm::vec3 COLOR_GROC=glm::vec3(239.0/255,251.0/255,38.0/255);
    glm::vec3 COLOR_MARRO=glm::vec3(153.0/255,121.0/255,52.0/255);

    float angleMan1, angleMan2;

};
#endif
