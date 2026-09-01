#include "raylib.h"
#include "intro.h"
#include "tilemap.h"
#include <math.h>

typedef enum {
    MENU_NEW_GAME,
    MENU_RESUME,
    MENU_QUIT,
    MENU_COUNT
} MenuOption;

static int selected;
static float animTimer;
static bool finished;
static int result;
static Texture2D bgTex;

void InitIntro(void){
    selected = 0;
    animTimer = 0;
    finished = false;
    result = 0;
    bgTex = GetTilemapTexture();
}

void UpdateIntro(void){
    if(finished) return;

    animTimer += GetFrameTime();

    if(IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)){
        selected--;
        if(selected < 0) selected = MENU_COUNT - 1;
    }
    if(IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)){
        selected++;
        if(selected >= MENU_COUNT) selected = 0;
    }

    Vector2 mouse = GetMousePosition();
    int startY = 320;
    int gap = 60;
    for(int i=0; i<MENU_COUNT; i++){
        Rectangle btn = {440, (float)(startY + i*gap), 400, 50};
        if(CheckCollisionPointRec(mouse, btn)){
            selected = i;
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                if(i == MENU_NEW_GAME) { finished = true; result = 1; }
                if(i == MENU_RESUME)   { finished = true; result = 2; }
                if(i == MENU_QUIT)     { finished = true; result = 3; }
            }
        }
    }

    if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)){
        if(selected == MENU_NEW_GAME) { finished = true; result = 1; }
        if(selected == MENU_RESUME)   { finished = true; result = 2; }
        if(selected == MENU_QUIT)     { finished = true; result = 3; }
    }
}

void DrawIntro(void){
    if(bgTex.width > 0){
        DrawTexture(bgTex, 0, 0, (Color){60, 60, 60, 255});
    } else {
        ClearBackground((Color){10, 8, 12, 255});
    }

    DrawRectangle(0, 0, 1280, 720, (Color){0, 0, 0, 120});

    float glow = (sinf(animTimer * 2.0f) + 1.0f) * 0.5f;
    Color titleColor = (Color){
        200 + (int)(55 * glow),
        180 + (int)(40 * glow),
        100,
        255
    };

    DrawText("CHRONICLES OF THE FALLEN HERO", 180, 120, 48, titleColor);
    DrawLine(300, 180, 980, 180, (Color){150, 120, 60, 200});

    DrawText("The Last Name on the Door", 420, 200, 24, GRAY);

    const char *labels[MENU_COUNT] = {"NEW GAME", "RESUME", "QUIT"};
    int startY = 320;
    int gap = 60;

    for(int i=0; i<MENU_COUNT; i++){
        int y = startY + i*gap;
        bool isSel = (i == selected);

        Color btnColor = isSel ? (Color){60, 50, 30, 220} : (Color){20, 18, 25, 180};
        DrawRectangle(440, y, 400, 50, btnColor);
        DrawRectangleLinesEx((Rectangle){440, (float)y, 400, 50}, isSel ? 3 : 1,
            isSel ? GOLD : DARKGRAY);

        if(isSel){
            float bounce = sinf(animTimer * 6.0f) * 5.0f;
            DrawText(">", (int)(400 + bounce), y + 8, 32, GOLD);
        }

        Color textColor = isSel ? WHITE : (Color){150, 150, 150, 255};
        int textX = 640 - MeasureText(labels[i], 28)/2;
        DrawText(labels[i], textX, y + 10, 28, textColor);
    }

    if(selected == MENU_RESUME){
        DrawText("[No save data found - Start a New Game]", 430, 520, 18, DARKGRAY);
    }

    DrawText("[UP / DOWN] Navigate     [ENTER] Select     [MOUSE] Click",
        360, 660, 18, (Color){100, 100, 100, 200});
}

int IsIntroFinished(void){
    if(!finished) return 0;
    return result;
}