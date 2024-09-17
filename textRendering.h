#ifndef TEXTRENDERING_CPP
#define TEXTRENDERING_CPP


#if defined(_unix) || defined(unix) || defined(_UNIX_) || defined(UNIX)
#include <freetype2/ft2build.h>
#endif
#ifdef WIN32
#include <ft2build.h>
#endif

#include FT_FREETYPE_H  

#define GLEW_STATIC
#include "glew.h"

#include <GLFW/glfw3.h>
#include <GL/gl.h>

#include <map>

#include "glm/glm.hpp" 
#include "glm/gtc/matrix_transform.hpp" 
#include "glm/gtc/type_ptr.hpp"

#include <string>

#include <shader.h>

#define GL_CLAMP_TO_EDGE                        0x812F
#define GL_CLAMP                                0x2900
#define GL_CLAMP_TO_BORDER                      0x812D



void framebuffer_size_callback(GLFWwindow* window, int width, int height);
void processInput(GLFWwindow *window);
void RenderText(Shader &shader, std::string text, float x, float y, float scale, glm::vec3 color);

void displayTextModif(GLFWwindow*& window, Shader* &shader, const std::string& newTemp, const std::string& temp, const std::string& tempDep);
void displayTextAffichage(GLFWwindow*& window, Shader* &shader,  const std::string& temp, const std::string& tempDep);
int textRenderingInit(GLFWwindow* window, Shader* &shaderptr);
void testTextRendering();
int loadFont();
int initLaLesTrucs();
Shader shaderSection();

void RenderText(Shader* &shader, std::string text, float x, float y, float scale, glm::vec3 color);

#endif