#define START 0		
#define PLAYING 1
#define GAME_OVER 2

void StateMachine(){
     int state = START; 
     int points = 0;
     while (1)
     {
     	switch (state)
	{
		START:
			if (PRESS_PLAY) {
                state = PLAYING
            }
			break;
		PLAYING:
			if (MOLE_MISS) {
                state = PLAYING
                points += 0
            }
            if (MOLE_HIT) {
                state = PLAYING
                points += 10
            }
			break;
		GAME_OVER:
			if (TWO_SECOND_DELAY) {
                state = START
            }
			break;
	}
     }
}