#include "Cell.h"
//#include "FunctionRules.h"
#include "raymath.h"
#include "rlgl.h"

#include <algorithm>
#include <array>
#include <iostream>
#include <map>
#include <memory>
#include <random>
#include <raylib.h>
//#include <string>
#include <unordered_map>

const char* PROJECTNAME = "VivaVox";
constexpr int size = 100;


int xyToElement(int target_x, int target_z)
{
    int element = 0;
    int current_x;
    int current_z;

    int top = 0, bottom = size - 1, left = 0, right = size - 1;

    while (top <= bottom && left <= right)
        {
        // Traverse top side from left to right
        for (int i = left; i <= right; ++i) {
            current_x = i;
            current_z = top;
            if (current_x == target_x && current_z == target_z)
                return element;
            ++element;
        }
        ++top;

        // Traverse right side from top to bottom
        for (int i = top; i <= bottom; ++i) {
            current_x = (right);
            current_z = (i);
            if (current_x == target_x && current_z == target_z)
                return element;
            ++element;
        }
        --right;

        // Traverse bottom side from right to left
        if (top <= bottom) {
            for (int i = right; i >= left; --i) {
                current_x = (i);
                current_z = (bottom);
                if (current_x == target_x && current_z == target_z)
                    return element;
                ++element;
            }
            --bottom;
        }

        // Traverse left side from bottom to top
        if (left <= right) {
            for (int i = bottom; i >= top; --i) {
                current_x = (left);
                current_z = (i);
                if (current_x == target_x && current_z == target_z)
                    return element;
                ++element;
            }
            ++left;
        }
    }

    // Return -1 if coordinates are not found
    return -1;
}

int xyzToElement(int x, int y, int z) {
  int element = xyToElement(x, z);
  if (element == -1)
    return -1;
  return element + (size * size) * y;
}
int xyzToElement(Vector3 pos) {
    return xyzToElement(pos.x,pos.y,pos.z);
}
void populate_grid(std::array<Cube, size * size * size>& grid) {
  int element = 0;

  /// POPULATE COORDS
  for (int layer = 0; layer < size; ++layer) {
    int top = 0, bottom = size - 1, left = 0, right = size - 1;

    while (top <= bottom && left <= right) {
      // Traverse top side from left to right
      for (int i = left; i <= right; ++i) {
        grid[element].pos.x = static_cast<float>(i);
        grid[element].pos.y = static_cast<float>(layer);
        grid[element].pos.z = static_cast<float>(top);
        ++element;
      }
      ++top;

      // Traverse right side from top to bottom
      for (int i = top; i <= bottom; ++i) {
        grid[element].pos.x = static_cast<float>(right);
        grid[element].pos.y = static_cast<float>(layer);
        grid[element].pos.z = static_cast<float>(i);
        ++element;
      }
      --right;

      // Traverse bottom side from right to left
      if (top <= bottom) {
        for (int i = right; i >= left; --i) {
          grid[element].pos.x = static_cast<float>(i);
          grid[element].pos.y = static_cast<float>(layer);
          grid[element].pos.z = static_cast<float>(bottom);
          ++element;
        }
        --bottom;
      }

      // Traverse left side from bottom to top
      if (left <= right) {
        for (int i = bottom; i >= top; --i) {
          grid[element].pos.x = static_cast<float>(left);
          grid[element].pos.y = static_cast<float>(layer);
          grid[element].pos.z = static_cast<float>(i);
          ++element;
        }
        ++left;
      }
    }
  }
}

/// \brief run program
/// \return 000:success
/// \return 666: size isnt size

