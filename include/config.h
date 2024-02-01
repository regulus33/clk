// changes what the screen considers to be 120 bpm vs 60 bpm etc....
// you'll need to play with this number.
#define PPQN 24
// Button debounce delay (50 seems like a lot right now)
#define DEBOUNCE_DELAY 50
#define HOLD_DELAY 1000
#define EXT_CLOCK_MSG_TIME 3000
#define DSBL_DSPL_MSG_TIME 1

/* defines when the external interrupt function fires. CHANGE will be 2x as often */
//#define PULSE_ON CHANGE
//#define PULSE_ON FALLING
#define PULSE_ON RISING