extern volatile unsigned char btn0Pressed;
extern volatile unsigned char btn0Changed;
extern volatile unsigned char btn1Pressed;
extern volatile unsigned char btn1Changed;

#ifdef QUT_TEENSY_TWO
extern volatile unsigned char dpadUpPressed;
extern volatile unsigned char dpadUpChanged;

extern volatile unsigned char dpadDownPressed;
extern volatile unsigned char dpadDownChanged;

extern volatile unsigned char dpadLeftPressed;
extern volatile unsigned char dpadLeftChanged;

extern volatile unsigned char dpadRightPressed;
extern volatile unsigned char dpadRightChanged;

extern volatile unsigned char dpadCenterPressed;
extern volatile unsigned char dpadCenterChanged;
#endif
