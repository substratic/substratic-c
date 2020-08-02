#include <stdio.h>

#include <substratic/window.h>
#include <substratic/render.h>
#include <substratic/asset.h>
#include <substratic/event.h>
#include <substratic/keyboard.h>

int main() {

  printf("Starting up.\n");

  SubstraticWindow window = subst_window_create("Substratic C Test", 1280, 720);

  // Initialize the renderer
  subst_render_init_2d(window);

  const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "\n"
    "out vec3 theColor;\n"
    "out vec2 texCoord;\n"
    "\n"
    "void main()\n"
    "{\n"
    "  gl_Position = vec4(aPos, 1.0);\n"
    "  theColor = aColor;\n"
    "  texCoord = vec2(aTexCoord.x, aTexCoord.y);\n"
    "}\0";

  const char *fragmentShaderSource = "#version 330 core\n"
    "out vec4 FragColor;\n"
    "\n"
    "in vec3 theColor;\n"
    "in vec2 texCoord;\n"
    "\n"
    "uniform sampler2D theTexture;\n"
    "\n"
    "void main()\n"
    "{\n"
    "  FragColor = texture(theTexture, texCoord) * vec4(theColor, 1.0);\n"
    "}\0";

  // Compile vertex and fragment shaders into a program
  uint vertexShader = subst_render_shader_compile(GL_VERTEX_SHADER, vertexShaderSource);
  uint fragmentShader = subst_render_shader_compile(GL_FRAGMENT_SHADER, fragmentShaderSource);
  uint shaderIds[] = {vertexShader, fragmentShader};
  uint shaderProgram = subst_render_shader_program_link(shaderIds, 2);

  // Use the linked shader program
  glUseProgram(shaderProgram);

  // Clean up shaders, they've already been linked
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  float vertices[] = {
     // positions          // colors           // texture coords
      0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   // top right
      0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,   // bottom right
     -0.5f, -0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f,   // bottom left
     -0.5f,  0.5f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f    // top left
  };
  unsigned int indices[] = {  // note that we start from 0!
      0, 1, 3,   // first triangle
      1, 2, 3    // second triangle
  };

  unsigned int VAO, VBO, EBO;
  glGenVertexArrays(1, &VAO);
  glGenBuffers(1, &VBO);
  glGenBuffers(1, &EBO);

  glBindVertexArray(VAO);

  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, EBO);
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

  // Define how vertices are stored
  // position attribute
  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)0);
  glEnableVertexAttribArray(0);
  // color attribute
  glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(3 * sizeof(float)));
  glEnableVertexAttribArray(1);
  // texture coord attribute
  glVertexAttribPointer(2, 2, GL_FLOAT, GL_FALSE, 8 * sizeof(float), (void*)(6 * sizeof(float)));
  glEnableVertexAttribArray(2);

  // Load an image
  StcImage image = subst_asset_load_image("dist/assets/images/tile.png");

  // Load a texture
  unsigned int texture;
  glGenTextures(1, &texture);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
  glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
  glBindTexture(GL_TEXTURE_2D, texture);
  glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, image.width, image.height, 0, GL_RGBA, GL_UNSIGNED_BYTE, image.data);
  glPixelStorei(GL_UNPACK_ALIGNMENT, 1);
  glGenerateMipmap(GL_TEXTURE_2D);

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

    // TEMP: Draw the tile
    glBindTexture(GL_TEXTURE_2D, texture);
    glUseProgram(shaderProgram);
    glBindVertexArray(VAO);
    glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);

    subst_render_swap(window);

    fpsFrames++;
    double currentTime = glfwGetTime();
    if (currentTime - lastFpsPrint > 1.0) {
      printf("%f fps\n", 1000.0 / (1000.0/(double)fpsFrames));
      fpsFrames = 0;
      lastFpsPrint = currentTime;
    }
  }

  printf("Exiting...\n\n");

  // TODO: Clean up GL context and window
}
