#ifndef _SUBSTRATIC_WINDOW_H
#define _SUBSTRATIC_WINDOW_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

typedef void* SubstraticWindow;

void _subst_window_resize_callback(GLFWwindow* window, int width, int height)
{
  // TODO: Raise an event somehow?
  printf("Window size -> %d / %d\n", width, height);
  glViewport(0, 0, width, height);
}

typedef void (* SubstraticWindowSizeCallback)(SubstraticWindow,int,int);

void subst_window_resize_callback_set(SubstraticWindow window, SubstraticWindowSizeCallback callback) {
  glfwSetFramebufferSizeCallback((GLFWwindow *)window, (GLFWwindowsizefun)callback);
}

SubstraticWindow subst_window_create(const char *title, uint width, uint height) {
  GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
  if (window == NULL)
  {
    // TODO: Panic
    printf("Can't create window!\n");
    glfwTerminate();
    return NULL;
  }

  glfwMakeContextCurrent(window);

  if (!gladLoadGLLoader((GLADloadproc)glfwGetProcAddress)) {
    // TODO: Panic
    printf("Can't load GLAD\n");
    exit(1);
  }

  return window;
}

int subst_window_should_close(SubstraticWindow window) {
  return glfwWindowShouldClose((GLFWwindow *)window);
}

#endif
