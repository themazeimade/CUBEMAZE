#include "imgui.h"
#include "renderer.h"
#include "mazeGen.h"
#include <iostream>
#include <memory>
// #include "physicsProperties.h"

// double renderer::currX;
// double renderer::currY

int main() {
  try {
    auto app = std::make_unique<renderer>();
    
    mazeGen Maze;
    Maze.createMaze2D();
    Maze.Queue3Dscene(app.get());
    // std::unique_ptr<renderobject> b = std::make_unique<renderobject>(app.get());
    // b->mesh = std::make_unique<CubeFace>(2.f);
    // b->prepareRenderProperties();
    // app->objectQueue->push_renderobject(std::move(b));
    
    // setup only mesh
    app->render3D();
  } catch (const std::exception &e) {
    std::cerr << e.what() << std::endl;
    return EXIT_FAILURE;
  }
  std::cout << "bye!!" << std::endl;
  return EXIT_SUCCESS;
}
