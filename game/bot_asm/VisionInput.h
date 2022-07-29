#ifndef SYSPROG_BOTVISION_H
#define SYSPROG_BOTVISION_H

#include "./../bot_automaton/bot_automaton.h"
#include "BotModule.h"

class VisionInput : public BotModule {
    // TODO Implementeer de virtuele methoden van BotModule, om 1 of 2 bytes te lezen uit de 32-bit
    // vision bitmask
    // TODO Implementeer de virtuele methoden van BotModule ivm schrijven door een exceptie te
    // gooien
  private:
    uint8_t* vision_mask;

  public:
    VisionInput(bot_addr_t, int);
    ~VisionInput();
    void setVisionBitMask(uint32_t);
    uint8_t readOneByte(bot_addr_t);
    uint16_t readTwoBytes(bot_addr_t);
};

#endif // SYSPROG_BOTVISION_H
