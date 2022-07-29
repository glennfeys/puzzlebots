#ifndef SYSPROG_BOTBERRYCONSTANTS_H
#define SYSPROG_BOTBERRYCONSTANTS_H

#include <cstdint>

//eventueel kunnen deze types gebruikt worden bij implementatie CPU, Bus, en BotModules.
typedef uint16_t bot_register_t;
typedef uint16_t bot_addr_t;


//voorbeeld fixed memory layout (gebruikt in .botbin voorbeelden):

//eerste 16 bytes niet gebruikt
#define USABLE_ADDRESS_START 0x0010u

//VisionInput van 0x0010 tot 0x0014
#define IO_VISION_START USABLE_ADDRESS_START
#define IO_VISION_SIZE 0x004u        //4 byte

//DecisionOutput van 0x0014 tot 0x001F
#define IO_DECISION_START (IO_VISION_START + IO_VISION_SIZE)
#define IO_DECISION_SIZE  0x000Bu        //1 + max 10 decisions

//FSM memory van 0x001F tot 0x041F
#define MEM_FSM_START  (IO_DECISION_START + IO_DECISION_SIZE)
#define MEM_FSM_SIZE  0x0400 //1024 byte = 8 * 8 * 16

//Code en stack memory van 0x041F tot ...
#define MEM_START (MEM_FSM_START + MEM_FSM_SIZE)

#endif //SYSPROG_BOTBERRYCONSTANTS_H
