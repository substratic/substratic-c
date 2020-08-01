#include <stdio.h>

#include <substratic/window.h>
#include <substratic/render.h>
#include <substratic/event.h>
#include <substratic/keyboard.h>

int main() {

  printf("Starting up.\n");

  SubstraticWindow window = subst_window_create("Substratic C Test", 1280, 720);

  // Initialize the renderer
  /* subst_render_init_2d(window); */

  uint fpsFrames = 0;
  double lastFpsPrint = glfwGetTime();

  float rectX = 0.0f, rectY = 0.0f;
  while (!subst_window_should_close(window)) {
    subst_event_pump();

    // TODO: Move this to a system
    if (subst_keyboard_key_state(window, KEY_ESCAPE)) {
      break;
    }

    subst_render_clear();

    subst_render_swap(window);

    fpsFrames++;
    double currentTime = glfwGetTime();
    if (currentTime - lastFpsPrint > 1.0) {
      printf("%f ms\n", 1000.0/(double)fpsFrames);
      fpsFrames = 0;
      lastFpsPrint = currentTime;
    }
  }

  printf("Exiting...\n\n");

  // TODO: Clean up GL context and window
}
