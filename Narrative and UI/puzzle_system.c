#include "raylib.h"
#include "puzzle_system.h"
#include <string.h>

typedef struct{
    const char *question;
    const char *options[4];
    int correctAnswer;
}QuizQuestion;

// ============================================
// SET 1: Technology
// The machines remember what we forget.
// ============================================
static QuizQuestion set1_tech[]={
    {"What does 'HTTP' stand for?",
     {"HyperText Transfer Protocol", "High Tech Transfer Process", "Home Tool Transfer Program", "Hyperlink Text Transmission Protocol"}, 0},
    {"Which company developed Android before Google acquired it?",
     {"Apple Inc.", "Android Inc. (founded by Andy Rubin)", "Microsoft Mobile", "Nokia Corporation"}, 1},
    {"What is the name of the first electronic general-purpose computer, completed in 1945?",
     {"UNIVAC", "Colossus", "ENIAC", "IBM 701"}, 2},
    {"What does 'RAM' stand for?",
     {"Read And Modify", "Random Access Memory", "Rapid Action Module", "Remote Application Manager"}, 1},
    {"Which programming language is named after a type of coffee?",
     {"Python", "C++", "Java", "Ruby"}, 2},
    {"What is the term for a malicious program that disguises itself as legitimate software?",
     {"Virus", "Worm", "Trojan horse", "Spyware"}, 2},
    {"Who is credited as the co-founder of Apple alongside Steve Jobs?",
     {"Bill Gates", "Steve Wozniak", "Tim Cook", "Paul Allen"}, 1},
    {"What does 'URL' stand for?",
     {"Universal Resource Link", "Uniform Resource Locator", "Unified Reference Label", "User Requested Location"}, 1},
    {"Which social media platform was originally called 'The Facebook'?",
     {"Twitter", "Instagram", "Facebook", "MySpace"}, 2},
    {"What year was the World Wide Web made publicly available?",
     {"1989", "1990", "1991", "1993"}, 2}
};

// ============================================
// SET 2: Logic & Riddles I
// The mind is a door that opens inward.
// ============================================
static QuizQuestion set2_riddles1[]={
    {"What has keys but can't open locks?",
     {"A map", "A piano", "A keyboard", "A code"}, 1},
    {"I am an odd number. Take away one letter and I become even. What number am I?",
     {"Seven", "Five", "Nine", "Eleven"}, 0},
    {"A man looks at a photo. Someone asks who it is. He says: 'Brothers and sisters I have none, but this man's father is my father's son.' Who is in the photo?",
     {"His brother", "His father", "His son", "Himself"}, 2},
    {"What comes once in a minute, twice in a moment, but never in a thousand years?",
     {"The letter 'M'", "The letter 'O'", "The number 1", "Time"}, 0},
    {"The more you take, the more you leave behind. What am I?",
     {"Memories", "Footsteps", "Breath", "Time"}, 1},
    {"Two fathers and two sons go fishing. They catch exactly 3 fish and each person gets one. How?",
     {"They shared one fish", "It's a grandfather, father, and son", "One son didn't eat", "They found an extra fish"}, 1},
    {"What can travel around the world while staying in a corner?",
     {"A bird", "A plane", "A stamp", "The internet"}, 2},
    {"If you have me, you want to share me. If you share me, you don't have me. What am I?",
     {"Money", "A secret", "Knowledge", "Love"}, 1},
    {"What building has the most stories?",
     {"A skyscraper", "A castle", "The library", "A hotel"}, 2},
    {"A farmer has 17 sheep, and all but 9 die. How many are left?",
     {"8", "9", "17", "0"}, 1}
};

// ============================================
// SET 3: Logic & Riddles II
// Some questions answer themselves if you wait long enough.
// ============================================
static QuizQuestion set3_riddles2[]={
    {"What has a neck but no head?",
     {"A snake", "A bottle", "A shirt", "A guitar"}, 1},
    {"I speak without a mouth and hear without ears. I have no body, but I come alive with wind. What am I?",
     {"A ghost", "A shadow", "An echo", "A dream"}, 2},
    {"A man pushes his car to a hotel and tells the owner he's bankrupt. Why?",
     {"He lost a bet", "He's playing Monopoly", "His car broke down", "He has no money"}, 1},
    {"What gets wetter the more it dries?",
     {"A sponge", "A towel", "The ground", "Paint"}, 1},
    {"Forward I am heavy, backward I am not. What am I?",
     {"The word 'ton'", "A truck", "Time", "A rock"}, 0},
    {"There's a one-story house where everything is pink. What color are the stairs?",
     {"Pink", "There are no stairs", "White", "Brown"}, 1},
    {"You see a boat filled with people, yet there isn't a single person on board. How?",
     {"They're all married", "It's a ghost ship", "They're below deck", "It's a model"}, 0},
    {"What can you catch but not throw?",
     {"A ball", "A cold", "A fish", "A frisbee"}, 1},
    {"I have branches but no fruit, trunk, or leaves. What am I?",
     {"A river", "A bank", "A family tree", "A road"}, 1},
    {"What has one eye but can't see?",
     {"A cyclops", "A needle", "A storm", "A potato"}, 1}
};

