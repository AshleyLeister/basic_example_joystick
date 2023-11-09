#include <ti/devices/msp432p4xx/driverlib/driverlib.h>



#include "application.h"

#include <HAL/HAL.h>
#include <HAL/Timer.h>
#include <Application.h>
#include <HAL/Graphics.h>

/* Standard Includes */
#include <stdint.h>
#include <stdbool.h>
#include <string.h>
#include <stdio.h>


#define LEFT_THRESHOLD  1500
#define RIGHT_THRESHOLD  12000
#define UP_THRESHOLD  12000
#define DOWN_THRESHOLD  1500


// This function initializes all the peripherals except graphics
void initialize();
void ModifyLEDColor(bool leftButtonWasPushed, bool rightButtonWasPushed);

void initADC();
void startADC();
void initJoyStick();

void getSampleJoyStick(unsigned *X, unsigned *Y);


unsigned colormix(unsigned r,unsigned g,unsigned b) {
    return ((r & 0xff) << 16) | ((g & 0xff) << 8) | (b & 0xff);
}





int main(void)
{
    WDT_A_holdTimer();

    // Initialize the system clock and background hardware timer, used to enable
    // software timers to time their measurements properly.


    Graphics_Context g_sContext;

    InitSystemTiming();
    initialize();

    InitGraphics(&g_sContext);






    // Initialize the main Application object and HAL object
    HAL hal = HAL_construct();
    Application app = Application_construct();

    // Main super-loop! In a polling architecture, this function should call
    // your main FSM function over and over.
    while (true)
    {
        Application_loop(&app, &hal);
        HAL_refresh(&hal);



    }
}


Application Application_construct()
{
    Application app;
    unsigned vx, vy;


    // a 1-second timer (i.e. 1000 ms as specified in the SWTimer_contruct)
    app.Launchpad_LED2_blinkingTimer = SWTimer_construct(1000);
    SWTimer_start(&app.Launchpad_LED2_blinkingTimer);

    app.frameIndex = 40;
    app.frameOffset = 0;

    app.gfx = GFX_construct(GRAPHICS_COLOR_WHITE, GRAPHICS_COLOR_BLACK);


    Graphics_Context g_sContext;

    InitGraphics(&g_sContext);

    draw_Base(&g_sContext);


    return app;
}

void Application_loop(Application* app, HAL* hal)
{
    Graphics_Context g_sContext;

    // using static variable
    static bool pause = false;
    unsigned int r, g, b;
    unsigned vx, vy;


    getSampleJoyStick(&vx, &vy);//gets x and y values of joystick
    drawXY(&app->gfx.context, vx, vy);//draws the x and y values of the joystick


        r = 25;
        g = app->frameIndex*2;
        b = 254 - g;


      Graphics_setForegroundColor(&app->gfx.context,GRAPHICS_COLOR_BLUE );
        Graphics_fillCircle(&app->gfx.context,  20, (app->frameIndex + app->frameOffset)%90, 5);//wipe previous circles drawn

        //Graphics_setForegroundColor(&app->gfx.context,colormix(r,g,b));

        Graphics_setForegroundColor(&app->gfx.context,GRAPHICS_COLOR_BLUE );
              Graphics_fillCircle(&app->gfx.context,  70, (app->frameIndex + app->frameOffset)%90, 5);//wipe previous circles


       //  Graphics_setForegroundColor(&app->gfx.context,colormix(r,g,b));

        app->frameIndex++;
        app->frameIndexx++;




        if (app->frameIndex==90)
        {
            app->frameIndex = 40;
            app->frameOffset++;
            if (app->frameOffset==90)
                app->frameOffset = 0;
        }

       Graphics_setForegroundColor(&app->gfx.context,GRAPHICS_COLOR_PINK );
        Graphics_fillCircle(&app->gfx.context,  20, (app->frameIndex + app->frameOffset)%90, 5);

        Graphics_setForegroundColor(&app->gfx.context,GRAPHICS_COLOR_GREEN );
        Graphics_fillCircle(&app->gfx.context,  70, (app->frameIndex + app->frameOffset)%90, 5);
        ///////////////////////////////////////////// joystick controls

        bool joyStickPushedtoRight = false;////boolean for each type of joystick position
        bool joyStickPushedtoLeft = false;
        bool joyStickPushedtoUp = false;
        bool joyStickPushedtoDown = false;


        if (vx < LEFT_THRESHOLD)//if joystick reaches threshold pushed to left turns true
        {
            joyStickPushedtoLeft = true;
        }

        if (vx > RIGHT_THRESHOLD)////if joystick reaches threshold pushed to right turns true
              {
                  joyStickPushedtoRight = true;
              }

        if (vy > UP_THRESHOLD)///if joystick reaches threshold pushed to Up turns true
                {
                    joyStickPushedtoUp = true;
                }

        if (vy < DOWN_THRESHOLD)///if joystick reaches threshold pushed to down turns true
          {
           joyStickPushedtoDown = true;
          }

        MoveCircle(&app->gfx.context, joyStickPushedtoLeft,joyStickPushedtoRight,joyStickPushedtoDown,joyStickPushedtoUp,&app);//old circle is removed and new circle is drawn
 ////////////////////////////////////





}




