#pragma once
#include "geometry.h"
#include <memory>
// #include <glm/ext/matrix_transform.hpp>

class camera {
public:
  camera(vkEngine* _context);
  ~camera();
  glm::mat4 getCameraViewM();
  glm::mat4 getCameraProjM();
  // void lookUpDown(float angle);
  // void lookSides(float angle);
  void MouseHandler(double xpos, double ypos);
  // void KeyboardHandler(int key, int scancode, int action, int mods);
  void KeyboardHandler(std::array<bool,GLFW_KEY_MENU+1> b);
  glm::vec3* getPosition();

  glm::vec3 frontCamera;
  glm::vec3 upVector;
  std::pair<std::vector<float>,std::vector<float>> minmax;
  std::unique_ptr<Shape> collisionmesh;
private:
  // glm::vec3 vpos;
  vkEngine* context;
  float yawAngle;
  float pitchAngle;
  glm::vec3 vvelocity;
  // float fspeed;
  float sensitivity;
  glm::mat4 cameraView;
  glm::mat4 cameraProj;
  glm::mat4 proj;
  glm::vec3 pitchVector;
  float lastX, lastY;
  bool firstMouse;
};
