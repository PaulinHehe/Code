#ifndef DISPLAYOPENGL_CPP
#define DISPLAYOPENGL_CPP

#ifdef WIN32
  #define WIN32_LEAN_AND_MEAN
  #include "windows.h"
#endif
#if defined(_unix) || defined(unix) || defined(_UNIX_) || defined(UNIX)
  #define UNIX
  #include "termios.h"
  #include "unistd.h"
  #include <fcntl.h>
  using namespace std;
#endif

#include <stdio.h>
#include <random>

#include "textRendering.h"
#include "recuit_simule.h"

class RecuitSimule;


void dispOpenGL(Graphe& G, bool useReelCoord=false);

void openGLShowEverything(const Graphe& G);
void openGLShowCells(const Graphe& G);
void openGLShowNodes(const Graphe& G);
void openGLShowEmplacements(const Graphe& G);
void openGLShowEdges(const Graphe& G);
void openGLShowBoiteEnglobante();
void openGLShowTargetCircles(const Graphe& G);
void drawCircle(float cx, float cy, double r, int num_segments=15);

void init();
void initRepere();
void initVariablesGraphe(Graphe& G);

void repereOrtho(Graphe &G);
void calculNouveauRepereZoomFocal(double cX, double cY);

Position<double> getPositionRepereInitialFromPixels(double clicX, double clicY);
Position<double> getPositionRepereActuelFromPixels(double clicX, double clicY);
Position<double> getPositionfromRepereActuelToInitial(double pX, double pY);
Position<double> getPositionOrtho(double x, double y);

void cursorDansPerimetreEmplacement(Graphe &G);
void cursorDansPerimetreNoeud(Graphe &G);
void saveLastMove();
void rewindSavedMove(Graphe& G);
void forwardSavedMove(Graphe& G);
void actionUtilisateurSurLeGraphe(Graphe& G);

void parseTextBuffer();
int parseTextBufferResult();

GLFWwindow* initAffichage(Graphe& G);
GLFWwindow* initTextRender();
GLFWwindow* initFenetre(int w, int h, const char* name);
void initFrame(GLFWwindow*& window);
void initFrameTextRender(GLFWwindow*& window);
void endAffichage(GLFWwindow*& window, GLFWwindow*& textRender);
void frameAffichage(GLFWwindow*& window, GLFWwindow*& textRender, Graphe& G);
void frameModif(GLFWwindow*& window, GLFWwindow*& textRender, Graphe& G);
void keyPressCheck(GLFWwindow*& window, GLFWwindow*& textRender, Graphe& G, RecuitSimule* r);

#endif