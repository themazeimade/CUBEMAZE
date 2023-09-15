#include "mazegen.h"
#include <iostream>
// #include <iostream>

void clearScreen() {
// Clear the console screen by issuing the appropriate command based on the
// platform
#ifdef _WIN32
  std::system("cls");
#else
  std::system("clear");
#endif
}

// mazeGen::mazeGen() {};
//
// mazeGen::~mazeGen() {};

void mazeGen::pickRandom(int &b) {
  std::random_device dev;
  std::mt19937 e1(dev());
  std::uniform_int_distribution<int> dist(1, b - 2);

  curr[0] = dist(e1);
  curr[1] = dist(e1);
  if (curr[0] % 2 == 0) {
    curr[0] = curr[0] - 1;
  }
  if (curr[1] % 2 == 0) {
    curr[1] = curr[1] - 1;
  }
};

std::vector<int> mazeGen::getmidpoint(std::vector<int> &a) {
  int xmid = ((a[0] + a[2]) / 2);
  int ymid = ((a[1] + a[3]) / 2);
  return {xmid, ymid};
}

void mazeGen::mazeiteration() {
  std::random_device dev;
  std::mt19937 e1(dev());
  std::uniform_int_distribution<int> dist(0, list.size() - 1);
  int a = dist(e1);
  // std::cout << list[a][0] << "curr en mazeIT " << list[a][1]<< list[a][2] <<
  // list[a][3] << std::endl;

  std::array<int, 2> keybefore = {list[a][0], list[a][1]};
  auto b = getmidpoint(list[a]);

  std::array<int, 2> key = {b[0], b[1]};

  if (list[a][0] == root[0] && list[a][1] == root[1]) {
    path++;
    distanceMap[key] = {list[a][0], list[a][1], distanceMap[keybefore][2] + 1,
                        path};
  } else
    distanceMap[key] = {list[a][0], list[a][1], distanceMap[keybefore][2] + 1,
                        distanceMap[keybefore][3]};

  maze[b[0]][b[1]] = 1;

  curr[0] = list[a][2];
  curr[1] = list[a][3];

  while (curr[0] == 0 || curr[1] == 0 || curr[0] == (maze.size() - 1) ||
         curr[1] == (maze.size() - 1)) {
    list.erase(list.begin() + a);
    std::uniform_int_distribution<int> dist1(0, list.size() - 1);
    a = dist1(e1);
    curr[0] = list[a][2];
    curr[1] = list[a][3];

    keybefore = {list[a][0], list[a][1]};
    // list size
    b = getmidpoint(list[a]);
    maze[b[0]][b[1]] = 1;

    key = {b[0], b[1]};
    if (list[a][0] == root[0] && list[a][1] == root[1]) {
      path++;
      distanceMap[key] = {list[a][0], list[a][1], distanceMap[keybefore][2] + 1,
                          path};
    } else
      distanceMap[key] = {list[a][0], list[a][1], distanceMap[keybefore][2] + 1,
                          distanceMap[keybefore][3]};
  }

  keybefore = {b[0], b[1]};
  key = {curr[0], curr[1]};
  distanceMap[key] = {b[0], b[1], distanceMap[keybefore][2] + 1,
                      distanceMap[keybefore][3]};
  maze[curr[0]][curr[1]] = 1;
  list.erase(list.begin() + a);

  for (std::vector<std::vector<int>>::iterator it = list.begin();
       it != list.end();) {
    if (*(it->begin() + 2) == curr[0] && *(it->begin() + 3) == curr[1]) {
      it = list.erase(it);
    } else
      it++;
  };
  std::vector<int> buff;
  for (int i = 0; i < list.size(); i++) {
    if (list[i][2] == curr[0] && list[i][3] == curr[1]) {
      buff.push_back(i);
    };
  }
  for (int j : buff) {
    list.erase(list.begin() + j);
  }

  // if (curr[0] != 0 && curr[1] != 0 && curr[0] != (maze.size() - 1) &&
  //
  //     curr[1] != (maze.size() - 1)) {
  //   maze[curr[0]][curr[1]] = 1;
  // } else {
  // list.erase(list.begin() + a);
  //   a = dist(e1);
  // }
  // set the appropiate cells to 1 (check if is on boundary)S
  // std::cout << b[0] << " b en mazeIT " << b[1] << std::endl;
  // then erase it from list
  //
};

