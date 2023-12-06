#define START 0		
#define ZERO 1
#define ONE 2
#define TWO 3
#define THREE 4
#define FOUR 5
#define FIVE 6
#define SIX 7
#define SEVEN 8
#define GAME_OVER 9

void StateMachine(){
     int state = START; 
     int points = 0;
     while (1) {
     	switch (state) {
            START:
                if (PRESS_PLAY) {
                    state = ZERO;
                }
                break;
            ZERO:
                if (RAND/GEN_MOLES) {
                    state = GENERATED_STATE; // Could be between ZERO to SEVEN
                }
                break;
            ONE:
                if (HIT_LEFT) {
                    state = ONE;
                    points += 0;
                }
                if (HIT_MIDDLE) {
                    state = ONE;
                    points += 0;
                }
                if (HIT_RIGHT) {
                    state = ZERO;
                    points += 10;
                }
                if (TIMER_OVER) {
                    state = ZERO;
                }
                if (TIME_PASSED) {
                    state = GAME_OVER; // 60 seconds passed
                }
                break;
            TWO:
                if (HIT_LEFT) {
                    state = TWO;
                    points += 0;
                }
                if (HIT_MIDDLE) {
                    state = ZERO;
                    points += 10;
                }
                if (HIT_RIGHT) {
                    state = TWO;
                    points += 0;
                }
                if (TIMER_OVER) {
                    state = ZERO;
                }
                if (TIME_PASSED) {
                    state = GAME_OVER; // 60 seconds passed
                }
                break;
            THREE:
                if (HIT_LEFT) {
                    state = THREE;
                    points += 0;
                }
                if (HIT_MIDDLE) {
                    state = ONE;
                    points += 10;
                }
                if (HIT_RIGHT) {
                    state = TWO;
                    points += 10;
                }
                if (TIMER_OVER) {
                    state = ZERO;
                }
                if (TIME_PASSED) {
                    state = GAME_OVER; // 60 seconds passed
                }
                break;
            FOUR:
                if (HIT_LEFT) {
                    state = ZERO;
                    points += 10;
                }
                if (HIT_MIDDLE) {
                    state = FOUR;
                    points += 0;
                }
                if (HIT_RIGHT) {
                    state = FOUR;
                    points += 0;
                }
                if (TIMER_OVER) {
                    state = ZERO;
                }
                if (TIME_PASSED) {
                    state = GAME_OVER; // 60 seconds passed
                }
                break;
            FIVE:
                if (HIT_LEFT) {
                    state = ONE;
                    points += 10;
                }
                if (HIT_MIDDLE) {
                    state = FIVE;
                    points += 0;
                }
                if (HIT_RIGHT) {
                    state = FOUR;
                    points += 10;
                }
                if (TIMER_OVER) {
                    state = ZERO;
                }
                if (TIME_PASSED) {
                    state = GAME_OVER; // 60 seconds passed
                }
                break;
            SIX:
                if (HIT_LEFT) {
                    state = TWO;
                    points += 10;
                }
                if (HIT_MIDDLE) {
                    state = FOUR;
                    points += 10;
                }
                if (HIT_RIGHT) {
                    state = SIX;
                    points += 0;
                }
                if (TIMER_OVER) {
                    state = ZERO;
                }
                if (TIME_PASSED) {
                    state = GAME_OVER; // 60 seconds passed
                }
                break;
            SEVEN:
                if (HIT_LEFT) {
                    state = THREE;
                    points += 10;
                }
                if (HIT_MIDDLE) {
                    state = FIVE;
                    points += 10;
                }
                if (HIT_RIGHT) {
                    state = SIX;
                    points += 10;
                }
                if (TIMER_OVER) {
                    state = ZERO;
                }
                if (TIME_PASSED) {
                    state = GAME_OVER; // 60 seconds passed
                }
                break;
            GAME_OVER:
                if (TWO_SECOND_DELAY) {
                    state = START;
                }
                break;
	    }
     }
}
