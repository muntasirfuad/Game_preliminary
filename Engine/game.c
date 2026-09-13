#include "raylib.h"
#include "game.h"
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
#include "battle_system.h"
#include "puzzle_system.h"
#include <stdio.h>

typedef enum{
    STATE_TITLE,
    STATE_PROLOGUE,
    STATE_CHAPTER_CARD,
    STATE_CHAPTER1_FOREST,
    STATE_ROUTE_CHOICE,
    STATE_CH1_BATTLE,
    STATE_CH1_PUZZLE,
    STATE_CHAPTER2_VILLAGE,
    STATE_CH2_BATTLE,
    STATE_CH2_PUZZLE,
    STATE_CHAPTER3_DIARY,
    STATE_CH3_BATTLE,
    STATE_CH3_PUZZLE,
    STATE_CHAPTER4_TRUTH,
    STATE_CH4_BATTLE,
    STATE_CH4_PUZZLE,
    STATE_CHAPTER5_FINAL,
    STATE_CH5_BATTLE,
    STATE_CH5_PUZZLE,
    STATE_CHOICE,
    STATE_BAD_ENDING_BATTLE,
    STATE_REVENGE_BOSS_BATTLE,
    STATE_ENDING_BAD,
    STATE_ENDING_GOOD,
    STATE_ENDING_SECRET,
    STATE_THE_END
}GameState;

static GameState state;
static int choiceSelected;
static int routeChapter;
static int routeSelected;
static bool quitRequested;
static int pendingChapter;
static float chapterCardTimer;
static Music backgroundMusic;
static Sound typingSound;
static bool audioLoaded;

static const char *AudioPath(const char *name){
    static char rooted[160];
    if(FileExists(name))return name;
    snprintf(rooted,sizeof(rooted),"src/%s",name);
    if(FileExists(rooted))return rooted;
    else return name;
}

static bool IsDialogueState(GameState value){
    return value==STATE_PROLOGUE||value==STATE_CHAPTER1_FOREST||value==STATE_CHAPTER2_VILLAGE||value==STATE_CHAPTER3_DIARY||value==STATE_CHAPTER4_TRUTH||value==STATE_CHAPTER5_FINAL||value==STATE_ENDING_BAD||value==STATE_ENDING_GOOD||value==STATE_ENDING_SECRET;
}

static void PlayTyping(void){
    if(!audioLoaded||typingSound.frameCount==0)return;
    StopSound(typingSound);
    PlaySound(typingSound);
}

static void OpenRouteChoice(int chapter){
    routeChapter=chapter;
    routeSelected=-1;
    state=STATE_ROUTE_CHOICE;
}

static GameState BattleStateFor(int chapter){
    static const GameState states[5]={STATE_CH1_BATTLE,STATE_CH2_BATTLE,
        STATE_CH3_BATTLE,STATE_CH4_BATTLE,STATE_CH5_BATTLE};
    return states[chapter-1];
}

static GameState PuzzleStateFor(int chapter){
    static const GameState states[5]={STATE_CH1_PUZZLE,STATE_CH2_PUZZLE,
        STATE_CH3_PUZZLE,STATE_CH4_PUZZLE,STATE_CH5_PUZZLE};
    return states[chapter-1];
}

static void ShowChapterCard(int chapter){
    pendingChapter=chapter;
    chapterCardTimer=2.0f;
    state=STATE_CHAPTER_CARD;
}

static void OpenChapterDialogue(int chapter){
    if(chapter==1){
        InitDialogueCh1();
        state=STATE_CHAPTER1_FOREST;
    }
    else if(chapter==2){
        InitDialogueCh2();
        state=STATE_CHAPTER2_VILLAGE;
    }
    else if(chapter==3){
        InitDialogueCh3();
        state=STATE_CHAPTER3_DIARY;
    }
    else if(chapter==4){
        InitDialogueCh4();
        state=STATE_CHAPTER4_TRUTH;
    }
    else{
        InitDialogueCh5();
        state=STATE_CHAPTER5_FINAL;
    }
}

