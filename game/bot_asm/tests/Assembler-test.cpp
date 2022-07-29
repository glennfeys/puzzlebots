#define BOOST_TEST_DYN_LINK
#include "../Assembler.h"
#include "../CPUInstruction.h"
#include <boost/test/unit_test.hpp>

// using CPUInstruction::Instruction;
using namespace CPUInstruction;

BOOST_AUTO_TEST_SUITE(AssemblerTest)

BOOST_AUTO_TEST_CASE(dummytest) { BOOST_CHECK_EQUAL(1, 1); }

BOOST_AUTO_TEST_CASE(split_test_1) {
    std::string line("   My test  string    ");
    std::string delim(" ");
    std::vector<std::string> rslt = split(line, delim);
    BOOST_CHECK_EQUAL(rslt.size(), 3);
    BOOST_CHECK_EQUAL(rslt[0], "My");
    BOOST_CHECK_EQUAL(rslt[1], "test");
    BOOST_CHECK_EQUAL(rslt[2], "string");
}
BOOST_AUTO_TEST_CASE(split_test_2) {
    std::string line("MOV %a, %e");
    std::string delim(" ");
    std::vector<std::string> rslt = split(line, delim);
    BOOST_CHECK_EQUAL(rslt.size(), 3);
    BOOST_CHECK_EQUAL(rslt[0], "MOV");
    BOOST_CHECK_EQUAL(rslt[1], "%a,");
    BOOST_CHECK_EQUAL(rslt[2], "%e");
}
BOOST_AUTO_TEST_CASE(split_test_double_delim) {
    std::string line("||MOV||%a,||%e");
    std::string delim("||");
    std::vector<std::string> rslt = split(line, delim);
    BOOST_CHECK_EQUAL(rslt.size(), 3);
    BOOST_CHECK_EQUAL(rslt[0], "MOV");
    BOOST_CHECK_EQUAL(rslt[1], "%a,");
    BOOST_CHECK_EQUAL(rslt[2], "%e");
}

BOOST_AUTO_TEST_CASE(split_test_no_delim) {
    std::string line("||MOV||%a,||%e");
    std::string delim("--");
    std::vector<std::string> rslt = split(line, delim);
    BOOST_CHECK_EQUAL(rslt.size(), 1);
    BOOST_CHECK_EQUAL(rslt[0], "||MOV||%a,||%e");
}
BOOST_AUTO_TEST_CASE(tokenize_test_1) {
    std::string line("MOV %a, %e");
    Assembler assem;
    std::vector rslt = assem.tokenize(line);
    BOOST_CHECK_EQUAL(rslt.size(), 3);
    BOOST_CHECK(rslt[0] == AssemblerToken(OPERATION, "MOV"));
    BOOST_CHECK(rslt[1] == AssemblerToken(OPERAND, "%a"));
    BOOST_CHECK(rslt[2] == AssemblerToken(OPERAND, "%e"));
}
BOOST_AUTO_TEST_CASE(build_bit_set_test) {
    std::bitset<4> test(std::string("0011"));
    std::bitset<2> bs1(std::string("00"));
    std::bitset<2> bs2(std::string("11"));
    std::bitset<4> rslt = build_bit_set<2, 2>(bs1, bs2);

    BOOST_CHECK(test == rslt);
}
BOOST_AUTO_TEST_CASE(build_bit_set_test_2) {
    std::bitset<12> test(std::string("110011101101"));
    std::bitset<6> bs1(std::string("110011"));
    std::bitset<6> bs2(std::string("101101"));
    std::bitset<12> rslt = build_bit_set<6, 6>(bs1, bs2);

    BOOST_CHECK(test == rslt);
}
BOOST_AUTO_TEST_CASE(toinstruction_test_NOP) {
    std::string line("NOP");
    Assembler assem;
    Instruction test(CPUInstruction::NOP);
    Instruction rslt = assem.to_instructions(line)[0];

    BOOST_CHECK(rslt == test);
}

