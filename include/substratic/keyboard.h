#ifndef _SUBSTRATIC_KEYBOARD_H
#define _SUBSTRATIC_KEYBOARD_H

#include <substratic/window.h>
#include <GLFW/glfw3.h>

#define KEY_ESCAPE GLFW_KEY_ESCAPE

#define KEY_STATE_RELEASE  GLFW_RELEASE
#define KEY_STATE_PRESS    GLFW_PRESS
#define KEY_STATE_REPEAT   GLFW_REPEAT

uint8_t subst_keyboard_key_state(SubstraticWindow window, int keyCode) {
  return glfwGetKey((GLFWwindow *)window, keyCode);
}

#endif
