#ifndef CAMERA_H
#define CAMERA_H
#include "raylib.h"

void InitGameCamera(Camera2D *cam);
void UpdateGameCamera(Camera2D *cam, Vector2 target);

#endif