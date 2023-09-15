#pragma once
#include "renderer.h"
#include <assert.h>
#include <chrono>
#include <cstdlib>
#include <iostream>
#include <array>
#include <map>
#include <ostream>
#include <random>
#include <thread>
#include <utility>
#include <vector>
#include <unordered_map>

// https://stackoverflow.com/questions/42701688/using-an-unordered-map-with-arrays-as-keys
struct ArrayHasher {
  std::size_t operator()(const std::array<int, 2> &a) const {
    std::size_t h = 0;

    for (auto e : a) {
      h ^= std::hash<int>{}(e) + 0x9e3779b9 + (h << 6) + (h >> 2);
    }
    return h;
  }
};

class mazeGen {
public:
  // mazeGen();
  // mazeGen(mazeGen &&) = default;
  // mazeGen(const mazeGen &) = default;
  // mazeGen &operator=(mazeGen &&) = default;
  // mazeGen &operator=(const mazeGen &) = default;
  // ~mazeGen();
  void createMaze2D();
  std::vector<int>container;
  void Queue3Dscene(renderer* _app);

private:
  std::vector<std::vector<int>> maze;
  std::unordered_map<std::array<int, 2>, std::array<int, 4>, ArrayHasher> distanceMap;
  void pickRandom(int &b);
  std::vector<int> getmidpoint(std::vector<int> &a);
  void mazeiteration();

  void printmaze();
  void handleAB();
  int distanceP(std::array<int,2> a, std::array<int,2> b);
  int path = 0;
  int size = 0;
  std::vector<int> curr;
  std::array<int,2> root;
  std::vector<std::vector<int>> list;
};
