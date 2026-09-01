#include "raylib.h"
#include "dialogue.h"
#include "player.h"
#include "tilemap.h"
#include "camera.h"
#include "scene_ch4_truth.h"
#include <string.h>

typedef struct{
    const char *title;
    const char *speaker;
    const char *text;
    const char *image;
}DialogueLine;

static DialogueLine lines[]={
    {"CHAPTER IV — Truth Burns", "Location", "Location: The Volcano Pass of Igrath", ""},
    {"CHAPTER IV — Truth Burns", "Narrator", "They don't speak on the climb through Igrath. The silence between them isn't hostile — it's the silence of two people who have spoken a true thing and are giving it room to settle.", ""},
    {"CHAPTER IV — Truth Burns", "Narrator", "Tonoy sets the pace. He always does. Sakib follows, and that too is an old habit, and they haven't decided yet whether old habits are comfort or a problem.", ""},
    {"CHAPTER IV — Truth Burns", "Sakib", "Do you hate me?", ""},
    {"CHAPTER IV — Truth Burns", "Narrator", "Tonoy doesn't break stride.", ""},
    {"CHAPTER IV — Truth Burns", "Tonoy", "No. I hate that you thought you had to carry this alone.", ""},
    {"CHAPTER IV — Truth Burns", "Sakib", "I was protecting you.", ""},
    {"CHAPTER IV — Truth Burns", "Tonoy", "I know. That's the part that makes it hard to be angry.", ""},
    {"CHAPTER IV — Truth Burns", "Narrator", "Rahim is sitting outside his hut when they arrive, as if he's been waiting for them for some time and there's nothing left to do while waiting. One leg is missing below the knee. He doesn't seem to notice anymore.", ""},
    {"CHAPTER IV — Truth Burns", "Narrator", "He looks at Tonoy the way people look at someone they've heard about for years and are finally meeting — checking whether the reality matches the description.", ""},
    {"CHAPTER IV — Truth Burns", "Rahim", "He talked about you the most, you know. Not Sakib. You.", ""},
    {"CHAPTER IV — Truth Burns", "Tonoy", "What did he say?", ""},
    {"CHAPTER IV — Truth Burns", "Rahim", "He said you were the kind of man who'd run back into a burning house — not out of courage, but because you can't leave something unfinished. He said that was your greatest strength, and the thing most likely to get you killed.", ""},
    {"CHAPTER IV — Truth Burns", "Tonoy", "He wasn't wrong.", ""},
    {"CHAPTER IV — Truth Burns", "Narrator", "Rahim tells them what happened five years ago. Sabin's return. The king's door. The words: dead men have no claims.", ""},
    {"CHAPTER IV — Truth Burns", "Narrator", "He's listening. He's also cataloguing. The king knew Sabin was alive. The king turned him away. Then the king sent Tonoy to stop him, knowing Tonoy knew none of it. The king weaponized Tonoy's trust against the one person Tonoy had always been searching for.", ""},
    {"CHAPTER IV — Truth Burns", "Narrator", "This is what betrayal looks like when it's done carefully. Not a knife in the back. A chain of small decisions, each deniable, that together build something that can't be undone.", ""},
    {"CHAPTER IV — Truth Burns", "Tonoy", "When he came back — did it seem like he wanted to destroy everything? Or did he want someone to tell him he didn't have to?", ""},
    {"CHAPTER IV — Truth Burns", "Narrator", "Rahim is quiet for a moment.", ""},
    {"CHAPTER IV — Truth Burns", "Rahim", "The second. Until the door closed.", ""},
    {"CHAPTER IV — Truth Burns", "Tonoy", "Then there's still something worth doing.", ""},
    {"CHAPTER IV — Truth Burns", "Narrator", "He stands. Checks his sword. Looks down at the lava field, where the cracked stone shows what's always burning underneath.", ""},
    {"CHAPTER IV — Truth Burns", "Sakib", "There's a pattern in the flow. Sabin taught me to read terrain like this, years ago — said a shield-bearer who can't read the ground is just a wall waiting to fall. The safe path isn't the straight one. It's the one that looks wrong but isn't.", ""},
    {"CHAPTER IV — Truth Burns", "Narrator", "He traces it with his eyes, then starts walking — confident, without waiting for confirmation. For once, he's leading.", ""},
    {"CHAPTER IV — Truth Burns", "Tonoy", "You kept this hidden. You're better at this than you let on.", ""},
    {"CHAPTER IV — Truth Burns", "Sakib", "I let you lead because you're good at deciding. I'm good at not dying. We make a good pair.", ""},
    {"CHAPTER IV — Truth Burns", "Tonoy", "We always have.", ""},
    {"CHAPTER IV — Truth Burns", "Rahim", "He's a tired man. Not an evil man. Learn to tell the difference.", ""},
    {"CHAPTER IV — Truth Burns", "Rahim", "The king sent you to stop Sabin. But if Sabin is stopped, the truth dies with him. Think carefully.", ""},
    {"CHAPTER IV — Truth Burns", "Rahim", "I love Sabin. But I don't have the power to save him. You do. Because you're not the ones who wounded him — the king did. But only you can show him that ending it all isn't the only path.", ""},
    {"CHAPTER IV — Truth Burns", "Narrator", "As they're leaving, Sakib says: Tonoy. If at Ashfeld I had left you behind instead — would that have been right? Tonoy stops. No. Then? A long silence. Even so, the right path was the other one. Sakib bows his head.", ""},
};

static int total = 31;
static int current = 0;
static const char *currentTitle = "";

#define MAX_TEXTURES 16
static Texture2D textureCache[MAX_TEXTURES];
static const char *textureNames[MAX_TEXTURES];
static int textureCount = 0;
static Texture2D currentBg;

typedef enum{
    CH4_DIALOGUE,
    CH4_TRUTH,
}Ch4State;

static Ch4State ch4State;
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

void InitDialogueCh4(void){
    current = 0;
    currentTitle = lines[0].title;
    currentBg = GetTexture(lines[0].image);
    ch4State = CH4_DIALOGUE;
}
static Camera2D cam;
void UpdateDialogueCh4(void){
    if(ch4State == CH4_DIALOGUE){
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
                ch4State = CH4_TRUTH;
            }
        }
    } 
    else{
        UpdatePlayer(&player);
        UpdateGameCamera(&cam, player.position);
    }
}

void DrawDialogueBoxCh4(void){
    if(ch4State == CH4_DIALOGUE){
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

int IsDialogueFinishedCh4(void){
    return(current == total - 1);
}

void CloseDialogueCh4(void){
    for(int i=0; i<textureCount; i++) UnloadTexture(textureCache[i]);
    if(ch4State == CH4_TRUTH){
        ClosePlayer(&player);
        CloseTilemap();
    }
}