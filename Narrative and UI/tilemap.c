#include "raylib.h"
#include "tilemap.h"

static Texture2D mapTexture;

void InitTilemap(void){
    mapTexture = LoadTexture("forest_map.png");
}

void DrawTilemap(void){
    DrawTexture(mapTexture, 0, 0, WHITE);
}

int GetMapWidth(void){ 
    return mapTexture.width; 
}
int GetMapHeight(void){ 
    return mapTexture.height; 
}

void CloseTilemap(void){
    UnloadTexture(mapTexture);
}

Texture2D GetTilemapTexture(void){
    return mapTexture;
}