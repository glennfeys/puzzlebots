#include "../bot_asm/Board.h"

#include "BotAutomaton_c_wrapper.h"

BotBerri* createBotBerri(uint16_t size) {
    BotBerri* botberri = new BotBerri;
    Board* board = new Board(size);
    botberri->obj = board;

    return botberri;
}

void freeBotBerri(BotBerri* botberri) {
    if (botberri == NULL)
        return;
    delete static_cast<Board*>(botberri->obj);
    delete botberri;
}

void initBotBerri(BotBerri* botberri) {
    std::string path("./game/bot_asm/programs/fsm.botasm");
    Board* brd = static_cast<Board*>(botberri->obj);
    brd->load_from_file(path);
    brd->initialise_botberri();
}

void doCPUStep(BotBerri* botberri) { static_cast<Board*>(botberri->obj)->cpu_step(); }

void setVisionMask(BotBerri* botberri, uint32_t mask) {
    static_cast<Board*>(botberri->obj)->set_vision_bitmask(mask);
}

uint8_t getNextMove(BotBerri* botberri) {
    return static_cast<Board*>(botberri->obj)->get_next_move();
}

int isDecisionAvailable(BotBerri* botberri) {
    return static_cast<Board*>(botberri->obj)->isDecisionAvailable();
}

void loadFSM(BotBerri* botberri, uint8_t* fsm_tabel) {
    static_cast<Board*>(botberri->obj)->loadFSM(fsm_tabel);
}