// ============================================
// SET 4: General Knowledge
// The world is larger than the maps we draw of it.
// ============================================
static QuizQuestion set4_general[]={
    {"What is the largest ocean on Earth?",
     {"Atlantic Ocean", "Indian Ocean", "Pacific Ocean", "Arctic Ocean"}, 2},
    {"Which planet is known as the 'Red Planet'?",
     {"Venus", "Mars", "Jupiter", "Saturn"}, 1},
    {"Who painted the Mona Lisa?",
     {"Michelangelo", "Leonardo da Vinci", "Raphael", "Donatello"}, 1},
    {"What is the capital city of Australia?",
     {"Sydney", "Melbourne", "Canberra", "Brisbane"}, 2},
    {"Which element has the chemical symbol 'Fe'?",
     {"Fluorine", "Francium", "Iron", "Fermium"}, 2},
    {"How many continents are there?",
     {"Five", "Six", "Seven", "Eight"}, 2},
    {"What is the longest river in the world?",
     {"Amazon", "Nile", "Yangtze", "Mississippi"}, 1},
    {"Who wrote 'Romeo and Juliet'?",
     {"Charles Dickens", "William Shakespeare", "Jane Austen", "Mark Twain"}, 1},
    {"What is the tallest mountain in the world?",
     {"K2", "Mount Kilimanjaro", "Mount Everest", "Mount Fuji"}, 2},
    {"Which country gifted the Statue of Liberty to the United States?",
     {"United Kingdom", "Germany", "France", "Spain"}, 2}
};

// ============================================
// SET 5: Twisted Maths
// Numbers lie less than people do. But they still lie.
// ============================================
static QuizQuestion set5_maths[]={
    {"If a hen and a half lays an egg and a half in a day and a half, how many eggs does one hen lay in one day?",
     {"0.5 eggs", "1 egg", "1.5 eggs", "2 eggs"}, 1},
    {"I am a three-digit number. My tens digit is five more than my units digit. My hundreds digit is eight less than my tens digit. What number am I?",
     {"194", "491", "950", "592"}, 0},
    {"A bat and a ball cost $1.10 together. The bat costs $1.00 more than the ball. How much does the ball cost?",
     {"5 cents", "10 cents", "$1.00", "$1.05"}, 0},
    {"If you multiply me by any other number, the answer will always remain the same. What number am I?",
     {"One", "Zero", "Negative one", "Infinity"}, 1},
    {"A rope burns unevenly and takes exactly 60 minutes to burn completely. How do you measure exactly 45 minutes using two such ropes?",
     {"Light both ends of one rope and one end of the other", "Fold the rope in half", "Burn them sequentially", "Use a stopwatch"}, 0},
    {"Three friends split a $30 bill evenly. They get a $5 refund, take $1 each, and the waiter keeps $2. They paid $9 each = $27, plus $2 tip = $29. Where is the missing $1?",
     {"The waiter stole it", "There is no missing dollar", "It's in the refund", "Math error"}, 1},
    {"What is the next number in the sequence: 1, 11, 21, 1211, 111221, ...?",
     {"221111", "312211", "13112221", "1113213211"}, 1},
    {"If two's company and three's a crowd, what are four and five?",
     {"A party", "Nine", "Too many", "A group"}, 1},
    {"A clock loses 15 minutes every hour. If set correctly at noon, what will it show at midnight (real time)?",
     {"9:00 PM", "10:00 PM", "11:00 PM", "Midnight"}, 0},
    {"I'm an even number. Remove one letter and I become odd. What number am I?",
     {"Eight", "Six", "Four", "Ten"}, 0}
};

