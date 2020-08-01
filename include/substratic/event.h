#ifndef _SUBSTRATIC_EVENT_H
#define _SUBSTRATIC_EVENT_H

#include <GLFW/glfw3.h>

void subst_event_pump() {
  glfwPollEvents();
}

#endif
