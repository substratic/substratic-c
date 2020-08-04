#ifndef _SUBSTRATIC_RENDER_H
#define _SUBSTRATIC_RENDER_H

#include <substratic/types.h>
#include <substratic/window.h>

#include <glad/glad.h>
#include <GLFW/glfw3.h>
#include <cglm/cglm.h>

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

uint subst_render_shader_compile(uint shaderType, const char *shaderSource) {
  uint shaderId;
  shaderId = glCreateShader(shaderType);
  glShaderSource(shaderId, 1, &shaderSource, NULL);
  glCompileShader(shaderId);

  int  success;
  char infoLog[512];
  glGetShaderiv(shaderId, GL_COMPILE_STATUS, &success);
  if (!success) {
    glGetShaderInfoLog(shaderId, 512, NULL, infoLog);
    // TODO: Panic
    printf("Shader compilation failed:\n%s\n", infoLog);
    exit(1);
  }

  return shaderId;
}

uint subst_render_shader_program_link(const uint* shaderIds, uint shaderCount) {
  unsigned int shaderProgram;
  shaderProgram = glCreateProgram();

  for (uint i = 0; i < shaderCount; i++) {
    glAttachShader(shaderProgram, shaderIds[i]);
  }

  glLinkProgram(shaderProgram);

  return shaderProgram;
}

void subst_render_shader_use(uint shaderProgramId) {
  glUseProgram(shaderProgramId);
}

void subst_render_shader_mat4_set(uint shaderProgramId, const char* uniformName, mat4 matrix) {
  unsigned int uniformLoc = glGetUniformLocation(shaderProgramId, uniformName);
  if (uniformLoc == -1) {
    // TODO: Panic
    printf("Could not find shader matrix parameter: %s\n", uniformName);
    exit(1);
  }

  glUniformMatrix4fv(uniformLoc, 1, GL_FALSE, matrix[0]);
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

  // Set the default clear color
  subst_render_clear_color_set(0.0f, 0.0f, 0.0f);

  // TODO: Better return value
  return 0;
}

#endif
