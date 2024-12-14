#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

GLFWwindow *initializeGLFW() {
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
                                        "Circle and Ellipse", NULL, NULL);
  if (!window) {
    glfwTerminate();
    std::cerr << "Failed to create GLFW window" << std::endl;
  }

  return window;
}

void draw8SymmetricPoints(int x, int y) {
  glVertex2i(x, y);
  glVertex2i(-x, y);
  glVertex2i(x, -y);
  glVertex2i(-x, -y);
  glVertex2i(y, x);
  glVertex2i(-y, x);
  glVertex2i(y, -x);
  glVertex2i(-y, -x);
}

void midPointCircle(int x_center, int y_center, int radius) {
  glBegin(GL_POINTS);
  glColor3f(0.0f, 0.0f, 1.0f);
  glPointSize(5.0f);

  int x = 0, y = radius;
  int pk = 1 - radius;
  glVertex2i(x, y);

  while (!(x >= y)) {
    if (pk < 0) {
      x = x + 1;
      y = y;
      pk = pk + 2 * x + 1;
      draw8SymmetricPoints(x + x_center, y + y_center);
    } else {
      x = x + 1;
      y = y - 1;
      pk = pk + 2 * x - 2 * y + 1;
      draw8SymmetricPoints(x + x_center, y + y_center);
    }
  }
  glEnd();
}

void draw4SymmetricPoints(int x, int y) {
  glVertex2i(x, y);
  glVertex2i(-x, y);
  glVertex2i(x, -y);
  glVertex2i(-x, -y);
}

void midPointEllipse(int x_center, int y_center, int rx, int ry) {
  glBegin(GL_POINTS);
  glColor3f(0.0f, 0.0f, 0.0f);
  glPointSize(5.0f);

  int a = rx, b = ry;
  int x = 0, y = b;
  float pk = b * b - a * a * b + 0.25 * a * a;
  glVertex2i(x + x_center, y + y_center);

  while (2 * b * b * x < 2 * a * a * y) {
    if (pk < 0) {
      x = x + 1;
      y = y;
      pk = pk + 2 * b * b * x + b * b;
    } else {
      x = x + 1;
      y = y - 1;
      pk = pk + 2 * b * b * x - 2 * a * a * y + b * b;
    }
    draw4SymmetricPoints(x + x_center, y + y_center);
  }

  pk =
      b * b * (x + 0.5) * (x + 0.5) + a * a * (y - 1) * (y - 1) - a * a * b * b;
  while (y > 0) {
    if (pk > 0) {
      x = x;
      y = y - 1;
      pk = pk - 2 * a * a * y + a * a;
    } else {
      x = x + 1;
      y = y - 1;
      pk = pk + 2 * b * b * x - 2 * a * a * y + a * a;
    }
    draw4SymmetricPoints(x + x_center, y + y_center);
  }
  glEnd();
}

void polarCoordinateCircle(int x_center, int y_center, int radius) {
  glBegin(GL_POINTS);
  glColor3f(0.0f, 0.0f, 0.0f);
  glPointSize(5.0f);

  for (int i = 0; i < 360; i++) {
    float theta = i * 3.14159 / 180;
    int x = radius * cos(theta);
    int y = radius * sin(theta);
    glVertex2i(x + x_center, y + y_center);
  }
  glEnd();
}

int main() {
  // Initialize GLFW
  GLFWwindow *window = initializeGLFW();

  glfwMakeContextCurrent(window);
  int width, height;
  glfwGetWindowSize(window, &width, &height);

  glOrtho(-width / 2.0, width / 2.0, -height / 2.0, height / 2.0, -1.0, 1.0);

  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  // Main loop: keep running until the window is closed
  while (!glfwWindowShouldClose(window)) {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);
    // Swap front and back buffers (draw the contents)

    // midPointCircle(0, 0, 100);
     midPointEllipse(0, 0, 200, 100);
    //polarCoordinateCircle(0, 0, 100);

    glfwSwapBuffers(window);

    // Poll for and process events
    glfwPollEvents();
  }
  // Clean up and close the window
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
