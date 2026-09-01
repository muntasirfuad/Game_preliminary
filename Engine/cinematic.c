#include "raylib.h"
#include "cinematic.h"
#include <math.h>
#include <string.h>

#define MAX_FIRE_PARTICLES 80
#define MAX_SPARKS 8
#define QUOTE_COUNT 7

typedef struct {
    Vector2 pos;
    Vector2 vel;
    float life;
    float maxLife;
    float size;
    float hue;
} FireParticle;

typedef struct {
    Vector2 pos;
    float life;
} Spark;

typedef struct {
    const char *line1;
    const char *line2;
    float holdTime;
} Quote;

static Quote quotes[QUOTE_COUNT] = {
    {"The dead do not remember.", "That is why the living must.", 4.0f},
    {"A war does not end when the swords are sheathed.", "It ends when the last person stops carrying it.", 4.5f},
    {"Truth is not loud.", "It is the thing that remains when everything else has been shouted away.", 4.5f},
    {"Fifteen years is not a measurement of time.", "It is a measurement of how long a man can hold his breath.", 4.5f},
    {"The king writes history with the blood of those", "who were never given a pen.", 4.0f},
    {"Some doors open outward. Some open inward.", "The hardest ones open in places you stopped looking.", 4.5f},
    {"To forget is a choice. To remember is a burden.", "To act on memory — that is something else entirely.", 5.0f},
};

static FireParticle fire[MAX_FIRE_PARTICLES];
static Spark sparks[MAX_SPARKS];
static int currentQuote;
static float phaseTimer;
static int phase; // 0=fade_in, 1=hold, 2=fade_out, 3=done
static float torchFlicker;
static bool finished;
static bool skipped;

void InitCinematic(void){
    currentQuote = 0;
    phaseTimer = 0.0f;
    phase = 0;
    torchFlicker = 0.0f;
    finished = false;
    skipped = false;

    // Init fire particles around torch position
    for(int i=0; i<MAX_FIRE_PARTICLES; i++){
        fire[i].pos = (Vector2){180 + GetRandomValue(-30, 50), 280 + GetRandomValue(0, 60)};
        fire[i].vel = (Vector2){(GetRandomValue(-10, 10)/10.0f), -(GetRandomValue(15, 45)/10.0f)};
        fire[i].life = GetRandomValue(20, 100)/50.0f;
        fire[i].maxLife = fire[i].life;
        fire[i].size = GetRandomValue(2, 8)/2.0f;
        fire[i].hue = GetRandomValue(5, 35);
    }

    for(int i=0; i<MAX_SPARKS; i++){
        sparks[i].pos = (Vector2){180 + GetRandomValue(-20, 20), 240 + GetRandomValue(-20, 10)};
        sparks[i].life = GetRandomValue(10, 40)/20.0f;
    }
}

static void UpdateFire(float dt){
    torchFlicker += dt * 8.0f;

    for(int i=0; i<MAX_FIRE_PARTICLES; i++){
        fire[i].pos.x += fire[i].vel.x + sinf(torchFlicker * 2.0f + fire[i].pos.y * 0.05f) * 0.5f;
        fire[i].pos.y += fire[i].vel.y;
        fire[i].life -= dt;
        fire[i].size *= 0.995f;

        if(fire[i].life <= 0){
            fire[i].pos = (Vector2){180 + GetRandomValue(-25, 45), 290 + GetRandomValue(0, 20)};
            fire[i].vel = (Vector2){(GetRandomValue(-10, 10)/10.0f), -(GetRandomValue(15, 45)/10.0f)};
            fire[i].life = GetRandomValue(30, 100)/50.0f;
            fire[i].maxLife = fire[i].life;
            fire[i].size = GetRandomValue(2, 10)/2.0f;
        }
    }

    for(int i=0; i<MAX_SPARKS; i++){
        sparks[i].pos.y -= dt * 40.0f;
        sparks[i].life -= dt;
        if(sparks[i].life <= 0){
            sparks[i].pos = (Vector2){180 + GetRandomValue(-15, 15), 250 + GetRandomValue(-10, 5)};
            sparks[i].life = GetRandomValue(15, 50)/20.0f;
        }
    }
}

void UpdateCinematic(void){
    if(finished) return;

    float dt = GetFrameTime();
    UpdateFire(dt);

    // Skip
    if(!skipped && (IsKeyPressed(KEY_SPACE) || IsKeyPressed(KEY_ENTER) || IsMouseButtonPressed(MOUSE_LEFT_BUTTON))){
        skipped = true;
        finished = true;
        return;
    }

    phaseTimer += dt;

    float fadeTime = 1.5f;

    if(phase == 0){ // fade in
        if(phaseTimer >= fadeTime){
            phaseTimer = 0;
            phase = 1;
        }
    }
    else if(phase == 1){ // hold
        if(phaseTimer >= quotes[currentQuote].holdTime){
            phaseTimer = 0;
            phase = 2;
        }
    }
    else if(phase == 2){ // fade out
        if(phaseTimer >= fadeTime){
            phaseTimer = 0;
            phase = 0;
            currentQuote++;
            if(currentQuote >= QUOTE_COUNT){
                finished = true;
            }
        }
    }
}

