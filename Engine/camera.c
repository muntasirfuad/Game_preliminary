#include "raylib.h"
#include "camera.h"
#include "tilemap.h"

void InitGameCamera(Camera2D *cam){
    cam->offset = (Vector2){640, 360};
    cam->rotation = 0.0f;

    int mapW = GetMapWidth();
    int mapH = GetMapHeight();

    float zoomX = 1280.0f / (float)mapW;
    float zoomY = 720.0f / (float)mapH;

    
    cam->zoom = (zoomX > zoomY) ? zoomX : zoomY;

    cam->target = (Vector2){0, 0};
}

void UpdateGameCamera(Camera2D *cam, Vector2 target){
    int mapW = GetMapWidth();
    int mapH = GetMapHeight();

    float halfViewW = cam->offset.x / cam->zoom;
    float halfViewH = cam->offset.y / cam->zoom;

    float clampedX = target.x;
    float clampedY = target.y;

    if(clampedX < halfViewW) clampedX = halfViewW;
    if(clampedY < halfViewH) clampedY = halfViewH;
    if(clampedX > mapW - halfViewW) clampedX = mapW - halfViewW;
    if(clampedY > mapH - halfViewH) clampedY = mapH - halfViewH;

    cam->target = (Vector2){clampedX, clampedY};
}