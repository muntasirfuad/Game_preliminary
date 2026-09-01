#include "raylib.h"
#include "dialogue.h"
#include "player.h"
#include "tilemap.h"
#include "camera.h"
#include "ending_bad.h"
#include <string.h>

typedef struct{
    const char *title;
    const char *speaker;
    const char *text;
    const char *image;
}DialogueLine;

static DialogueLine lines[]={
    {"BAD ENDING — The Cycle Continues", "Narrator", "Sabin fights because he doesn't know how to do anything else anymore. Tonoy fights because someone has to.", ""},
    {"BAD ENDING — The Cycle Continues", "Narrator", "When it's over, Sabin lies on the ground, and what's in his eyes isn't defeat — it's the particular exhaustion of a man who spent fifteen years searching for something and finally stopped.", ""},
    {"BAD ENDING — The Cycle Continues", "Sabin", "I only wanted to know... why.", ""},
    {"BAD ENDING — The Cycle Continues", "Tonoy", "I know. I'm sorry I couldn't get here sooner.", ""},
    {"BAD ENDING — The Cycle Continues", "Narrator", "The curse breaks. Valdris is saved. The king holds a celebration.", ""},
    {"BAD ENDING — The Cycle Continues", "Narrator", "Tonoy doesn't go. He returns to Maren. He doesn't tear down the hut. He takes a stone from Ashfeld's broken dais and keeps it by his window, and every morning he looks at it, and every morning he knows exactly what it cost.", ""},
    {"BAD ENDING — The Cycle Continues", "Narrator", "Sakib visits once. They don't talk about what happened. They sit together in the dark for an hour, then Sakib leaves. They never speak again after that.", ""},
    {"BAD ENDING — The Cycle Continues", "Narrator", "The king keeps his throne. The pact stays buried. The three hundred people who died in the surrendered villages get no justice.", ""},
    {"BAD ENDING — The Cycle Continues", "Narrator", "And Tonoy spends the rest of his life knowing: he had exactly one chance to arrive in time, and he let a conversation come an hour too late.", ""},
    {"BAD ENDING — The Cycle Continues", "Narrator", "Valdris was saved. The king celebrated. The truth was buried again. And Tonoy spent the rest of his life carrying the weight of an answer that came an hour too late.", ""},
};

static int total = 10;
static int current = 0;
static const char *currentTitle = "";

#define MAX_TEXTURES 16
static Texture2D textureCache[MAX_TEXTURES];
static const char *textureNames[MAX_TEXTURES];
static int textureCount = 0;
static Texture2D currentBg;

typedef enum{
    CH_DIALOGUE_BAD,
    CH_BAD_ENDING,
}Ch_bad_State;

static Ch_bad_State ch_bad_state;
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

void InitDialogueBad(void){
    current = 0;
    currentTitle = lines[0].title;
    currentBg = GetTexture(lines[0].image);
    ch_bad_state = CH_DIALOGUE_BAD;
}
static Camera2D cam;
void UpdateDialogueBad(void){
    if(ch_bad_state == CH_DIALOGUE_BAD){
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
                ch_bad_state = CH_BAD_ENDING;
            }
        }
    } 
    else{
        UpdatePlayer(&player);
        UpdateGameCamera(&cam, player.position);
    }
}

void DrawDialogueBoxBad(void){
    if(ch_bad_state == CH_DIALOGUE_BAD){
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

int IsDialogueFinishedBad(void){
    return(current == total - 1);
}

void CloseDialogueBad(void){
    for(int i=0; i<textureCount; i++) UnloadTexture(textureCache[i]);
    if(ch_bad_state == CH_BAD_ENDING){
        ClosePlayer(&player);
        CloseTilemap();
    }
}