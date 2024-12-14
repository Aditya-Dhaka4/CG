#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

#define PI 3.14159265

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

  std::cout << "Resolution: " << videoMode->width << "*" << videoMode->height
            << std::endl;

  GLFWwindow *window = glfwCreateWindow(videoMode->width, videoMode->height,
                                        "Nepal Tourism Board Logo", NULL, NULL);
  if (!window) {
    glfwTerminate();
    std::cerr << "Failed to create GLFW window" << std::endl;
  }

  return window;
}

void drawCircle(float radius, int segments) {
  glBegin(GL_TRIANGLE_FAN);
  glVertex2f(0, 0); // Center of the circle
  for (int i = 0; i <= segments; ++i) {
    float theta = 2.0f * PI * float(i) / float(segments);
    float x = radius * cosf(theta);
    float y = radius * sinf(theta);
    glVertex2f(x, y);
  }
  glEnd();
}

void drawRect(float x, float y, float width, float height) {
  glBegin(GL_QUADS);
  glVertex2f(x - width / 2, y - height / 2);
  glVertex2f(x + width / 2, y - height / 2);
  glVertex2f(x + width / 2, y + height / 2);
  glVertex2f(x - width / 2, y + height / 2);
  glEnd();
}

void drawArc(float radius, float startAngle, float endAngle, int segments) {
  glLineWidth(50.0f);       // Set the line width
  glEnable(GL_LINE_SMOOTH); // Enable anti-aliasing for smoother lines
  glHint(GL_LINE_SMOOTH_HINT, GL_NICEST); // Set the quality of anti-aliasing

  glBegin(GL_LINE_STRIP);
  for (int i = 0; i <= segments; ++i) {
    float angle = startAngle + (endAngle - startAngle) * i / segments;
    float x = radius * cos(angle);
    float y = radius * sin(angle);
    glVertex2f(x, y); // Set the vertex of the arc
  }
  glEnd();
}

void drawFilledArc(float outerRadius, float innerRadius, float startAngle,
                   float endAngle, int segments) {
  glBegin(GL_TRIANGLE_STRIP); // Use triangle strip to fill between inner and
                              // outer arcs
  for (int i = 0; i <= segments; ++i) {
    // Interpolate the angle from startAngle to endAngle
    float angle = startAngle + (endAngle - startAngle) * i / segments;

    // Outer arc point (larger radius)
    float outerX = outerRadius * cosf(angle);
    float outerY = outerRadius * sinf(angle);
    glVertex2f(outerX, outerY);

    // Inner arc point (smaller radius)
    float innerX = innerRadius * cosf(angle);
    float innerY = innerRadius * sinf(angle);
    glVertex2f(innerX, innerY);
  }
  glEnd();
}

void drawLogo() {
  glColor3f(1.0f, 0.0f, 0.0f); // Red color
  glColor3f(1.0f, 0.0f, 0.0f); // White for the "N" shape

  drawRect(-60.0f, 0.0f, 50.0f, 200.0f); // Draw the vertical line of "N"
  drawRect(60.0f, 0.0f, 50.0f, 200.0f);  // Draw the vertical line of "N"
  drawRect(0.0f, 0.0f, 360.0f, 40.0f);   // Draw the horizontal line of "N"

  // Draw the diagonal line of "N"
  // drawDiagonal(-60.0f, 0.0f, 40.0f, 200.0f);
  glColor3f(1.0f, 0.0f, 0.0f);
  glBegin(GL_QUADS);
  glVertex2f(-85.0f, 100.0f);
  glVertex2f(-35.0f, 100.0f);
  glVertex2f(85.0f, -100.0f);
  glVertex2f(35.0f, -100.0f);
  glEnd();

  // drawArc(150.0f, PI / 6, PI, 500);
  // drawArc(150.0f, 7 * PI / 6, 2 * PI, 500);

  // Draw the Arcs
  drawFilledArc(180.0f, 150.0f, PI / 6, PI, 500);
  drawFilledArc(180.0f, 150.0f, 7 * PI / 6, 2 * PI, 500);
}

int main() {
  GLFWwindow *window = initializeGLFW();
  glfwMakeContextCurrent(window);
  glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  int width, height;
  glfwGetWindowSize(window, &width, &height);
  glOrtho(-width / 2.0, width / 2.0, -height / 2.0, height / 2.0, -1.0, 1.0);

  glMatrixMode(GL_MODELVIEW);
  glLoadIdentity();

  while (!glfwWindowShouldClose(window)) {
    glClear(GL_COLOR_BUFFER_BIT);

    // Draw the Nepal Tourism Board logo
    drawLogo();

    glfwSwapBuffers(window);
    glfwPollEvents();
  }

  glfwDestroyWindow(window);
  glfwTerminate();
  return 0;
}
