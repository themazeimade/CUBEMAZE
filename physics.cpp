#include "physics.h"
#include "imgui.h"
#include <glm/ext/matrix_transform.hpp>
#include <glm/geometric.hpp>
#include <memory>
#include <vector>

// void physicsEngine::checkObjectsCollisions(renderObjectQueue *queue) {
//   // int frontier = queue->frontier;
//   int &_frontier = queue->frontier;
//   if (_frontier == -1) {
//     std::cout << "frontier not iniated" << std::endl;
//     return;
//   }
//   // double dt = _TIMESTEP;
//   auto boundingBox = queue->shapes.back().get();
//
//   // std::deque<glm::vec3> displacement;
//   for (size_t i = 0; i <= _frontier; i++) {
//     for (size_t j = i + 1; j <= _frontier; j++) {
//       // if (i == j)
//       //   continue;
//       auto object01 = queue->shapes[i]->mesh->properties.get();
//       auto object02 = queue->shapes[j]->mesh->properties.get();
//       // if(object01->bCollision == false) object01->bCollision
//
//       float r = 0.0f;
//       ;                  // sum of objects radiuses;
//       glm::vec3 d(0.0f); // distance from center points of both obj;
//       float s = 0.0f;    // difference between d and r
//       glm::vec3 n(0.0f);
//       glm::vec3 vr01(0.0f);
//       glm::vec3 vr02(0.0f);
//       float vrn01 = 0.0f;
//       float vrn02 = 0.0f;
//       glm::vec3 Fi(0.0f);
//       float J = 0.0f;
//
//       r = object01->fRadius + object02->fRadius;
//       d = object01->vpos - object02->vpos;
//       s = glm::length(d) - r;
//       // std::cout << "new col pass output:" << std::endl
//       //           << "s:  " << s << std::endl;
//
//       if (s <= 0.0f) { // there is collision between these two;
//         // d = glm::normalize(d);
//         n = glm::normalize(d);
//         // std::cout << "n components:  " << n.x << " " << n.y << " " << n.z
//         //           << std::endl;
//         vr01 = object01->vvelocity - object02->vvelocity;
//         vr02 = object02->vvelocity - object01->vvelocity;
//         vrn01 = glm::dot(vr01, n);
//         vrn02 = glm::dot(vr02, -n);
//         float vrnplus = glm::abs(vrn01) + glm::abs(vrn02);
//         if (vrn01 < 0.0f) {
//
//           J = -1.0f * (glm::dot(vr01, n)) *
//               (1 + static_cast<float>(_RESTITUTION)) /
//               (1 / object01->fmass + 1 / object02->fmass);
//           Fi = n;
//           Fi *= J / static_cast<float>(_TIMESTEP);
//           object01->vImpactforces += Fi;
//           object01->vpos -= (glm::abs(vrn01) / vrnplus) * n * s;
//           // displacement.push_front(n * s);
//           object01->bObjectCollision = true;
//         }
//         if (vrn02 < 0.0f) {
//
//           J = -1.0f * (glm::dot(vr02, -n)) *
//               (1 + static_cast<float>(_RESTITUTION)) /
//               (1 / object01->fmass + 1 / object02->fmass);
//           Fi = -n;
//           Fi *= J / static_cast<float>(_TIMESTEP);
//           object02->vImpactforces += Fi;
//           object02->vpos -= (glm::abs(vrn02) / vrnplus) * -n * s;
//           // displacement.push_front(n * s);
//           object02->bObjectCollision = true;
//         }
//         object01 = nullptr;
//         object02 = nullptr;
//       }
//     }
//   }
//   // int i = 0;
//   // for (auto it = displacement.begin(); it <= (displacement.begin() +
//   // frontier);
//   //      it++) {
//   //   queue->shapes[i]->mesh->properties->vpos -= *it;
//   //   i++;
//   // }
// }

