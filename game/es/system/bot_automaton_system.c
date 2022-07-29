#include "bot_automaton_system.h"
#include "../../bot_automaton/BotAutomaton_c_wrapper.h"
#include "./../../fsm/fsm.h"

BotAutomatonSystem* system_botautomaton_alloc(Engine* engine) {
    BotAutomatonSystem* res = calloc(1, sizeof(BotAutomatonSystem));
    res->fsm = fsm_alloc();
    res->state_table = calloc(1024, sizeof(uint8_t));
    system_botautomaton_init(res, engine);
    return res;
}

void system_botautomaton_init(BotAutomatonSystem* system, Engine* engine) {
    system->system_id = BOT_AUTOMATON_SYSTEM;
    init_fsm(system->fsm, engine->context.currentLevel);
    for (int i = 0; i < 1024; i++) {
        system->state_table[i] = 0;
    }
    set_binairy(system->fsm, system->state_table);
}

void system_botautomaton_free(BotAutomatonSystem* system) { 
    free(system->fsm); 
    free(system->state_table);
}

void move_bot(Engine* engine, int decision, EntityId bot) {
    switch (decision) {
    case 0: {
        MoveActionComponent* move_comp = create_component(engine, bot, COMP_MOVE_ACTION);
        move_comp->is_forward = 1;
        break;
    }
    case 1: {
        DirectionComponent* dir_comp = get_component(engine, bot, COMP_DIRECTION);
        dir_comp->dir = (dir_comp->dir + 1) % DIRECTION_COUNT;
        break;
    }
    case 2: {
        DirectionComponent* dir_comp = get_component(engine, bot, COMP_DIRECTION);
        dir_comp->dir = (dir_comp->dir - 1) % DIRECTION_COUNT;
        break;
    }
    case 4: {
        ItemActionComponent* iac = get_component(engine, bot, COMP_ITEMACTION);
        iac->do_action = 1;
        break;
    }
    default:
        break;
    }
}

void system_botautomaton_update(BotAutomatonSystem* system, Engine* engine) {
    EntityIterator automaton_it;
    search_entity_1(engine, COMP_BOT_AUTOMATON_1, &automaton_it, system->system_id);
    while (next_entity(&automaton_it)) {
        EntityId bot_id = automaton_it.entity_id;
        BotAutomaton1Component* botauto1 = get_component(engine, bot_id, COMP_BOT_AUTOMATON_1);
        BotAutomaton2Component* botauto2 = get_component(engine, bot_id, COMP_BOT_AUTOMATON_2);
        if (botauto2->is_active && !has_component(engine, bot_id, COMP_MOVE_ANIMATION)) {
            BotVisionComponent* vision_comp = get_component(engine, bot_id, COMP_BOT_VISION);
            setVisionMask(botauto2->botberri, vision_comp->vision_mask);
            int steps = 0;
            while (steps < 20) {
                doCPUStep(botauto2->botberri);
                if (isDecisionAvailable(botauto2->botberri)) {
                    move_bot(engine, getNextMove(botauto2->botberri), bot_id);
                    steps = 20;
                }
                steps++;
            }
        }
        if (botauto1->is_active && !has_component(engine, bot_id, COMP_MOVE_ANIMATION)) {
            int moves[MAX_FSM_ACTION];
            BotVisionComponent* vision_comp = get_component(engine, bot_id, COMP_BOT_VISION);
            int amount = next_move(system->fsm, vision_comp->vision_mask, moves, botauto1->state);
            int i = 0;
            DelayedActionComponent* dac = get_component(engine, bot_id, COMP_DELAYED_ACTION);
            while (i < amount) {
                if (!has_component(engine, bot_id, COMP_MOVE_ANIMATION)) {
                    if (has_component(engine, bot_id, COMP_MOVE_ACTION)) {
                        dac->actions[dac->amount] = moves[i];
                        dac->amount++;
                    } else {
                        if (dac->amount > 0) {
                            for (int i = 0; i < dac->amount; i++) {
                                move_bot(engine, dac->actions[i], bot_id);
                            }
                            dac->amount = 0;
                        }
                        move_bot(engine, moves[i], bot_id);
                    }
                    i++;
                }
            }
        }
    }
}
