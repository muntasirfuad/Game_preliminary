#include "raylib.h"
#include "game.h"
#include "loading.h"
#include "cinematic.h"
#include "intro.h"
#include "dialogue.h"
#include "scene_ch1_forest.h"
#include "scene_ch2_burned_village.h"
#include "scene_ch3_diary.h"
#include "scene_ch4_truth.h"
#include "scene_ch5_final.h"
#include "ending_bad.h"
#include "ending_good.h"
#include "ending_secret.h"
#include "door_system.h"
#include "battle_system.h"
#include "puzzle_system.h"

// ============================================
// GAME STATE
// The journey has stages. Each one costs something.
// ============================================
typedef enum{
    STATE_LOADING,
    STATE_CINEMATIC,
    STATE_TITLE,
    STATE_PROLOGUE,
    STATE_CHAPTER1_FOREST,
    STATE_CH1_DOOR,
    STATE_CH1_BATTLE,
    STATE_CH1_PUZZLE,
    STATE_CHAPTER2_VILLAGE,
    STATE_CH2_DOOR,
    STATE_CH2_BATTLE,
    STATE_CH2_PUZZLE,
    STATE_CHAPTER3_DIARY,
    STATE_CH3_DOOR,
    STATE_CH3_BATTLE,
    STATE_CH3_PUZZLE,
    STATE_CHAPTER4_TRUTH,
    STATE_CH4_DOOR,
    STATE_CH4_BATTLE,
    STATE_CH4_PUZZLE,
    STATE_CHAPTER5_FINAL,
    STATE_CH5_DOOR,
    STATE_CH5_BATTLE,
    STATE_CH5_PUZZLE,
    STATE_CHOICE,
    STATE_BAD_ENDING_BATTLE,
    STATE_REVENGE_BOSS_BATTLE,
    STATE_ENDING_BAD,
    STATE_ENDING_GOOD,
    STATE_ENDING_SECRET
}GameState;

static GameState state;
static int choiceSelected;

void InitGame(void){
    state=STATE_LOADING;
    choiceSelected=0;
    InitLoadingScreen();
}