void mazeGen::printmaze() {

  std::this_thread::sleep_for(std::chrono::milliseconds(20));
  clearScreen();

  for (int i = 0; i < maze.size(); i++) {
    for (int j = 0; j < maze.size(); j++) {
      if (maze[i][j] == 0)
        std::cout << 'M';
      else if (maze[i][j] == 1)
        std::cout << ' ';
      else if (maze[i][j] == 2)
        std::cout << 'A';
      else if (maze[i][j] == 3)
        std::cout << 'B';
    }
    std::cout << std::endl;
  }
  std::cout << std::endl;
}

void mazeGen::createMaze2D() {
  int b;
  std::cout << "Ingrese el tamaño del laberinto (impar y mayor a 4):"
            << std::endl;
  std::cin >> b;
  std::cout << "Creando Laberinto..." << std::endl;

  size = b;
  assert(b >= 3 && b % 2 == 1);
  curr.resize(2);
  maze.resize(b, std::vector<int>(b, 0));
  pickRandom(b);
  maze[curr[0]][curr[1]] = 1;
  root[0] = curr[0];
  root[1] = curr[1];

  std::array<int, 2> key = {root[0], root[1]};
  distanceMap[key] = {root[0], root[1], 1, 0};

#ifdef DEBUG
  container = curr;
#endif // DEBUG
  //
  while (true) {
    for (int i = 0; i < 2; i++)
      for (int j = 0; j < 2; j++) {
        std::vector<int> v1 = curr;
        v1.resize(4);
        v1[i + 2] = v1[i] + (j ? -2 : 2);
        v1[i ? 2 : 3] = v1[i ^ 1];
        // check if Out of bounds
        // checkin if it is out of bounds;
        if (v1[2] < b && v1[2] >= 0 && v1[3] < b && v1[3] >= 0 &&
            maze[v1[2]][v1[3]] == 0) {
          list.push_back(v1);
          // std::cout << v1[0] << ' ' << v1[1] << "y en create " << v1[2]<< 'a'
          // << v1[3] << std::endl;
        }
      };
    // std::cout << list[0][0] << ' ' << list[0][2] << "y en create "<<
    // std::endl;
    mazeiteration();
    printmaze();
    if (list.empty()) {
      std::cout << "finished list" << std::endl;
      break;
    }
  }
  handleAB();
  printmaze();
  std::cout << "finished createmaze" << std::endl;
}

void mazeGen::handleAB() {
  maze[1][1] = 2;
  std::array<int, 2> pA = {1, 1};
  std::array<int, 2> pB;
  int pathOA = distanceMap[pA][3];
  std::cout << "pathOA" << pathOA << std::endl;
  std::array<int, 2> End;
  for (int i = 1; i < size; i++) {
    pB = {i + 1, 1};
    if (i + 1 < size && maze[i + 1][1] == 1) {
      if (distanceP(pA, End) < distanceP(pA, pB)) {
        End = pB;
      }
      // if (distanceMap[pB][3] == pathOA) {
      //   if ((distanceMap[pA][2] + distanceMap[End][2]-1) < distanceP(pA, pB))
      //     End = pB;
      // } else {
      //   // if (distanceMap[pB].empty())
      //   //   End = pB;
      //   if (distanceMap[pB][2] > distanceMap[End][2])
      //     End = pB;
      // }
    }
    pB = {1, i + 1};
    if (i + 1 < size && maze[1][i + 1] == 1) {
      if (distanceP(pA, End) < distanceP(pA, pB)) {
        End = pB;
      }
      // if (distanceMap[pB][3] == pathOA) {
      //   if ((distanceMap[pA][2] + distanceMap[End][2] - 1) < distanceP(pA,
      //   pB))
      //     End = pB;
      // } else {
      //   if (distanceMap[pB][2] > distanceMap[End][2])
      //     End = pB;
      // }
    }
    pB = {size - 2, i};
    if (maze[size - 2][i] == 1) {
      if (distanceP(pA, End) < distanceP(pA, pB)) {
        End = pB;
      }
      // if (distanceMap[pB][3] == pathOA) {
      //   if ((distanceMap[pA][2] + distanceMap[End][2]-1) < distanceP(pA, pB))
      //     End = pB;
      // } else {
      //   if (distanceMap[pB][2] > distanceMap[End][2])
      //     End = pB;
      // }
    }
    pB = {i, size - 2};
    if (maze[i][size - 2] == 1) {
      if (distanceP(pA, End) < distanceP(pA, pB)) {
        End = pB;
      }
      // if (distanceMap[pB][3] == pathOA) {
      //   if ((distanceMap[pA][2] + distanceMap[End][2]-1) < distanceP(pA, pB))
      //     End = pB;
      // } else {
      //   if (distanceMap[pB][2] > distanceMap[End][2])
      //     End = pB;
      // }
    }
  }
  // same path get overruled in the next iteration
  maze[End[0]][End[1]] = 3;
}

