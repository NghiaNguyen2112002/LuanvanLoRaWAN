#ifndef _LED_RGB_H_
#define _LED_RGB_H_



#include <Arduino.h>


/* RGB color 24bit */



#define DURATION_RESET_US                   60

#define DURATION_1H_NS                      600
#define DURATION_1L_NS                      600

#define DURATION_0H_NS                      300
#define DURATION_0L_NS                      900



#define NS_PER_SEC (1000000000L)          // Note that this has to be SIGNED since we want to be able to check for negative values of derivatives

#define CYCLES_PER_SEC (F_CPU)

#define NS_PER_CYCLE ( NS_PER_SEC / CYCLES_PER_SEC )

#define NS_TO_CYCLES(n) ( (n) / NS_PER_CYCLE )


void SK6812_Init(uint8_t ledPin);
void SK6812_Display(uint8_t red, uint8_t green, uint8_t blue);

#endif // !_LED_RGB_H_
