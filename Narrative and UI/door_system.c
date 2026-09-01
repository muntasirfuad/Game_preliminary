#include "raylib.h"
#include "door_system.h"
#include <stdlib.h>

// ============================================
// THE DOOR
// Every choice is a door. Most people walk through without looking back.
// ============================================
typedef enum{
    DOOR_CHOOSING,
    DOOR_REVEALING,
    DOOR_DONE
}DoorState;

static DoorState doorState;
static int doorContents[2];
static int selectedDoor;
static int resultType;
static float revealTimer;

void InitDoorSystem(void){
    doorState=DOOR_CHOOSING;
    selectedDoor=-1;
    resultType=0;
    revealTimer=0;

    // One door hides a blade. The other hides a question.
    // The traveler does not know which is which.
    int battleDoor=GetRandomValue(0, 1);
    doorContents[0]=(battleDoor==0) ? 1 : 2;
    doorContents[1]=(battleDoor==0) ? 2 : 1;
}

void UpdateDoorSystem(void){
    switch(doorState){
        case DOOR_CHOOSING:{
            Vector2 mouse=GetMousePosition();
            Rectangle door1={150, 200, 400, 450};
            Rectangle door2={730, 200, 400, 450};

            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                if(CheckCollisionPointRec(mouse, door1)){
                    selectedDoor=0;
                    resultType=doorContents[0];
                    doorState=DOOR_REVEALING;
                }
                else if(CheckCollisionPointRec(mouse, door2)){
                    selectedDoor=1;
                    resultType=doorContents[1];
                    doorState=DOOR_REVEALING;
                }
            }

            static int hover=0;
            if(IsKeyPressed(KEY_LEFT)) hover=0;
            if(IsKeyPressed(KEY_RIGHT)) hover=1;
            if(IsKeyPressed(KEY_ENTER)){
                selectedDoor=hover;
                resultType=doorContents[hover];
                doorState=DOOR_REVEALING;
            }
            break;
        }

        case DOOR_REVEALING:
            revealTimer+=GetFrameTime();
            if(revealTimer>2.0f){
                doorState=DOOR_DONE;
            }
            break;

        case DOOR_DONE:
            break;
    }
}

void DrawDoorSystem(void){
    ClearBackground(BLACK);

    switch(doorState){
        case DOOR_CHOOSING:{
            DrawText("THE PATH SPLITS", 450, 60, 40, GOLD);
            DrawText("Two doors. One leads to battle, one to a trial of knowledge.", 250, 120, 24, GRAY);
            DrawText("You cannot know which is which. Choose.", 380, 160, 24, GRAY);

            // Door I
            DrawRectangle(150, 220, 400, 450, DARKBROWN);
            DrawRectangleLinesEx((Rectangle){150, 220, 400, 450}, 4, GOLD);
            DrawText("DOOR I", 290, 420, 36, GOLD);
            DrawText("???", 320, 470, 28, GRAY);

            // Door II
            DrawRectangle(730, 220, 400, 450, DARKBROWN);
            DrawRectangleLinesEx((Rectangle){730, 220, 400, 450}, 4, GOLD);
            DrawText("DOOR II", 860, 420, 36, GOLD);
            DrawText("???", 890, 470, 28, GRAY);

            // Hover
            Vector2 mouse=GetMousePosition();
            if(CheckCollisionPointRec(mouse, (Rectangle){150, 220, 400, 450})){
                DrawRectangle(150, 220, 400, 450, (Color){100, 80, 60, 100});
            }
            if(CheckCollisionPointRec(mouse, (Rectangle){730, 220, 400, 450})){
                DrawRectangle(730, 220, 400, 450, (Color){100, 80, 60, 100});
            }

            DrawText("[Click or LEFT/RIGHT + ENTER]", 400, 700, 22, GRAY);
            break;
        }

        case DOOR_REVEALING:{
            DrawText("THE DOOR OPENS...", 450, 300, 40, GOLD);

            const char *reveal=(resultType==1) ? "BATTLE AWAITS!" : "A TRIAL OF KNOWLEDGE!";
            Color revealColor=(resultType==1) ? RED : SKYBLUE;
            DrawText(reveal, 380, 380, 36, revealColor);

            if(resultType==1){
                DrawText("Steel will be tested. Blood will be spilled.", 350, 450, 24, GRAY);
            } else {
                DrawText("The mind will be weighed. Truth is the only currency.", 320, 450, 24, GRAY);
            }

            DrawText(TextFormat("Revealing in %.1f...", 2.0f-revealTimer), 500, 520, 24, GRAY);
            break;
        }

        case DOOR_DONE:
            break;
    }
}

int IsDoorSystemFinished(void){
    if(doorState==DOOR_DONE) return resultType;
    return 0;
}

int GetSelectedDoorType(void){
    return resultType;
}