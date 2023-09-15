// #include "geometry.h"
#pragma once
#include "imgui.h"
#include "physics.h"
#include "renderobjects.h"
#include <array>
#include <memory>
#include <string>

// #include <queue>
struct Input {
  // https://stackoverflow.com/questions/46631814/handling-multiple-keys-input-at-once-with-glfw
  Input(vkEngine* _context) ;
  std::array<bool, GLFW_KEY_MENU + 1> button;
  void keys(GLFWwindow *win, int key, int scancode, int action, int mods);
  void handle(void);
  vkEngine* context;
};

class renderer : public vkEngine {
public:
  renderer() : vkEngine() {
    objectQueue = std::make_unique<renderObjectQueue>(this);
    physics = std::make_unique<physicsEngine>();
    KeyInput = std::make_unique<Input>(this);
    inputHandler();
    init_imgui();
  };
  ~renderer() { std::cout << "renderer destructor called" << std::endl; };
  void renderLoop();

  std::unique_ptr<renderObjectQueue> objectQueue;
  std::unique_ptr<physicsEngine> physics;
  std::unique_ptr<Input> KeyInput;

private:
  void inputHandler();
  void draw();
  void draw_imgui();
  void recordCommandBuffer(VkCommandBuffer commandBuffer, uint32_t imageIndex,
                           ImDrawData *draw_data);
  void appInput();
  static void mouse_callback(GLFWwindow *window, double xpos, double ypos);
  static void mouse_button_callback(GLFWwindow *window, int button, int action,
                                    int mods);
  static void key_callback(GLFWwindow *window, int key, int scancode,
                           int action, int mods);
};;