void InitGame(void){
    state=STATE_TITLE;
    choiceSelected=0;
    quitRequested=false;
    backgroundMusic=LoadMusicStream(AudioPath("main_theme.wav"));
    typingSound=LoadSound(AudioPath("typing.mp3"));
    audioLoaded=backgroundMusic.frameCount>0||typingSound.frameCount>0;
    if(backgroundMusic.frameCount>0){
        backgroundMusic.looping=true;
        SetMusicVolume(backgroundMusic,0.75f);
        PlayMusicStream(backgroundMusic);
    }
    if(typingSound.frameCount>0)SetSoundVolume(typingSound,0.28f);
    InitIntro();
}

// ============================================
// UPDATE
// Time moves forward. The game does not wait for you to be ready.
// ============================================
void UpdateGame(void){
    if(backgroundMusic.frameCount>0)UpdateMusicStream(backgroundMusic);
    GameState stateBefore=state;
    bool dialogueAdvanced=IsDialogueState(state)&&IsKeyPressed(KEY_SPACE);
    switch(state){
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
                    quitRequested=true;
                }
            }
            break;

        // --- PROLOGUE ---
        case STATE_PROLOGUE:
            UpdateDialogue();
            if(IsDialogueFinished()){
                CloseDialogue();
                ShowChapterCard(1);
            }
            break;

        case STATE_CHAPTER_CARD:
            chapterCardTimer-=GetFrameTime();
            if(chapterCardTimer<=0||IsMouseButtonPressed(MOUSE_LEFT_BUTTON))
                OpenChapterDialogue(pendingChapter);
            break;

        // --- CHAPTER 1 ---
        case STATE_CHAPTER1_FOREST:
            UpdateDialogueCh1();
            if(IsDialogueFinishedCh1()){
                CloseDialogueCh1();
                OpenRouteChoice(1);
            }
            break;

        case STATE_ROUTE_CHOICE:{
            Vector2 mouse=GetMousePosition();
            Rectangle fightButton={330,320,280,70};
            Rectangle quizButton={670,320,280,70};
            if(CheckCollisionPointRec(mouse,fightButton))routeSelected=0;
            else if(CheckCollisionPointRec(mouse,quizButton))routeSelected=1;
            else routeSelected=-1;
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)){
                if(routeSelected==0){
                    InitBattleSystem(routeChapter);
                    state=BattleStateFor(routeChapter);
                }
                else if(routeSelected==1){
                    InitPuzzleSystem(routeChapter);
                    state=PuzzleStateFor(routeChapter);
                }
            }
            break;
        }

        case STATE_CH1_BATTLE:
            UpdateBattleSystem();
            if(IsBattleFinished()==1){
                ShowChapterCard(2);
            }
            else if(IsBattleFinished()==-1){
                InitPuzzleSystem(1);
                state=STATE_CH1_PUZZLE;
            }
            break;

        case STATE_CH1_PUZZLE:
            UpdatePuzzleSystem();
            if(IsPuzzleFinished()==1){
                ShowChapterCard(2);
            }
            else if(IsPuzzleFinished()==2){
                InitBattleSystem(1);
                state=STATE_CH1_BATTLE;
            }
            break;

        // --- CHAPTER 2 ---
        case STATE_CHAPTER2_VILLAGE:
            UpdateDialogueCh2();
            if(IsDialogueFinishedCh2()){
                CloseDialogueCh2();
                OpenRouteChoice(2);
            }
            break;

        case STATE_CH2_BATTLE:
            UpdateBattleSystem();
            if(IsBattleFinished()==1){
                ShowChapterCard(3);
            }
            else if(IsBattleFinished()==-1){
                InitPuzzleSystem(2);
                state=STATE_CH2_PUZZLE;
            }
            break;

        case STATE_CH2_PUZZLE:
            UpdatePuzzleSystem();
            if(IsPuzzleFinished()==1){
                ShowChapterCard(3);
            }
            else if(IsPuzzleFinished()==2){
                InitBattleSystem(2);
                state=STATE_CH2_BATTLE;
            }
            break;

        // --- CHAPTER 3 ---
        case STATE_CHAPTER3_DIARY:
            UpdateDialogueCh3();
            if(IsDialogueFinishedCh3()){
                CloseDialogueCh3();
                OpenRouteChoice(3);
            }
            break;

        case STATE_CH3_BATTLE:
            UpdateBattleSystem();
            if(IsBattleFinished()==1){
                ShowChapterCard(4);
            }
            else if(IsBattleFinished()==-1){
                InitPuzzleSystem(3);
                state=STATE_CH3_PUZZLE;
            }
            break;

        case STATE_CH3_PUZZLE:
            UpdatePuzzleSystem();
            if(IsPuzzleFinished()==1){
                ShowChapterCard(4);
            }
            else if(IsPuzzleFinished()==2){
                InitBattleSystem(3);
                state=STATE_CH3_BATTLE;
            }
            break;

        // --- CHAPTER 4 ---
        case STATE_CHAPTER4_TRUTH:
            UpdateDialogueCh4();
            if(IsDialogueFinishedCh4()){
                CloseDialogueCh4();
                OpenRouteChoice(4);
            }
            break;

        case STATE_CH4_BATTLE:
            UpdateBattleSystem();
            if(IsBattleFinished()==1){
                ShowChapterCard(5);
            }
            else if(IsBattleFinished()==-1){
                InitPuzzleSystem(4);
                state=STATE_CH4_PUZZLE;
            }
            break;

        case STATE_CH4_PUZZLE:
            UpdatePuzzleSystem();
            if(IsPuzzleFinished()==1){
                ShowChapterCard(5);
            }
            else if(IsPuzzleFinished()==2){
                InitBattleSystem(4);
                state=STATE_CH4_BATTLE;
            }
            break;

        // --- CHAPTER 5 ---
        case STATE_CHAPTER5_FINAL:
            UpdateDialogueCh5();
            if(IsDialogueFinishedCh5()){
                CloseDialogueCh5();
                OpenRouteChoice(5);
            }
            break;

        case STATE_CH5_BATTLE:
            UpdateBattleSystem();
            if(IsBattleFinished()==1){
                choiceSelected=0;
                state=STATE_CHOICE;
            }
            else if(IsBattleFinished()==-1){
                InitPuzzleSystem(5);
                state=STATE_CH5_PUZZLE;
            }
            break;

        case STATE_CH5_PUZZLE:
            UpdatePuzzleSystem();
            if(IsPuzzleFinished()==1){
                choiceSelected=0;
                state=STATE_CHOICE;
            }
            else if(IsPuzzleFinished()==2){
                InitBattleSystem(5);
                state=STATE_CH5_BATTLE;
            }
            break;

        // --- CHOICE ---
        case STATE_CHOICE:
            {
                Vector2 mouse=GetMousePosition();
                choiceSelected=-1;
                for(int i=0;i<3;i++)if(CheckCollisionPointRec(mouse,(Rectangle){300,275+i*90,680,64}))choiceSelected=i;
            }
            if(IsMouseButtonPressed(MOUSE_LEFT_BUTTON)&&choiceSelected>=0){
                if(choiceSelected==0){
                    InitDialogueBad();
                    state=STATE_ENDING_BAD;
                }
                else if(choiceSelected==1){
                    InitDialogueGood();
                    state=STATE_ENDING_GOOD;
                }
                else{
                    InitDialogueSecret();
                    state=STATE_ENDING_SECRET;
                }
            }
            break;

        case STATE_BAD_ENDING_BATTLE:
            UpdateBattleSystem();
            if(IsBattleFinished()==1) state=STATE_THE_END;
            else if(IsBattleFinished()==-1) InitBattleSystem(6);
            break;

        case STATE_REVENGE_BOSS_BATTLE:
            UpdateBattleSystem();
            if(IsBattleFinished()==1) state=STATE_THE_END;
            else if(IsBattleFinished()==-1) InitBattleSystem(7);
            break;

        case STATE_ENDING_BAD:
            UpdateDialogueBad();
            if(IsDialogueFinishedBad()){
                CloseDialogueBad();
                InitBattleSystem(6);
                state=STATE_BAD_ENDING_BATTLE;
            }
            break;

        case STATE_ENDING_GOOD:
            UpdateDialogueGood();
            if(IsDialogueFinishedGood()){
                CloseDialogueGood();
                state=STATE_THE_END;
            }
            break;

        case STATE_ENDING_SECRET:
            UpdateDialogueSecret();
            if(IsDialogueFinishedSecret()){
                CloseDialogueSecret();
                InitBattleSystem(7);
                state=STATE_REVENGE_BOSS_BATTLE;
            }
            break;

        case STATE_THE_END:
            break;

        default:
            break;
    }
    if(state!=stateBefore){
        if(typingSound.frameCount>0)StopSound(typingSound);
        if(IsDialogueState(state))PlayTyping();
    }else if(dialogueAdvanced&&IsDialogueState(state)){
        PlayTyping();
    }
}

