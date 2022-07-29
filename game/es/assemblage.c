#include "assemblage.h"
#include "../bot_automaton/BotAutomaton_c_wrapper.h"
#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <stdlib.h>

int DIRS[4] = {N, W, S, E};

void makeEntity(Engine* engine, Level* level, LevelCell* c, t_ivec3 pos);
int getNeighbourZ(Level* level, t_ivec3 fromPos, Direction dir);

void createLevelEntities(Engine* engine, Level* level) {

    EntityId player_entity_id = get_new_entity_id(engine);

    int counter[ECOLOR_MAX + 1] = {0};

    t_ivec3 eyeStart;
    glmc_ivec3_set(eyeStart, 0, 0, level->cells[0][0].z);
    createEye(engine, eyeStart, 1);
    // initialize teleport collection array
    t_ivec3 teleports[ECOLOR_MAX + 1][2];
    t_ivec3 initialVec;
    glmc_ivec3_set(initialVec, -1, -1, -1);
    for (int i = ECOLOR_A; i <= ECOLOR_MAX; i++) {
        glmc_assign_ivec3(teleports[i][0], initialVec);
        glmc_assign_ivec3(teleports[i][1], initialVec);
    }

    for (int i = 0; i < level->width; i++) {
        for (int j = 0; j < level->height; j++) {
            LevelCell* c = &(level->cells[i][j]);
            if (c->content_mask == CELL_TELEPORT) {
                // first collect all teleports according to colors
                glmc_ivec3_set(teleports[c->color][counter[c->color]], i, j, c->z);
                counter[c->color] += 1;
            }

            t_ivec3 pos;
            glmc_ivec3_set(pos, i, j, c->z);

            makeEntity(engine, level, c, pos);
        };
    };
    // create the teleports pair by pair, this assumes there will be 2 of each
    for (EntityColor i = ECOLOR_A; i <= ECOLOR_MAX; i++) {
        if (!glmc_equal_ivec3(teleports[i][0], initialVec)) {
            createTeleports(engine, teleports[i][0], teleports[i][1], i);
        }
    }
}

void makeEntity(Engine* engine, Level* level, LevelCell* c, t_ivec3 pos) {
    // every cell will have at least a floor
    createFloorAndWall(engine, level, c, pos);

    switch (c->content_mask) {
    case CELL_DOOR: {
        Direction dir = E;
        if (getNeighbourZ(level, pos, N) != pos[2]) {
            dir = N;
        }
        createDoor(engine, pos, dir, c->color);
        break;
    }
    case CELL_START: {
        // TODO: not sure if bots are supposed to work this way
        createBot(engine, pos, 0, level->nr, 0);
        break;
    }
    case CELL_EXIT:
        createExit(engine, pos);
        break;
    case CELL_BUTTON_HOLD:
        createButton(engine, pos, c->color, 1);
        break;
    case CELL_BUTTON_ONCE:
        createButton(engine, pos, c->color, 0);
        break;
    case CELL_WEIGHTED_HEXPRISM:
        createWeightedHexPrism(engine, pos);
        break;
    case CELL_WEIGHTED_TETRAHEDRON:
        createWeightedTetrahedron(engine, pos);
        break;
    default:
        break;
    }
}

int getNeighbourZ(Level* level, t_ivec3 fromPos, Direction dir) {
    t_ivec2 gridpos;
    glmc_ivec2_set(gridpos, fromPos[0], fromPos[1]);
    LevelCell* neighbor = levelcell_get_neighbor(level, gridpos, dir);
    return neighbor->z;
}

EntityId createFloorAndWall(Engine* engine, Level* level, LevelCell* c, t_ivec3 pos) {
    EntityId eid = get_new_entity_id(engine);
    GridLocationComponent* gl = create_component(engine, eid, COMP_GRIDLOCATION);
    glmc_assign_ivec3(gl->pos, pos);

    WallArtComponent* wac = create_component(engine, eid, COMP_WALLART);
    wac->has_floor = 1;

    ArtComponent* ac = create_component(engine, eid, COMP_ART);
    ac->type = ART_WALL;
    // calculate the required wallheight based on surrounding cells
    t_ivec2 gridpos;
    glmc_ivec2_set(gridpos, pos[0], pos[1]);
    for (int i = 0; i < 4; i++) {
        int wh = levelcell_wall_height(level, gridpos, DIRS[i]);
        wac->wall_height[DIRS[i]] = wh;
    }
    return eid;
}