BOOST_AUTO_TEST_CASE(toinstruction_test_MOV_reg) {
    std::string line("MOV %a, %e");
    Assembler assem;
    Instruction test(MOV);
    test.addArgument(Argument(REGISTER, Assembler::regBits.at("%a")));
    test.addArgument(Argument(REGISTER, Assembler::regBits.at("%e")));

    Instruction rslt = assem.to_instructions(line)[0];

    BOOST_CHECK(rslt == test);
}

BOOST_AUTO_TEST_CASE(toinstruction_test_MOV_dirval) {
    std::string line("MOV $5, %e");
    Assembler assem;
    Instruction test(MOV);
    test.addArgument(Argument(IMMEDIATE, 5));
    test.addArgument(Argument(REGISTER, Assembler::regBits.at("%e")));

    Instruction rslt = assem.to_instructions(line)[0];

    BOOST_CHECK(rslt == test);
}

BOOST_AUTO_TEST_CASE(toinstruction_test_MOV_diraddr) {
    std::string line("MOV ($5), %e");
    Assembler assem;
    Instruction test(MOV);
    uint16_t testaddr = 5;
    test.addArgument(Argument(MEM_IMMEDIATE, testaddr));
    test.addArgument(Argument(REGISTER, Assembler::regBits.at("%e")));

    Instruction rslt = assem.to_instructions(line)[0];

    BOOST_CHECK(rslt == test);
}
BOOST_AUTO_TEST_CASE(toinstruction_test_MOV_regaddr) {
    std::string line("MOV (%a), %e");
    Assembler assem;
    Instruction test(MOV);
    Argument arg1(MEM_REGISTER, Assembler::regBits.at("%a"));
    arg1.setDisplacement(0);

    test.addArgument(arg1);
    test.addArgument(Argument(REGISTER, Assembler::regBits.at("%e")));

    Instruction rslt = assem.to_instructions(line)[0];

    BOOST_CHECK(rslt == test);
}
BOOST_AUTO_TEST_CASE(toinstruction_test_MOV_regaddr_offset) {
    std::string line("MOV 4(%a), %e");
    Assembler assem;
    Instruction test(MOV);
    Argument arg1(MEM_REGISTER, Assembler::regBits.at("%a"));
    arg1.setDisplacement(4);
    test.addArgument(arg1);
    test.addArgument(Argument(REGISTER, Assembler::regBits.at("%e")));

    Instruction rslt = assem.to_instructions(line)[0];

    BOOST_CHECK(rslt == test);
}

BOOST_AUTO_TEST_CASE(label_test) {
    std::string path("./game/bot_asm/programs/label.botasm");
    Assembler assem;
    std::vector<Instruction> rslt = assem.assemble_file(path, MEM_START);
    BOOST_CHECK_EQUAL(rslt.size(), 3);

    // todo
}

BOOST_AUTO_TEST_CASE(shouldnt_crash) {
    std::string path("./game/bot_asm/programs/fsm.botasm");
    Assembler assem;
    std::vector<Instruction> rslt = assem.assemble_file(path, MEM_START);
}
BOOST_AUTO_TEST_CASE(equ_test) {
    std::string path("./game/bot_asm/programs/equtest.botasm");
    Assembler assem;
    std::vector<Instruction> rslt = assem.assemble_file(path, MEM_START);
    Instruction ins(MOV);
    ins.addArgument(Argument(IMMEDIATE, 0x10));
    ins.addArgument(Argument(REGISTER, Assembler::regBits.at("%a")));
    BOOST_CHECK_EQUAL(rslt.size(), 1);
    BOOST_CHECK(rslt[0] == ins);
}
BOOST_AUTO_TEST_CASE(forward_shouldnt_crash) {
    std::string path("./game/bot_asm/programs/forward.botasm");
    Assembler assem;
    std::vector<Instruction> rslt = assem.assemble_file(path, MEM_START);
}

BOOST_AUTO_TEST_SUITE_END()
