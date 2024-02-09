
#include "Cell.h"
#include "FunctionRules.h"
#include <algorithm>
#include <array>
#include <iostream>
#include <memory>
#include <random>
#include <raylib.h>
#include <string>

const char* PROJECTNAME = "VivaVox";
constexpr int size = 50;


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
    int element = xyToElement(x,z);
    if (element == -1)
        return -1;
    return element + (size*size) * y;
}





int main()
{
    InitWindow(1920,1080,PROJECTNAME);
    SetTargetFPS(60);
    std::cout << "Hello, World!" << std::endl;
    Camera3D cam;
    cam.position = { size+10, size+10, size+10 };
    cam.target = { size/2, size/2, size/2};
    cam.up = { 0.0f, 1.0f, 0.0f };
    cam.fovy = 45.0f;
    cam.projection = CAMERA_PERSPECTIVE;
    DisableCursor();
    int cubesammount = 0;



    //std::vector<Cube> water;
    if (size != size)
    {
        return 666;
    }

    std::array<std::unique_ptr<Cube>, size * size * size> grid = {};
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

    int element = 0;
    for (auto & cell : grid)
    {
        cell = std::make_unique<Cube>(Cube{Vector3{0,0,0},true,false,null,0});
    }

    ///POPULATE COORDS
    for (int layer = 0; layer < size; ++layer)
    {
        int top = 0, bottom = size - 1, left = 0, right = size - 1;

        while (top <= bottom && left <= right) {
            // Traverse top side from left to right
            for (int i = left; i <= right; ++i) {
                grid[element]->pos.x = static_cast<float>(i);
                grid[element]->pos.y = static_cast<float>(layer);
                grid[element]->pos.z = static_cast<float>(top);
                ++element;
            }
            ++top;

            // Traverse right side from top to bottom
            for (int i = top; i <= bottom; ++i) {
                grid[element]->pos.x = static_cast<float>(right);
                grid[element]->pos.y = static_cast<float>(layer);
                grid[element]->pos.z = static_cast<float>(i);
                ++element;
            }
            --right;

            // Traverse bottom side from right to left
            if (top <= bottom) {
                for (int i = right; i >= left; --i) {
                    grid[element]->pos.x = static_cast<float>(i);
                    grid[element]->pos.y = static_cast<float>(layer);
                    grid[element]->pos.z = static_cast<float>(bottom);
                    ++element;
                }
                --bottom;
            }

            // Traverse left side from bottom to top
            if (left <= right) {
                for (int i = bottom; i >= top; --i) {
                    grid[element]->pos.x = static_cast<float>(left);
                    grid[element]->pos.y = static_cast<float>(layer);
                    grid[element]->pos.z = static_cast<float>(i);
                    ++element;
                }
                ++left;
            }
        }
    }

    for (auto& unique_ptr : grid) {
        unique_ptr->empty = false;
        unique_ptr->celltype = water;
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
            if (grid.back()->empty)
            {
                cubesammount++;
                grid.back()->empty = false;
                if (num == 1)
                {
                    grid.back()->celltype = water;
                }
                else if (num == 2)
                {
                    grid.back()->celltype = sand;
                }
                else if (num == 3)
                {
                    grid.back()->celltype = brick;
                }
                else {
                    grid.back()->empty = true;
                    cubesammount--;
                }
            }
        }


        // PHYSICS
        for (int i = 0; i < grid.size(); ++i)
        {
            if (grid[i]->stationary >= 5)
            {
                continue;
            }

            // skip empty
            if (grid[i]->empty) {
                grid[i]->celltype = null;
                continue;
            }

            // skip unmovable
            if (grid[i]->celltype - unmovable_solid >= 0)
            {
                continue;
            }

            // can go down?
            if (i >= (size*size))
            {

                int new_place = i - (size*size);
                if (new_place < 0)
                {continue;}
                //if empty swap down
                if (grid[new_place]->empty )
                {
                    grid[new_place]->empty = false;
                    grid[new_place]->celltype = grid[i]->celltype;
                    grid[i]->empty = true;
                    grid[i]->stationary = 0;
                    grid[new_place]->stationary = 0;
                    //std::cout << "down" << std::endl;
                    continue;
                }
                //if not empty and diffrent types
                if (grid[new_place]->celltype != grid[i]->celltype)
                {
                    // if bellow block is a liquid
                    if (grid[new_place]->celltype < moving_solid)
                    {
                        // if top cell is denser swap
                        if (grid[new_place]->celltype < grid[i]->celltype)
                        {
                            grid[new_place].swap(grid[i]);
                            const Vector3 vec = grid[new_place]->pos;
                            grid[new_place]->pos = grid[i]->pos;
                            grid[i]->pos = vec;
                            grid[i]->stationary = 0;
                            grid[new_place]->stationary = 0;
                            continue;
                        }
                    }
                }

                // go diagnally
                int x ,y ,z;
                x = static_cast<int>(grid[new_place]->pos.x);
                y = static_cast<int>(grid[new_place]->pos.y);
                z = static_cast<int>(grid[new_place]->pos.z);
                auto elements = new std::vector<int>;
                elements->clear();
                elements->reserve(4);
                int element = 0;
                element = xyzToElement(x-1,y,z);
                if (element != -1 and grid[element]->empty) {
                    elements->emplace_back(element);
                }
                element =xyzToElement(x+1,y,z);
                if (element != -1 and grid[element]->empty) {
                    elements->emplace_back(element);
                }
                element = xyzToElement(x,y,z-1);
                if (element != -1 and grid[element]->empty) {
                    elements->emplace_back(element);
                }
                element = xyzToElement(x,y,z+1);
                if (element != -1 and grid[element]->empty) {
                    elements->emplace_back(element);
                }
                if (elements->empty()) {
                    grid[i]->stationary++;
                    continue;
                }
                std::ranges::shuffle(*elements, std::mt19937(std::random_device()()));
                // if empty go randomy diagnal
                grid[elements->at(0)]->empty = false;
                grid[elements->at(0)]->celltype = grid[i]->celltype;
                grid[i]->empty = true;
                grid[i]->stationary = 0;
                grid[elements->at(0)]->stationary = 0;
                continue;
            }
            // if water
            if (grid[i]->celltype != null and grid[i]->celltype < moving_solid)
            {
                // swap sideways
                int x = static_cast<int>(grid[i]->pos.x);
                int y = static_cast<int>(grid[i]->pos.y);
                int z = static_cast<int>(grid[i]->pos.z);
                auto elements = new std::vector<int>;
                elements->clear();
                elements->reserve(4);
                int element = 0;
                element = xyzToElement(x-1,y,z);
                if (element != -1 and grid[element]->empty) {
                    elements->emplace_back(element);
                }
                element =xyzToElement(x+1,y,z);
                if (element != -1 and grid[element]->empty) {
                    elements->emplace_back(element);
                }
                element = xyzToElement(x,y,z-1);
                if (element != -1 and grid[element]->empty) {
                    elements->emplace_back(element);
                }
                element = xyzToElement(x,y,z+1);
                if (element != -1 and grid[element]->empty) {
                    elements->emplace_back(element);
                }
                if (elements->empty()) {
                    grid[i]->stationary++;
                    continue;
                }
                std::ranges::shuffle(*elements, std::mt19937(std::random_device()()));
                // if empty go sideways
                grid[elements->at(0)]->empty = false;
                grid[elements->at(0)]->celltype = grid[i]->celltype;
                grid[i]->empty = true;
                grid[i]->stationary = 0;
                grid[elements->at(0)]->stationary = 0;
                continue;
            }
            grid[i]->stationary++;
            continue;
        }

        BeginDrawing();
        ClearBackground(BLACK);


        BeginMode3D(cam);
        {
            DrawCubeWires({5,5,5},10,10,10,RED);
            DrawGrid(100,1);
            for (const auto &cube : grid)
            {
                if (cube->empty)
                {continue;}
                if (cube->celltype == water)
                {
                    cube->stationary >= 5
                      ? DrawCubeV (cube->pos, {0.9, 0.9, 0.9}, { 102, 191, 255, 200 })
                      : DrawCubeV (cube->pos, {0.9, 0.9, 0.9}, { 102, 191, 255, 255 });
                    DrawCubeWiresV(cube->pos,{0.9,0.9,0.9},{ 0, 82, 172, 255 });
                }
                if (cube->celltype == lava) {
                    DrawCubeV(cube->pos,Vector3{0.9,0.9,0.9},RED);
                    DrawCubeWiresV(cube->pos,Vector3{0.9,0.9,0.9},MAROON);
                }
                if (cube->celltype == sand) {
                    DrawCubeV(cube->pos,Vector3{0.9,0.9,0.9},BEIGE);
                    DrawCubeWiresV(cube->pos,Vector3{0.9,0.9,0.9},GOLD);
                }
            }
        }
        EndMode3D();
        DrawFPS(10,10);
        DrawText(TextFormat("FRAME_TIME:%f", GetFrameTime()), 10, 40, 20, LIME);
        DrawText(TextFormat("Cubes:%i", cubesammount), 10, 60, 20, LIME);
        EndDrawing();
    }

    return 0;
}