static QuizQuestion *allSets[5]={set1_tech, set2_riddles1, set3_riddles2, set4_general, set5_maths};
static int setSizes[5]={10, 10, 10, 10, 10};

// ============================================
// The trial begins here. What you know will be tested.
// What you don't know will cost you.
// ============================================
static QuizQuestion *currentQuestions;
static int currentSetSize;
static int currentQuestion;
static int selectedOption;
static int puzzleState;
static int lives;
static int score;
static float timer;
static const float TIME_LIMIT=15.0f;
static int chapter;
static int questionsPerChapter;

static void DrawWrapped(const char *text, int x, int y, int maxWidth, int fontSize, Color color){
    int len=strlen(text);
    int lineStart=0;
    int lineY=y;
    int lastSpace=-1;

    for(int i=0; i<=len; i++){
        if(text[i]==' ' || text[i]=='\0'){
            int wordLen=i-lineStart;
            if(wordLen>0){
                char word[256];
                strncpy(word, text+lineStart, wordLen);
                word[wordLen]='\0';

                char testLine[4096];
                if(lineStart>0 && lastSpace>0){
                    int lineLen=lastSpace-lineStart+wordLen+1;
                    strncpy(testLine, text+lineStart, lineLen);
                    testLine[lineLen]='\0';
                } else {
                    strcpy(testLine, word);
                }

                if(MeasureText(testLine, fontSize)>maxWidth && lastSpace>lineStart){
                    int cut=lastSpace-lineStart;
                    char line[4096];
                    strncpy(line, text+lineStart, cut);
                    line[cut]='\0';
                    DrawText(line, x, lineY, fontSize, color);
                    lineY+=fontSize+8;
                    lineStart=lastSpace+1;
                }
            }
            lastSpace=i;
        }
    }
    if(lineStart<len){
        DrawText(text+lineStart, x, lineY, fontSize, color);
    }
}

void InitPuzzleSystem(int ch){
    chapter=ch;
    int setIndex=(ch-1)%5;
    currentQuestions=allSets[setIndex];
    currentSetSize=setSizes[setIndex];
    questionsPerChapter=5;
    if(questionsPerChapter>currentSetSize) questionsPerChapter=currentSetSize;

    // Shuffle the questions. The order of testing should not be predictable.
    for(int i=currentSetSize-1; i>0; i--){
        int j=GetRandomValue(0, i);
        QuizQuestion temp=currentQuestions[i];
        currentQuestions[i]=currentQuestions[j];
        currentQuestions[j]=temp;
    }

    currentQuestion=0;
    selectedOption=0;
    lives=3;
    score=0;
    timer=TIME_LIMIT;
    puzzleState=0;
}

void UpdatePuzzleSystem(void){
    if(puzzleState!=0){
        if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)){
            puzzleState=1;
        }
        return;
    }

    if(currentQuestion>=questionsPerChapter){
        puzzleState=1;
        return;
    }

    QuizQuestion *q=&currentQuestions[currentQuestion];

    // Time is not your ally here. It never was.
    timer-=GetFrameTime();
    if(timer<=0){
        timer=0;
        lives--;
        if(lives<=0){
            puzzleState=2;
            return;
        }
        currentQuestion++;
        timer=TIME_LIMIT;
        selectedOption=0;
        return;
    }

    // Navigate the choices. One of them is true. The others are distractions.
    if(IsKeyPressed(KEY_UP) || IsKeyPressed(KEY_W)){
        selectedOption--;
        if(selectedOption<0) selectedOption=3;
    }
    if(IsKeyPressed(KEY_DOWN) || IsKeyPressed(KEY_S)){
        selectedOption++;
        if(selectedOption>3) selectedOption=0;
    }

    // Commit to your answer. There is no undoing what you choose.
    if(IsKeyPressed(KEY_ENTER) || IsKeyPressed(KEY_SPACE)){
        if(selectedOption==q->correctAnswer){
            score++;
            currentQuestion++;
            timer=TIME_LIMIT;
            selectedOption=0;
        } else {
            lives--;
            if(lives<=0){
                puzzleState=2;
                return;
            }
            currentQuestion++;
            timer=TIME_LIMIT;
            selectedOption=0;
        }
    }
}

