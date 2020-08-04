#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <time.h>

#include <cglm/cglm.h>

#include <substratic/core.h>
#include <substratic/window.h>
#include <substratic/render.h>
#include <substratic/asset.h>
#include <substratic/event.h>
#include <substratic/keyboard.h>

int gShaderProgram = -1;
mat4 gViewMatrix;

/* const uint SpriteCount = 100000; */
const uint SpriteCount = 10000;

const uint VirtualScreenWidth = 640;
const uint VirtualScreenHeight = 360;

typedef struct {
  float x;
  float y;
} Position;

typedef struct {
  float x;
  float y;
} Velocity;

float rand_float() {
  return (float)rand() / (float)RAND_MAX;
}

void window_resize_callback(SubstraticWindow window, int width, int height)
{
  printf("Window size -> %d / %d\n", width, height);
  glfwGetFramebufferSize((GLFWwindow *)window, &width, &height);
  printf("Framebuffer size -> %d / %d\n", width, height);

  int scaleX = floor((float)width  / (float)VirtualScreenWidth);
  int offsetX = (width % VirtualScreenWidth) / 2;
  /* int offsetX = round((width % VirtualScreenWidth) / 2.0f); */
  int scaleY = floor((float)height / (float)VirtualScreenHeight);

  int offsetY = (height % VirtualScreenHeight) / 2;
  /* int offsetY = round((height % VirtualScreenHeight) / 2.0f); */

  /* printf("X: %d -> %d\n", width, (offsetX * 2)+(VirtualScreenWidth * scaleX)); */
  /* printf("y: %d -> %d\n", height, (offsetY * 2)+(VirtualScreenHeight * scaleY)); */

  glViewport(0, 0, width, height);

  // TODO: Remove this disgusting global variable hack

  // Reset projection matrix
  if (gShaderProgram > -1) {
    mat4 orthoMatrix;
    glm_mat4_identity(orthoMatrix);
    glm_ortho(0.0f, (GLfloat)width, (GLfloat)height, 0.0f, -1.0f, 1.0f, orthoMatrix);

    subst_render_shader_use(gShaderProgram);
    subst_render_shader_mat4_set(gShaderProgram, "projection", orthoMatrix);

    vec3 scale = {(GLfloat)scaleX, (GLfloat)scaleY, 0.0f};
    glm_scale_make(gViewMatrix, scale);
  }
}

int main() {

  printf("Starting up.\n");

  subst_init();

  SubstraticWindow window = subst_window_create("Substratic C Test", 1280, 720);
  subst_window_resize_callback_set(window, window_resize_callback);

  // Set the random seed
  srand(time(NULL));

  Position* positions  = malloc(sizeof(Position) * SpriteCount);
  Velocity* velocities = malloc(sizeof(Position) * SpriteCount);

  for (int i = 0; i < SpriteCount; i++) {
    positions[i].x = rand_float() * 1280;
    positions[i].y = rand_float() * 720;
    /* printf("%f / %f\n", positions[i].x, positions[i].y); */
  }

  // Initialize the renderer
  subst_render_init_2d(window);
  subst_render_clear_color_set(0.17f, 0.016f, 0.322f);

  const char *vertexShaderSource = "#version 330 core\n"
    "layout (location = 0) in vec3 aPos;\n"
    "layout (location = 1) in vec3 aColor;\n"
    "layout (location = 2) in vec2 aTexCoord;\n"
    "\n"
    "out vec3 theColor;\n"
    "out vec2 texCoord;\n"
    "\n"
    "uniform mat4 model;\n"
    "uniform mat4 view;\n"
    "uniform mat4 projection;\n"
    "\n"
    "void main()\n"
    "{\n"
    "  gl_Position = projection * view * model * vec4(aPos, 1.0);\n"
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
  gShaderProgram = shaderProgram;

  // Use the linked shader program
  glUseProgram(shaderProgram);

  // Clean up shaders, they've already been linked
  glDeleteShader(vertexShader);
  glDeleteShader(fragmentShader);

  float vertices[] = {
     // positions         // colors          // texture coords
     1.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 0.0f,   // top right
     1.0f, 1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   1.0f, 1.0f,   // bottom right
     0.0f, 1.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 1.0f,   // bottom left
     0.0f, 0.0f, 0.0f,   1.0f, 1.0f, 1.0f,   0.0f, 0.0f    // top left
  };
  unsigned int indices[] = {
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

  // Load a texture
  StcTexture texture = subst_asset_texture_load("dist/assets/images/tile.png");

  // Define orthographic transform for screen size
  mat4 orthoMatrix;
  glm_mat4_identity(orthoMatrix);
  glm_ortho(0.0f, 1280.0f, 720.0f, 0.0f, -1.0f, 1.0f, orthoMatrix);
  /* glm_mat4_print(orthoMatrix, stderr); */
  subst_render_shader_use(shaderProgram);
  subst_render_shader_mat4_set(shaderProgram, "projection", orthoMatrix);

  vec3 scale = {2.0f, 2.0f, 0.0f};
  glm_scale_make(gViewMatrix, scale);

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

    // Bind the texture
    glBindTexture(GL_TEXTURE_2D, texture.id);

    subst_render_shader_use(shaderProgram);
    subst_render_shader_mat4_set(shaderProgram, "view", gViewMatrix);

    // Set up the shader
    for (int i = 0; i < SpriteCount; i++) {

      // Set up a model transform
      mat4 modelMatrix;
      glm_mat4_identity(modelMatrix);

      // Move the sprite into place
      vec3 position = {positions[i].x, positions[i].y, 0.0f};
      glm_translate(modelMatrix, position);

      // Scale the vertices to the texture size
      vec3 modelScale = {(GLfloat)texture.width, (GLfloat)texture.height, 0.0f};
      glm_scale(modelMatrix, modelScale);

      subst_render_shader_mat4_set(shaderProgram, "model", modelMatrix);

      glBindVertexArray(VAO);
      glDrawElements(GL_TRIANGLES, 6, GL_UNSIGNED_INT, 0);
    }

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

  subst_exit();
}