// ============================================
// DRAW
// What you see is what the world chooses to show you.
// ============================================                                                                                       
void DrawGame(void){
    switch(state){
        case STATE_TITLE:
            DrawIntro();
            break;

        case STATE_PROLOGUE:
            DrawDialogueBox();
            break;

        case STATE_CHAPTER_CARD:{
            static const char *numbers[5]={"CHAPTER I","CHAPTER II","CHAPTER III","CHAPTER IV","CHAPTER V"};
            static const char *names[5]={"THE KING'S CALL","ASH DOES NOT LIE","THE DIARY PAGES","TRUTH BURNS","THE FORGOTTEN OATH"};
            int chapter;
            if(pendingChapter<1)chapter=1;
            else if(pendingChapter>5)chapter=5;
            else chapter=pendingChapter;
            ClearBackground((Color){8,8,11,255});
            const char *number=numbers[chapter-1],*name=names[chapter-1];
            DrawText(number,640-MeasureText(number,48)/2,265,48,GOLD);
            DrawText(name,640-MeasureText(name,25)/2,335,25,(Color){235,220,190,255});
            break;
        }

        case STATE_CHAPTER1_FOREST:
            DrawDialogueBoxCh1();
            break;

        case STATE_ROUTE_CHOICE:{
            ClearBackground((Color){12,12,18,255});
            const char *title=TextFormat("CHAPTER %d - CHOOSE YOUR TRIAL",routeChapter);
            DrawText(title,640-MeasureText(title,34)/2,190,34,GOLD);
            Rectangle fightButton={330,320,280,70};
            Rectangle quizButton={670,320,280,70};
            Color fightColor;
            Color quizColor;
            Color fightBorder;
            Color quizBorder;
            if(routeSelected==0){fightColor=(Color){125,45,40,255};fightBorder=GOLD;}
            else{fightColor=(Color){40,35,38,255};fightBorder=GRAY;}
            if(routeSelected==1){quizColor=(Color){55,90,135,255};quizBorder=GOLD;}
            else{quizColor=(Color){40,35,38,255};quizBorder=GRAY;}
            DrawRectangleRounded(fightButton,.15f,6,fightColor);
            DrawRectangleRounded(quizButton,.15f,6,quizColor);
            DrawRectangleRoundedLinesEx(fightButton,.15f,6,2,fightBorder);
            DrawRectangleRoundedLinesEx(quizButton,.15f,6,2,quizBorder);
            DrawText("FIGHT",430,342,28,RAYWHITE);
            DrawText("QUIZ",775,342,28,RAYWHITE);
            DrawText("Clear either trial to continue",462,440,22,LIGHTGRAY);
            break;
        }

        case STATE_CH1_BATTLE:
            DrawBattleSystem();
            break;

        case STATE_CH1_PUZZLE:
            DrawPuzzleSystem();
            break;

        case STATE_CHAPTER2_VILLAGE:
            DrawDialogueBoxCh2();
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

        case STATE_CH3_BATTLE:
            DrawBattleSystem();
            break;

        case STATE_CH3_PUZZLE:
            DrawPuzzleSystem();
            break;

        case STATE_CHAPTER4_TRUTH:
            DrawDialogueBoxCh4();
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

        case STATE_CH5_BATTLE:
            DrawBattleSystem();
            break;

        case STATE_CH5_PUZZLE:
            DrawPuzzleSystem();
            break;

        case STATE_CHOICE:{
            ClearBackground((Color){9,8,11,255});
            const char *question="What does Tonoy do?";
            DrawText(question,640-MeasureText(question,32)/2,190,32,GOLD);

            const char *options[3]={
                "1. Turn on the king alone  (Bad Ending)",
                "2. Confront the king with Sabin and Sakib  (Good Ending)",
                "3. Take revenge and face the king's champion  (Revenge Ending)"
            };

            for(int i=0; i<3; i++){
                Rectangle button={300,275+i*90,680,64};
                bool hover=i==choiceSelected;
                Color buttonColor;
                Color borderColor;
                Color c;
                if(hover){buttonColor=(Color){102,62,34,255};borderColor=GOLD;c=GOLD;}
                else{buttonColor=(Color){35,31,34,255};borderColor=(Color){115,103,92,255};c=WHITE;}
                DrawRectangleRounded(button,.14f,6,buttonColor);
                DrawRectangleRoundedLinesEx(button,.14f,6,2,borderColor);
                DrawText(options[i],330,296+i*90,20,c);
            }
            DrawText("Click an ending to continue",510,565,18,LIGHTGRAY);
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

        case STATE_THE_END:{
            ClearBackground(BLACK);
            const char *ending="THE END";
            int width=MeasureText(ending, 72);
            DrawText(ending, 640-width/2, 300, 72, GOLD);
            break;
        }

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
        case STATE_TITLE:                                 break;
        case STATE_PROLOGUE:        CloseDialogue();       break;
        case STATE_CHAPTER_CARD:                           break;
        case STATE_CHAPTER1_FOREST: CloseDialogueCh1();    break;
        case STATE_CH1_BATTLE:                            break;
        case STATE_CH1_PUZZLE:                            break;
        case STATE_CHAPTER2_VILLAGE:CloseDialogueCh2();    break;
        case STATE_CH2_BATTLE:                            break;
        case STATE_CH2_PUZZLE:                            break;
        case STATE_CHAPTER3_DIARY:  CloseDialogueCh3();  break;
        case STATE_CH3_BATTLE:                            break;
        case STATE_CH3_PUZZLE:                            break;
        case STATE_CHAPTER4_TRUTH:  CloseDialogueCh4();  break;
        case STATE_CH4_BATTLE:                            break;
        case STATE_CH4_PUZZLE:                            break;
        case STATE_CHAPTER5_FINAL:  CloseDialogueCh5();  break;
        case STATE_CH5_BATTLE:                            break;
        case STATE_CH5_PUZZLE:                            break;
        case STATE_ROUTE_CHOICE:                          break;
        case STATE_CHOICE:                                break;
        case STATE_BAD_ENDING_BATTLE:                     break;
        case STATE_REVENGE_BOSS_BATTLE:                   break;
        case STATE_ENDING_BAD:      CloseDialogueBad();  break;
        case STATE_ENDING_GOOD:     CloseDialogueGood(); break;
        case STATE_ENDING_SECRET:   CloseDialogueSecret();break;
        case STATE_THE_END:                                break;
        default:                                          break;
    }
    if(audioLoaded){
        if(typingSound.frameCount>0)UnloadSound(typingSound);
        if(backgroundMusic.frameCount>0)UnloadMusicStream(backgroundMusic);
        audioLoaded=false;
    }
}

bool ShouldGameQuit(void){
    return quitRequested;
}
