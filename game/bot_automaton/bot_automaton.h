#ifndef PUZZLEBOTS_BOT_AUTOMATION_H
#define PUZZLEBOTS_BOT_AUTOMATION_H

//////////////////////////////
// Automaton Decision Output  //
/////////////////////////////

typedef enum AutomatonDecision {
    DECISION_FORWARD = 0,
    DECISION_TURN_LEFT = 1,
    DECISION_TURN_RIGHT = 2,
    DECISION_IDLE = 3,
    DECISION_ACTION = 4, // opnemen of iets laten vallen

    AUTOMATON_DECISION_COUNT
} AutomatonDecision;

//////////////////////////
// Automaton Vision Input //
//////////////////////////

// bitmask for Automaton Vision Input
#define VISION_WALL 1u
#define VISION_TETRA 2u
#define VISION_HEX 4u
#define VISION_EXIT 8u
#define VISION_PIT 16u
#define VISION_BUTTON 32u
#define VISION_DOOR 64u

typedef enum VisionPosition {
    VISION_FRONT = 0,
    VISION_LEFT = 1,
    VISION_RIGHT = 2,
    VISION_CURR = 3,

    VISION_POSITION_COUNT
} VisionPosition;

#endif // PUZZLEBOTS_BOT_AUTOMATION_H