void physicsEngine::checkObjectsCollisions(renderObjectQueue *queue) {
  // int frontier = queue->frontier;
  int &_frontier = queue->frontier;
  if (_frontier == -1) {
    std::cout << "frontier not iniated" << std::endl;
    return;
  }
  // double dt = _TIMESTEP;
  auto boundingBox = queue->shapes.back().get();

  // std::deque<glm::vec3> displacement;
  for (size_t i = 0; i <= _frontier; i++) {
    for (size_t j = 0; j <= _frontier; j++) {
      if (i == j)
        continue;
      auto object01 = queue->shapes[i]->mesh->properties.get();
      auto object02 = queue->shapes[j]->mesh->properties.get();
      // if(object01->bCollision == false) object01->bCollision

      float r = 0.0f;
      ;                  // sum of objects radiuses;
      glm::vec3 d(0.0f); // distance from center points of both obj;
      float s = 0.0f;    // difference between d and r
      glm::vec3 n(0.0f);
      glm::vec3 vr01(0.0f);
      float vrn01 = 0.0f;
      glm::vec3 Fi(0.0f);
      float J = 0.0f;

      r = object01->fRadius + object02->fRadius;
      d = object01->vpos - object02->vpos;
      s = glm::length(d) - r;
      // std::cout << "new col pass output:" << std::endl
      //           << "s:  " << s << std::endl;

      if (s <= 0.0f) { // there is collision between these two;
        // d = glm::normalize(d);
        n = glm::normalize(d);
        // std::cout << "n components:  " << n.x << " " << n.y << " " << n.z
        //           << std::endl;
        vr01 = object01->vvelocity - object02->vvelocity;
        vrn01 = glm::dot(vr01, n);
        if (vrn01 < 0.0f) {

          J = -1.0f * (glm::dot(vr01, n)) *
              (1 + static_cast<float>(_RESTITUTION)) /
              (1 / object01->fmass + 1 / object02->fmass);
          Fi = n;
          Fi *= J / static_cast<float>(_TIMESTEP);
          object01->vImpactforces += Fi;
          if (object01->bCollision == false)
            object01->vpos -= n * s;
          // displacement.push_front(n * s);
          object01->bObjectCollision = true;
        }
        object01 = nullptr;
      }
    }
  }
  // int i = 0;
  // for (auto it = displacement.begin(); it <= (displacement.begin() +
  // frontier);
  //      it++) {
  //   queue->shapes[i]->mesh->properties->vpos -= *it;
  //   i++;
  // }
}

