#include "raylib.h"
#include "dialogue.h"
#include "player.h"
#include "tilemap.h"
#include "camera.h"
#include "ending_secret.h"
#include <string.h>

typedef struct{
    const char *title;
    const char *speaker;
    const char *text;
    const char *image;
}DialogueLine;

static DialogueLine lines[]={
    {"SECRET ENDING — The Oath Kept", "Narrator", "This is what it looks like when Tonoy decides something: he doesn't announce it. He doesn't build up to it. He simply acts, and the world rearranges itself around the decision.", ""},
    {"SECRET ENDING — The Oath Kept", "Narrator", "He walks to the center of the dais. Kneels in front of Sabin — not pleading, but in a way that says: I'm meeting you at your level because I chose to, not because I was forced to.", ""},
    {"SECRET ENDING — The Oath Kept", "Tonoy", "You wrote my name on the door because you wanted to know if I'd come. I'm here. That's the answer. It always was. I just understand the question now.", ""},
    {"SECRET ENDING — The Oath Kept", "Tonoy", "You were left alone. We were left alone too — differently, but alone. The king is very good at making people feel alone. That's how he keeps them controllable. But I don't want to be controllable anymore.", ""},
    {"SECRET ENDING — The Oath Kept", "Tonoy", "Come back. Not for Valdris. Not for the king. Come back because we're not finished. We were never finished. We've just been on pause for fifteen years.", ""},
    {"SECRET ENDING — The Oath Kept", "Narrator", "Sakib steps forward. He's waited for this moment with the patience of someone who has accepted he doesn't get to decide the timing of his own release.", ""},
    {"SECRET ENDING — The Oath Kept", "Sakib", "I left you behind. I've known for fifteen years it was wrong and I can't undo it. I can't bring it back. But I'm here now. And I'm not leaving again.", ""},
    {"SECRET ENDING — The Oath Kept", "Narrator", "The darkness in Sabin's eyes doesn't dramatically lift. It recedes slowly — the way light returns after an eclipse — not all at once, but inevitably, moving in one direction.", ""},
    {"SECRET ENDING — The Oath Kept", "Narrator", "He stands. Removes the broken armour — not a gesture of surrender, but of someone setting down something they've carried far longer than it served them to. Underneath: an old soldier's clothes. Worn. Real.", ""},
    {"SECRET ENDING — The Oath Kept", "Sabin", "The king still has his throne.", ""},
    {"SECRET ENDING — The Oath Kept", "Tonoy", "Not for long.", ""},
    {"SECRET ENDING — The Oath Kept", "Sabin", "Then we won't walk in as victims with evidence. We'll walk in the way we always should have. I led men at Ashfeld. I can still give an order men will follow. Let me give one more. Let me walk in front.", ""},
    {"SECRET ENDING — The Oath Kept", "Narrator", "Tonoy looks at him for a long moment — and steps aside. That's not a small thing. Tonoy rarely steps aside. But he understands what's being asked: not permission, but the chance for Sabin to be a commander one last time, on his own terms — not as someone who was rescued, but as someone who chose to come back.", ""},
    {"SECRET ENDING — The Oath Kept", "Tonoy", "Then lead. We'll be right behind you. Both of us.", ""},
    {"SECRET ENDING — The Oath Kept", "Narrator", "Three soldiers walk out of Ashfeld together. The same three who walked in fifteen years ago and were never supposed to return. They don't hurry. They don't make speeches. They just walk — toward the king's city, toward the truth that has been buried under a victory parade and three hundred dead names no one was ever allowed to say out loud.", ""},
    {"SECRET ENDING — The Oath Kept", "Narrator", "Mitu writes it all down. Every name. Every pact. Every silence bought at the cost of someone else's life. She writes it the way Sabin painted the cave walls: not to be heroic, but to make sure that this time, the record is accurate.", ""},
    {"SECRET ENDING — The Oath Kept", "Narrator", "The king is brought before his own council. Tonoy doesn't speak at the trial. He already said what needed saying, in a shattered valley, to the right people, at the right time. That's enough.", ""},
    {"SECRET ENDING — The Oath Kept", "Narrator", "Valdris doesn't become good overnight. Kingdoms don't. But it becomes honest. And honesty, Tonoy has learned, is not a small thing. It's the only thing that keeps the ground under your feet from being sold while you sleep.", ""},
    {"SECRET ENDING — The Oath Kept", "Narrator", "He returns to Maren one last time. Takes the stone from the window. Puts it in his coat pocket. Leaves the hut door open — not an invitation, but because he's no longer waiting inside it.", ""},
    {"SECRET ENDING — The Oath Kept", "Narrator", "The forgotten were remembered. The oath, finally kept. And Valdris learned what Tonoy had known for fifteen years: the most dangerous enemy doesn't live outside the walls. It lives inside the throne room, deciding what the walls are for.", ""},
};

static int total = 20;
static int current = 0;
static const char *currentTitle = "";

#define MAX_TEXTURES 16
static Texture2D textureCache[MAX_TEXTURES];
static const char *textureNames[MAX_TEXTURES];
static int textureCount = 0;
static Texture2D currentBg;

typedef enum{
    CH_DIALOGUE_SECRET,
    CH_SECRET_ENDING,
}Ch_secret_State;

static Ch_secret_State ch_secret_state;
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

void InitDialogueSecret(void){
    current = 0;
    currentTitle = lines[0].title;
    currentBg = GetTexture(lines[0].image);
    ch_secret_state = CH_DIALOGUE_SECRET;
}
static Camera2D cam;
void UpdateDialogueSecret(void){
    if(ch_secret_state == CH_DIALOGUE_SECRET){
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
                ch_secret_state = CH_SECRET_ENDING;
            }
        }
    } 
    else{
        UpdatePlayer(&player);
        UpdateGameCamera(&cam, player.position);
    }
}

void DrawDialogueBoxSecret(void){
    if(ch_secret_state == CH_DIALOGUE_SECRET){
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

int IsDialogueFinishedSecret(void){
    return(current == total - 1);
}

void CloseDialogueSecret(void){
    for(int i=0; i<textureCount; i++) UnloadTexture(textureCache[i]);
    if(ch_secret_state == CH_SECRET_ENDING){
        ClosePlayer(&player);
        CloseTilemap();
    }
}