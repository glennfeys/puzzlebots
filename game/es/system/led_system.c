#include "led_system.h"
#include "./../../led/sense_led.h"

#define FRAMEBUFFER "/dev/fb1"

SPGM_RGBTRIPLE leds[64];

LedSystem* system_led_alloc(Engine* engine) {
    LedSystem* res = calloc(1, sizeof(LedSystem));
    system_led_init(res, engine);
    return res;
}

void system_led_init(LedSystem* system, Engine* engine) {
    system->system_id = LED_SYSTEM;
    const ComponentId components[2] = {COMP_BOT_VISION, COMP_INPUTRECEIVER};
    register_system_component(&engine->es_memory, LED_SYSTEM, 2, components);
}

void fill_colors(int direction, SPGM_RGBTRIPLE* color) {
    int color_mapping[4][20] = {
        {0, 1, 2, 3, 4, 5, 6, 7, 9, 10, 11, 12, 13, 14, 18, 19, 20, 21, 27, 28},
        {8, 16, 17, 24, 25, 26, 32, 33, 34, 40, 41, 48},
        {15, 22, 23, 29, 30, 31, 37, 38, 39, 46, 47, 55},
        {35, 36, 42, 43, 44, 45, 49, 50, 51, 52, 53, 54, 56, 57, 58, 59, 60, 61, 62, 63}};
    int number_of_leds = 20;
    if (direction == 1 || direction == 2) {
        number_of_leds = 12;
    }

    for (int i = 0; i < number_of_leds; i++) {
        leds[color_mapping[direction][i]].rgbRed = color->rgbRed;
        leds[color_mapping[direction][i]].rgbGreen = color->rgbGreen;
        leds[color_mapping[direction][i]].rgbBlue = color->rgbBlue;
    }
}

void find_color(SPGM_RGBTRIPLE* color, uint32_t vision) {
    if (vision & 16) {      // pit
        if (vision & 239) { // every thing but a pit
            find_color(color, vision & 239);
        } else {
            color->rgbRed = 0;
            color->rgbGreen = 255;
            color->rgbBlue = 255;
        }
    } else if (vision & 1) { // wall
        color->rgbRed = 255;
        color->rgbGreen = 0;
        color->rgbBlue = 0;
    } else if (vision & 2) { // Tetra
        color->rgbRed = 0;
        color->rgbGreen = 0;
        color->rgbBlue = 255;
    } else if (vision & 4) { // Hex
        color->rgbRed = 0;
        color->rgbGreen = 255;
        color->rgbBlue = 0;
    } else if (vision & 8) { // exit
        color->rgbRed = 255;
        color->rgbGreen = 255;
        color->rgbBlue = 255;
    } else if (vision & 32) { // button
        color->rgbRed = 255;
        color->rgbGreen = 0;
        color->rgbBlue = 255;
    } else if (vision & 64) { // door
        color->rgbRed = 255;
        color->rgbGreen = 255;
        color->rgbBlue = 0;
    } else {
        color->rgbRed = 0;
        color->rgbGreen = 0;
        color->rgbBlue = 0;
    }

    // case 32: {
    //     color->rgbRed = 255;
    //     color->rgbGreen = 0;
    //     color->rgbBlue = 255;

    // } break;
    // case 64: {
    //     color->rgbRed = 255;
    //     color->rgbGreen = 255;
    //     color->rgbBlue = 0;
    // } break;
    // default:
    //     color->rgbRed = 0;
    //     color->rgbGreen = 0;
    //     color->rgbBlue = 0;
    //     break;
    // }
}

void system_led_update(LedSystem* system, Engine* engine) {
    EntityId focusedbot_eid =
        search_first_entity_2(engine, COMP_BOT_VISION, COMP_INPUTRECEIVER, system->system_id);

    if (focusedbot_eid != NO_ENTITY) {
        BotVisionComponent* botvision_comp = get_component(engine, focusedbot_eid, COMP_BOT_VISION);
        uint32_t vision_mask = botvision_comp->vision_mask;
        uint32_t mask = 255;
        uint32_t shift_amt = 0;
        int size = 8;
        for (int i = 0; i < DIRECTION_COUNT; i++) {
            uint32_t vision = vision_mask & mask;
            vision = vision >> shift_amt;
            SPGM_RGBTRIPLE* color = malloc(sizeof(SPGM_RGBTRIPLE));
            find_color(color, vision);
            fill_colors(3 - i, color);
            mask = mask << size;
            shift_amt += size;
            free(color);
        }
        display_ledgrid(leds, FRAMEBUFFER);
    } else {
        clear_leds(FRAMEBUFFER);
    }
}

void system_led_free(LedSystem* system) {
    // TODO
}
