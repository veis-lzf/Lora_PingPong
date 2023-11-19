#ifndef __EXIT_APP_H__
#define __EXIT_APP_H__

#include "main.h"
#include "exit_app.h"

typedef enum
{
	e_Falling = 0,
	e_Rising,
	e_Falling_Rising,
	e_Low_Level,
	e_High_Level,
} eEXIT_Type_t;

void InitExitCapture(eEXIT_Type_t type);
uint32_t GetTickCount(void);
void startExitCapture(void);
void stopExitCapture(void);
void clearTickCount(void);

#endif /* __EXIT_APP_H_ */
