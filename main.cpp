#include "raylib.h"
#include "raymath.h"
#include "Character.h"
#include "Prop.h"
#include "Enemy.h"
#include <string>

int main()
{
    const int windowWidth{384};
    const int windowHeight{384};


    InitWindow(windowWidth,windowHeight,"Mini Tiny RPG C++!!");

    Texture2D map = LoadTexture("nature_tileset/WorldMap.png");
    Vector2 mapPos{0.0,0.0};
    const float mapScale{4.0f};


    Character knight{windowWidth,windowHeight};
    Enemy Goblin(Vector2{800.f,300.f},LoadTexture("characters/goblin_idle_spritesheet.png"),LoadTexture("characters/goblin_run_spritesheet.png"));

    Enemy Slime{
        Vector2{500.f,700.f},
        LoadTexture("characters/slime_idle_spritesheet.png"),
        LoadTexture("characters/slime_run_spritesheet.png")
    };
    Prop props[2]{
        Prop{Vector2{600.f,300.f},LoadTexture("nature_tileset/Rock.png")},
        Prop{Vector2{400.f,500.f},LoadTexture("nature_tileset/Log.png")}
    };

    Enemy* enemies[]{
        &Goblin,
        &Slime
    };

    for(auto enemy : enemies)
    {
        enemy->SetTarget(&knight);
    }

    SetTargetFPS(60);
    while(!WindowShouldClose())
    {
        BeginDrawing();
        ClearBackground(WHITE);
        mapPos = Vector2Scale(knight.GetWorldPos(),-1.f);
        // draw the map
        DrawTextureEx(map,mapPos,0.0f,mapScale,WHITE);
        knight.Tick(GetFrameTime());

        // Draw the props
        for(auto prop : props){
            prop.Render(knight.GetWorldPos());
        }

        if(!knight.GetAlive()) // Character is not alive
        {
            DrawText("Game Over!",55.f,45.f,50,RED);
            EndDrawing();
            continue;
        }
        else
        {

            std::string knightsHealth = "Health: ";
            knightsHealth.append(std::to_string(knight.GetHealth()),0,5);
            DrawText(knightsHealth.c_str(),55.f,45.f,40,RED);

        }

        // Checking map bounds
        if(knight.GetWorldPos().x< 0.f ||
           knight.GetWorldPos().y<0.f  ||
           knight.GetWorldPos().x+windowWidth > (map.width*mapScale)-25 ||
           knight.GetWorldPos().y+windowHeight > (map.height*mapScale)-25)
        {
            knight.UndoMovement();
        }

        //check prop collisions
        for(auto prop :props)
        {
            if(CheckCollisionRecs(prop.getCollisionRec(knight.GetWorldPos()),knight.getCollisionRec()))
            {
                knight.UndoMovement();
            }
        }
        for(auto enemy:enemies)
        {
            enemy->Tick(GetFrameTime());
        }

        if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
        {
            for(auto enemy : enemies)
            {

                if(CheckCollisionRecs(enemy->getCollisionRec(),knight.GetWeaponCollisionRec()))
                {
                    enemy->SetAlive(false);
                }

            }
        }

        EndDrawing();
    }

    CloseWindow();
}
