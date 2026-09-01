#include "raylib.h"
#include "dialogue.h"
#include "player.h"
#include "tilemap.h"
#include "camera.h"
#include "scene_ch2_burned_village.h"
#include <string.h>

typedef struct{
    const char *title;
    const char *speaker;
    const char *text;
    const char *image;
}DialogueLine;

static DialogueLine lines[]={
    {"CHAPTER II — Ash Does Not Lie", "Location", "Location: The Burned Village of Corrath", ""},
    {"CHAPTER II — Ash Does Not Lie", "Narrator", "Corrath didn't burn. Burned implies evidence of fire — charred remains, wreckage, the chemical language of destruction. What happened to Corrath is different. It was erased. Fifty houses reduced to fine grey powder, as though someone translated the entire village into silence.", ""},
    {"CHAPTER II — Ash Does Not Lie", "Narrator", "Tonoy sheathes his sword and walks through it. Whatever did this is already gone. He can feel the absence — the way you feel a word deliberately left out of a sentence.", ""},
    {"CHAPTER II — Ash Does Not Lie", "Narrator", "No bodies. That detail matters most. Fire kills, but it doesn't take. Whatever came here, took.", ""},
    {"CHAPTER II — Ash Does Not Lie", "Narrator", "He kneels by an old doorway. Presses two fingers into the ash. Lifts them to the light.", ""},
    {"CHAPTER II — Ash Does Not Lie", "Tonoy", "Three days. Maybe four. The ash is still packed — the moisture from that night is still in it. Whatever happened here happened fast. No one had time to run.", ""},
    {"CHAPTER II — Ash Does Not Lie", "Sakib", "Or no one wanted to.", ""},
    {"CHAPTER II — Ash Does Not Lie", "Narrator", "Sakib says nothing. Looks away.", ""},
    {"CHAPTER II — Ash Does Not Lie", "[Tonoy — to himself, perfectly still]", "That careful emptiness again. Sakib has been doing it since the forest — starting a half-sentence and pulling back. He knows something. Has known something for a long time. The question is whether he'll say it himself, or whether Tonoy will have to dig it out. He's starting to think it will be the second one.", ""},
    {"CHAPTER II — Ash Does Not Lie", "Narrator", "The old woman inside the well is named Lena. She's at least ninety, and the calmest person Tonoy has spoken to in weeks.", ""},
    {"CHAPTER II — Ash Does Not Lie", "Narrator", "She describes the stranger with a specificity that suggests she's rehearsed this memory many times in the dark, turning it over — making sure she gets it right.", ""},
    {"CHAPTER II — Ash Does Not Lie", "Lena", "Tall. Old armour — not decorative, not for show. The kind of armour worn because it's needed, not displayed. He went to every door. Didn't push, didn't knock. Just stood. And then the doors opened from the inside.", ""},
    {"CHAPTER II — Ash Does Not Lie", "Tonoy", "He compelled them.", ""},
    {"CHAPTER II — Ash Does Not Lie", "Lena", "The people walked out like they were still asleep. Like they were going somewhere they'd always wanted to go.", ""},
    {"CHAPTER II — Ash Does Not Lie", "Tonoy", "Then what?", ""},
    {"CHAPTER II — Ash Does Not Lie", "Lena", "He said two names. Over and over. Like a prayer. Or a verdict.", ""},
    {"CHAPTER II — Ash Does Not Lie", "Narrator", "The old woman meets Tonoy's eyes — the direct gaze that only comes with great age.", ""},
    {"CHAPTER II — Ash Does Not Lie", "Lena", "Your name. And his.", ""},
    {"CHAPTER II — Ash Does Not Lie", "Narrator", "She nods toward Sakib.", ""},
    {"CHAPTER II — Ash Does Not Lie", "Lena", "He said: they will come. They will know. And then they will choose.", ""},
    {"CHAPTER II — Ash Does Not Lie", "Narrator", "Tonoy doesn't look at Sakib. He keeps his eyes on Lena.", ""},
    {"CHAPTER II — Ash Does Not Lie", "Tonoy", "Choose what?", ""},
    {"CHAPTER II — Ash Does Not Lie", "Lena", "He didn't say. I think that was the point.", ""},
    {"CHAPTER II — Ash Does Not Lie", "[Observer — Sakib, in this moment]", "Sakib's hands are shaking. Not enough to notice — unless you look closely. Tonoy always looks closely.", ""},
    {"CHAPTER II — Ash Does Not Lie", "Narrator", "The undead soldiers rise silently from the ash — the most terrifying way to announce yourself. They wear Ashfeld's insignia. Tonoy recognizes some of them — not the faces, which are gone, but the way they stand. A particular tilt of the shoulder.", ""},
    {"CHAPTER II — Ash Does Not Lie", "Narrator", "The way a man holds a weapon when he's grown used to his off-hand because his true hand was injured in training twenty years ago. He knew these men.", ""},
    {"CHAPTER II — Ash Does Not Lie", "Narrator", "He fights them with the precision of grief — no wasted movement, no hesitation, but no display either. Just quietly finishing something that should have ended fifteen years ago.", ""},
    {"CHAPTER II — Ash Does Not Lie", "Narrator", "When it's over, he stands in the ash looking at his own hands.", ""},
    {"CHAPTER II — Ash Does Not Lie", "[Tonoy — to the empty air, to no one]", "I'm sorry. I came back the moment I knew.", ""},
    {"CHAPTER II — Ash Does Not Lie", "Observer", "He isn't saying it to Sakib.", ""},
};

static int total = 30;
static int current = 0;
static const char *currentTitle = "";

#define MAX_TEXTURES 16
static Texture2D textureCache[MAX_TEXTURES];
static const char *textureNames[MAX_TEXTURES];
static int textureCount = 0;
static Texture2D currentBg;

typedef enum{
    CH2_DIALOGUE,
    CH2_BURNED_VILLAGE,
}Ch2State;

static Ch2State ch2State;
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

void InitDialogueCh2(void){
    current = 0;
    currentTitle = lines[0].title;
    currentBg = GetTexture(lines[0].image);
    ch2State = CH2_DIALOGUE;
}
static Camera2D cam;
void UpdateDialogueCh2(void){
    if(ch2State == CH2_DIALOGUE){
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
                ch2State = CH2_BURNED_VILLAGE;
            }
        }
    } 
    else{
        UpdatePlayer(&player);
        UpdateGameCamera(&cam, player.position);
    }
}

void DrawDialogueBoxCh2(void){
    if(ch2State == CH2_DIALOGUE){
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

int IsDialogueFinishedCh2(void){
    return(current == total - 1);
}

void CloseDialogueCh2(void){
    for(int i=0; i<textureCount; i++) UnloadTexture(textureCache[i]);
    if(ch2State == CH2_BURNED_VILLAGE){
        ClosePlayer(&player);
        CloseTilemap();
    }
}