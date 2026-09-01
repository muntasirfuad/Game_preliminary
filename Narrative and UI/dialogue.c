#include "raylib.h"
#include "dialogue.h"
#include <string.h>

typedef struct{
    const char *title;
    const char *speaker;
    const char *text;
    const char *image;
}DialogueLine;

static DialogueLine lines[]={
    {"THE FORGOTTEN OATH", "Chronicles of the Fallen Throne", "", "scene_door.png"},
    {"", "PROLOGUE — The Last Name on the Door", "Location: Kingdom of Valdris — Present Day", "scene_door.png"},
    {"The Last Name on the Door", "Narration", "Some people carry the war inside them even after the battlefield has gone quiet. Tonoy is one of them.", "scene_door.png"},
    {"The Last Name on the Door", "Narration", "He doesn't talk about Ashfeld. He doesn't need to.  It lives in the way he holds his sword — not beside him, but slightly forward, as though the enemy is always one step ahead. It lives in the way he wakes before dawn, sitting by the window staring into the dark — as if waiting for the darkness itself to explain something.", "scene_door.png"},
    {"The Last Name on the Door", "Narration", "For fifteen years, Valdris has called the Battle of Ashfeld a 'victory'. Tonoy has never once used that word.", "scene_door.png"},
    {"The Last Name on the Door", "Narration", "Five thousand soldiers marched into that valley. Two thousand returned. The king held a parade. Tonoy didn't go. He stood at the city gate counting the faces that were missing, and when he finished counting those, he started counting the ones who came back but came back 'wrong' — quieter, emptier, trembling for no reason.", "scene_door.png"},
    {"The Last Name on the Door", "Narration", "He left the king's service not out of anger, but something worse: certainty. The certainty that the ones who gave the orders would never lose a night's sleep over the people who carried them out.", "scene_door.png"},
    {"The Last Name on the Door", "Narration", "He built a small hut near the forest of Maren. Every morning he sharpened his sword. He was waiting for something — though he himself couldn't say what.", "scene_door.png"},
    {"The Last Name on the Door", "Narration", "Then one morning he opened the door and saw it — his own name written in the mud. The letters careful. Deliberate. The writing still wet.", "scene_door.png"},
    {"The Last Name on the Door", "Narration", "Tonoy looked at it for a long time. He didn't call for Sakib. He didn't go back inside. He just knelt and studied the letters, and quietly said to no one:", "scene_door.png"},
    {"The Last Name on the Door", "Tonoy", "So you're alive.", "tonoy.png"},
    {"The Last Name on the Door", "Narration", "He had always suspected.", "tonoy.png"},
    {"The Last Name on the Door", "Tonoy", "--- --- ---", "tonoy.png"},
    {"The Last Name on the Door", "[Sabin — now seen only in shadow]", "Somewhere beyond the row of trees, a shadowy figure stops walking. Turns. Looks toward the light in Tonoy's window. After fifteen years of enemy captivity — fifteen years of silence, darkness, and a king who declared him dead — Sabin has learned one thing with total clarity: Tonoy was never the enemy.", "sabin.png"},
    {"The Last Name on the Door", "[Sakib — the weight he carries]", "Sakib learns about the name on the door the same way he learns most things: Tonoy tells him, and he goes completely still. His face does the thing it always does — reveals nothing. He's practiced this for fifteen years. He's had to.", "sakib.png"},
    {"PROLOGUE — The Last Name on the Door", "Location: Kingdom of Valdris — Present Day", "--THE CHRONICLES OF THE FALLEN THRONE--", "scene_door.png"},
    {"PROLOGUE — The Last Name on the Door", "Location: Kingdom of Valdris — Present Day", "The Journey Begins with us. 3 Charecters, 5 Chapters...", "scene_door.png"},
};

static int total = 17;
static int current = 0;
static const char *currentTitle = "";

#define MAX_TEXTURES 16
static Texture2D textureCache[MAX_TEXTURES];
static const char *textureNames[MAX_TEXTURES];
static int textureCount = 0;
static Texture2D currentBg;

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
    char buffer[1024];
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

void InitDialogue(void){
    current = 0;
    currentTitle = lines[0].title;
    currentBg = GetTexture(lines[0].image);
}

void UpdateDialogue(void){
    if(IsKeyPressed(KEY_SPACE)){
        if(current<total-1){
            current++;
            if(lines[current].title[0] != '\0') currentTitle = lines[current].title;
            currentBg = GetTexture(lines[current].image);
        }
    }
}

void DrawDialogueBox(void){
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

int IsDialogueFinished(void){
    return(current == total - 1);
}

void CloseDialogue(void){
    for(int i=0; i<textureCount; i++) UnloadTexture(textureCache[i]);
}