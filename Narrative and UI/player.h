#ifndef PLAYER_H
#define PLAYER_H
#include "raylib.h"

typedef struct{
    Vector2 position;
    float speed;
    Texture2D sprite;
    int frame;
    float frameTimer;
    bool facingLeft;
}Player;

void InitPlayer(Player *p);
void UpdatePlayer(Player *p);
void DrawPlayer(Player *p);
void ClosePlayer(Player *p);

#endif