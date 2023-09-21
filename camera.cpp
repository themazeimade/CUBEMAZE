#include "camera.h"
#include "GLFW/glfw3.h"

camera ::camera(vkEngine *_context) {
  
  collisionmesh = std::make_unique<Cube>(0.75f);
  // minmax = {{},{},{}}
  //  https://stackoverflow.com/questions/22194424/creating-a-view-matrix-with-glm
  collisionmesh->properties->vpos = {0.0f, 0.0f, 0.0f};
  upVector = glm::vec3(0, 1, 0);
  cameraView = glm::mat4(1.0f);
  // pitchVector = glm::vec3(1, 0, 0);
  // directionVector = glm::vec3(0, 0, 1);
  fspeed = 0.05f;
  context = _context;
  float width = context->getswExtent().width;
  float height = context->getswExtent().height;
  firstMouse = true;
  ;
  sensitivity = 0.1;
  float yawAngle = 90.0f;
  float pitchAngle = 0.0f;
  lastY = height / 2;
  lastX = width / 2;

  glm::vec3 direction = {
      cos(glm::radians(yawAngle)) * cos(glm::radians(pitchAngle)),
      sin(glm::radians(pitchAngle)),
      sin(glm::radians(yawAngle)) * cos(glm::radians(pitchAngle))};
  frontCamera = glm::normalize(direction);

  cameraProj =
      glm::perspective(glm::radians(60.0f), width / height, 0.005f, 100.f);
  cameraProj[1][1] *= -1;
}

camera ::~camera() {}

glm::mat4 camera::getCameraViewM() { return cameraView; };

glm::mat4 camera::getCameraProjM() { return cameraProj; };

// void camera::lookUpDown(float angle) {
//   yawAngle += angle;
//
//   glm::vec3 direction = {
//       cos(glm::radians(yawAngle)) * cos(glm::radians(pitchAngle)),
//       sin(glm::radians(pitchAngle)),
//       sin(glm::radians(yawAngle)) * cos(glm::radians(pitchAngle))};
//   frontCamera = glm::normalize(direction);
//
//   cameraView = glm::lookAt(vpos,vpos+frontCamera,upVector);
// };
// void camera::lookSides(float angle) {
//   pitchAngle += angle;
//
//   glm::vec3 direction = {
//       cos(glm::radians(yawAngle)) * cos(glm::radians(pitchAngle)),
//       sin(glm::radians(pitchAngle)),
//       sin(glm::radians(yawAngle)) * cos(glm::radians(pitchAngle))};
//   frontCamera = glm::normalize(direction);
//
//   cameraView = glm::lookAt(vpos,vpos+frontCamera,upVector);
// };

void camera::MouseHandler(double xpos, double ypos) {
  if (firstMouse) {
    lastX = xpos;
    lastY = ypos;
    firstMouse = false;
  }
  float width = context->getswExtent().width;
  float height = context->getswExtent().height;
  // float lastX = width/2, lastY = height/2;
  float xoffset = xpos - lastX;
  float yoffset = lastY - ypos;

  lastX = xpos;
  lastY = ypos;
  lockjump = false;

  xoffset *= sensitivity;
  yoffset *= sensitivity;

  yawAngle += xoffset;
  pitchAngle += yoffset;

  if (pitchAngle > 89.0f)
    pitchAngle = 89.0f;
  if (pitchAngle < -89.0f)
    pitchAngle = -89.0f;

  glm::vec3 direction = {
      cos(glm::radians(yawAngle)) * cos(glm::radians(pitchAngle)),
      sin(glm::radians(pitchAngle)),
      sin(glm::radians(yawAngle)) * cos(glm::radians(pitchAngle))};
  frontCamera = glm::normalize(direction);

  // cameraView = glm::lookAt(vpos,vpos+frontCamera,upVector);

  cameraProj =
      glm::perspective(glm::radians(90.0f), width / height, 0.1f, 100.f);
  cameraProj[1][1] *= -1;
};

// void camera::KeyboardHandler(int key, int scancode, int action, int mods) {
//   if (key == GLFW_KEY_W && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
//     vpos += fspeed * frontCamera;
//   }
//   if (key == GLFW_KEY_S && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
//     vpos -= fspeed * frontCamera;
//   }
//   if (key == GLFW_KEY_A && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
//     vpos -= fspeed * glm::normalize(glm::cross(frontCamera, upVector));
//   }
//   if (key == GLFW_KEY_D && (action == GLFW_PRESS || action == GLFW_REPEAT)) {
//     vpos += fspeed * glm::normalize(glm::cross(frontCamera, upVector));
//   }
//   // cameraView = glm::lookAt(vpos,vpos+frontCamera,upVector);
//   float width = context->getswExtent().width;
//   float height  = context->getswExtent().height;
//   cameraProj =
//       glm::perspective(glm::radians(90.0f), width / height, 0.1f, 100.f);
//   cameraProj[1][1] *= -1;
// };

void camera::KeyboardHandler(std::array<bool, GLFW_KEY_MENU + 1> b) {
  if (b[GLFW_KEY_W]) {
    collisionmesh->properties->vpos += fspeed * frontCamera;
  }
  if (b[GLFW_KEY_S]) {
    collisionmesh->properties->vpos -= fspeed * frontCamera;
  }
  if (b[GLFW_KEY_A]) {
    collisionmesh->properties->vpos -= fspeed * glm::normalize(glm::cross(frontCamera, upVector));
  }
  if (b[GLFW_KEY_D]) {
    collisionmesh->properties->vpos += fspeed * glm::normalize(glm::cross(frontCamera, upVector));
  }
  if(b[GLFW_KEY_SPACE]) {
    if(lockjump == false) {
    collisionmesh->properties->vImpactforces += glm::vec3(0.0f,900.0f,0.0f);
      lockjump = true;
    // b[GLFW_KEY_SPACE] = false;
    }
  } else {
    lockjump = false;
  }
  
  // cameraView = glm::lookAt(vpos,vpos+frontCamera,upVector);
  float width = context->getswExtent().width;
  float height  = context->getswExtent().height;
  cameraProj =
      glm::perspective(glm::radians(90.0f), width / height, 0.1f, 100.f);
  cameraProj[1][1] *= -1;
};

glm::vec3& camera::getPosition() { return collisionmesh->properties->vpos; };
