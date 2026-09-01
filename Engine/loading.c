#include "raylib.h"
#include "loading.h"
#include<math.h>

static float progress;
static float timer;
static bool done;
static const char *loreTips[] = {
    "Some people carry the war inside them even after the battlefield has gone quiet.",
    "Five thousand soldiers marched into that valley. Two thousand returned.",
    "The king held a parade. Tonoy didn't go.",
    "He left the king's service not out of anger, but something worse: certainty.",
    "The forest of Maren does not welcome people.",
    "The ash is still packed — the moisture from that night is still in it.",
    "He spent fifteen years looking for something that didn't want to be found.",
    "The most dangerous enemy doesn't live outside the walls.",
};
static int tipIndex;

void InitLoadingScreen(void){
    progress = 0.0f;
    timer = 0.0f;
    done = false;
    tipIndex = GetRandomValue(0, 7);
}

void UpdateLoadingScreen(void){
    float dt = GetFrameTime();
    timer += dt;

    float target = 1.0f;
    if(timer < 2.5f) target = timer / 3.0f;
    progress += (target - progress) * 0.05f;

    if(progress >= 0.98f) progress = 1.0f;

    if(IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER) || 
       IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
        progress = 1.0f;
        done = true;
    }

    if(progress >= 1.0f && timer > 0.5f){
        done = true;
    }
}

void DrawLoadingScreen(void){
    ClearBackground((Color){10, 8, 12, 255});

    int W = GetScreenWidth();
    int H = GetScreenHeight();
    int cx = W / 2;

    DrawRectangle(0, 0, W, 80, (Color){0, 0, 0, 120});
    DrawRectangle(0, H-80, W, 80, (Color){0, 0, 0, 120});

    for(int i=0; i<12; i++){
        int px = (int)((i * 137 + (int)(timer*20*i)) % W);
        int py = (int)((i * 89 + (int)(timer*15*i)) % H);
        int alpha = 30 + (i % 20);
        DrawCircle(px, py, 1.5f + (i%3), (Color){106, 90, 58, alpha});
    }

    DrawLine(cx-300, 220, cx+300, 220, (Color){180, 150, 50, 120});
    DrawLine(cx-200, 225, cx+200, 225, (Color){180, 150, 50, 60});

    const char *title1 = "CHRONICLES";
    const char *title2 = "OF THE FALLEN HERO";
    int t1w = MeasureText(title1, 48);
    int t2w = MeasureText(title2, 48);

    float glow = (sinf(timer * 3.0f) + 1.0f) * 0.5f;
    Color gold = (Color){
        180 + (int)(55 * glow),
        150 + (int)(40 * glow),
        60,
        255
    };

    DrawText(title1, cx - t1w/2, 260, 48, gold);
    DrawText(title2, cx - t2w/2, 315, 48, gold);

    const char *sub = "THE LAST NAME ON THE DOOR";
    int sw = MeasureText(sub, 18);
    DrawText(sub, cx - sw/2, 370, 18, (Color){138, 122, 90, 200});

    DrawLine(cx-200, 400, cx+200, 400, (Color){180, 150, 50, 60});
    DrawLine(cx-300, 405, cx+300, 405, (Color){180, 150, 50, 120});

    int barW = 500;
    int barH = 14;
    int barX = cx - barW/2;
    int barY = 500;

    DrawRectangle(barX, barY, barW, barH, (Color){26, 21, 16, 255});
    DrawRectangleLines(barX, barY, barW, barH, (Color){58, 42, 21, 255});

    int fillW = (int)(barW * progress);
    if(fillW > 0){
        DrawRectangle(barX+2, barY+2, fillW-4, barH-4, (Color){100, 80, 30, 255});
        DrawRectangle(barX+2, barY+2, fillW-4, 3, (Color){255, 255, 255, 20});
    }

    const char *loadTxt = (progress >= 1.0f) ? "PRESS ANY KEY" : "LOADING...";
    int lw = MeasureText(loadTxt, 14);
    DrawText(loadTxt, cx - lw/2, 530, 14, (Color){90, 74, 58, 255});

    int tw = MeasureText(loreTips[tipIndex], 13);
    if(tw > W - 100){
        DrawText(loreTips[tipIndex], 50, 600, 13, (Color){74, 58, 42, 200});
    } else {
        DrawText(loreTips[tipIndex], cx - tw/2, 600, 13, (Color){74, 58, 42, 200});
    }

    DrawText("v1.0", cx - 20, H - 40, 11, (Color){42, 32, 24, 200});
}

bool IsLoadingDone(void){
    return done;
}