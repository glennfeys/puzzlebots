#ifndef SYSPROG_BOTBOARD_H
#define SYSPROG_BOTBOARD_H

#include "BotBerryConstants.h"
#include "Bus.h"
#include "CPU.h"

#include "DecisionOutput.h"
#include "Memory.h"
#include "VisionInput.h"

#include "Assembler.h"
#include "BitHelper.h"
#include "CPUInstruction.h"

#include <fstream>

class Board {
    // Board bevat alle onderdelen van de botberri: CPU, Bus, ...
  private:
    uint16_t max_size;
    uint16_t memsize;
    Assembler assembler;

    std::unique_ptr<CPU> cpu;
    std::unique_ptr<Bus> bus;

    std::shared_ptr<Memory> instr_mem;
    std::shared_ptr<Memory> stack_mem;
    std::shared_ptr<Memory> fsm_mem;
    std::shared_ptr<DecisionOutput> dec_op;
    std::shared_ptr<VisionInput> vis_ip;
    std::vector<CPUInstruction::Instruction> instructions;

  public:
    Board(uint16_t);
    uint16_t get_mem_size();
    void initialise_botberri();
    void load_and_start_code(std::string& filename);
    void load_from_file(std::string& filename);
    void cpu_step();
    void add_instruction(CPUInstruction::Instruction ins);
    void set_vision_bitmask(uint32_t mask);
    uint8_t get_next_move();
    bool isDecisionAvailable();
    void loadFSM(uint8_t* fsm_tabel);

    // TODO: implementeer een methode om de botberri te initialiseren. Dit houdt o.a. in dat alle
    // nodige modules aan de bus toegevoegd worden.
    // TODO: implementeer een methode om de botberri te initialiseren om meegegeven assembler code
    // uit te voeren. Dit houdt in dat de code geassambleerd moet worden, op de juiste plaats in het
    // juiste Memory object moet terecht komen, en dat de CPU ingesteld moet worden zodanig dat het
    // de code kan uitvoeren.
};

#endif // SYSPROG_BOTBOARD_H
