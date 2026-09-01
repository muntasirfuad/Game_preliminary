#ifndef INTRO_H
#define INTRO_H

void InitIntro(void);
void UpdateIntro(void);
void DrawIntro(void);
int IsIntroFinished(void);  // Returns: 0=still on intro, 1=new game, 2=resume, 3=quit

#endif