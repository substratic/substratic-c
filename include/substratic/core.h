#ifndef _SUBSTRATIC_CORE_H
#define _SUBSTRATIC_CORE_H

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void _subst_glfw_error_callback(int code, const char *error) {
  if (code != 65537) {
    printf("An error occurred: [%d] %s\n", code, error);
    /* exit(1); */
  }
}

void subst_init() {
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
}

void subst_exit() {
  glfwTerminate();
}

#endif