int mazeGen::distanceP(std::array<int, 2> a, std::array<int, 2> b) {
  if (distanceMap[a][3] != distanceMap[b][3]) {
    return distanceMap[a][2] + distanceMap[b][2] - 1;
  };
  std::array<int, 2> c = a;
  std::array<int, 2> d = {distanceMap[b][0], distanceMap[b][1]};
  // which has more distance
  while (c != d) {

    d = {distanceMap[d][0], distanceMap[d][1]};
    if (distanceMap[d][3] == 0) {
      c = {distanceMap[c][0], distanceMap[c][1]};
      d = {distanceMap[b][0], distanceMap[b][1]};
    }
  }

  int dist = distanceMap[a][2] + distanceMap[b][2] - 2 * distanceMap[c][2] + 1;

  return dist;
  ;
};

void mazeGen::Queue3Dscene(renderer *_app) {
  for (int i = 0; i < size; i++) {
    for (int j = 0; j < size; j++) {
      if (maze[i][j] == 0) {
        std::unique_ptr<renderobject> b = std::make_unique<renderobject>(_app);
        b->mesh = std::make_unique<CubeFace>(1.f);
        b->prepareRenderProperties();
        b->mesh->properties->vpos = {static_cast<float>(i), 0.5f,
                                     static_cast<float>(j)};

        glm::vec4 minpos = {b->mesh->vertices[4].pos, 0.0f};
        glm::vec4 maxpos = {b->mesh->vertices[2].pos, 0.0f};
        glm::vec3 min =
            glm::translate(glm::mat4(1.0f), b->mesh->properties->vpos) * minpos;
        glm::vec3 max =
            glm::translate(glm::mat4(1.0f), b->mesh->properties->vpos) * maxpos;
        b->minmax.first = {min[0],min[1],min[2]};
        b->minmax.second = {max[0],max[1],max[2]};

        _app->objectQueue->push_renderobject(std::move(b));

      } else if (maze[i][j] == 2) {
        *_app->objectQueue->getSceneCamera()->getPosition() = {
            static_cast<float>(i), 0.8, static_cast<float>(j)};

        glm::vec4 minpos = {_app->objectQueue->getSceneCamera()->collisionmesh->vertices[4].pos, 0.0f};
        glm::vec4 maxpos = {_app->objectQueue->getSceneCamera()->collisionmesh->vertices[2].pos, 0.0f};
        glm::vec3 min =
            glm::translate(glm::mat4(1.0f), *_app->objectQueue->getSceneCamera()->getPosition()) * minpos;
        glm::vec3 max =
            glm::translate(glm::mat4(1.0f), *_app->objectQueue->getSceneCamera()->getPosition()) * maxpos;
        _app->objectQueue->getSceneCamera()->minmax.first = {min[0],min[1],min[2]};
        _app->objectQueue->getSceneCamera()->minmax.second = {max[0],max[1],max[2]};
      }
    }
  }
  std::unique_ptr<renderobject> b = std::make_unique<renderobject>(_app);
  b->mesh = std::make_unique<Square>(static_cast<float>(size));
  b->prepareRenderProperties();
  b->mesh->properties->vpos = {static_cast<float>(size) / 2, 0.0f,
                               static_cast<float>(size) / 2};
  b->mesh->properties->fAngle = glm::radians(-90.0f);
  _app->objectQueue->push_renderobject(std::move(b));
}
