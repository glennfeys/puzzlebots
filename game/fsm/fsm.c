#include "fsm.h"
#include <stdio.h>
#include <string.h>

#define MAX_ELEMENTS 4

Line current_line;

Fsm* fsm_alloc() {
    Fsm* fsm = malloc(sizeof(Fsm));
    return fsm;
}
// funtion to split a string with a separator and put it into an array
int split(char* string, char* separator, char* split_line[]) {
    char* token_1;
    token_1 = strtok(string, separator);
    int size_split = 0;
    while (token_1 != NULL) {
        split_line[size_split] = token_1;
        size_split++;
        token_1 = strtok(NULL, separator);
    }
    return size_split;
}

void bitshift(char* location, char* object, int neg_mask) {
    int number_of_bitshifts = 0;
    uint32_t object_bits = 0;
    if (strcmp(location, "FRONT") == 0) {
        number_of_bitshifts = 24;
    } else if (strcmp(location, "LEFT") == 0) {
        number_of_bitshifts = 16;
    } else if (strcmp(location, "RIGHT") == 0) {
        number_of_bitshifts = 8;
    }

    if (strcmp(object, "WALL") == 0) {
        object_bits += 1;
    }
    if (strcmp(object, "TETRA") == 0) {
        object_bits += 2;
    }
    if (strcmp(object, "HEX") == 0) {
        object_bits += 4;
    }
    if (strcmp(object, "EXIT") == 0) {
        object_bits += 8;
    }
    if (strcmp(object, "PIT") == 0) {
        object_bits += 16;
    }
    if (strcmp(object, "BUTTON") == 0) {
        object_bits += 32;
    }
    if (strcmp(object, "DOOR") == 0) {
        object_bits += 64;
    }
    uint32_t mask = (object_bits << number_of_bitshifts);
    if (neg_mask) {
        current_line.neg_input_bitmask = mask | current_line.neg_input_bitmask;
    } else {
        current_line.pos_input_bitmask = mask | current_line.pos_input_bitmask;
    }
}

// initialize the finite state machine from a fsm file
void init_fsm(Fsm* fsm, LevelName level) {
    fsm->level = level;
    fsm->disabled = 0;
    fsm->delayed_action = -1;
    int decisions_index = 0;
    char level_name[] = "game/fsm/fsm-levels/x.fsm";
    level_name[20] = fsm->level + '0';
    FILE* fp;
    char line[100];

    printf("levelname = %s\n", level_name);

    fp = fopen(level_name, "r");
    if (fp == NULL) {
        fsm->disabled = 1;
        return;
    }

    while (fgets(line, 100, fp) != NULL) {
        // remove the \n at the end
        if (line[strlen(line) - 1] == '\n') {
            line[strlen(line) - 1] = '\0';
        }

        if (line[0] != '/' && line[1] != '/' && line[0] != '\0') {
            char separator[2] = " ";
            char* elements[MAX_ELEMENTS];
            int size = split(line, separator, elements);
            // the array elements now contains a line split by spaces.
            // elements[0] = State, elements[1] = all inputs, elements[2] = all ouputs, elements[3]
            // = end state
            strcpy(current_line.state, elements[0]);
            // this tests if the state stays the same or not
            if (size == 4) {
                strcpy(current_line.next_state, elements[3]);
            } else {
                strcpy(current_line.next_state, elements[0]);
            }

            // processing the inputs
            separator[0] = ',';
            char* inputs[MAX_FSM_INPUTS];
            int input_size = split(elements[1], separator, inputs);
            separator[0] = '_';
            for (int i = 0; i < input_size; i++) {
                char* array_input[3];
                split(inputs[i], separator, array_input);
                if (strcmp(array_input[0], "NO") == 0) {
                    bitshift(array_input[1], array_input[2], 1);
                } else {
                    bitshift(array_input[0], array_input[1], 0);
                }
            }

            // processing the outputs
            separator[0] = ',';
            char* ouputs[MAX_FSM_ACTION];
            size = split(elements[2], separator, ouputs);
            for (int i = 0; i < MAX_FSM_ACTION; i++) {
                if (i < size) {
                    fsm->lines[decisions_index].decisions[i] = convert_decision(ouputs[i]);
                } else {
                    fsm->lines[decisions_index].decisions[i] = 5;
                }
            }

            strcpy(fsm->lines[decisions_index].state, current_line.state);
            strcpy(fsm->lines[decisions_index].next_state, current_line.next_state);
            (fsm->lines)[decisions_index].neg_input_bitmask = current_line.neg_input_bitmask;
            (fsm->lines)[decisions_index].pos_input_bitmask = current_line.pos_input_bitmask;
            current_line.pos_input_bitmask = (uint32_t)0;
            current_line.neg_input_bitmask = (uint32_t)0;
            decisions_index++;
        }
    }
    // set all the other states that are not used to "" and set decisions to 5 (because 5 is always the end of decisions)
    Line others = { .state = "", .decisions = 5};
    for (int i = decisions_index; i < 64; i++) {
        (fsm->lines)[i] = others;
    }
    // this code here sets the state_strings of the fsm
    // bassicly state_strings maps the string of a state to the state number (= index)
    // used in set_binairy to convert strings of states to numbers
    uint8_t cstate_index = 0;
    char* previous = "";
    for (int index = 0; index < 64; index++) {
        if (fsm->lines[index].state[0] != '\0' && strcmp(fsm->lines[index].state, previous) != 0) {
            fsm->state_strings[cstate_index] = fsm->lines[index].state;
            previous = fsm->state_strings[cstate_index];
            cstate_index++;
        }
    }
    fclose(fp);
}

