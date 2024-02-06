#pragma once
#include <raylib.h>

enum TYPE : int
{
  null = 999,
  liquid = 0,
  water,
  lava,

  gas = 100,
  steam,

  moving_solid = 200,
  sand,
  dirt,
  snow,

  unmovable_solid = 300,
  brick,
  ice,
};


struct Cube
{
  Vector3 pos = {0,0,0};
  bool empty = true;
  bool hidden = false;
  TYPE celltype = null;
  int stationary = 0;
};