// ============================================
// UPDATE
// Time moves forward. The game does not wait for you to be ready.
// ============================================
void UpdateGame(void){
    switch(state){
        // --- LOADING ---
        case STATE_LOADING:
            UpdateLoadingScreen();
            if(IsLoadingDone()){
                InitCinematic();
                state=STATE_CINEMATIC;
            }
            break;

        // --- CINEMATIC ---
        case STATE_CINEMATIC:
            UpdateCinematic();
            if(IsCinematicDone()){
                InitIntro();
                state=STATE_TITLE;
            }
            break;

        // --- TITLE ---
        case STATE_TITLE:
            UpdateIntro();
            {
                int introRes=IsIntroFinished();
                if(introRes==1){          // NEW GAME
                    InitDialogue();
                    state=STATE_PROLOGUE;
                }
                else if(introRes==2){     // RESUME (same as new for now)
                    InitDialogue();
                    state=STATE_PROLOGUE;
                }
                else if(introRes==3){     // QUIT
                    // Handled in main.c via ShouldGameQuit() if you add it
                }
            }
            break;

        // --- PROLOGUE ---
        case STATE_PROLOGUE:
            UpdateDialogue();
            if(IsDialogueFinished()){
                CloseDialogue();
                InitDialogueCh1();
                state=STATE_CHAPTER1_FOREST;
            }
            break;

        // --- CHAPTER 1 ---
        case STATE_CHAPTER1_FOREST:
            UpdateDialogueCh1();
            if(IsDialogueFinishedCh1()){
                CloseDialogueCh1();
                InitDoorSystem();
                state=STATE_CH1_DOOR;
            }
            break;

        case STATE_CH1_DOOR:
            UpdateDoorSystem();
            {
                int doorResult=IsDoorSystemFinished();
                if(doorResult==1){
                    InitBattleSystem(1);
                    state=STATE_CH1_BATTLE;
                }
                else if(doorResult==2){
                    InitPuzzleSystem(1);
                    state=STATE_CH1_PUZZLE;
                }
            }
            break;

        case STATE_CH1_BATTLE:
            UpdateBattleSystem();
            if(IsBattleFinished()!=0){
                InitDialogueCh2();
                state=STATE_CHAPTER2_VILLAGE;
            }
            break;

        case STATE_CH1_PUZZLE:
            UpdatePuzzleSystem();
            if(IsPuzzleFinished()!=0){
                InitDialogueCh2();
                state=STATE_CHAPTER2_VILLAGE;
            }
            break;

        // --- CHAPTER 2 ---
        case STATE_CHAPTER2_VILLAGE:
            UpdateDialogueCh2();
            if(IsDialogueFinishedCh2()){
                CloseDialogueCh2();
                InitDoorSystem();
                state=STATE_CH2_DOOR;
            }
            break;

        case STATE_CH2_DOOR:
            UpdateDoorSystem();
            {
                int doorResult=IsDoorSystemFinished();
                if(doorResult==1){
                    InitBattleSystem(2);
                    state=STATE_CH2_BATTLE;
                }
                else if(doorResult==2){
                    InitPuzzleSystem(2);
                    state=STATE_CH2_PUZZLE;
                }
            }
            break;

        case STATE_CH2_BATTLE:
            UpdateBattleSystem();
            if(IsBattleFinished()!=0){
                InitDialogueCh3();
                state=STATE_CHAPTER3_DIARY;
            }
            break;

        case STATE_CH2_PUZZLE:
            UpdatePuzzleSystem();
            if(IsPuzzleFinished()!=0){
                InitDialogueCh3();
                state=STATE_CHAPTER3_DIARY;
            }
            break;

        // --- CHAPTER 3 ---
        case STATE_CHAPTER3_DIARY:
            UpdateDialogueCh3();
            if(IsDialogueFinishedCh3()){
                CloseDialogueCh3();
                InitDoorSystem();
                state=STATE_CH3_DOOR;
            }
            break;

        case STATE_CH3_DOOR:
            UpdateDoorSystem();
            {
                int doorResult=IsDoorSystemFinished();
                if(doorResult==1){
                    InitBattleSystem(3);
                    state=STATE_CH3_BATTLE;
                }
                else if(doorResult==2){
                    InitPuzzleSystem(3);
                    state=STATE_CH3_PUZZLE;
                }
            }
            break;

        case STATE_CH3_BATTLE:
            UpdateBattleSystem();
            if(IsBattleFinished()!=0){
                InitDialogueCh4();
                state=STATE_CHAPTER4_TRUTH;
            }
            break;

        case STATE_CH3_PUZZLE:
            UpdatePuzzleSystem();
            if(IsPuzzleFinished()!=0){
                InitDialogueCh4();
                state=STATE_CHAPTER4_TRUTH;
            }
            break;

        // --- CHAPTER 4 ---
        case STATE_CHAPTER4_TRUTH:
            UpdateDialogueCh4();
            if(IsDialogueFinishedCh4()){
                CloseDialogueCh4();
                InitDoorSystem();
                state=STATE_CH4_DOOR;
            }
            break;

        case STATE_CH4_DOOR:
            UpdateDoorSystem();
            {
                int doorResult=IsDoorSystemFinished();
                if(doorResult==1){
                    InitBattleSystem(4);
                    state=STATE_CH4_BATTLE;
                }
                else if(doorResult==2){
                    InitPuzzleSystem(4);
                    state=STATE_CH4_PUZZLE;
                }
            }
            break;

        case STATE_CH4_BATTLE:
            UpdateBattleSystem();
            if(IsBattleFinished()!=0){
                InitDialogueCh5();
                state=STATE_CHAPTER5_FINAL;
            }
            break;

        case STATE_CH4_PUZZLE:
            UpdatePuzzleSystem();
            if(IsPuzzleFinished()!=0){
                InitDialogueCh5();
                state=STATE_CHAPTER5_FINAL;
            }
            break;

        // --- CHAPTER 5 ---
        case STATE_CHAPTER5_FINAL:
            UpdateDialogueCh5();
            if(IsDialogueFinishedCh5()){
                CloseDialogueCh5();
                InitDoorSystem();
                state=STATE_CH5_DOOR;
            }
            break;

        case STATE_CH5_DOOR:
            UpdateDoorSystem();
            {
                int doorResult=IsDoorSystemFinished();
                if(doorResult==1){
                    InitBattleSystem(5);
                    state=STATE_CH5_BATTLE;
                }
                else if(doorResult==2){
                    InitPuzzleSystem(5);
                    state=STATE_CH5_PUZZLE;
                }
            }
            break;

        case STATE_CH5_BATTLE:
            UpdateBattleSystem();
            if(IsBattleFinished()!=0){
                choiceSelected=0;
                state=STATE_CHOICE;
            }
            break;

        case STATE_CH5_PUZZLE:
            UpdatePuzzleSystem();
            if(IsPuzzleFinished()!=0){
                choiceSelected=0;
                state=STATE_CHOICE;
            }
            break;

        // --- CHOICE ---
        case STATE_CHOICE:
            if(IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)){
                choiceSelected--;
                if(choiceSelected<0) choiceSelected=2;
            }
            if(IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)){
                choiceSelected++;
                if(choiceSelected>2) choiceSelected=0;
            }
            if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)){
                if(choiceSelected==0){
                    InitBattleSystem(6);
                    state=STATE_BAD_ENDING_BATTLE;
                }
                else if(choiceSelected==1){
                    InitDialogueGood();
                    state=STATE_ENDING_GOOD;
                }
                else{
                    InitBattleSystem(7);
                    state=STATE_REVENGE_BOSS_BATTLE;
                }
            }
            break;

        case STATE_BAD_ENDING_BATTLE:
            UpdateBattleSystem();
            if(IsBattleFinished()!=0){InitDialogueBad();state=STATE_ENDING_BAD;}
            break;

        case STATE_REVENGE_BOSS_BATTLE:
            UpdateBattleSystem();
            if(IsBattleFinished()!=0){InitDialogueSecret();state=STATE_ENDING_SECRET;}
            break;

        case STATE_ENDING_BAD:
            UpdateDialogueBad();
            break;

        case STATE_ENDING_GOOD:
            UpdateDialogueGood();
            break;

        case STATE_ENDING_SECRET:
            UpdateDialogueSecret();
            break;

        default:
            break;
    }
}

