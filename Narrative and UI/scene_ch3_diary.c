#include "raylib.h"
#include "dialogue.h"
#include "player.h"
#include "tilemap.h"
#include "camera.h"
#include "scene_ch3_diary.h"
#include <string.h>


typedef struct{
    const char *title;
    const char *speaker;
    const char *text;
    const char *image;
}DialogueLine;

static DialogueLine lines[]={
    {"CHAPTER III — The Diary Pages", "Location", "Location: The Ancient Dungeon of Vel'Mora", ""},
    {"CHAPTER III — The Diary Pages", "Narrator", "Above the cave entrance, written in a tongue older than Valdris itself: he who comes seeking truth shall no longer be alone.", ""},
    {"CHAPTER III — The Diary Pages", "Narrator", "Tonoy reads it twice. Something about it aches.", ""},
    {"CHAPTER III — The Diary Pages", "Narrator", "Alone. Not in the sense of who stands beside you — he's been alone that way for fifteen years. Since Ashfeld. Since that morning he walked away from the battlefield carrying an answer he was never given, and a question no one ever let him ask.", ""},
    {"CHAPTER III — The Diary Pages", "Narrator", "He walks in without waiting for Sakib.", ""},
    {"CHAPTER III — The Diary Pages", "Narrator", "The paintings on the cave wall stop Tonoy cold.", ""},
    {"CHAPTER III — The Diary Pages", "Narrator", "He's seen historical paintings before — clean, arranged things where heroes stand in front and chaos is tastefully arranged behind. These are not that. These were painted by someone who was there. Someone who remembers the smell. The confusion. The truth that in the moment, you can't always tell which way is forward.", ""},
    {"CHAPTER III — The Diary Pages", "Narrator", "He walks the length of the wall slowly, reading it like a report.", ""},
    {"CHAPTER III — The Diary Pages", "Narrator", "Then he reaches the final panel.", ""},
    {"CHAPTER III — The Diary Pages", "Narrator", "A commander, alone. Surrounded. And behind him — two soldiers walking away. Their faces blank — not left out of mercy, but because the painter himself couldn't finish them. On one soldier's armour: a sword. On the other's: a shield.", ""},
    {"CHAPTER III — The Diary Pages", "Narrator", "Tonoy stands in front of it for a long time.", ""},
    {"CHAPTER III — The Diary Pages", "Narrator", "He painted us. He was dying, surrounded, and he still had enough of himself left to paint this. What does that mean? Was it an accusation? A record? Was it — and this is what hurts him most — forgiveness? Is this what a man does with the last of himself, when he decides not to blame the ones who couldn't save him?", ""},
    {"CHAPTER III — The Diary Pages", "Narrator", "He looks away. His jaw is tight. His eyes are dry. He's learned to process things in stages, and this isn't the moment for grief. The moment for grief comes later, when there's no more work left to do.", ""},
    {"CHAPTER III — The Diary Pages", "Sakib", "He used to say painting was the only thing that made him feel like he wasn't a soldier. He'd do it in the middle of campaigns. Small things. Mountains. A bowl of fruit. I used to tease him about it.", ""},
    {"CHAPTER III — The Diary Pages", "Tonoy", "I didn't know that.", ""},
    {"CHAPTER III — The Diary Pages", "Sakib", "There's a lot you didn't know. Not because you weren't paying attention. I never told you. He showed me things he never showed anyone — because I was the quiet one. People talk to quiet people. It's its own kind of weight, Tonoy. What people hand you, you carry.", ""},
    {"CHAPTER III — The Diary Pages", "Narrator", "For the first time, Tonoy turns and actually looks at him — not as the friend who let him down, but as someone who's been carrying an entirely different war this whole time.", ""},
    {"CHAPTER III — The Diary Pages", "Mitu", "Tonoy. Sakib. Sabin wrote about you both. Often. More toward the end.", ""},
    {"CHAPTER III — The Diary Pages", "Tonoy", "How long have you been here?", ""},
    {"CHAPTER III — The Diary Pages", "Mitu", "Long enough to know that whoever wrote these pages wasn't what the king said he was. And long enough to know that what he's doing now isn't what he wanted at the start.", ""},
    {"CHAPTER III — The Diary Pages", "Narrator", "The diary pages, once decoded, read:", ""},
    {"CHAPTER III — The Diary Pages", "Narrator", "I spent a long time trying to decide whether to hate them. That would have been easier. Hatred has a shape — you can carry it, set it down, pick it up again. What I have has no shape. I watched them walk away and thought: Sakib knows. Sakib knows exactly what's happening and he's leaving anyway. Then I thought: and he's crying. Then I thought: Tonoy doesn't know. Tonoy has never known. Tonoy is walking around with a wound from Ashfeld that still has no name.", ""},
    {"CHAPTER III — The Diary Pages", "Narrator", "I decided I didn't want them to know the truth. Not for my own sake. Because it would hurt Tonoy, and Tonoy has already suffered enough for something that wasn't his fault, and I don't want to be another reason.", ""},
    {"CHAPTER III — The Diary Pages", "Narrator", "Tonoy reads it twice. Then sets it down very carefully — the way you set down something you've just realized you've been holding wrong for a long time.", ""},
    {"CHAPTER III — The Diary Pages", "Narrator", "He turns to Sakib.", ""},
    {"CHAPTER III — The Diary Pages", "Narrator", "He doesn't raise his voice. He's never raised his voice at Sakib. He has no wish to start now.", ""},
    {"CHAPTER III — The Diary Pages", "Tonoy", "Sakib.", ""},
    {"CHAPTER III — The Diary Pages", "Sakib", "Tonoy—", ""},
    {"CHAPTER III — The Diary Pages", "Tonoy", "Just tell me. That's all. Just tell me the truth.", ""},
    {"CHAPTER III — The Diary Pages", "Narrator", "And the silence that follows is the longest silence in fifteen years.", ""},
    {"CHAPTER III — The Diary Pages", "Sakib", "The king called me the night of the final attack. Said Sabin had found proof of the pact. Said Sabin had to be left behind. I said no. He said — 'Then Tonoy goes too.' And I — I couldn't. I couldn't let that happen to you. So I did what I was told. And I haven't been able to look at Ashfeld on a map since.", ""},
    {"CHAPTER III — The Diary Pages", "Tonoy", "You should have told me.", ""},
    {"CHAPTER III — The Diary Pages", "Sakib", "I know.", ""},
    {"CHAPTER III — The Diary Pages", "Tonoy", "I would have found another way.", ""},
    {"CHAPTER III — The Diary Pages", "Sakib", "I know that too. That's why I didn't tell you.", ""},
    {"CHAPTER III — The Diary Pages", "Narrator", "Tonoy stands still. Walks to the other side of the room. Stands with his back to everything.", ""},
    {"CHAPTER III — The Diary Pages", "Narrator", "This is the moment Tonoy has been walking toward for fifteen years without knowing it. The answer to the question he could never ask. And now that he has it, it doesn't feel like the relief he expected. It feels like standing in a room where something heavy has just been set down, and realizing the room is smaller than you thought, and you don't know how to breathe in it.", ""},
    {"CHAPTER III — The Diary Pages", "Narrator", "He isn't angry at Sakib.", ""},
    {"CHAPTER III — The Diary Pages", "Narrator", "He is furious at the king.", ""},
};

