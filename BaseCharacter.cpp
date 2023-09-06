#include "BaseCharacter.h"
#include "raymath.h"

BaseCharacter::BaseCharacter()
{

}

void BaseCharacter::UndoMovement()
{
    worldPos = worldPosLastFrame;
}

Rectangle BaseCharacter::getCollisionRec()
{
    return Rectangle{
        GetScreenPos().x,
        GetScreenPos().y,
        width*scale,
        height*scale
    };

}
void BaseCharacter::Tick(float deltaTime)
{
    worldPosLastFrame = worldPos;
    // update aniamtion frame
    runningTime += deltaTime;
    if (runningTime >= updateTime)
    {
            frame++;
            runningTime = 0.f;

            if (frame > maxFrames)
                frame = 0;
    }


    if(Vector2Length(velocity)!=0.0f)
        {
            // set worldPos = worldPos+direction;
            
            worldPos = Vector2Add(worldPos,Vector2Scale(Vector2Normalize(velocity),speed));
            velocity.x < 0.f ? rightLeft =-1.f:rightLeft=1.f;
            texture = run;
        }
        else
        {
            texture = idle;

        }
        velocity = {};

    // draw the character
    Rectangle source{width * frame, 0.f, rightLeft * width, height};
    Rectangle dest{GetScreenPos().x, GetScreenPos().y, scale * width, scale * height};
    DrawTexturePro(texture, source, dest, Vector2{}, 0.f, WHITE);
}