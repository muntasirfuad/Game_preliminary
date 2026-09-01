#ifndef PUZZLE_SYSTEM_H
#define PUZZLE_SYSTEM_H

void InitPuzzleSystem(int chapter);
void UpdatePuzzleSystem(void);
void DrawPuzzleSystem(void);
int IsPuzzleFinished(void);
int GetPuzzleScore(void);
int GetPuzzleLives(void);

#endif