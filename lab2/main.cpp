#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <cmath>
#include <iostream>

#define PI 3.14159265

// Initialize GLFW and create a window
GLFWwindow *initializeGLFW() {
    if (!glfwInit()) {
        std::cerr << "Failed to initialize GLFW" << std::endl;
        return nullptr;
    }

    GLFWmonitor *primaryMonitor = glfwGetPrimaryMonitor();
    if (!primaryMonitor) {
        std::cerr << "Failed to get primary monitor" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    const GLFWvidmode *videoMode = glfwGetVideoMode(primaryMonitor);
    if (!videoMode) {
        std::cerr << "Failed to get video mode" << std::endl;
        glfwTerminate();
        return nullptr;
    }

    GLFWwindow *window = glfwCreateWindow(videoMode->width, videoMode->height, "Line Drawing Algorithm", NULL, NULL);
    if (!window) {
        glfwTerminate();
        std::cerr << "Failed to create GLFW window" << std::endl;
        return nullptr;
    }

    return window;
}

// Bresenham Line Algorithm to plot points
void bresenham(int x1, int y1, int x2, int y2) {
    int dx = abs(x2 - x1);
    int dy = abs(y2 - y1);
    double m = static_cast<double>(dy) / dx;
    int dx2 = 2 * dx;
    int dy2 = 2 * dy;

    int sx = (x1 < x2) ? 1 : -1;
    int sy = (y1 < y2) ? 1 : -1;

    int x = x1, y = y1;

    glColor3f(0.0f, 0.0f, 0.0f);
    glBegin(GL_POINTS);

    if (m < 1) {
        int pk = dy2 - dx;
        while (x != x2) {
            glVertex2f(x, y);
            if (pk >= 0) {
                y += sy;
                pk -= dx2;
            }
            pk += dy2;
            x += sx;
        }
    } else {
        int pk = dx2 - dy;
        while (y != y2) {
            glVertex2f(x, y);
            if (pk >= 0) {
                x += sx;
                pk -= dy2;
            }
            pk += dx2;
            y += sy;
        }
    }

    glVertex2f(x, y);
    glEnd();
}

// DDA Line Algorithm to plot points
void DDA_line(int x1, int y1, int x2, int y2) {
    int dx = x2 - x1, dy = y2 - y1;
    int steps = std::max(abs(dx), abs(dy));
    float xInc = dx / float(steps), yInc = dy / float(steps);
    float x = x1, y = y1;

    glColor3f(0.0f, 0.0f, 0.0f);  // Set point color to black
    glBegin(GL_POINTS);  // Start drawing points

    for (int i = 0; i <= steps; i++) {
        glVertex2f(round(x), round(y));  // Plot the point
        x += xInc;
        y += yInc;
    }

    glEnd();  // End drawing points
}

int main() {
    int x0, y0, x1, y1, n;

    std::cout << "Enter starting points (x0, y0): ";
    std::cin >> x0 >> y0;  // Fixed input reading

    std::cout << "Enter ending points (x1, y1): ";
    std::cin >> x1 >> y1;  // Fixed input reading

    std::cout << "Choose algorithm:" << std::endl;
    std::cout << "1. DDA Algorithm" << std::endl;
    std::cout << "2. Bresenham Algorithm" << std::endl;
    std::cin >> n;

    // Initialize GLFW and create the window
    GLFWwindow *window = initializeGLFW();
    if (!window) return -1;

    glfwMakeContextCurrent(window);

    // Set background color to white
    glClearColor(1.0f, 1.0f, 1.0f, 1.0f);

    // Set up orthogonal projection
    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    int width, height;
    glfwGetWindowSize(window, &width, &height);
    glOrtho(-width / 2.0, width / 2.0, -height / 2.0, height / 2.0, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    // Main rendering loop
    while (!glfwWindowShouldClose(window)) {
        glClear(GL_COLOR_BUFFER_BIT);

        switch (n) {
        case 1:
            DDA_line(x0, y0, x1, y1);
            break;
        case 2:
            bresenham(x0, y0, x1, y1);
            break;
        default:
            glfwSetWindowShouldClose(window, GLFW_TRUE);
            break;
        }

        glfwSwapBuffers(window);
        glfwPollEvents();
    }

    glfwDestroyWindow(window);
    glfwTerminate();
    return 0;
}