bool physicsEngine::checkWallCollisions(renderobject *boundary,
                                        renderobject *simObject) {
  // std::cout << std::endl << "Physics output" << std::endl;
  // glm::vec3 n(0.0f, 0.0f, 0.0f);
  glm::vec3 vr(0.0f, 0.0f, 0.0f);
  float vrn(0.0f);
  float J(0.0f);
  glm::vec3 Fi(0.0f, 0.0f, 0.0f);
  bool hasCollision = false;

  std::vector<Vertex> wallverts = boundary->mesh->vertices;
  objProperties *objectData = simObject->mesh->properties.get();
  // objectData->vImpactforces = glm::vec3(0.0f, 0.0f, 0.0f);
  glm::vec3 storePrevPos(0.0f);
  storePrevPos = objectData->vpos;

  Shape *check = boundary->mesh.get();

  //  setup checkCollisions
  if (typeid(*(check)) != typeid(Square))
    // if (typeid(*(boundary->mesh.get())) != typeid(Square))
    throw std::runtime_error("boundary not Square");
  check = nullptr;

  // if (objectData->vpos.x <= wallverts[0].pos.x + objectData->fRadius ||
  //     objectData->vpos.y <= wallverts[0].pos.y + objectData->fRadius ||
  //     objectData->vpos.y >= wallverts[2].pos.y - objectData->fRadius ||
  //     objectData->vpos.x >= wallverts[2].pos.x - objectData->fRadius) {
  // left vertical wall, down wall, up wall, right wall;

  // check for collision inside boundary
  glm::vec3 collidedWall(0.0f);
  glm::vec2 intersectionpoint(0.0f);
  glm::vec3 impactDir(0.0f);
  glm::mat4 rotationMat =
      glm::rotate(glm::identity<glm::mat4>(), glm::radians(90.0f),
                  glm::vec3(0.0f, 0.0f, 1.0f));
  int collisionCount = 0;
  bool wallColl = false;

  for (int i = 0; i <= 3; i++) {
    glm::vec3 wallPoint02(0.0f);
    glm::vec3 wallPoint01(0.0f);
    int before = (i + 3) % 4;
    wallPoint02 = wallverts[before].pos;
    wallPoint01 = wallverts[i].pos;
    int wall = -1;
    switch (i) {
    case 0:
      wallPoint01.x += objectData->fRadius;
      wallPoint02.x += objectData->fRadius;
      wallColl = objectData->vpos.x <= wallverts[0].pos.x + objectData->fRadius;
      wall = 0;
      break;
    case 1:
      wallPoint01.y += objectData->fRadius;
      wallPoint02.y += objectData->fRadius;
      wallColl = objectData->vpos.y <= wallverts[0].pos.y + objectData->fRadius;
      wall = 1;
      break;
    case 2:
      wallPoint01.x -= objectData->fRadius;
      wallPoint02.x -= objectData->fRadius;
      wallColl = objectData->vpos.x >= wallverts[2].pos.x - objectData->fRadius;
      wall = 2;
      break;
    case 3:
      wallPoint01.y -= objectData->fRadius;
      wallPoint02.y -= objectData->fRadius;
      wallColl = objectData->vpos.y >= wallverts[2].pos.y - objectData->fRadius;
      wall = 3;
      break;
    }
    if (wallColl == true) {
      // std::cout << "there is collision" << std::endl;
      collisionCount++;
      impactDir += glm::normalize(wallPoint01 - wallPoint02);
      if (collisionCount == 1) {
        line::pIntersection(objectData->vpos, objectData->vprevPos, wallPoint01,
                            wallPoint02, intersectionpoint);
      }
    }
    if (collisionCount == 2) {
      impactDir = glm::normalize(impactDir);
      break;
    }
  }
  objectData->vTangent = impactDir;
  impactDir = rotationMat * glm::vec4(impactDir, 0.f);
  vr = objectData->vvelocity;
  vrn = glm::dot(vr, impactDir);

  if (vrn < 0.0f) {
    // std::cout << "inside impulse handling" << std::endl;
    J = -1.0f * (glm::dot(vr, impactDir)) *
        (1 + static_cast<float>(_RESTITUTION)) * objectData->fmass;
    Fi = impactDir;
    Fi *= J / static_cast<float>(_TIMESTEP);
    objectData->vImpactforces = Fi;
    objectData->fNormalForce = dot(objectData->vforces, impactDir);
    objectData->vpos.y = intersectionpoint.y;
    objectData->vpos.x = intersectionpoint.x;
    // if (intersectionpoint.x == 0.0f) {
    // std::cout << "fuck no intersection point" << std::endl;
    // }

    hasCollision = true;
  } else {
    objectData->goingOut = true;
  }
  // };
  // objectData->vprevPos = storePrevPos;
  objectData = nullptr;
  return hasCollision;
};
void physicsEngine::updatesimulation(renderObjectQueue *queue) {
  // int &_frontier = queue->frontier;
  // if (_frontier == -1) {
  //   std::cout << "frontier not iniated" << std::endl;
  //   return;
  // }
  double dt = _TIMESTEP;

  // auto boundingBox = queue->shapes.back().get();
    auto camera = queue->getSceneCamera();
  for (int i = 0; i < _STEPCOUNT; i++) {

    for (auto it = queue->shapes.begin(); it < queue->shapes.end(); it++) {
      simplex local;
      camera->collisionmesh->properties->bCollision = CameracollisionDetection(
          camera->collisionmesh.get(), (*it)->mesh.get(), local);

      if (camera->collisionmesh->properties->bCollision == true) {
        if ((*it)->mesh->type != vQuad) {
          gtk::epa(camera->collisionmesh.get(), (*it)->mesh.get(), local);
        }
      }
    }
        camera->collisionmesh->properties->CalcF();
        camera->collisionmesh->properties->updateEuler(dt);
  // std::cout << camera->collisionmesh->properties->fspeed<< std::endl;
  }
}

bool gtk::SameDirection(glm::vec3 a, glm::vec3 b) { return dot(a, b) > 0; }

bool simplex::handleSimplex(simplex &_simplex, glm::vec3 &direction) {
  switch (_simplex.vertices.size()) {
  case 2:
    return gtk::line(_simplex, direction);
  case 3:
    return gtk::triangle(_simplex, direction);
  case 4:
    return gtk::tetrahedron(_simplex, direction);
  }
  return false;
};

