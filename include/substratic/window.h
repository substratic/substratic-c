#ifndef _SUBSTRATIC_WINDOW_H
#define _SUBSTRATIC_WINDOW_H

#include <SDL2/SDL.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

typedef void* SubstraticWindow;

void _subst_window_resize_callback(GLFWwindow* window, int width, int height)
{
  // TODO: Raise an event somehow?
  printf("Window size -> %d / %d\n", width, height);
  glViewport(0, 0, width, height);
}

void _subst_glfw_error_callback(int code, const char *error) {
  if (code != 65537) {
    printf("An error occurred: [%d] %s\n", code, error);
    /* exit(1); */
  }
}

SubstraticWindow subst_window_create(const char *title, uint width, uint height) {
  // TODO: Init should go somewhere else...
  glfwInit();
  glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
  glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
  glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
  glfwWindowHint(GLFW_RESIZABLE, GLFW_FALSE);

#ifdef __APPLE__
  glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);
#endif

  glfwSetErrorCallback(_subst_glfw_error_callback);

  GLFWwindow* window = glfwCreateWindow(width, height, title, NULL, NULL);
  if (window == NULL)
  {
    // TODO: Panic
    printf("Can't create window!\n");
    glfwTerminate();
    return NULL;
  }

  glfwMakeContextCurrent(window);
  glfwSetFramebufferSizeCallback(window, _subst_window_resize_callback);

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
