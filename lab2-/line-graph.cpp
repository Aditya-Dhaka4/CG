#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>
#include <vector>

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
                                        "Hello World", NULL, NULL);
  if (!window) {
    glfwTerminate();
    std::cerr << "Failed to create GLFW window" << std::endl;
  }
  return window;
}

// 0 -5 20 10
void drawLineDDA(float start_x, float start_y, float end_x, float end_y) {
  float slope, step_size, x_inc, y_inc;
  // Avoid division by zero
  if (end_x - start_x != 0) {
    slope = (float)(end_y - start_y) / (end_x - start_x);
    // std::cout << "The slope of the line is: " << slope << std::endl;
  } else {
    // std::cout << "The line is vertical; slope is undefined." << std::endl;
  }

  step_size = abs(end_x - start_x) > abs(end_y - start_y)
                  ? abs(end_x - start_x)
                  : abs(end_y - start_y);
  x_inc = (end_x - start_x) / step_size;
  y_inc = (end_y - start_y) / step_size;

  glColor3f(1.0f, 0.0f, 0.0f);
  // glPointSize(5.0f);

  glBegin(GL_POINTS);

  int i = 0;
  while (i != step_size) {
    // std::cout << start_x << ", " << start_y << std::endl;
    glVertex2f(start_x, start_y);
    start_x += x_inc;
    start_y += y_inc;
    i++;
  }

  // std::cout << start_x + x_inc << ", " << start_y + y_inc << std::endl;
  // std::cout << end_x << ", " << end_y << std::endl;

  glEnd();
}

void bresenham(int x1, int y1, int x2, int y2) {
  int dx = abs(x2 - x1);
  int dy = abs(y2 - y1);
  int sx = (x1 < x2) ? 1 : -1;
  int sy = (y1 < y2) ? 1 : -1;
  int x = x1, y = y1;

  // glPointSize(5.0f);
  glColor3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_POINTS);

  if (dx > dy) {
    // Slope < 1
    int pk = 2 * dy - dx;
    while (x != x2) {
      glVertex2i(x, y);
      if (pk >= 0) {
        y += sy;
        pk -= 2 * dx;
      }
      pk += 2 * dy;
      x += sx;
    }
  } else {
    // Slope >= 1
    int pk = 2 * dx - dy;
    while (y != y2) {
      glVertex2i(x, y);
      if (pk >= 0) {
        x += sx;
        pk -= 2 * dy;
      }
      pk += 2 * dx;
      y += sy;
    }
  }

  glVertex2i(x, y); // Plot the final point
  glEnd();
}

void drawLineGraph(const std::vector<int> &yValues, int xStart, int xStep) {
  for (size_t i = 0; i < yValues.size() - 1; ++i) {
    int x1 = xStart + i * xStep;
    int y1 = yValues[i];
    int x2 = xStart + (i + 1) * xStep;
    int y2 = yValues[i + 1];

    // Draw line between consecutive points
    bresenham(x1, y1, x2, y2);
  }
}

// Display callback for OpenGL
void displayLineGraph() {
  glClear(GL_COLOR_BUFFER_BIT);

  // Define some example y-values for the line graph
  std::vector<int> yValues = {10, 30, 25, 40, 20, 35, 50, 45};
  int xStart = 50; // Starting x-coordinate
  int xStep = 20;  // Horizontal distance between points

  glColor3f(0.0f, 0.0f, 0.0f); // Set color to black
  drawLineGraph(yValues, xStart, xStep);

  glFlush();
}

int main() {
  // Initialize GLFW
  GLFWwindow *window = initializeGLFW();
  glfwMakeContextCurrent(window);

  // Set the clear color (white background)
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  int width, height;
  glfwGetWindowSize(window, &width, &height);
  glOrtho(-width / 2.0, width / 2.0, -height / 2.0, height / 2.0, -1.0, 1.0);

  // Main render loop
  while (!glfwWindowShouldClose(window)) {
    // Clear the screen
    glClear(GL_COLOR_BUFFER_BIT);
    // bresenham(0, -5, 300, 400);
    displayLineGraph();
    glfwSwapBuffers(window);
    // Poll for and process events
    glfwPollEvents();
  }

  // Clean up and close the window
  glfwDestroyWindow(window);
  glfwTerminate();

  return 0;
}
