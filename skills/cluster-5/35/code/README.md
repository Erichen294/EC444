Author: Eric Chen
Date: 12/3/23

Summary: This is the pseudo-code for the state table and the transitions. The states from ZERO to SEVEN represent it's equivalent binary number. The points start from zero and increment by ten if a mole is hit and zero if a mole isn't hit or is missed. The ZERO state represents a generation state that determines which moles appear where and at the end of the timer period every other state (besides START and GAME_OVER) will transition to ZERO to determine the next state. All states (besides ZERO and START) will transition to GAME_OVER when the 60 second gameplay timer is over.
