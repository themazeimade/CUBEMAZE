#pragma once
#include "renderobjects.h"
#include <algorithm>
#include <glm/fwd.hpp>
#include <glm/geometric.hpp>
#include <stdexcept>

struct simplex {
  std::deque<glm::vec3> vertices;
  std::array<uint16_t, 9> indices;
  static bool handleSimplex(simplex &_simplex, glm::vec3 &direction);
};
// https://winter.dev/articles/epa-algorithm

namespace gtk {

bool line(simplex &_simplex, glm::vec3 &direction);
bool triangle(simplex &_simplex, glm::vec3 &direction);
bool tetrahedron(simplex &_simplex, glm::vec3 &direction);
bool SameDirection(glm::vec3 a, glm::vec3 b);
void epa(Shape *A, Shape *B, simplex &_simplex); // collision resolution
std::pair<std::vector<glm::vec4>, size_t>
GetFaceNormals(const std::vector<glm::vec3> &polytope,
               const std::vector<size_t> &faces);
void AddIfUniqueEdge(std::vector<std::pair<size_t, size_t>> &edges,
                     const std::vector<size_t> &faces, size_t a, size_t b);
} // namespace gtk

class physicsEngine {
public:
  bool checkWallCollisions(renderobject *boundary, renderobject *simObject);
  void checkObjectsCollisions(renderObjectQueue *queue);
  bool CameracollisionDetection(Shape *camera, Shape *object, simplex& simplex);
  void updatesimulation(renderObjectQueue *queue);
  static glm::vec3 vSupport(Shape *camera, Shape *object, glm::vec3 direction);
  uint32_t FrameCounter;
};
