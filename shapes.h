#pragma once
#include "vulkanApp.h"
#include <glm/fwd.hpp>
#include <memory>
#include <stdint.h>
#include <tuple>
#include <vector>

struct objProperties {
  objProperties();
  // physicsProperties();
  // ~physicsProperties();
  float fmass;
  float fAngle;
  glm::vec3 vpos;
  glm::vec3 vvelocity;
  float fspeed;
  glm::vec3 vforces;
  float fRadius;
  glm::vec3 vgravity;
  glm::vec3 vprevPos;
  glm::vec3 vImpactforces;
  glm::vec3 vTangent;
  float fNormalForce;
  bool bCollision;
  bool bObjectCollision;
  bool goingOut;

  void CalcF();
  void updateEuler(double dt);
};

class Shape {
public:
  Shape() {
    // buildBuffers();
    properties = std::make_unique<objProperties>();
  };
  virtual ~Shape(){
      // destroyBuffers();
  };
  // virtual void render(VkCommandBuffer commandBuffer);
  // std::vector<uint16_t> getIndices();
  // vkEngine* context;
  // VkBuffer vertexBuffer;
  // VkDeviceMemory vertexBufferMemory;
  ShapeType type;
  glm::vec3 center_pos = {0.f, 0.f, 0.f};
  // VkBuffer indexBuffer;
  // VkDeviceMemory indexBufferMemory;
  uint32_t indexCount;
  std::vector<Vertex> vertices;
  std::vector<uint16_t> indices;
  std::vector<Vertex> collvertices;
  std::vector<uint16_t> collindices;

  glm::vec3 findfurthestpoint(glm::vec3 direction) {
    float dotMax = -INFINITY;
    glm::vec3 furthestpoint(0.0f);
    auto transformedVerts = getWorldVertices();
    for (auto v : transformedVerts) {
      float localDot = glm::dot(v, direction);
      if (dotMax < localDot) {
        dotMax = localDot;
        furthestpoint = v;
      }
    }
    return furthestpoint;
  };

  // void updateCollisionVerts() {
  //   for (auto &vert : collvertices) {
  //     glm::vec3 newvert = glm::translate(glm::mat4(1.0f), properties->vpos) *
  //                         glm::vec4(vert.pos, 1.0f);
  //     vert.pos = newvert;
  //   }
  // }

  std::vector<glm::vec3> getWorldVertices() {
    std::vector<glm::vec3> a;
    auto TranforMat = glm::translate(glm::mat4(1.0f), properties->vpos);
    TranforMat = glm::rotate(TranforMat, properties->fAngle, glm::vec3(1.0f,0.0f,0.0f));
    for (auto &vert : collvertices) {
      a.push_back(TranforMat * glm::vec4(vert.pos, 1.0f));
    }
    return a;
  }

  bool shaderPrimitive = false;
  bool transparent = false;
  std::unique_ptr<objProperties> properties;
  // protected:
  // void buildBuffers();
  // std::vector<Vertex> getVertices();
  // void destroyBuffers();
};