bool gtk::line(simplex &_simplex, glm::vec3 &direction) {
  glm::vec3 a = _simplex.vertices[0];
  glm::vec3 b = _simplex.vertices[1];

  glm::vec3 ab = b - a;
  glm::vec3 ao = -a;

  if (SameDirection(ab, ao)) {
    direction = cross(cross(ab, ao), ab);
  }

  else {
    _simplex.vertices.pop_back();
    _simplex.vertices = {a};
    direction = ao;
  }

  return false;
};

bool gtk::triangle(simplex &_simplex, glm::vec3 &direction) {
  glm::vec3 a = _simplex.vertices[0];
  glm::vec3 b = _simplex.vertices[1];
  glm::vec3 c = _simplex.vertices[2];

  glm::vec3 ab = b - a;
  glm::vec3 ac = c - a;
  glm::vec3 ao = -a;

  glm::vec3 abc = glm::cross(ab, ac);

  if (SameDirection(glm::cross(abc, ac), ao)) {
    if (SameDirection(ac, ao)) {
      _simplex.vertices.pop_back();
      _simplex.vertices = {a, c};
      direction = glm::cross(glm::cross(ac, ao), ac);
    }

    else {
      _simplex.vertices.pop_back();
      _simplex.vertices = {a, b};
      return gtk::line(_simplex, direction);
    }
  }

  else {
    if (SameDirection(cross(ab, abc), ao)) {
      _simplex.vertices.pop_back();
      _simplex.vertices = {a, b};
      return gtk::line(_simplex, direction);
    }

    else {
      if (SameDirection(abc, ao)) {
        direction = abc;
      }

      else {
        _simplex.vertices = {a, c, b};
        direction = -abc;
      }
    }
  }

  return false;
};

bool gtk::tetrahedron(simplex &_simplex, glm::vec3 &direction) {
  glm::vec3 a = _simplex.vertices[0];
  glm::vec3 b = _simplex.vertices[1];
  glm::vec3 c = _simplex.vertices[2];
  glm::vec3 d = _simplex.vertices[3];

  glm::vec3 ab = b - a;
  glm::vec3 ac = c - a;
  glm::vec3 ad = d - a;
  glm::vec3 ao = -a;

  glm::vec3 abc = cross(ab, ac);
  glm::vec3 acd = cross(ac, ad);
  glm::vec3 adb = cross(ad, ab);

  if (SameDirection(abc, ao)) {
    _simplex.vertices.pop_back();
    _simplex.vertices = {a, b, c};
    return gtk::triangle(_simplex, direction);
  }

  if (SameDirection(acd, ao)) {
    _simplex.vertices.pop_back();
    _simplex.vertices = {a, c, d};
    return gtk::triangle(_simplex, direction);
  }

  if (SameDirection(adb, ao)) {
    _simplex.vertices.pop_back();
    _simplex.vertices = {a, d, b};
    return gtk::triangle(_simplex, direction);
  }

  return true;
};

bool physicsEngine::CameracollisionDetection(Shape *camera, Shape *object,
                                             simplex &simplex) {

  // simplex simplex;
  if (object->type == vQuad) {
    glm::vec3 orientation(0.0f);

    auto objectVerts = object->getWorldVertices();
    auto ab = objectVerts[1] - objectVerts[0];
    auto ac = objectVerts[2] - objectVerts[0];
    orientation = glm::normalize(glm::cross(ab, ac));

    auto camera_point = camera->findfurthestpoint(-orientation);

    auto oC = objectVerts[0] - camera_point;
    auto mindistance = glm::dot(oC, orientation);
    if (mindistance > 0.0f) {
      // collision true;
      mindistance += 0.001f;
      glm::vec3 deltaPos = orientation * mindistance;

      camera->properties->vpos += deltaPos;
      return true;
    }
  }

  glm::vec3 direction =
      glm::normalize(object->properties->vpos - camera->properties->vpos);

  simplex.vertices[0] = vSupport(camera, object, direction);
  direction = glm::vec3(0.0f) - simplex.vertices[0];
  while (true) {
    auto A = vSupport(camera, object, direction);
    if (glm::dot(A, direction) < 0.0f)
      return false;
    simplex.vertices.push_front(A);
    if (simplex::handleSimplex(simplex, direction) == true) {
      return true;
    };
  };
  return true;
};

glm::vec3 physicsEngine::vSupport(Shape *camera, Shape *object,
                                  glm::vec3 direction) {
  return camera->findfurthestpoint(direction) -
         object->findfurthestpoint(-direction);
}