int main() {

    InitWindow(1920,1080,PROJECTNAME);
    SetTargetFPS(60);
    std::cout << "Hello, World!" << std::endl;
    Camera3D cam;
    cam.position = { size+10, size+10, size+10 };
    cam.target = { size/2.f, size/2.f, size/2.f};
    cam.up = { 0.0f, 1.0f, 0.0f };
    cam.fovy = 45.0f;
    cam.projection = CAMERA_PERSPECTIVE;
    DisableCursor();
    int cubesammount = (size * size * size) * 2;
    rlEnableBackfaceCulling();
    Mesh mesh = GenMeshCube(0.9,0.9,0.9);

    // shaders
    auto data = shader_data{};
    Shader shader = LoadShaderFromMemory(data.instance_shader_vs,data.instance_shader_fs);
    shader.locs[SHADER_LOC_MATRIX_MVP] = GetShaderLocation(shader, "mvp");
    shader.locs[SHADER_LOC_VECTOR_VIEW] = GetShaderLocation(shader, "viewPos");
    shader.locs[SHADER_LOC_MATRIX_MODEL] = GetShaderLocationAttrib(shader, "instanceTransform");

    // Create one light
    ///Materials

    std::map<TYPE,Material> instance_matrials;
    instance_matrials.try_emplace(water,LoadMaterialDefault());
    instance_matrials.try_emplace(sand,LoadMaterialDefault());
    instance_matrials.at(water).maps[MATERIAL_MAP_DIFFUSE].color = BLUE;
    instance_matrials.at(sand).maps[MATERIAL_MAP_DIFFUSE].color = BEIGE;
    instance_matrials.at(water).shader = shader;
    instance_matrials.at(sand).shader = shader;

    //std::vector<Cube> water;
    if (size != size)
    {
        return 666;
    }

    std::unique_ptr<std::array<Cube, size * size * size>> grid = {};
    grid = std::make_unique<std::array<Cube, size * size * size>>();
    /**
        int element = 0;
        for (int layer = 0; layer <= size; ++layer)
        {
            int cordx = 0;
            int cordy = layer;
            int cordz = 0;
            int current_size = size;
            while (current_size > 0)
            {
                // Traverse bottom side of the layer
                for (int x = 0; x < current_size; ++x) {
                    sand[element].pos.x = (float)cordx++;
                    sand[element].pos.y = (float)cordy;
                    sand[element].pos.z = (float)cordz;
                    ++element;
                }

                // Traverse right side of the layer
                for (int z = 0; z < current_size; ++z) {
                    sand[element].pos.x = (float)cordx;
                    sand[element].pos.y = (float)cordy;
                    sand[element].pos.z = (float)cordz++;
                    ++element;
                }

                // Traverse top side of the layer
                for (int i = current_size - 1; i >= 0; --i)
                {
                    sand[element].pos.x = (float)cordx--;
                    sand[element].pos.y = (float)cordy;
                    sand[element].pos.z = (float)cordz;
                    ++element;
                }
                current_size--;
                // Traverse left side of the layer
                for (int i = current_size - 1; i >= 0; --i)
                {
                    sand[element].pos.x = (float)cordx;
                    sand[element].pos.y = (float)cordy;
                    sand[element].pos.z = (float)cordz--;
                    ++element;
                }
            }
            // Move to the next layer (upwards)
            std::cout << "layer:" << layer <<std::endl;
        }
    **/

    populate_grid(*grid);


    for (int i = grid->size()/2; i < grid->size(); ++i)
    {
        grid->at(i).empty = false;
        grid->at(i).celltype = sand;
    }
    for (int i = 0; i < grid->size()/2; ++i)
    {
        grid->at(i).empty = false;
        grid->at(i).celltype = water;
    }

    bool tap = false;
    int num = 999;
    while (!WindowShouldClose()) {
        if (IsMouseButtonPressed(MOUSE_BUTTON_RIGHT))
        {
            if(IsCursorHidden())
                EnableCursor();
            else
                DisableCursor();
        }
        if (IsKeyPressed(KEY_H))
        {
            tap = !tap;
        }
        for (int i = 0; i < 10; ++i)
        {
            if (IsKeyPressed(48+i)) {
                num = i;
                std::cout << num << "\n";
            }
        }

        /// update
        UpdateCamera(&cam,CAMERA_FREE);

        if (tap)
        {
            if (grid->back().empty)
            {
                cubesammount++;
                grid->back().empty = false;
                if (num == 1)
                {
                    grid->back().celltype = water;
                }
                else if (num == 2)
                {
                    grid->back().celltype = sand;
                }
                else if (num == 3)
                {

                }
                else {
                    grid->back().empty = true;
                    cubesammount--;
                }
            }
            if (num == 7)
            {
                if (grid->at(size*size).celltype == BLACKHOLE)
                {
                    grid->at(size*size).celltype = null;
                    grid->at(size*size).empty = true;
                }

                if (grid->at(size*size).celltype != BLACKHOLE)
                {
                    grid->at(size*size).celltype = BLACKHOLE;
                    grid->at(size*size).empty = false;
                }
            }
        }


        // PHYSICS
        for (int i = 0; i < grid->size(); ++i)
        {
            if (grid->at(i).stationary <= 5)
            {
                continue;
            }

            // skip empty
            if (grid->at(i).empty) {
                grid->at(i).celltype = null;
                continue;
            }

            // skip unmovable
            if (grid->at(i).celltype - unmovable_solid >= 0)
            {
                if (grid->at(i).celltype == BLACKHOLE) {
                    auto pos = grid->at(i).pos;

                    grid->at(xyzToElement(pos.x+1,pos.y,pos.z)).celltype = null;
                    grid->at(xyzToElement(pos.x+1,pos.y,pos.z)).empty = true;

                    grid->at(xyzToElement(pos.x,pos.y+1,pos.z)).celltype = null;
                    grid->at(xyzToElement(pos.x,pos.y+1,pos.z)).empty = true;

                    grid->at(xyzToElement(pos.x,pos.y,pos.z+1)).celltype = null;
                    grid->at(xyzToElement(pos.x,pos.y,pos.z+1)).empty = true;

                    grid->at(xyzToElement(pos.x-1,pos.y,pos.z)).celltype = null;
                    grid->at(xyzToElement(pos.x-1,pos.y,pos.z)).empty = true;

                    grid->at(xyzToElement(pos.x,pos.y,pos.z-1)).celltype = null;
                    grid->at(xyzToElement(pos.x,pos.y,pos.z-1)).empty = true;

                }
                continue;
            }

            // can go down?
            if (i >= (size*size))
            {

                int new_place = i - (size*size);
                if (new_place < 0)
                {continue;}
                //if empty swap down
                if (grid->at(new_place).empty )
                {
                    grid->at(new_place).empty = false;
                    grid->at(new_place).celltype = grid->at(i).celltype;
                    grid->at(i).empty = true;
                    grid->at(i).stationary = 0;
                    grid->at(new_place).stationary = 0;
                    //std::cout << "down" << std::endl;
                    continue;
                }
                //if not empty and diffrent types
                if (grid->at(new_place).celltype != grid->at(i).celltype && grid->at(new_place).celltype < moving_solid && grid->at(new_place).celltype < grid->at(i).celltype)
                {
                    // if bellow block is a liquid
                    // if top cell is denser swap
                    const Vector3 vec = grid->at(new_place).pos;
                    const TYPE type = grid->at(new_place).celltype;
                    grid->at(new_place).pos = grid->at(i).pos;
                    grid->at(new_place).celltype = grid->at(i).celltype;
                    grid->at(i).pos = vec;
                    grid->at(i).celltype = type;
                    grid->at(i).stationary = 0;
                    grid->at(new_place).stationary = 0;
                    continue;
                }

                // go diagnally
                int x ,y ,z;
                x = static_cast<int>(grid->at(new_place).pos.x);
                y = static_cast<int>(grid->at(new_place).pos.y);
                z = static_cast<int>(grid->at(new_place).pos.z);
                auto elements = new std::vector<int>;
                elements->clear();
                elements->reserve(4);
                int element = 0;
                element = xyzToElement(x-1,y,z);
                if (element != -1 and grid->at(element).empty) {
                    elements->emplace_back(element);
                }
                element =xyzToElement(x+1,y,z);
                if (element != -1 and grid->at(element).empty) {
                    elements->emplace_back(element);
                }
                element = xyzToElement(x,y,z-1);
                if (element != -1 and grid->at(element).empty) {
                    elements->emplace_back(element);
                }
                element = xyzToElement(x,y,z+1);
                if (element != -1 and grid->at(element).empty) {
                    elements->emplace_back(element);
                }
                if (elements->empty()) {
                    grid->at(i).stationary++;
                    continue;
                }
                std::ranges::shuffle(*elements, std::mt19937(std::random_device()()));
                // if empty go randomy diagnal
                grid->at(elements->at(0)).empty = false;
                grid->at(elements->at(0)).celltype = grid->at(i).celltype;
                grid->at(i).empty = true;
                grid->at(i).stationary = 0;
                grid->at(elements->at(0)).stationary = 0;
                continue;
            }
            // if water
            if (grid->at(i).celltype != null and grid->at(i).celltype < moving_solid)
            {
                // swap sideways
                int x = static_cast<int>(grid->at(i).pos.x);
                int y = static_cast<int>(grid->at(i).pos.y);
                int z = static_cast<int>(grid->at(i).pos.z);
                auto elements = new std::vector<int>;
                elements->clear();
                elements->reserve(4);
                int element = 0;
                element = xyzToElement(x-1,y,z);
                if (element != -1 and grid->at(element).empty) {
                    elements->emplace_back(element);
                }
                element =xyzToElement(x+1,y,z);
                if (element != -1 and grid->at(element).empty) {
                    elements->emplace_back(element);
                }
                element = xyzToElement(x,y,z-1);
                if (element != -1 and grid->at(element).empty) {
                    elements->emplace_back(element);
                }
                element = xyzToElement(x,y,z+1);
                if (element != -1 and grid->at(element).empty) {
                    elements->emplace_back(element);
                }
                if (elements->empty()) {
                    grid->at(i).stationary++;
                    continue;
                }
                std::ranges::shuffle(*elements, std::mt19937(std::random_device()()));
                // if empty go sideways
                grid->at(elements->at(0)).empty = false;
                grid->at(elements->at(0)).celltype = grid->at(i).celltype;
                grid->at(i).empty = true;
                grid->at(i).stationary = 0;
                grid->at(elements->at(0)).stationary = 0;
                continue;
            }
            grid->at(i).stationary++;
            continue;
        }

        //std::unordered_map<TYPE,std::vector<Matrix>> groups;
        //for (const auto &cube : *grid)
        //{
        //    if (cube.celltype == null)
        //        continue;
        //    groups[cube.celltype].emplace_back(MatrixTranslate(cube.pos.x,cube.pos.y,cube.pos.z));
        //}

        BeginDrawing();
        ClearBackground(BLACK);
        //BeginMode3D(cam);
        //{
        //    DrawCubeWires({size/2,size/2,size/2},size,size,size,RED);
        //    DrawGrid(100,1);

        //    for (const auto &[fst, snd] : groups) {
        //        //rlEnableWireMode();
        //        DrawMeshInstanced(mesh,instance_matrials[fst],snd.data(),snd.size());
        //        //rlDisableWireMode();
        //    }

        //}
        //EndMode3D();
        DrawFPS(10,10);
        DrawText(TextFormat("FRAME_TIME:%f", GetFrameTime()), 10, 40, 20, LIME);
        DrawText(TextFormat("Cubes:%i", cubesammount), 10, 60, 20, LIME);
        EndDrawing();
    }

    return 0;
}