void DrawPuzzleSystem(void){
    ClearBackground((Color){10, 10, 25, 255});

    // Title bar — the trial has a name, even if you don't know it yet.
    DrawRectangle(0, 0, 1280, 70, (Color){20, 20, 40, 255});
    DrawText(TextFormat("CHAPTER %d — TRIAL OF KNOWLEDGE", chapter), 380, 20, 32, GOLD);

    if(puzzleState==0 && currentQuestion<questionsPerChapter){
        QuizQuestion *q=&currentQuestions[currentQuestion];

        // Lives. You start with three. Most people lose the first one without noticing.
        DrawText("LIVES: ", 50, 90, 24, WHITE);
        for(int i=0; i<3; i++){
            Color heartColor=(i<lives) ? RED : DARKGRAY;
            DrawText("<3", 140+(i*40), 90, 24, heartColor);
        }

        DrawText(TextFormat("SCORE: %d / %d", score, questionsPerChapter), 1000, 90, 24, GREEN);
        DrawText(TextFormat("QUESTION %d / %d", currentQuestion+1, questionsPerChapter), 520, 90, 24, WHITE);

        // Timer. It runs out the way all things run out — quietly, then all at once.
        DrawRectangle(100, 140, 1080, 25, DARKGRAY);
        float timerPct=timer/TIME_LIMIT;
        Color timerColor=(timerPct>0.5f) ? GREEN : (timerPct>0.25f) ? YELLOW : RED;
        DrawRectangle(100, 140, 1080*timerPct, 25, timerColor);
        DrawText(TextFormat("%.1fs", timer), 615, 143, 20, BLACK);

        // The question. Read it carefully. The answer is always in the asking.
        DrawRectangle(80, 190, 1120, 140, (Color){25, 25, 45, 255});
        DrawRectangleLinesEx((Rectangle){80, 190, 1120, 140}, 3, GOLD);
        DrawWrapped(q->question, 120, 220, 1040, 26, WHITE);

        // Four doors. One opens. Three do not.
        const char *labels[]={"A", "B", "C", "D"};
        for(int i=0; i<4; i++){
            int y=360+(i*85);
            Color bgColor=(i==selectedOption) ? (Color){70, 70, 120, 255} : (Color){30, 30, 55, 255};
            Color borderColor=(i==selectedOption) ? GOLD : DARKGRAY;

            DrawRectangle(180, y, 920, 75, bgColor);
            DrawRectangleLinesEx((Rectangle){180, y, 920, 75}, 3, borderColor);

            DrawCircle(230, y+37, 25, (i==selectedOption) ? GOLD : DARKGRAY);
            DrawText(labels[i], 222, y+28, 24, BLACK);

            DrawText(q->options[i], 280, y+22, 22, WHITE);
        }

        DrawText("[UP/DOWN] Navigate  [ENTER] Select", 430, 710, 20, GRAY);
    }

    // The trial is complete. Whether you passed depends on what you brought with you.
    if(puzzleState==1){
        DrawRectangle(0, 0, 1280, 720, (Color){0, 0, 0, 200});

        if(lives>0){
            DrawText("TRIAL COMPLETE", 400, 200, 60, GREEN);
            DrawText(TextFormat("Final Score: %d / %d", score, questionsPerChapter), 450, 300, 36, GOLD);
            DrawText(TextFormat("Lives Remaining: %d", lives), 480, 360, 28, RED);

            if(score==questionsPerChapter){
                DrawText("Perfect. You know the story well.", 380, 430, 28, GREEN);
            } else if(score>=questionsPerChapter/2){
                DrawText("Well done. The path is open.", 420, 430, 28, YELLOW);
            } else {
                DrawText("You survived... barely.", 450, 430, 28, ORANGE);
            }
        }

        DrawText("Press ENTER to continue", 440, 550, 28, WHITE);
    }

    // The trial has failed. Some doors, once closed, do not open again.
    if(puzzleState==2){
        DrawRectangle(0, 0, 1280, 720, (Color){0, 0, 0, 200});
        DrawText("TRIAL FAILED", 430, 200, 60, RED);
        DrawText("You have lost all your lives.", 420, 300, 32, ORANGE);
        DrawText(TextFormat("Questions answered correctly: %d / %d", score, questionsPerChapter), 350, 370, 28, WHITE);

        DrawText("The door remains closed...", 430, 450, 28, DARKGRAY);
        DrawText("Press ENTER to retry", 460, 550, 28, WHITE);
    }
}

int IsPuzzleFinished(void){
    return puzzleState;
}

int GetPuzzleScore(void){
    return score;
}

int GetPuzzleLives(void){
    return lives;
}