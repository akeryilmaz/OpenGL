#include "helper.h"
#include "glm/glm.hpp"
#include "glm/gtc/matrix_transform.hpp"
#include "glm/gtc/type_ptr.hpp"
#include <vector>

static GLFWwindow * window = NULL;

GLuint idProgramShader;
GLuint idFragmentShader;
GLuint idVertexShader;
GLuint idJpegTexture;
GLuint idPMatrix;
GLuint VertexBuffer, VertexAttributes;
GLuint cameraPositionLocation;
GLuint heightFactorLocation;
GLuint MVLocation;
GLuint PLocation;
GLuint heightLocation;
GLuint widthLocation;

glm::vec3 cameraPosition(0.0,0.0,0.0);
glm::vec3 cameraGaze(0.0,0.0,1.0);
glm::vec3 cameraUp(0.0,1.0,0.0);
float cameraVelocity = 0;
GLfloat heightFactor = 10.0;
float pitch = 0;
float yaw = 90.0f;

int widthTexture, heightTexture;
vector<float> vertices;

static void errorCallback(int error,
  const char * description) {
  fprintf(stderr, "Error: %s\n", description);
}

void processInput(GLFWwindow *window)
{
  if(glfwGetKey(window, GLFW_KEY_O) == GLFW_PRESS)
    heightFactor += 0.5;
  if(glfwGetKey(window, GLFW_KEY_L) == GLFW_PRESS)
    heightFactor -= 0.5;
  if(glfwGetKey(window, GLFW_KEY_U) == GLFW_PRESS)
    cameraVelocity += 0.5;
  if(glfwGetKey(window, GLFW_KEY_J) == GLFW_PRESS)
    cameraVelocity -= 0.5;
  if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS){
    pitch += 0.05;
    glm::vec3 newGaze;
    newGaze.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    newGaze.y = sin(glm::radians(pitch));
    newGaze.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    cameraGaze = glm::normalize(newGaze);
  }
  if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS){
    pitch -= 0.05;
    glm::vec3 newGaze;
    newGaze.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    newGaze.y = sin(glm::radians(pitch));
    newGaze.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    cameraGaze = glm::normalize(newGaze);
  }
  if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS){
    yaw += 0.05;
    glm::vec3 newGaze;
    newGaze.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    newGaze.y = sin(glm::radians(pitch));
    newGaze.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    cameraGaze = glm::normalize(newGaze);
  }
  if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS){
    yaw -= 0.05;
    glm::vec3 newGaze;
    newGaze.x = cos(glm::radians(pitch)) * cos(glm::radians(yaw));
    newGaze.y = sin(glm::radians(pitch));
    newGaze.z = cos(glm::radians(pitch)) * sin(glm::radians(yaw));
    cameraGaze = glm::normalize(newGaze);
  }
  if(glfwGetKey(window, GLFW_KEY_F) == GLFW_PRESS)
    GLFWwindow* window = glfwCreateWindow(widthTexture, heightTexture, "CENG477 - HW3", glfwGetPrimaryMonitor(), NULL);
}

