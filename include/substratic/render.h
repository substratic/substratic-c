#ifndef _SUBSTRATIC_RENDER_H
#define _SUBSTRATIC_RENDER_H

#include <substratic/types.h>
#include <substratic/window.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>

void subst_render_clear_color_set(float r, float g, float b) {
  glClearColor(r, g, b, 1.0f);
}

void subst_render_clear() {
  glClear(GL_COLOR_BUFFER_BIT);
}

void subst_render_swap(SubstraticWindow window) {
  glfwSwapBuffers((GLFWwindow *)window);
}

void subst_render_color_rgb(float r, float g, float b) {
  /* glColor3f(0, 1, 0); */
}

void subst_render_rect_line(float x, float y, float w, float h) {
  // TODO: In debug mode, check for quads mode?
  /* glVertex2f(x, y); */
  /* glVertex2f(x + w, y); */
  /* glVertex2f(x + w, y + h); */
  /* glVertex2f(x, y + h); */
}

int subst_render_init_2d(SubstraticWindow window) {
  // Enable blending
  glEnable(GL_BLEND);
  glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

  glEnable(GL_TEXTURE_2D);

  // TODO: Set up default vertex and fragment shaders

  // Set the default clear color
  subst_render_clear_color_set(0.0f, 0.0f, 0.0f);

  // TODO: Better return value
  return 0;
}

#endif
