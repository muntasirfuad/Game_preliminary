#include "raylib.h"
#include "game.h"

int main(void)
{
    InitWindow(1280, 720, "Chronicles of the Fallen Hero");
    SetTargetFPS(60);

    InitGame();

    while (!WindowShouldClose())
    {
        UpdateGame();

        BeginDrawing();
        ClearBackground(BLACK);
        DrawGame();
        EndDrawing();
    }

    CloseGame();
    CloseWindow();
    return 0;
}