int main(int argc, char * argv[]) {

  if (argc != 2) {
    printf("Only one texture image expected!\n");
    exit(-1);
  }

  glfwSetErrorCallback(errorCallback);

  if (!glfwInit()) {
    exit(-1);
  }

  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

  initShaders();
  initTexture(argv[1], & widthTexture, & heightTexture);

  window = glfwCreateWindow(600, 600, "CENG477 - HW3", NULL, NULL);

  if (!window) {
    glfwTerminate();
    exit(-1);
  }
  glfwMakeContextCurrent(window);

  GLenum err = glewInit();
  if (err != GLEW_OK) {
    fprintf(stderr, "Error: %s\n", glewGetErrorString(err));

    glfwTerminate();
    exit(-1);
  }


  vertices.resize(18*widthTexture*heightTexture); //SHOULD BE INITIALIZED

  for(int i=0; i<heightTexture; i++){
    for(int j=0; j<widthTexture; j++){

      vertices[18*(i*widthTexture+j)] = j;
      vertices[18*(i*widthTexture+j)+1] = 0;
      vertices[18*(i*widthTexture+j)+2] = i;

      vertices[18*(i*widthTexture+j)+3] = j+1;
      vertices[18*(i*widthTexture+j)+4] = 0;
      vertices[18*(i*widthTexture+j)+5] = i;

      vertices[18*(i*widthTexture+j)+6] = j;
      vertices[18*(i*widthTexture+j)+7] = 0;
      vertices[18*(i*widthTexture+j)+8] = i+1;

      vertices[18*(i*widthTexture+j)+9] = j+1;
      vertices[18*(i*widthTexture+j)+10] = 0;
      vertices[18*(i*widthTexture+j)+11] = i;

      vertices[18*(i*widthTexture+j)+12] = j+1;
      vertices[18*(i*widthTexture+j)+13] = 0;
      vertices[18*(i*widthTexture+j)+14] = i+1;

      vertices[18*(i*widthTexture+j)+15] = j;
      vertices[18*(i*widthTexture+j)+16] = 0;
      vertices[18*(i*widthTexture+j)+17] = i+1;
    }
  }

  glGenBuffers(1, &VertexBuffer);
  glGenVertexArrays(1, &VertexAttributes);
  glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
  glBindVertexArray(VertexAttributes);
  glBufferData(GL_ARRAY_BUFFER, vertices.size()*sizeof(float), vertices.data(), GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);

  glViewport(0, 0, widthTexture, heightTexture);
  glEnable(GL_DEPTH_TEST);

  cameraPosition = glm::vec3((float)widthTexture/2, (float)widthTexture/10, -(float)widthTexture/4);
  glm::mat4 view = glm::mat4(1.0f);
  glm::mat4 projection;
  projection = glm::perspective(glm::radians(45.0f), 1.0f, 0.1f, 1000.0f);

  glUseProgram(idProgramShader);

  MVLocation = glGetUniformLocation(idProgramShader, "MV");
  glUniformMatrix4fv(MVLocation, 1, GL_FALSE, glm::value_ptr(view));
  PLocation = glGetUniformLocation(idProgramShader, "P");
  glUniformMatrix4fv(PLocation, 1, GL_FALSE, glm::value_ptr(projection));

  cameraPositionLocation = glGetUniformLocation(idProgramShader, "cameraPosition");
  glUniform3f(cameraPositionLocation, cameraPosition.x, cameraPosition.y, cameraPosition.z);
  heightFactorLocation = glGetUniformLocation(idProgramShader, "heightFactor");
  glUniform1f(heightFactorLocation, heightFactor);
  heightLocation = glGetUniformLocation(idProgramShader, "heightTexture");
  glUniform1i(heightLocation, heightTexture);
  widthLocation = glGetUniformLocation(idProgramShader, "widthTexture");
  glUniform1i(widthLocation, widthTexture);

  while (!glfwWindowShouldClose(window)) {

    processInput(window);

    cameraPosition += cameraGaze * cameraVelocity;

    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);

    glUseProgram(idProgramShader);

    glm::mat4 view = glm::mat4(1.0f);
    view = glm::lookAt(cameraPosition, cameraPosition + cameraGaze, cameraUp);
    glUniformMatrix4fv(MVLocation, 1, GL_FALSE, glm::value_ptr(view));
    glUniform3f(cameraPositionLocation, cameraPosition.x, cameraPosition.y, cameraPosition.z);
    glUniform1f(heightFactorLocation, heightFactor);
    glUniform1i(heightLocation, heightTexture);
    glUniform1i(widthLocation, widthTexture);

    glDrawArrays(GL_TRIANGLES, 0, 18*widthTexture*heightTexture);

    glfwSwapBuffers(window);
    glfwPollEvents();

  }

  glDeleteVertexArrays(1, &VertexAttributes);
  glDeleteBuffers(1, &VertexBuffer);

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
