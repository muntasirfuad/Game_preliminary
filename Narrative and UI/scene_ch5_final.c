#include "raylib.h"
#include "dialogue.h"
#include "player.h"
#include "tilemap.h"
#include "camera.h"
#include "scene_ch5_final.h"
#include <string.h>

typedef struct{
    const char *title;
    const char *speaker;
    const char *text;
    const char *image;
}DialogueLine;

static DialogueLine lines[]={
    {"CHAPTER V — The Forgotten Oath (Finale)", "Location", "Location: The Shattered Throne — Ashfeld", ""},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Narrator", "The road to Ashfeld is the same road it's always been. The stones are the same stones. The way the valley opens up past the last hill — Tonoy remembers it the way you remember the moment right before your life changed.", ""},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Narrator", "He's walked this in his mind so many times that the real place feels like a memory of a memory. Smaller than it should be. Quieter.", ""},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Tonoy", "He sang here. The night before the battle. His voice was terrible and he had no idea. He sang like he was alone in a room.", ""},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Sakib", "I remember.", ""},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Tonoy", "I used to laugh at him. Told him he'd wake the enemy. He'd say — 'Good. Let them come. I'm not hiding.'", ""},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Narrator", "A pause.", ""},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Tonoy", "I didn't understand what that meant until much later. He was never hiding from anything. Not even at the end. Not even when we left him behind.", ""},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Narrator", "Sabin stands on Ashfeld's broken dais when they arrive. He looks the way fifteen years of war, captivity, and abandonment look when they decide to keep walking anyway: broken, but upright. His eyes, when they find Tonoy, are the eyes of someone who has already decided not to be surprised by anything.", ""},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Narrator", "Then he's surprised.", ""},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Sabin", "You came.", ""},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Tonoy", "Did you think I wouldn't?", ""},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Sabin", "I thought you might. I didn't let myself believe it.", ""},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Tonoy", "I searched for you straight from Ashfeld. Three years, before I stopped. And when I stopped, it wasn't surrender. Someone convinced me you were gone.", ""},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Sabin", "The king.", ""},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Tonoy", "Yes. The king. We're going back to him.", ""},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Narrator", "He stops in front of Sabin. Close enough that there's no ambiguity about whether this is confrontation or reunion. He chooses reunion.", ""},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Tonoy", "You wrote my name on my door.", ""},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Sabin", "I wanted to know if you'd come.", ""},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Tonoy", "You already knew.", ""},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Sabin", "I needed to see it.", ""},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Narrator", "Tonoy turns to Sakib. This is the moment he's prepared for and feared. He isn't speaking to make peace — he's speaking to tell the truth, the thing neither of them has ever been able to say in front of Sabin.", ""},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Tonoy", "What Sakib did at Ashfeld — the king forced him to choose between you and me. He chose me. I'm not asking you to be fine with that. I'm asking you to understand it wasn't betrayal. It was a man forced to make an impossible decision in the dark, and he chose the one he thought he could live with.", ""},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Tonoy", "He didn't live with it. Not for one day. He's carried it every day since. If you want to be angry at someone, be angry at the one who handed him that choice.", ""},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Narrator", "Sabin looks at Sakib for a long time. Sakib doesn't look away. He's stopped looking away.", ""},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Sabin", "You were crying. At Ashfeld. I saw it. I didn't understand it then. I thought it meant you knew you were doing something wrong.", ""},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Sakib", "I knew I was doing something wrong. That's why I was crying.", ""},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Sabin", "I've spent fifteen years angry at the wrong person.", ""},
    {"CHAPTER V — The Forgotten Oath (Finale)", "Tonoy", "No. You were angry at the right person, just the wrong order. Fix the order. Start with the king.", ""},
};

static int total = 29;
static int current = 0;
static const char *currentTitle = "";

#define MAX_TEXTURES 16
static Texture2D textureCache[MAX_TEXTURES];
static const char *textureNames[MAX_TEXTURES];
static int textureCount = 0;
static Texture2D currentBg;

typedef enum{
    CH5_DIALOGUE,
    CH5_FINAL,
}Ch5State;

static Ch5State ch5State;
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

void InitDialogueCh5(void){
    current = 0;
    currentTitle = lines[0].title;
    currentBg = GetTexture(lines[0].image);
    ch5State = CH5_DIALOGUE;
}
static Camera2D cam;
void UpdateDialogueCh5(void){
    if(ch5State == CH5_DIALOGUE){
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
                ch5State = CH5_FINAL;
            }
        }
    } 
    else{
        UpdatePlayer(&player);
        UpdateGameCamera(&cam, player.position);
    }
}

void DrawDialogueBoxCh5(void){
    if(ch5State == CH5_DIALOGUE){
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

int IsDialogueFinishedCh5(void){
    return(current == total - 1);
}

void CloseDialogueCh5(void){
    for(int i=0; i<textureCount; i++) UnloadTexture(textureCache[i]);
    if(ch5State == CH5_FINAL){
        ClosePlayer(&player);
        CloseTilemap();
    }
}