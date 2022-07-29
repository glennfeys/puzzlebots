#include "engine.h"

#include <assert.h>
#include <stdlib.h>

#include <SDL_timer.h>

Engine* engine_alloc(Graphics* graphics, LevelName lvl) {
    Engine* res = calloc(1, sizeof(Engine));
    engine_init(res, graphics, lvl);
    return res;
}
void engine_init(Engine* engine, Graphics* graphics, LevelName lvl) {
    context_init(&engine->context, lvl);
    assert(!engine->context.is_exit_game);

    es_memory_manager_init(&engine->es_memory);

    engine->render_system = system_render_alloc(graphics, engine);
    engine->visibility_system = system_visibility_alloc(engine);
    engine->botvision_system = system_botvision_alloc(engine);
    engine->botautomaton_system = system_botautomaton_alloc(engine);
    engine->input_system = system_input_alloc(engine);
    engine->button_system = system_button_alloc(engine);
    engine->action_system = system_action_alloc(engine);
    engine->activation_system = system_activation_alloc(engine);
    engine->animation_system = system_animation_alloc(engine);
    engine->camera_system = system_camera_alloc(engine);
    engine->container_system = system_container_alloc(engine);
    engine->end_system = system_endlevel_alloc(engine);
    engine->move_system = system_move_alloc(engine);
    engine->gravity_system = system_gravity_alloc(engine);
    engine->teleport_system = system_teleport_alloc(engine);
#ifdef RPI
    engine->led_system = system_led_alloc(engine);
    engine->eyeimu_system = system_eyeimu_alloc(engine);
#endif

    assert(engine->render_system != NULL);
    assert(engine->visibility_system != NULL);
    assert(engine->botvision_system != NULL);
    assert(engine->botautomaton_system != NULL);
    assert(engine->input_system != NULL);
    assert(engine->button_system != NULL);
    assert(engine->action_system != NULL);
    assert(engine->activation_system != NULL);
    assert(engine->animation_system != NULL);
    assert(engine->camera_system != NULL);
    assert(engine->container_system != NULL);
    assert(engine->end_system != NULL);
    assert(engine->move_system != NULL);
    assert(engine->gravity_system != NULL);
    assert(engine->teleport_system != NULL);
#ifdef RPI
    assert(engine->led_system != NULL);
    assert(engine->eyeimu_system != NULL);
#endif
}

void engine_free(Engine* e) {
    context_free(&e->context);

    // free all systems
    system_render_free(e->render_system);
    system_visibility_free(e->visibility_system);
    system_botvision_free(e->botvision_system);
    system_botautomaton_free(e->botautomaton_system);
    system_input_free(e->input_system);
    system_button_free(e->button_system);
    system_action_free(e->action_system);
    system_activation_free(e->activation_system);
    system_animation_free(e->animation_system);
    system_camera_free(e->camera_system);
    system_container_free(e->container_system);
    system_endlevel_free(e->end_system);
    system_move_free(e->move_system);
    system_gravity_free(e->gravity_system);
    system_teleport_free(e->teleport_system);
#ifdef RPI
    system_led_free(e->led_system);
    system_eyeimu_free(e->eyeimu_system);
    free(e->eyeimu_system);
    free(e->led_system);
#endif

    es_memory_manager_free(&e->es_memory);
    free(e->render_system);
    free(e->visibility_system);
    free(e->botvision_system);
    free(e->botautomaton_system);
    free(e->input_system);
    free(e->button_system);
    free(e->action_system);
    free(e->activation_system);
    free(e->animation_system);
    free(e->camera_system);
    free(e->container_system);
    free(e->end_system);
    free(e->move_system);
    free(e->gravity_system);
    free(e->teleport_system);

    // note: don't free graphics here
}

void engine_update(Engine* engine) {
    engine->context.time = SDL_GetTicks();

    system_input_update(engine->input_system, engine);
    system_button_update(engine->button_system, engine);
    system_activation_update(engine->activation_system, engine);
    system_gravity_update(engine->gravity_system, engine);
    system_move_update(engine->move_system, engine);
    system_botvision_update(engine->botvision_system, engine);
    system_botautomaton_update(engine->botautomaton_system, engine);
    system_teleport_update(engine->teleport_system, engine);
    system_action_update(engine->action_system, engine);
    system_container_update(engine->container_system, engine);
    system_camera_update(engine->camera_system, engine);
    system_animation_update(engine->animation_system, engine);
    system_endlevel_update(engine->end_system, engine);
#ifdef RPI
    system_led_update(engine->led_system, engine);
    system_eyeimu_update(engine->eyeimu_system, engine);
#endif
    system_visibility_update(engine->visibility_system, engine);
    system_render_update(engine->render_system, engine);
}
