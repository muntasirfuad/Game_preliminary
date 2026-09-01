#include "raylib.h"
#include "dialogue.h"
#include "player.h"
#include "tilemap.h"
#include "camera.h"
#include "ending_good.h"
#include <string.h>

typedef struct{
    const char *title;
    const char *speaker;
    const char *text;
    const char *image;
}DialogueLine;

static DialogueLine lines[]={
    {"GOOD ENDING — Confession", "Narrator", "Tonoy speaks. Not loudly, not dramatically. He speaks the way he does everything — with precision, because the words matter and he has no wish to waste them.", ""},
    {"GOOD ENDING — Confession", "Tonoy", "Sakib didn't abandon you. He was given a choice with a gun to his head. He chose wrong and he's known it was wrong every day since. That's not betrayal. Betrayal is what the king did. Betrayal is calm and comfortable, and holds a parade afterward.", ""},
    {"GOOD ENDING — Confession", "Narrator", "Sabin listens. Sabin, who hasn't truly listened to anyone in fifteen years, listens.", ""},
    {"GOOD ENDING — Confession", "Narrator", "Then he sits down on Ashfeld's broken dais, and something leaves him — not life, but the particular thing that had kept him standing despite absolute ruin. For the first time he shows what he actually is: an exhausted, aging man who has survived on the wrong kind of strength for far too long.", ""},
    {"GOOD ENDING — Confession", "Sabin", "I'm so tired.", ""},
    {"GOOD ENDING — Confession", "Tonoy", "I know. You can stop now.", ""},
    {"GOOD ENDING — Confession", "Narrator", "The curse breaks quietly. Sabin closes his eyes on the same ground where he was abandoned fifteen years ago, and this time the two soldiers standing guard don't leave.", ""},
    {"GOOD ENDING — Confession", "Narrator", "Tonoy and Sakib take Sabin's evidence to the king's court. They don't announce themselves. They don't make speeches. Tonoy sets the papers on the council table and says:", ""},
    {"GOOD ENDING — Confession", "Tonoy", "Three hundred people. Their names are in the second appendix. I suggest you read it before deciding how to respond.", ""},
    {"GOOD ENDING — Confession", "Narrator", "The king loses his throne before winter.", ""},
    {"GOOD ENDING — Confession", "Narrator", "Some wounds don't heal. But the truth, once spoken, can't be buried again. Valdris didn't celebrate. For the first time in fifteen years, it mourned honestly.", ""},
};

static int total = 11;
static int current = 0;
static const char *currentTitle = "";

#define MAX_TEXTURES 16
static Texture2D textureCache[MAX_TEXTURES];
static const char *textureNames[MAX_TEXTURES];
static int textureCount = 0;
static Texture2D currentBg;

typedef enum{
    CH_DIALOGUE_GOOD,
    CH_GOOD_ENDING,
}Ch_good_State;

static Ch_good_State ch_good_state;
static Player player;

static Texture2D GetTexture(const char *name){
    if(name[0] == '\0') return (Texture2D){0};
    for(int i=0; i<textureCount; i++){
        if(strcmp(textureNames[i], name)==0) return textureCache[i];
    }
    textureCache[textureCount] = LoadTexture(name);
    textureNames[textureCount] = name;
    textureCount++;
    return textureCache[textureCount-1];
}

static void DrawWrapped(const char *text, int x, int y, int maxWidth, int fontSize, Color color){
    char buffer[4096];
    int lineY = y;
    int lineStart = 0;
    int lastSpace = -1;
    int len = strlen(text);

    for(int i=0; i<=len; i++){
        if(text[i]==' ' || text[i]=='\0'){
            int testLen = i - lineStart;
            if(testLen>0 && testLen<(int)sizeof(buffer)){
                strncpy(buffer, text+lineStart, testLen);
                buffer[testLen] = '\0';
                if(MeasureText(buffer, fontSize) > maxWidth && lastSpace > lineStart){
                    int cut = lastSpace - lineStart;
                    strncpy(buffer, text+lineStart, cut);
                    buffer[cut] = '\0';
                    DrawText(buffer, x, lineY, fontSize, color);
                    lineY += fontSize + 8;
                    lineStart = lastSpace + 1;
                }
            }
            lastSpace = i;
        }
    }
    if(lineStart < len) DrawText(text+lineStart, x, lineY, fontSize, color);
}

void InitDialogueGood(void){
    current = 0;
    currentTitle = lines[0].title;
    currentBg = GetTexture(lines[0].image);
    ch_good_state = CH_DIALOGUE_GOOD;
}
static Camera2D cam;
void UpdateDialogueGood(void){
    if(ch_good_state == CH_DIALOGUE_GOOD){
        if(IsKeyPressed(KEY_SPACE)){
                if(current < total-1){
                current++;
                if(lines[current].title[0] != '\0') currentTitle = lines[current].title;
                currentBg = GetTexture(lines[current].image);
            } 
            else{
                InitTilemap();
                InitPlayer(&player);
                InitGameCamera(&cam); 
                ch_good_state = CH_GOOD_ENDING;
            }
        }
    } 
    else{
        UpdatePlayer(&player);
        UpdateGameCamera(&cam, player.position);
    }
}

void DrawDialogueBoxGood(void){
    if(ch_good_state == CH_DIALOGUE_GOOD){
        DrawTexture(currentBg, 0, 0, WHITE);

        DrawRectangle(40, 20, 1200, 50, BLACK);
        DrawRectangleLines(40, 20, 1200, 50, MAGENTA);
        DrawText(currentTitle, 70, 35, 24, YELLOW);

        DrawRectangle(40, 500, 1200, 180, BLACK);
        DrawRectangleLines(40, 500, 1200, 180, MAGENTA);

        DrawText(lines[current].speaker, 70, 515, 26, RED);
        DrawWrapped(lines[current].text, 70, 550, 1140, 22, WHITE);

        DrawText("[Press SPACE]", 1080, 655, 18, PURPLE);
    } 
    else{ 
        BeginMode2D(cam);
        DrawTilemap();
        DrawPlayer(&player);
        EndMode2D();
    }
}

int IsDialogueFinishedGood(void){
    return(current == total - 1);
}

void CloseDialogueGood(void){
    for(int i=0; i<textureCount; i++) UnloadTexture(textureCache[i]);
    if(ch_good_state == CH_GOOD_ENDING){
        ClosePlayer(&player);
        CloseTilemap();
    }
}