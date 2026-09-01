#include "raylib.h"
#include "player.h"
#include "tilemap.h"

#define FRAME_COUNT 8
#define FRAME_SIZE 100

void InitPlayer(Player *p){
    p->position = (Vector2){200, 250};
    p->speed = 250.0f;
    p->sprite = LoadTexture("Soldier_Walk.png");
    p->frame = 0;
    p->frameTimer = 0.0f;
    p->facingLeft = false;
}

void UpdatePlayer(Player *p){
    float dt = GetFrameTime();
    bool moving = false;

    if(IsKeyDown(KEY_RIGHT)){ 
        p->position.x+= p->speed*dt; 
        p->facingLeft = false; 
        moving = true; 
    }
    if(IsKeyDown(KEY_LEFT)){ 
        p->position.x-= p->speed*dt; 
        p->facingLeft = true;  
        moving = true; }
    if(IsKeyDown(KEY_UP)){ 
        p->position.y-= p->speed*dt; 
        moving = true; 
    }
    if(IsKeyDown(KEY_DOWN)){ 
        p->position.y+= p->speed*dt; 
        moving = true; }

    if(p->position.x < 0) 
        p->position.x = 0;
    if(p->position.y < 0) 
        p->position.y = 0;
    if(p->position.x > GetMapWidth() - FRAME_SIZE) 
        p->position.x = GetMapWidth() - FRAME_SIZE;
    if(p->position.y > GetMapHeight() - FRAME_SIZE) 
        p->position.y = GetMapHeight() - FRAME_SIZE;

    if(moving){
        p->frameTimer+= dt;
        if(p->frameTimer > 0.08f){
            p->frame = (p->frame + 1) % FRAME_COUNT;
            p->frameTimer = 0.0f;
        }
    } 
    else p->frame = 0;
}

void DrawPlayer(Player *p){
    float w;

    if(p->facingLeft) w = -FRAME_SIZE;
    else w = FRAME_SIZE;
    Rectangle src = {(float)(p->frame * FRAME_SIZE), 0, w, (float)FRAME_SIZE};
    Rectangle dest = {p->position.x, p->position.y, 400, 400};
    DrawTexturePro(p->sprite, src, dest, (Vector2){0, 0}, 0.0f, WHITE);
}

void ClosePlayer(Player *p){
    UnloadTexture(p->sprite);
}