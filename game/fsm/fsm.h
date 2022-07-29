#ifndef PUZZLEBOTS_FSM_H
#define PUZZLEBOTS_FSM_H

#include "../bot_automaton/bot_automaton.h"
#include "../level/levelloader.h"
#include <assert.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define MAX_FSM_STATES 8
#define MAX_FSM_INPUTS 8
#define MAX_FSM_ACTION 8

typedef struct {
    char state[20];
    uint32_t pos_input_bitmask;
    uint32_t neg_input_bitmask;
    char next_state[20];
    uint8_t decisions[4];
} Line;

typedef struct {
    Line lines[64];
    LevelName level;
    int disabled;
    int delayed_action;
    char* state_strings[8];
} Fsm;

Fsm* fsm_alloc();
void init_fsm(Fsm* fsm, LevelName level);
int split(char*, char*, char**);
void bitshift(char*, char*, int);
int test(Fsm*, uint32_t, int);
int convert_decision(char*);
int next_move(Fsm* fsm, uint32_t, int*, char*);
void set_binairy(Fsm* fsm, uint8_t* table);
uint8_t state_to_int(char**, char*);

#endif // PUZZLEBOTS_FSM_H