// checks if the inputs are correct and if so return true
int test(Fsm* fsm, uint32_t mask, int index) {
    uint32_t pos_mask = (fsm->lines)[index].pos_input_bitmask;
    uint32_t neg_mask = (fsm->lines)[index].neg_input_bitmask;
    return ((neg_mask & mask) || (pos_mask & (~mask))) == 0;
}

// Convert a decision to a integer
int convert_decision(char* text) {
    if (strcmp(text, "FORWARD") == 0) {
        return DECISION_FORWARD;
    } else if (strcmp(text, "TURN_LEFT") == 0) {
        return DECISION_TURN_LEFT;
    } else if (strcmp(text, "TURN_RIGHT") == 0) {
        return DECISION_TURN_RIGHT;
    } else if (strcmp(text, "IDLE") == 0) {
        return DECISION_IDLE;
    } else {
        return DECISION_ACTION;
    }
}

int next_move(Fsm* fsm, uint32_t vision_bitmask, int* moves, char* current_state) {
    int index = 0;
    printf("%s\n", current_state);

    while (strcmp((fsm->lines)[index].state, current_state)) {
        index++;
    }
    int same_state = strcmp((fsm->lines)[index].state, current_state) == 0;

    while (same_state && !test(fsm, vision_bitmask, index)) {
        index++;
        same_state = strcmp((fsm->lines)[index].state, current_state) == 0;
    }
    strcpy(current_state, (fsm->lines)[index].next_state);
    int i = 0;
    while (((fsm->lines)[index].decisions)[i] != 5) {
        int dec = (fsm->lines)[index].decisions[i];
        moves[i] = dec;
        i++;
    }
    return i;
}

// read the fsm table into the array table
void set_binairy(Fsm* fsm, uint8_t* table) {

    for (int i = 0; i < 64; i++) {
        Line cline = fsm->lines[i];
        if (strcmp(cline.state, "") != 0) {
            int start_index = i * 16;
            // set the correct bits to positive input bitmask
            for (int j = 0; j < 4; j++) {
                table[start_index + j] = (uint8_t) (cline.pos_input_bitmask >> (24 - (8 * j)));
            }
            // set the correct bits to negative input bitmask
            for (int k = 0; k < 4; k++) {
                table[start_index + 4 + k] = (uint8_t) (cline.neg_input_bitmask >> (24 - (8 * k)));
            }
            // where the new state should be set to
            table[start_index + 9] = state_to_int(fsm->state_strings, cline.state);
            // determine the amount of decisions that need to be done
            uint8_t size_decisions = 0;
            while (cline.decisions[size_decisions] != 5) {
                size_decisions++;
            }
            table[start_index + 11] = size_decisions;
            // set the actions in table
            for (int l = 0; l < size_decisions; l++) {
                table[start_index + 12 + l] = cline.decisions[l];
            }
        }
    }
}

// convert a string representation of a state to a integer representation
uint8_t state_to_int(char* state_strings[], char* state) {
    for (int i = 0; i < 8; i++) {
        if (strcmp(state_strings[i], state) == 0) {
            return i;
        }
    }
    return 9;
}
