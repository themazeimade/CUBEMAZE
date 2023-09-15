#include "shapes.h"
#include <memory>

class Square : public Shape {
public:
  Square(float size_) : size(size_) {
    vertices = {{{-1.0f, -1.0f, 0.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
                {{1.0f, -1.0f, 0.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
                {{1.0f, 1.0f, 0.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
                {{-1.0f, 1.0f, 0.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}};
    indices = {0, 1, 2, 2, 3, 0};
    for (auto &v : vertices) {
      v.pos = {(size / 2) * v.pos[0], (size / 2) * v.pos[1], 0.0f};
    }
    // buildBuffers();
  }
  ~Square(){};

private:
  float size;
};

class Scircle : public Square {
public:
  Scircle(float radius_) : Square(radius_ * 2) {
    shaderPrimitive = true;
    transparent = true;
    properties->fRadius = radius_;
  }
  // float radius;
};

class circleFan : public Shape {
public:
  circleFan(float radius_, uint32_t nSegments)
      : radius(radius_), Segments(nSegments) {
    vertices.push_back({{0.0f, 0.0f, 0.0f},
                        {1.0f, 1.0f, 1.0f}}); // Center vertex with white color

    // Generate the vertices for the circle fan
    for (int i = 0; i <= nSegments; ++i) {
      float angle = static_cast<float>(i) / static_cast<float>(nSegments) *
                    2.0f * 3.14159f;
      float x = radius_ * std::cos(angle);
      float y = radius_ * std::sin(angle);
      vertices.push_back({{x, y, 0.0f}, {1.0f, 0.0f, 0.0f}}); // Vertex with red
                                                              // color indices
      indices.push_back(0);     // Index of the center vertex
      indices.push_back(i + 1); // Index of the current vertex
      indices.push_back(i + 2); // Index of the next vertex
    };
    indexCount = indices.size();
    // buildBuffers();
  };
  ~circleFan(){};

private:
  float radius;
  uint32_t Segments;
};

class Cube : public Shape {
public:
  Cube(float size_) : size(size_) {
    vertices = {{{-1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
                {{1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
                {{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
                {{-1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},
                {{-1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
                {{1.0f, -1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
                {{1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
                {{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}}};
    indices = {0, 1, 2, 2, 3, 0, 6, 5, 4, 4, 7, 6, 0, 3, 4, 3, 7, 4,
               1, 6, 2, 6, 1, 5, 2, 6, 3, 3, 6, 7, 1, 0, 4, 1, 4, 5};
    for (auto &v : vertices) {
      v.pos = {(size / 2) * v.pos[0], (size / 2) * v.pos[1],
               (size / 2) * v.pos[2]};
    }
    // buildBuffers();
  }
  ~Cube(){};

private:
  float size;
};

class CubeFace : public Shape {
public:
  CubeFace(float size_) : size(size_) {
    vertices = {
        {{-1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        {{1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{-1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},


        {{-1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        {{1.0f, -1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},


        {{-1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {1.0f, 1.0f}}, //8 , 9, 10, 11
        {{-1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {1.0f, 0.0f}},
        {{-1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        {{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},


        {{1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {0.0f, 1.0f}}, //12, 13, 14, 15
        {{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {0.0f, 0.0f}},
        {{1.0f, -1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},
        {{1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},


        {{1.0f, 1.0f, 1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 1.0f}}, //16, 17,18,19
        {{-1.0f, 1.0f, 1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 1.0f}},
        {{1.0f, 1.0f, -1.0f}, {0.0f, 0.0f, 1.0f}, {1.0f, 0.0f}},
        {{-1.0f, 1.0f, -1.0f}, {1.0f, 1.0f, 1.0f}, {0.0f, 0.0f}},


        {{-1.0f, -1.0f, 1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 0.0f}},
        {{1.0f, -1.0f, 1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 0.0f}},
        {{-1.0f, -1.0f, -1.0f}, {1.0f, 0.0f, 0.0f}, {0.0f, 1.0f}},
        {{1.0f, -1.0f, -1.0f}, {0.0f, 1.0f, 0.0f}, {1.0f, 1.0f}},






    };
    indices = {0, 1, 2, 
      2, 3, 0, 
      6, 5, 4, 
      4, 7, 6, 
      8, 9, 10, // left face
      10, 9, 11, 
      12, 14, 13,                            // right face
      14, 15, 13, 
      16, 18, 17,                            // top
      17, 18, 19,
      22, 23, 21, 22, 21, 20};
    for (auto &v : vertices) {
      v.pos = {(size / 2) * v.pos[0], (size / 2) * v.pos[1],
               (size / 2) * v.pos[2]};
    };
    // buildBuffers();
  }
  ~CubeFace(){};

private:
  float size;
};