// ============================================
// DRAW
// What you see is what the world chooses to show you.
// ============================================
void DrawGame(void){
    switch(state){
        case STATE_LOADING:
            DrawLoadingScreen();
            break;

        case STATE_CINEMATIC:
            DrawCinematic();
            break;

        case STATE_TITLE:
            DrawIntro();
            break;

        case STATE_PROLOGUE:
            DrawDialogueBox();
            break;

        case STATE_CHAPTER1_FOREST:
            DrawDialogueBoxCh1();
            break;

        case STATE_CH1_DOOR:
            DrawDoorSystem();
            break;

        case STATE_CH1_BATTLE:
            DrawBattleSystem();
            break;

        case STATE_CH1_PUZZLE:
            DrawPuzzleSystem();
            break;

        case STATE_CHAPTER2_VILLAGE:
            DrawDialogueBoxCh2();
            break;

        case STATE_CH2_DOOR:
            DrawDoorSystem();
            break;

        case STATE_CH2_BATTLE:
            DrawBattleSystem();
            break;

        case STATE_CH2_PUZZLE:
            DrawPuzzleSystem();
            break;

        case STATE_CHAPTER3_DIARY:
            DrawDialogueBoxCh3();
            break;

        case STATE_CH3_DOOR:
            DrawDoorSystem();
            break;

        case STATE_CH3_BATTLE:
            DrawBattleSystem();
            break;

        case STATE_CH3_PUZZLE:
            DrawPuzzleSystem();
            break;

        case STATE_CHAPTER4_TRUTH:
            DrawDialogueBoxCh4();
            break;

        case STATE_CH4_DOOR:
            DrawDoorSystem();
            break;

        case STATE_CH4_BATTLE:
            DrawBattleSystem();
            break;

        case STATE_CH4_PUZZLE:
            DrawPuzzleSystem();
            break;

        case STATE_CHAPTER5_FINAL:
            DrawDialogueBoxCh5();
            break;

        case STATE_CH5_DOOR:
            DrawDoorSystem();
            break;

        case STATE_CH5_BATTLE:
            DrawBattleSystem();
            break;

        case STATE_CH5_PUZZLE:
            DrawPuzzleSystem();
            break;

        case STATE_CHOICE:{
            ClearBackground(BLACK);
            DrawText("What does Tonoy do?", 400, 220, 32, YELLOW);

            const char *options[3]={
                "1. Turn on the king alone  (Bad Ending)",
                "2. Confront the king with Sabin and Sakib  (Good Ending)",
                "3. Take revenge and face the king's champion  (Revenge Ending)"
            };

            for(int i=0; i<3; i++){
                Color c=(i==choiceSelected) ? YELLOW : WHITE;
                DrawText(options[i], 400, 300+i*50, 22, c);
                if(i==choiceSelected) DrawText(">", 370, 300+i*50, 22, YELLOW);
            }

            DrawText("[UP/DOWN to choose, ENTER to confirm]", 400, 500, 18, PURPLE);
            break;
        }

        case STATE_BAD_ENDING_BATTLE:
        case STATE_REVENGE_BOSS_BATTLE:
            DrawBattleSystem();
            break;

        case STATE_ENDING_BAD:
            DrawDialogueBoxBad();
            break;

        case STATE_ENDING_GOOD:
            DrawDialogueBoxGood();
            break;

        case STATE_ENDING_SECRET:
            DrawDialogueBoxSecret();
            break;

        default:
            break;
    }
}

