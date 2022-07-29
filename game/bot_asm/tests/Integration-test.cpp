#define BOOST_TEST_DYN_LINK
#include "../Board.h"
#include <boost/test/unit_test.hpp>

using namespace CPUInstruction;

BOOST_AUTO_TEST_SUITE(IntegrationTest)

BOOST_AUTO_TEST_CASE(test1) {
    Board brd(10000);
    CPUInstruction::Instruction ip(NOP);
    brd.add_instruction(ip);
    brd.initialise_botberri();
    brd.cpu_step();
}

BOOST_AUTO_TEST_CASE(test1_assem) {
    Board brd(10000);
    CPUInstruction::Instruction ip(NOP);
    std::string path("./game/bot_asm/programs/nop.botasm");
    brd.load_from_file(path);
    brd.initialise_botberri();
    brd.cpu_step();
}

BOOST_AUTO_TEST_CASE(test2_assem) {
    Board brd(10000);
    std::string path("./game/bot_asm/programs/test.botasm");
    brd.load_from_file(path);
    brd.initialise_botberri();
    for (size_t i = 0; i < 100; i++) {
        brd.cpu_step();
    }
}

BOOST_AUTO_TEST_CASE(testForward) {
    Board brd(10000);
    std::string path("./game/bot_asm/programs/forward.botasm");
    brd.load_from_file(path);
    brd.initialise_botberri();
    for (size_t i = 0; i < 100; i++) {
        brd.cpu_step();
    }
}

BOOST_AUTO_TEST_CASE(testFsm) {
    Board brd(10000);
    std::string path("./game/bot_asm/programs/fsm.botasm");
    brd.load_from_file(path);
    brd.initialise_botberri();
    for (size_t i = 0; i < 100; i++) {
        brd.cpu_step();
    }
}

BOOST_AUTO_TEST_CASE(testSquare) {
    Board brd(10000);
    std::string path("./game/bot_asm/programs/square.botasm");
    brd.load_from_file(path);
    brd.initialise_botberri();
    for (size_t i = 0; i < 20; i++) {
        brd.cpu_step();
    }
}

BOOST_AUTO_TEST_CASE(testWallBump) {
    Board brd(10000);
    std::string path("./game/bot_asm/programs/wallbump.botasm");
    brd.load_from_file(path);
    brd.initialise_botberri();
    for (size_t i = 0; i < 100; i++) {
        brd.cpu_step();
    }
}

// BOOST_AUTO_TEST_CASE(test2) {
//     Board brd(10000);

//     Instruction ip1(MOV);
//     Argument arg1(0, IO_DECISION_START);
//     Argument arg2(1, 3);
//     ip1.addArgument(arg1);
//     ip1.addArgument(arg2);
//     brd.add_instruction(ip1);
//     Instruction ip2(MOVB);
//     Argument arg11(0, DECISION_FORWARD);
//     Argument arg12(3, 3);
//     arg2.setDisplacement(1);
//     ip2.addArgument(arg11);
//     ip2.addArgument(arg12);
//     brd.add_instruction(ip2);
//     Instruction ip3(MOVB);
//     Argument arg21(0, 1);
//     Argument arg22(1, 3);
//     ip3.addArgument(arg21);
//     ip3.addArgument(arg22);
//     brd.add_instruction(ip3);
//     Instruction ip4(JMP);
//     Argument arg31(0, MEM_START + 4);
//     ip3.addArgument(arg31);
//     brd.add_instruction(ip4);

//     brd.initialise_botberri();
//     brd.start_code();
// }

BOOST_AUTO_TEST_SUITE_END()
