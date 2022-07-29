#include "bot_vision_system.h"

BotVisionSystem* system_botvision_alloc(Engine* engine) {
    BotVisionSystem* res = calloc(1, sizeof(BotVisionSystem));
    system_botvision_init(res, engine);
    return res;
}

void system_botvision_init(BotVisionSystem* system, Engine* engine) {
    system->system_id = BOT_VISION_SYSTEM;
}

void system_botvision_free(BotVisionSystem* system) {}

void system_botvision_update(BotVisionSystem* system, Engine* engine) {
    uint32_t vision_mask = 0;
    EntityIterator vision_it;
    search_entity_1(engine, COMP_BOT_VISION, &vision_it, system->system_id);
    // d = the direction you want to look at
    uint8_t d;
    while (next_entity(&vision_it)) {
        EntityId vision_id = vision_it.entity_id;
        BotVisionComponent* vision_comp = get_component(engine, vision_id, COMP_BOT_VISION);
        DirectionComponent* direction_component =
            get_component(engine, vision_id, COMP_DIRECTION); // direction the bot is looking
        GridLocationComponent* location = get_component(engine, vision_id, COMP_GRIDLOCATION);

        // search for the wall entity where the bot is currently standing in
        WallArtComponent* wall_comp;
        EntityIterator current_wall;
        search_grid_entity_2(engine, location->pos, 0, COMP_GRIDLOCATION, COMP_WALLART,
                             &current_wall, system->system_id);
        while (next_entity(&current_wall)) {
            EntityId wall_id = current_wall.entity_id;
            wall_comp = get_component(engine, wall_id, COMP_WALLART);
        }

        t_ivec2 direction;

        for (uint8_t i = 0; i < VISION_POSITION_COUNT; i++) {
            vision_mask = vision_mask << 8;
            t_ivec3 new_pos;
            if (i == 0) { // front
                d = direction_component->dir;
                get_direction(direction, d);
            } else if (i == 1) { // left
                d = (direction_component->dir + 1) % DIRECTION_COUNT;
                get_direction(direction, d);
            } else if (i == 2) { // right
                d = (direction_component->dir - 1) % DIRECTION_COUNT;
                get_direction(direction, d);
            } else if (i == 3) { // current
                glmc_ivec2_set(direction, 0, 0);
            }
            new_pos[0] = location->pos[0] + direction[0];
            new_pos[1] = location->pos[1] + direction[1];
            new_pos[2] = location->pos[2];

            glmc_assign_ivec3(vision_comp->locations[i], new_pos);

            EntityIterator visible_items;
            search_grid_entity_1(engine, new_pos, 1, COMP_GRIDLOCATION, &visible_items,
                                 system->system_id);
            uint8_t pit = 0;
            uint8_t is_walkable = 0;
            while (next_entity(&visible_items)) {
                EntityId visible_id = visible_items.entity_id;
                GridLocationComponent* visible_location =
                    get_component(engine, visible_id, COMP_GRIDLOCATION);
                if (visible_location->pos[2] < location->pos[2]) {
                    if (has_component(engine, visible_id, COMP_WALKABLE)) {
                        is_walkable = 1;
                    }
                    pit = 1;
                }
                if (has_component(engine, visible_id, COMP_ART)) {
                    ArtComponent* art_comp = get_component(engine, visible_id, COMP_ART);
                    if (art_comp->type == ART_DOOR) { // Door
                        WalkComponent* walk_component =
                            get_component(engine, visible_id, COMP_WALKABLE);
                        if (walk_component->walkHeight) {
                            vision_mask += 64;
                        }
                    } else if (art_comp->type == ART_END) { // EXIT
                        vision_mask += 8;
                    } else if (art_comp->type == ART_BUTTON_HOLD ||
                               art_comp->type == ART_BUTTON_ONCE) { // Button
                        vision_mask += 32;
                    } else if (art_comp->type == ART_TETRAHEDRON) { // Tetra
                        vision_mask += 2;
                    } else if (art_comp->type == ART_HEXPRISM) { // Hex
                        vision_mask += 4;
                    }
                }
            }
            if (!(vision_mask & 255) && i != 3 && wall_comp != NULL) {
                if (wall_comp->wall_height[d] == 1) {
                    vision_mask += 1;
                }
            }
            if (pit && !is_walkable && !(vision_mask & 255)) {
                vision_mask += 16;
            }
        }
        vision_comp->vision_mask = vision_mask;
    }
}

void get_direction(t_ivec2 direction, uint8_t d) {
    if (d == 0) { // North
        direction[0] = 0;
        direction[1] = 1;
    } else if (d == 1) { // West
        direction[0] = -1;
        direction[1] = 0;
    } else if (d == 2) { // South
        direction[0] = 0;
        direction[1] = -1;
    } else { // East
        direction[0] = 1;
        direction[1] = 0;
    }
}
