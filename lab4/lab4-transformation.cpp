#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <thread>
#include <chrono> 
#include <vector>

using namespace std;

class Transformation {
public:
    static GLFWwindow *initializeGLFW() {
      if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
      }
      GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
      if (!primaryMonitor) {
        std::cerr << "Failed to get primary monitor" << std::endl;
        glfwTerminate();
      }
      const GLFWvidmode *videoMode = glfwGetVideoMode(primaryMonitor);
      if (!videoMode) {
        std::cerr << "Failed to get video mode of the primary monitor" << std::endl;
        glfwTerminate();
      }
      GLFWwindow *window = glfwCreateWindow(videoMode->width, videoMode->height,
                                            "2D Transformation", NULL, NULL);
      if (!window) {
        glfwTerminate();
        std::cerr << "Failed to create GLFW window" << std::endl;
      }
      return window;
    }

    static void plotPoints(vector<vector<float>> points) {
      glBegin(GL_POLYGON);
      for (auto& point : points) {
          glVertex2f(point[0], point[1]);
      }
      glEnd();
    }

    // Matrix multiplication
    static void applyTransformation(vector<vector<float>>& points, float transformationMatrix[3][3]){
      for (auto& point : points) {
          vector<float> result(3, 0);
          for (int i = 0; i < 3; i++) {
              for (int j = 0; j < 3; j++) {
                  result[i] += transformationMatrix[i][j] * point[j];
              }
          }
          point = result;
      }
    }

    // Translation matrix
    static void translate(float matrix[3][3], float tx, float ty) {
        matrix[0][0] = 1; matrix[0][1] = 0; matrix[0][2] = tx;
        matrix[1][0] = 0; matrix[1][1] = 1; matrix[1][2] = ty;
        matrix[2][0] = 0; matrix[2][1] = 0; matrix[2][2] = 1;
    }

    // Scaling matrix
    static void scale(float matrix[3][3], float sx, float sy) {
        matrix[0][0] = sx; matrix[0][1] = 0;  matrix[0][2] = 0;
        matrix[1][0] = 0;  matrix[1][1] = sy; matrix[1][2] = 0;
        matrix[2][0] = 0;  matrix[2][1] = 0;  matrix[2][2] = 1;
    }

    // Rotation matrix
    static void rotate(float matrix[3][3], float angle) {
        float rad = angle * M_PI / 180;
        matrix[0][0] = cos(rad); matrix[0][1] = -sin(rad); matrix[0][2] = 0;
        matrix[1][0] = sin(rad); matrix[1][1] = cos(rad);  matrix[1][2] = 0;
        matrix[2][0] = 0;        matrix[2][1] = 0;         matrix[2][2] = 1;
    }

};

void windmill() {
    // Define the blades of the windmill as triangles in homogeneous coordinates
    vector<vector<float>> blade1 = { {0, 0, 1}, {20, 100, 1}, {-20, 100, 1} };
    vector<vector<float>> blade2 = { {0, 0, 1}, {20, -100, 1}, {-20, -100, 1} };
    vector<vector<float>> blade3 = { {0, 0, 1}, {100, 20, 1}, {100, -20, 1} };
    vector<vector<float>> blade4 = { {0, 0, 1}, {-100, 20, 1}, {-100, -20, 1} };

    float angle = 0.0f; // Initial angle

    while (!glfwWindowShouldClose(glfwGetCurrentContext())) {
        // Clear the screen
        glClear(GL_COLOR_BUFFER_BIT);

        // Create the rotation matrix for the current angle
        float rotationMatrix[3][3];
        Transformation::rotate(rotationMatrix, angle);

        // Apply the rotation to each blade
        Transformation::applyTransformation(blade1, rotationMatrix);
        Transformation::applyTransformation(blade2, rotationMatrix);
        Transformation::applyTransformation(blade3, rotationMatrix);
        Transformation::applyTransformation(blade4, rotationMatrix);

        // Draw each blade
        glColor3f(1.0f, 0.0f, 0.0f); // Red color for blade 1
        Transformation::plotPoints(blade1);

        glColor3f(0.0f, 1.0f, 0.0f); // Green color for blade 2
        Transformation::plotPoints(blade2);

        glColor3f(0.0f, 0.0f, 1.0f); // Blue color for blade 3
        Transformation::plotPoints(blade3);

        glColor3f(1.0f, 1.0f, 0.0f); // Yellow color for blade 4
        Transformation::plotPoints(blade4);

        angle += 1.0f;
        if (angle >= 360.0f) {
            angle = 0.0f;
        }

        glfwSwapBuffers(glfwGetCurrentContext());
        glfwPollEvents();

        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
}


int main() {
  GLFWwindow *window = Transformation::initializeGLFW();
  glfwMakeContextCurrent(window);

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  int width, height;
  glfwGetWindowSize(window, &width, &height);
  glOrtho(-width / 2.0, width / 2.0, -height / 2.0, height / 2.0, -1.0, 1.0);

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    vector<vector<float>> points = {{0, 0, 1},{100,10,1},{10,100,1}}; 

    glColor3f(1.0f, 0.0f, 0.0f); 
    // Transformation::plotPoints(points);
    //
    // // Translation
    // float tranformMatrix[3][3];
    // Transformation::scale(tranformMatrix,2,2);
    // Transformation::applyTransformation(points,tranformMatrix);
    //
    //
    // // Draw the translated point
    // glBegin(GL_POLYGON);
    // glColor3f(0.0f, 1.0f, 0.0f); // Red color
    // Transformation::plotPoints(points);
    windmill();

    glfwSwapBuffers(window);
    // Poll for and process events
    glfwPollEvents();
  }

  // Clean up and close the window
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}