EntityId createBot(Engine* engine, t_ivec3 gridlocation, int controlled, int level_nr,
                   int bot_index) {

    EntityId eid = get_new_entity_id(engine);

    GridLocationComponent* grid_comp = create_component(engine, eid, COMP_GRIDLOCATION);
    glmc_assign_ivec3(grid_comp->pos, gridlocation);

    BotAutomaton1Component* ba1c = create_component(engine, eid, COMP_BOT_AUTOMATON_1);
    ba1c->is_active = 0;
    strcpy(ba1c->state, "lost");

    BotAutomaton2Component* ba2c = create_component(engine, eid, COMP_BOT_AUTOMATON_2);
    ba2c->is_active = 0;
    ba2c->botberri = createBotBerri(10000);
    initBotBerri(ba2c->botberri);
    loadFSM(ba2c->botberri, engine->botautomaton_system->state_table);

    DelayedActionComponent* dac = create_component(engine, eid, COMP_DELAYED_ACTION);
    dac->amount = 0;

    ArtComponent* art_comp = create_component(engine, eid, COMP_ART);
    art_comp->type = ART_PLAYER;

    ExitAffectedComponent* exitaffected_comp = create_component(engine, eid, COMP_EXIT_AFFECTED);
    exitaffected_comp->required_for_level_finish = 1;

    WalkComponent* wc = create_component(engine, eid, COMP_WALKABLE);
    wc->walkHeight = 1;

    create_component(engine, eid, COMP_GRAVITY);
    create_component(engine, eid, COMP_BOT_VISION);
    create_component(engine, eid, COMP_VISION);
    create_component(engine, eid, COMP_BUTTON_ACTIVATOR);
    ItemActionComponent* itemaction = create_component(engine, eid, COMP_ITEMACTION);
    itemaction->do_action = 0;
    create_component(engine, eid, COMP_CONTROLLABLE);
    ContainerComponent* cont = create_component(engine, eid, COMP_CONTAINER);
    cont->item = NO_ENTITY;

    DirectionComponent* dir_comp = create_component(engine, eid, COMP_DIRECTION);
    dir_comp->dir = N;

    CameraLookFromComponent* cameralookfrom = create_component(engine, eid, COMP_CAMERA_LOOK_FROM);
    cameralookfrom->distance = 1.0f;
    cameralookfrom->XYdegrees = 0.0f;
    cameralookfrom->Zdegrees = 25.0f;
    // this normally gets overridden by camera system
    glmc_vec3_set(cameralookfrom->pos, 0.0f, -4.0f, 7.0f);
    return eid;
}

EntityId createExit(Engine* engine, t_ivec3 tvec3) {
    EntityId eid = get_new_entity_id(engine);

    GridLocationComponent* gl = create_component(engine, eid, COMP_GRIDLOCATION);
    glmc_assign_ivec3(gl->pos, tvec3);

    ExitComponent* exit_c = create_component(engine, eid, COMP_EXIT);

    ArtComponent* art_comp = create_component(engine, eid, COMP_ART);
    art_comp->type = ART_END;

    return eid;
}

EntityId createDoor(Engine* engine, t_ivec3 tvec3, Direction direction, EntityColor entity_color) {
    EntityId eid = get_new_entity_id(engine);

    GridLocationComponent* gl_comp = create_component(engine, eid, COMP_GRIDLOCATION);
    glmc_assign_ivec3(gl_comp->pos, tvec3);

    DirectionComponent* dir_comp = create_component(engine, eid, COMP_DIRECTION);
    dir_comp->dir = direction;

    ColorComponent* color_comp = create_component(engine, eid, COMP_COLOR);
    color_comp->color = entity_color;

    ArtComponent* art_comp = create_component(engine, eid, COMP_ART);
    art_comp->type = ART_DOOR;

    WalkComponent* wc = create_component(engine, eid, COMP_WALKABLE);
    wc->walkHeight = 1;

    ActivationComponent* act_comp = create_component(engine, eid, COMP_ACTIVATION);
    act_comp->active = 0;

    ConditionalActivationComponent* condact_comp =
        create_component(engine, eid, COMP_CONDITIONAL_ACTIVATION);
    condact_comp->activation_id = entity_color;

    return eid;
}

EntityId createButton(Engine* engine, t_ivec3 gridlocation, EntityColor color, int requires_hold) {
    EntityId eid = get_new_entity_id(engine);

    ButtonComponent* button_comp = create_component(engine, eid, COMP_BUTTON);
    button_comp->requires_hold = requires_hold;

    GridLocationComponent* gl_comp = create_component(engine, eid, COMP_GRIDLOCATION);
    glmc_assign_ivec3(gl_comp->pos, gridlocation);

    ColorComponent* color_comp = create_component(engine, eid, COMP_COLOR);
    color_comp->color = color;

    ActivationComponent* act_comp = create_component(engine, eid, COMP_ACTIVATION);
    act_comp->active = 0;
    act_comp->activation_id = color;

    ArtComponent* art_comp = create_component(engine, eid, COMP_ART);
    if (requires_hold > 0) {
        art_comp->type = ART_BUTTON_HOLD;
    } else {
        art_comp->type = ART_BUTTON_ONCE;
    }
    return eid;
}

