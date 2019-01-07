#include "helper.h"

static GLFWwindow * window = NULL;

// Shaders
GLuint idProgramShader;
GLuint idFragmentShader;
GLuint idVertexShader;
GLuint idJpegTexture;
GLuint idMVPMatrix;
GLuint VertexBuffer;
GLuint VertexAttributes;
GLuint cameraPositionLocation;
GLuint heightFactorLocation;

int widthTexture, heightTexture;

static void errorCallback(int error,
  const char * description) {
  fprintf(stderr, "Error: %s\n", description);
}

void processInput(GLFWwindow *window)
{
   // if(glfwGetKey(window, GLFW_KEY_ESCAPE) == GLFW_PRESS)
     //   glfwSetWindowShouldClose(window, true);
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

  //glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
  //glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
  //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_COMPAT_PROFILE);

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

  initShaders();
  initTexture(argv[1], & widthTexture, & heightTexture);

  float vertices[6*widthTexture*heightTexture]; //SHOULD BE INITIALIZED

  for(int i=0; i<heightTexture; i++){
    for(int j=0; j<widthTexture; j++){
      std::cout << 6*(i*widthTexture+j) <<  std::endl;
      
      vertices[6*(i*widthTexture+j)] = j;
      vertices[6*(i*widthTexture+j)+1] = 0;
      vertices[6*(i*widthTexture+j)+2] = i;

      vertices[6*(i*widthTexture+j)+3] = j+1;
      vertices[6*(i*widthTexture+j)+4] = 0;
      vertices[6*(i*widthTexture+j)+5] = i;

      vertices[6*(i*widthTexture+j)+6] = j;
      vertices[6*(i*widthTexture+j)+7] = 0;
      vertices[6*(i*widthTexture+j)+8] = i+1;

      vertices[6*(i*widthTexture+j)+9] = j+1;
      vertices[6*(i*widthTexture+j)+10] = 0;
      vertices[6*(i*widthTexture+j)+11] = i;

      vertices[6*(i*widthTexture+j)+12] = j+1;
      vertices[6*(i*widthTexture+j)+13] = 0;
      vertices[6*(i*widthTexture+j)+14] = i+1;

      vertices[6*(i*widthTexture+j)+15] = j;
      vertices[6*(i*widthTexture+j)+16] = 0;
      vertices[6*(i*widthTexture+j)+17] = i+1;
    }
  }

  std::cout << vertices << std::endl;

  GLfloat cameraPosition[3] = {(float)widthTexture/2, (float)widthTexture/10, (float)-widthTexture/4};
  float cameraGaze[3] = {0,0,1};
  float cameraVelocity[3] = {0};
  GLfloat heightFactor = 10.0;

  glGenBuffers(1, &VertexBuffer);
  glGenVertexArrays(1, &VertexAttributes);
  glBindBuffer(GL_ARRAY_BUFFER, VertexBuffer);
  glBindVertexArray(VertexAttributes);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);  
  glBindBuffer(GL_ARRAY_BUFFER, 0);
  glBindVertexArray(0); 

  glUseProgram(idProgramShader);
  cameraPositionLocation = glGetUniformLocation(idProgramShader, "cameraPosition");
  glUniform3f(cameraPositionLocation, cameraPosition[0], cameraPosition[1], cameraPosition[2]);
  heightFactorLocation = glGetUniformLocation(idProgramShader, "heightFactor");
  glUniform1f(heightFactorLocation, heightFactor);

  while (!glfwWindowShouldClose(window)) {
    
    processInput(window);

    glClear(GL_COLOR_BUFFER_BIT);
    glClear(GL_DEPTH_BUFFER_BIT);
    
    glUseProgram(idProgramShader);
    glUniform3f(cameraPositionLocation, cameraPosition[0], cameraPosition[1], cameraPosition[2]);
    glUniform1f(heightFactorLocation, heightFactor);
    glDrawArrays(GL_TRIANGLES, 0, 6*widthTexture*heightTexture);

    glfwSwapBuffers(window);
    glfwPollEvents();

  }

  glDeleteVertexArrays(1, &VertexAttributes);
  glDeleteBuffers(1, &VertexBuffer);

  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}