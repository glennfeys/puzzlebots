#ifndef SYSPROG_COMPONENT_ENUMS_H
#define SYSPROG_COMPONENT_ENUMS_H

#include <assert.h>

typedef enum ComponentId {
    COMP_GRIDLOCATION,
    COMP_MOVE_ANIMATION,
    COMP_ONE_TIME_ANIMATION,
    COMP_WALLART,
    COMP_WALKABLE,
    COMP_BLOCKING,
    COMP_ITEM,
    COMP_INPUTRECEIVER,
    COMP_EXIT_AFFECTED,
    COMP_MOVE_ACTION,
    COMP_ITEMACTION,
    COMP_CAMERA_LOOK_AT,
    COMP_CAMERA_LOOK_FROM,
    COMP_ART,
    COMP_DIRECTION,
    COMP_CONTAINER,
    COMP_INCONTAINER,
    COMP_EXIT,
    COMP_EXITED,
    COMP_VISION,
    COMP_VISIBLE,
    COMP_CONDITIONAL_ACTIVATION, // activates if a number of COMP_ACTIVATION with matching
                                 // activation_id activate
    COMP_ACTIVATION,       // implies something can be activated, and keeps activation state
    COMP_BUTTON,           // can activate COMP_ACTIVATABLE of same color
    COMP_BUTTON_ACTIVATOR, // activates button if at same position
    COMP_PUSHABLE,
    COMP_COLOR,
    COMP_TELEPORT,
    COMP_LAST_TELEPORT_INFO,
    COMP_GRAVITY,
    COMP_CONTROLLABLE,
    COMP_BOT_AUTOMATON_1,
    COMP_BOT_AUTOMATON_2,
    COMP_BOT_VISION,
    COMP_DELAYED_ACTION,
    COMP_DUMMY, // replace this by your own component ID
    // Optionally add components

    COMPONENT_ID_SIZE
} ComponentId;

// Make sure there are at least 35 components, the provided benchmark file depends on this.
static_assert(
    COMPONENT_ID_SIZE >= 35,
    "There must be a minimum number of components to be able to replay the provided benchmark");

typedef enum ArtType {
    ART_DOOR,
    ART_PLAYER,
    ART_EYE,
    ART_END,
    ART_WALL,
    ART_BUTTON_HOLD,
    ART_BUTTON_ONCE,
    ART_TETRAHEDRON,
    ART_HEXPRISM,
    ART_TELEPORT
} ArtType;

#endif // SYSPROG_COMPONENT_ENUMS_H