void createTeleports(Engine* engine, t_ivec3 gridlocation1, t_ivec3 gridlocation2,
                     EntityColor color) {
    EntityId eid1 = get_new_entity_id(engine);
    EntityId eid2 = get_new_entity_id(engine);

    GridLocationComponent* glc1 = create_component(engine, eid1, COMP_GRIDLOCATION);
    glmc_assign_ivec3(glc1->pos, gridlocation1);
    GridLocationComponent* glc2 = create_component(engine, eid2, COMP_GRIDLOCATION);
    glmc_assign_ivec3(glc2->pos, gridlocation2);

    ColorComponent* cc1 = create_component(engine, eid1, COMP_COLOR);
    cc1->color = color;
    ColorComponent* cc2 = create_component(engine, eid2, COMP_COLOR);
    cc2->color = color;

    ArtComponent* art_comp1 = create_component(engine, eid1, COMP_ART);
    art_comp1->type = ART_TELEPORT;
    ArtComponent* art_comp2 = create_component(engine, eid2, COMP_ART);
    art_comp2->type = ART_TELEPORT;

    TeleportComponent* tp1 = create_component(engine, eid1, COMP_TELEPORT);
    tp1->linked_teleporter = eid2;
    TeleportComponent* tp2 = create_component(engine, eid2, COMP_TELEPORT);
    tp2->linked_teleporter = eid1;
    LastTeleportInfo* li1 = create_component(engine, eid1, COMP_LAST_TELEPORT_INFO);
    li1->last_teleported = NO_ENTITY;
    LastTeleportInfo* li2 = create_component(engine, eid2, COMP_LAST_TELEPORT_INFO);
    li2->last_teleported = NO_ENTITY;
}

EntityId createWeightedTetrahedron(Engine* engine, t_ivec3 gridlocation) {
    EntityId eid = get_new_entity_id(engine);

    GridLocationComponent* gl = create_component(engine, eid, COMP_GRIDLOCATION);
    glmc_assign_ivec3(gl->pos, gridlocation);

    create_component(engine, eid, COMP_BUTTON_ACTIVATOR);

    ArtComponent* art = create_component(engine, eid, COMP_ART);
    art->type = ART_TETRAHEDRON;

    ItemComponent* item = create_component(engine, eid, COMP_ITEM);
    return eid;
}

EntityId createWeightedHexPrism(Engine* engine, t_ivec3 gridlocation) {
    EntityId eid = get_new_entity_id(engine);

    create_component(engine, eid, COMP_BUTTON_ACTIVATOR);

    GridLocationComponent* gl = create_component(engine, eid, COMP_GRIDLOCATION);
    glmc_assign_ivec3(gl->pos, gridlocation);

    WalkComponent* wc = create_component(engine, eid, COMP_WALKABLE);
    wc->walkHeight = 1;

    PushableComponent* pc = create_component(engine, eid, COMP_PUSHABLE);

    DirectionComponent* dc = create_component(engine, eid, COMP_DIRECTION);
    dc->dir = 0;

    GravityComponent* gc = create_component(engine, eid, COMP_GRAVITY);

    ArtComponent* art = create_component(engine, eid, COMP_ART);
    art->type = ART_HEXPRISM;
    return eid;
}

EntityId createEye(Engine* engine, t_ivec3 pos, int controlled) {
    EntityId eid = get_new_entity_id(engine);

    GridLocationComponent* gl = create_component(engine, eid, COMP_GRIDLOCATION);
    glmc_assign_ivec3(gl->pos, pos);

    ArtComponent* art = create_component(engine, eid, COMP_ART);
    art->type = ART_EYE;

    DirectionComponent* dir = create_component(engine, eid, COMP_DIRECTION);
    dir->dir = N;

    create_component(engine, eid, COMP_CAMERA_LOOK_AT);

    create_component(engine, eid, COMP_CONTROLLABLE);
    create_component(engine, eid, COMP_INPUTRECEIVER);
    CameraLookFromComponent* cameralookfrom = create_component(engine, eid, COMP_CAMERA_LOOK_FROM);
    cameralookfrom->distance = 10.0f;
    cameralookfrom->XYdegrees = 0.0f;
    cameralookfrom->Zdegrees = 25.0f;
    // this normally gets overridden by camera system
    glmc_vec3_set(cameralookfrom->pos, 0.0f, -4.0f, 7.0f);
    return eid;
}
