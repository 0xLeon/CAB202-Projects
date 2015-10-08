#include <stdint.h>

extern volatile uint8_t btn0Pressed;
extern volatile uint8_t btn0Changed;
extern volatile uint8_t btn1Pressed;
extern volatile uint8_t btn1Changed;

#ifdef QUT_TEENSY_TWO
extern volatile uint8_t dpadUpPressed;
extern volatile uint8_t dpadUpChanged;

extern volatile uint8_t dpadDownPressed;
extern volatile uint8_t dpadDownChanged;

extern volatile uint8_t dpadLeftPressed;
extern volatile uint8_t dpadLeftChanged;

extern volatile uint8_t dpadRightPressed;
extern volatile uint8_t dpadRightChanged;

extern volatile uint8_t dpadCenterPressed;
extern volatile uint8_t dpadCenterChanged;
#endif
