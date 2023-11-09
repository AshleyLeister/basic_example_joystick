/*
 * application.h
 *
 *  Created on: Nov 9, 2023
 *      Author: user
 */

#ifndef APPLICATION_H_
#define APPLICATION_H_
/*
 * Application.h
 *
 *  Created on: Dec 29, 2019
 *      Author: Matthew Zhong
 */


#include <HAL/HAL.h>
#include <HAL/Timer.h>
#include <HAL/Graphics.h>

struct _Application
{
    // Put your application members and FSM state variables here!
    // =========================================================================
    GFX gfx;  //gfx stands for grahics!!!

    unsigned int frameIndex;
    unsigned int frameOffset;

    unsigned int frameIndexx;
     unsigned int frameOffsetx;

    SWTimer Launchpad_LED2_blinkingTimer;
};
typedef struct _Application Application;

// Called only a single time - inside of main(), where the application is constructed
Application Application_construct();

// Called once per super-loop of the main application.
void Application_loop(Application* app, HAL* hal);


void MoveCircle(GFX* gfx, bool moveToLeft, bool moveToRight,bool moveToUp, bool moveToDown, Application* app);


#endif /* APPLICATION_H_ */