void gtk::epa(Shape *A, Shape *B, simplex &_simplex) {
  std::vector<glm::vec3> polytope(_simplex.vertices.begin(),
                                  _simplex.vertices.end());
  std::vector<size_t> faces = {0, 1, 2, 0, 3, 1, 0, 2, 3, 1, 3, 2};

  auto [normals, minFace] = gtk::GetFaceNormals(polytope, faces);

  glm::vec3 minNormal;
  float minDistance = FLT_MAX;

  while (minDistance == FLT_MAX) {
    minNormal = {normals[minFace].x, normals[minFace].y, normals[minFace].z};
    minDistance = normals[minFace].w;

    glm::vec3 support = physicsEngine::vSupport(A, B, minNormal);
    float sDistance = glm::dot(minNormal, support);

    if (glm::abs(sDistance - minDistance) > 0.001f) {
      minDistance = FLT_MAX;
      std::vector<std::pair<size_t, size_t>> uniqueEdges;

      for (size_t i = 0; i < normals.size(); i++) {
        if (gtk::SameDirection(normals[i], support)) {
          size_t f = i * 3;

          gtk::AddIfUniqueEdge(uniqueEdges, faces, f, f + 1);
          gtk::AddIfUniqueEdge(uniqueEdges, faces, f + 1, f + 2);
          gtk::AddIfUniqueEdge(uniqueEdges, faces, f + 2, f);

          faces[f + 2] = faces.back();
          faces.pop_back();
          faces[f + 1] = faces.back();
          faces.pop_back();
          faces[f] = faces.back();
          faces.pop_back();

          normals[i] = normals.back(); // pop-erase
          normals.pop_back();

          i--;
        }
      }
      std::vector<size_t> newFaces;
      for (auto [edgeIndex1, edgeIndex2] : uniqueEdges) {
        newFaces.push_back(edgeIndex1);
        newFaces.push_back(edgeIndex2);
        newFaces.push_back(polytope.size());
      }

      polytope.push_back(support);

      auto [newNormals, newMinFace] = GetFaceNormals(polytope, newFaces);

      float oldMinDistance = FLT_MAX;
      for (size_t i = 0; i < normals.size(); i++) {
        if (normals[i].w < oldMinDistance) {
          oldMinDistance = normals[i].w;
          minFace = i;
        }
      }

      if (newNormals[newMinFace].w < oldMinDistance) {
        minFace = newMinFace + normals.size();
      }

      faces.insert(faces.end(), newFaces.begin(), newFaces.end());
      normals.insert(normals.end(), newNormals.begin(), newNormals.end());
    }
  }
  glm::vec3 vDeltaPos =
      glm::normalize(minNormal) * (minDistance + 0.001f) * -1.0f;
  A->properties->vpos += vDeltaPos;
}; // collision resolution

std::pair<std::vector<glm::vec4>, size_t>
gtk::GetFaceNormals(const std::vector<glm::vec3> &polytope,
                    const std::vector<size_t> &faces) {
  std::vector<glm::vec4> normals;
  size_t minTriangle = 0;
  float minDistance = FLT_MAX;

  for (size_t i = 0; i < faces.size(); i += 3) {
    glm::vec3 a = polytope[faces[i]];
    glm::vec3 b = polytope[faces[i + 1]];
    glm::vec3 c = polytope[faces[i + 2]];

    glm::vec3 normal = glm::normalize(cross(b - a, c - a));
    float distance = glm::dot(normal, a);

    if (distance < 0) {
      normal *= -1;
      distance *= -1;
    }

    normals.emplace_back(normal, distance);

    if (distance < minDistance) {
      minTriangle = i / 3;
      minDistance = distance;
    }
  }

  return {normals, minTriangle};
}

void gtk::AddIfUniqueEdge(std::vector<std::pair<size_t, size_t>> &edges,
                          const std::vector<size_t> &faces, size_t a,
                          size_t b) {
  auto reverse = std::find(              //      0--<--3
      edges.begin(),                     //     / \ B /   A: 2-0
      edges.end(),                       //    / A \ /    B: 0-2
      std::make_pair(faces[b], faces[a]) //   1-->--2
  );

  if (reverse != edges.end()) {
    edges.erase(reverse);
  }

  else {
    edges.emplace_back(faces[a], faces[b]);
  }
}