static void DrawTorch(int W, int H){
    // Torch stick
    DrawRectangle(175, (int)(H * 0.55f), 10, 120, (Color){42, 26, 10, 255});
    // Torch head
    DrawCircle(180, (int)(H * 0.55f), 12, (Color){58, 42, 21, 255});

    // Glow on wall
    float glowR = 140 + sinf(torchFlicker) * 35 + sinf(torchFlicker * 2.3f) * 18;
    float glowA = 0.06f + sinf(torchFlicker * 1.7f) * 0.02f;

    // Manual radial gradient approximation
    for(int r = (int)glowR; r > 0; r -= 8){
        float t = (float)r / glowR;
        Color c = (Color){
            (unsigned char)(255 * (1-t) * glowA * 3),
            (unsigned char)(160 * (1-t) * glowA * 3),
            (unsigned char)(40 * (1-t) * glowA * 3),
            (unsigned char)(255 * (1-t) * 0.3f)
        };
        DrawCircle(180, (int)(H * 0.45f), r, c);
    }

    // Fire particles
    for(int i=0; i<MAX_FIRE_PARTICLES; i++){
        float alpha = fminf(1.0f, fire[i].life / fire[i].maxLife);
        int hue = fire[i].hue + (int)(sinf(torchFlicker * 4.0f) * 8);

        // Simple color from hue: orange to yellow
        Color fc;
        if(hue < 20){
            fc = (Color){255, 120 + (int)(alpha * 80), 30, (unsigned char)(255 * alpha * 0.8f)};
        } else {
            fc = (Color){255, 200 + (int)(alpha * 55), 80, (unsigned char)(255 * alpha * 0.7f)};
        }

        DrawCircle((int)fire[i].pos.x, (int)fire[i].pos.y, fire[i].size * alpha, fc);
    }

    // Sparks
    for(int i=0; i<MAX_SPARKS; i++){
        float sa = 0.3f + sinf(torchFlicker * 5.0f + i) * 0.2f;
        DrawRectangle((int)sparks[i].pos.x, (int)sparks[i].pos.y, 2, 2,
            (Color){255, 220, 120, (unsigned char)(255 * sa * sparks[i].life)});
    }
}

static void DrawCave(int W, int H){
    // Dark cave background
    ClearBackground((Color){8, 6, 4, 255});

    // Cave wall texture lines
    DrawLine(0, (int)(H*0.30f), (int)(W*0.10f), (int)(H*0.25f), (Color){26, 18, 10, 255});
    DrawLine((int)(W*0.10f), (int)(H*0.25f), (int)(W*0.20f), (int)(H*0.35f), (Color){26, 18, 10, 255});
    DrawLine((int)(W*0.20f), (int)(H*0.35f), (int)(W*0.30f), (int)(H*0.20f), (Color){26, 18, 10, 255});
    DrawLine((int)(W*0.30f), (int)(H*0.20f), (int)(W*0.50f), (int)(H*0.30f), (Color){26, 18, 10, 255});
    DrawLine((int)(W*0.50f), (int)(H*0.30f), (int)(W*0.70f), (int)(H*0.22f), (Color){26, 18, 10, 255});
    DrawLine((int)(W*0.70f), (int)(H*0.22f), (int)(W*0.85f), (int)(H*0.32f), (Color){26, 18, 10, 255});
    DrawLine((int)(W*0.85f), (int)(H*0.32f), W, (int)(H*0.28f), (Color){26, 18, 10, 255});

    // Floor
    DrawRectangle(0, (int)(H*0.75f), W, (int)(H*0.25f), (Color){10, 7, 5, 255});
    DrawRectangle(0, (int)(H*0.75f), W, 3, (Color){18, 13, 8, 255});

    // Shadow figure (Tonoy silhouette, far right)
    DrawEllipse((int)(W*0.78f), (int)(H*0.72f), 25, 55, (Color){6, 4, 3, 255});
    DrawCircle((int)(W*0.78f), (int)(H*0.58f), 12, (Color){6, 4, 3, 255});
    // Sword hint
    DrawLine((int)(W*0.78f + 15), (int)(H*0.65f), (int)(W*0.78f + 35), (int)(H*0.45f), (Color){10, 8, 6, 255});

    // Torch
    DrawTorch(W, H);
}

static void DrawQuote(int W, int H){
    if(finished) return;

    float fadeTime = 1.5f;
    float alpha = 1.0f;

    if(phase == 0){
        alpha = phaseTimer / fadeTime;
    }
    else if(phase == 2){
        alpha = 1.0f - (phaseTimer / fadeTime);
    }

    alpha = fmaxf(0.0f, fminf(1.0f, alpha));

    unsigned char a = (unsigned char)(255 * alpha);
    Color textCol = {196, 168, 130, a};

    const char *l1 = quotes[currentQuote].line1;
    const char *l2 = quotes[currentQuote].line2;

    int x1 = W/2 - MeasureText(l1, 22)/2;
    int x2 = W/2 - MeasureText(l2, 22)/2;

    // Shadow
    DrawText(l1, x1+2, (int)(H*0.58f)+2, 22, (Color){0,0,0,(unsigned char)(a*0.5f)});
    DrawText(l2, x2+2, (int)(H*0.58f)+32, 22, (Color){0,0,0,(unsigned char)(a*0.5f)});

    // Text
    DrawText(l1, x1, (int)(H*0.58f), 22, textCol);
    DrawText(l2, x2, (int)(H*0.58f) + 32, 22, textCol);

    // Skip hint (bottom right)
    if(phaseTimer > 1.0f || phase > 0){
        DrawText("[SPACE to skip]", W - 160, H - 40, 14, (Color){74, 58, 42, (unsigned char)(120 * alpha)});
    }
}

void DrawCinematic(void){
    int W = GetScreenWidth();
    int H = GetScreenHeight();

    DrawCave(W, H);
    DrawQuote(W, H);

    // Vignette
    DrawRectangle(0, 0, W, 60, (Color){0, 0, 0, 100});
    DrawRectangle(0, H-60, W, 60, (Color){0, 0, 0, 100});
}

bool IsCinematicDone(void){
    return finished;
}