static int total = 39;
static int current = 0;
static const char *currentTitle = "";

#define MAX_TEXTURES 16
static Texture2D textureCache[MAX_TEXTURES];
static const char *textureNames[MAX_TEXTURES];
static int textureCount = 0;
static Texture2D currentBg;

typedef enum{
    CH3_DIALOGUE,
    CH3_DIARY,
}Ch3State;

static Ch3State ch3State;
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

void InitDialogueCh3(void){
    current = 0;
    currentTitle = lines[0].title;
    currentBg = GetTexture(lines[0].image);
    ch3State = CH3_DIALOGUE;
}
static Camera2D cam;
void UpdateDialogueCh3(void){
    if(ch3State == CH3_DIALOGUE){
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
                ch3State = CH3_DIARY;
            }
        }
    } 
    else{
        UpdatePlayer(&player);
        UpdateGameCamera(&cam, player.position);
    }
}

void DrawDialogueBoxCh3(void){
    if(ch3State == CH3_DIALOGUE){
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

int IsDialogueFinishedCh3(void){
    return(current == total - 1);
}

void CloseDialogueCh3(void){
    for(int i=0; i<textureCount; i++) UnloadTexture(textureCache[i]);
    if(ch3State == CH3_DIARY){
        ClosePlayer(&player);
        CloseTilemap();
    }
}