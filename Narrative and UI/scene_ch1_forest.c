#include "raylib.h"
#include "dialogue.h"
#include "player.h"
#include "tilemap.h"
#include "camera.h"
#include "scene_ch1_forest.h"
#include <string.h>

typedef struct{
    const char *title;
    const char *speaker;
    const char *text;
    const char *image;
}DialogueLine;

static DialogueLine lines[]={
    {"CHAPTER I — The King's Call", "Location: The Dark Forest of Maren", "", "The_Dark_Forest_of_Maren.png"},
    {"CHAPTER I — The King's Call", "Narrator", "The messenger arrives at dawn, and the first thing Tonoy notices is that the man won't meet his eyes.", "letter.png"},
    {"CHAPTER I — The King's Call", "Narrator", "That alone tells him everything. A messenger who looks you in the eye brings news. A messenger who looks at the ground brings orders someone else is ashamed of.", "letter.png"},
    {"CHAPTER I — The King's Call", "Narrator", "He reads the letter once. Sets it down. Picks it up and reads it again — not because the words are unclear, but because he's deciding something.", "letter.png"},
    {"CHAPTER I — The King's Call", "Narrator", "Three villages. Ash. No bodies. The king wants answers.", "letter.png"},
    {"CHAPTER I — The King's Call", "Narrator", "The king always wants something from Tonoy, and it always costs Tonoy more than it costs the king.", "read.png"},
    {"CHAPTER I — The King's Call", "[Tonoy — to himself]", "He thinks: the king doesn't call me when things are under control. He calls me when something has gone wrong and he needs someone to stand in front of the damage. I'm not being asked to fix this. I'm being asked to absorb it.", "read.png"},
    {"CHAPTER I — The King's Call", "Narrator", "He rolls up the letter. Looks at Sakib.", "thinks.png"},
    {"CHAPTER I — The King's Call", "Tonoy", "We leave at dawn", "look.png"},
    {"CHAPTER I — The King's Call", "Narrator", "Not a question. Not a request. A statement of something already decided.", "look.png"},
    {"CHAPTER I — The King's Call", "Narrator", "[Sakib nods immediately. A little too immediately.]", "look.png"},
    {"CHAPTER I — The King's Call", "Tonoy", "[Notices] Files it away. Says nothing.", "look.png"},
    {"CHAPTER I — The King's Call", "Narrator", "The forest of Maren does not welcome people. The leaves are so thick that even noon looks like dusk. The air smells of old bark and something older still — something that has been slowly rotting for centuries and is in no hurry.", "talk.png"},
    {"CHAPTER I — The King's Call", "Narrator", "Tonoy moves through it the way he moves through everything: no hesitation, no waste. He's learned to read the ground like a sentence. These footprints — heavy, one direction, no sign of return — are a sentence that ends badly.", "foot.png"},
    {"CHAPTER I — The King's Call", "Tonoy", "[Quitely] Someone came through here bleeding. Three, four days ago. They weren't fleeing. They were choosing where to go.", "foot.png"},
    {"CHAPTER I — The King's Call", "Sakib", "[Looks at the footprints]", "foot.png"},
    {"CHAPTER I — The King's Call", "Sakib", "You can tell all that?", "foot.png"},
    {"CHAPTER I — The King's Call", "Tonoy", "I've spent fifteen years looking for something that didn't want to be found. You learn to read what the ground remembers.", "talk.png"},
    {"CHAPTER I — The King's Call", "Narrator", "The burned hut sits in the middle of the forest like an apology. On the charred door: a mark. A sword, and above it, a broken shield.", "door.png"},
    {"CHAPTER I — The King's Call", "Narrator", "Tonoy goes completely still.", "door.png"},
    {"CHAPTER I — The King's Call", "Narrator", "He's seen this mark exactly once before — carved into the hilt of a sword that a man named Sabin once put in his hand before the march to Ashfeld.", "door.png"},
    {"CHAPTER I — The King's Call", "Sabin", "If we're ever separated, look for this mark. It means I'm near.", "sabin.png"},
    {"CHAPTER I — The King's Call", "Narrator", "They were separated. Tonoy never found the mark. He searched for three years.", "door.png"},
    {"CHAPTER I — The King's Call", "Narrator", "He turns to Sakib. Sakib's face has that particular emptiness that only happens when something is being carefully held back.", "conv.png"},
    {"CHAPTER I — The King's Call", "Tonoy", "[calm, precise]", "conv.png"},
    {"CHAPTER I — The King's Call", "Tonoy", "When were you going to tell me?", "conv.png"},
    {"CHAPTER I — The King's Call", "Sakib", "Tell you what?", "conv.png"},
    {"CHAPTER I — The King's Call", "Tonoy", "What you already know.", "conv.png"},
    {"CHAPTER I — The King's Call", "Narrator", "Silence. The forest holds its breath.", "conv.png"},
    {"CHAPTER I — The King's Call", "Sakib", "Let's keep moving.", "conv.png"},
    {"CHAPTER I — The King's Call", "Narrator", "Tonoy looks at him for a full three seconds. Then turns and walks into the hut.", "conv.png"},
    {"CHAPTER I — The King's Call", "[Observer — what Tonoy doesn't say]", "He isn't angry. Anger is for people who are surprised.", "observer.png"},
    {"CHAPTER I — The King's Call", "[Observer — what Tonoy doesn't say]", "Tonoy has known for years that Sakib is carrying something he hasn't been told to put down yet.", "observer.png"},
    {"CHAPTER I — The King's Call", "[Observer — what Tonoy doesn't say]", "He was waiting. He can wait a little longer. But not much longer.", "observer.png"},
    {"CHAPTER I — The King's Call", "", "Now Choose Your Decision Wisely", "chapter1.png"},
};

static int total = 35;
static int current = 0;
static const char *currentTitle = "";

#define MAX_TEXTURES 16
static Texture2D textureCache[MAX_TEXTURES];
static const char *textureNames[MAX_TEXTURES];
static int textureCount = 0;
static Texture2D currentBg;

typedef enum{
    CH1_DIALOGUE,
    CH1_FOREST
}Ch1State;

static Ch1State ch1State;
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

void InitDialogueCh1(void){
    current = 0;
    currentTitle = lines[0].title;
    currentBg = GetTexture(lines[0].image);
    ch1State = CH1_DIALOGUE;
}
static Camera2D cam;
void UpdateDialogueCh1(void){
    if(ch1State == CH1_DIALOGUE){
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
                ch1State = CH1_FOREST;
            }
        }
    } 
    else{
        UpdatePlayer(&player);
        UpdateGameCamera(&cam, player.position);
    }
}

void DrawDialogueBoxCh1(void){
    if(ch1State == CH1_DIALOGUE){
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

int IsDialogueFinishedCh1(void){
    return(current == total - 1);
}

void CloseDialogueCh1(void){
    for(int i=0; i<textureCount; i++) UnloadTexture(textureCache[i]);
    if(ch1State == CH1_FOREST){
        ClosePlayer(&player);
        CloseTilemap();
    }
}