// ============================================
// CLOSE
// Every journey ends. The question is what you leave behind.
// ============================================
void CloseGame(void){
    switch(state){
        case STATE_LOADING:                               break;
        case STATE_CINEMATIC:                             break;
        case STATE_TITLE:                                 break;
        case STATE_PROLOGUE:        CloseDialogue();       break;
        case STATE_CHAPTER1_FOREST: CloseDialogueCh1();    break;
        case STATE_CH1_DOOR:                              break;
        case STATE_CH1_BATTLE:                            break;
        case STATE_CH1_PUZZLE:                            break;
        case STATE_CHAPTER2_VILLAGE:CloseDialogueCh2();    break;
        case STATE_CH2_DOOR:                              break;
        case STATE_CH2_BATTLE:                            break;
        case STATE_CH2_PUZZLE:                            break;
        case STATE_CHAPTER3_DIARY:  CloseDialogueCh3();  break;
        case STATE_CH3_DOOR:                              break;
        case STATE_CH3_BATTLE:                            break;
        case STATE_CH3_PUZZLE:                            break;
        case STATE_CHAPTER4_TRUTH:  CloseDialogueCh4();  break;
        case STATE_CH4_DOOR:                              break;
        case STATE_CH4_BATTLE:                            break;
        case STATE_CH4_PUZZLE:                            break;
        case STATE_CHAPTER5_FINAL:  CloseDialogueCh5();  break;
        case STATE_CH5_DOOR:                              break;
        case STATE_CH5_BATTLE:                            break;
        case STATE_CH5_PUZZLE:                            break;
        case STATE_CHOICE:                                break;
        case STATE_BAD_ENDING_BATTLE:                     break;
        case STATE_REVENGE_BOSS_BATTLE:                   break;
        case STATE_ENDING_BAD:      CloseDialogueBad();  break;
        case STATE_ENDING_GOOD:     CloseDialogueGood(); break;
        case STATE_ENDING_SECRET:   CloseDialogueSecret();break;
        default:                                          break;
    }
}