void initialize()
{
    // stop the watchdog timer
    WDT_A_hold(WDT_A_BASE);

    initADC();
    initJoyStick();
    startADC();
}


// Initializing the ADC which resides on SoC
void initADC() {
    ADC14_enableModule();

    ADC14_initModule(ADC_CLOCKSOURCE_SYSOSC,
                     ADC_PREDIVIDER_1,
                     ADC_DIVIDER_1,
                      0
                     );

    // This configures the ADC to store output results
    // in ADC_MEM0 for joystick X.
    // Todo: if we want to add joystick Y, then, we have to use more memory locations
    ADC14_configureMultiSequenceMode(ADC_MEM0, ADC_MEM1, true);//made to use memory locations mem0 to mem1

    // This configures the ADC in manual conversion mode
    // Software will start each conversion.
    ADC14_enableSampleTimer(ADC_AUTOMATIC_ITERATION);
}


void startADC() {
   // Starts the ADC with the first conversion
   // in repeat-mode, subsequent conversions run automatically
   ADC14_enableConversion();
   ADC14_toggleConversionTrigger();
}


// Interfacing the Joystick with ADC (making the proper connections in software)
void initJoyStick() {

    // This configures ADC_MEM0 to store the result from
    // input channel A15 (Joystick X), in non-differential input mode
    // (non-differential means: only a single input pin)
    // The reference for Vref- and Vref+ are VSS and VCC respectively
    ADC14_configureConversionMemory(ADC_MEM0,
                                  ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                  ADC_INPUT_A15,                 // joystick X
                                  ADC_NONDIFFERENTIAL_INPUTS);

    ADC14_configureConversionMemory(ADC_MEM1,
                                     ADC_VREFPOS_AVCC_VREFNEG_VSS,
                                     ADC_INPUT_A9,                 // joystick Y
                                     ADC_NONDIFFERENTIAL_INPUTS);

    // This selects the GPIO as analog input
    // A15 is multiplexed on GPIO port P6 pin PIN0
    // TODO: which one of GPIO_PRIMARY_MODULE_FUNCTION, or
    //                    GPIO_SECONDARY_MODULE_FUNCTION, or
    //                    GPIO_TERTIARY_MODULE_FUNCTION
    // should be used in place of 0 as the last argument?
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P6,
                                               GPIO_PIN0,
                                               GPIO_TERTIARY_MODULE_FUNCTION);

    // TODO: add joystick Y
    GPIO_setAsPeripheralModuleFunctionInputPin(GPIO_PORT_P4,
                                               GPIO_PIN4,
                                               GPIO_TERTIARY_MODULE_FUNCTION);


}

void getSampleJoyStick(unsigned *X, unsigned *Y) {
    // ADC runs in continuous mode, we just read the conversion buffers
    *X = ADC14_getResult(ADC_MEM0);

    // TODO: Read the Y channel
    *Y = ADC14_getResult(ADC_MEM1);
}

