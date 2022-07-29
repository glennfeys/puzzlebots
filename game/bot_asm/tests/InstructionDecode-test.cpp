#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "../CPUInstruction.h"

using namespace CPUInstruction;

BOOST_AUTO_TEST_SUITE(InstructionDecodeTest)

    BOOST_AUTO_TEST_CASE( testNOP ) {
        uint8_t model[4] = {0, 0, 20, 176};
        Instruction inst(NOP);
        Argument arg1(0,10);
        Argument arg2(1,3);
        inst.addArgument(arg1);
        inst.addArgument(arg2);
        int i = 0;
        ByteSource source = [&i, &model](){ return model[i++]; };
        uint8_t bytesRead = 4;
        Instruction newInst = inst.decode(source, bytesRead);
        BOOST_CHECK_EQUAL(inst.getOperation(), newInst.getOperation());
        for (int i=0; i<operationArguments[inst.getOperation()]; i++){
            BOOST_CHECK_EQUAL(inst.getArgument(i).getType(), newInst.getArgument(i).getType());
            BOOST_CHECK_EQUAL(inst.getArgument(i).getValue(), newInst.getArgument(i).getValue());
        }
    }

    BOOST_AUTO_TEST_CASE( testMOV ) {
        uint8_t model[4] = {8, 0, 20, 176};
        Instruction inst(MOV);
        Argument arg1(0,10);
        Argument arg2(1,3);
        inst.addArgument(arg1);
        inst.addArgument(arg2);
        int i = 0;
        ByteSource source = [&i, &model](){ return model[i++]; };
        uint8_t bytesRead = 4;
        Instruction newInst = inst.decode(source, bytesRead);
        BOOST_CHECK_EQUAL(inst.getOperation(), newInst.getOperation());
        for (int i=0; i<operationArguments[inst.getOperation()]; i++){
            BOOST_CHECK_EQUAL(inst.getArgument(i).getType(), newInst.getArgument(i).getType());
            BOOST_CHECK_EQUAL(inst.getArgument(i).getValue(), newInst.getArgument(i).getValue());
        }
    }

    BOOST_AUTO_TEST_CASE( testMOVB ) {
        uint8_t model[4] = {16, 0, 20, 176};
        Instruction inst(MOVB);
        Argument arg1(0,10);
        Argument arg2(1,3);
        inst.addArgument(arg1);
        inst.addArgument(arg2);
        int i = 0;
        ByteSource source = [&i, &model](){ return model[i++]; };
        uint8_t bytesRead = 4;
        Instruction newInst = inst.decode(source, bytesRead);
        BOOST_CHECK_EQUAL(inst.getOperation(), newInst.getOperation());
        for (int i=0; i<operationArguments[inst.getOperation()]; i++){
            BOOST_CHECK_EQUAL(inst.getArgument(i).getType(), newInst.getArgument(i).getType());
            BOOST_CHECK_EQUAL(inst.getArgument(i).getValue(), newInst.getArgument(i).getValue());
        }
    }

    BOOST_AUTO_TEST_CASE( testPUSH ) {
        uint8_t model[4] = {24, 0, 20, 176};
        Instruction inst(PUSH);
        Argument arg1(0,10);
        Argument arg2(1,3);
        inst.addArgument(arg1);
        inst.addArgument(arg2);
        int i = 0;
        ByteSource source = [&i, &model](){ return model[i++]; };
        uint8_t bytesRead = 4;
        Instruction newInst = inst.decode(source, bytesRead);
        BOOST_CHECK_EQUAL(inst.getOperation(), newInst.getOperation());
        for (int i=0; i<operationArguments[inst.getOperation()]; i++){
            BOOST_CHECK_EQUAL(inst.getArgument(i).getType(), newInst.getArgument(i).getType());
            BOOST_CHECK_EQUAL(inst.getArgument(i).getValue(), newInst.getArgument(i).getValue());
        }
    }

    BOOST_AUTO_TEST_CASE( testPOP ) {
        uint8_t model[4] = {32, 0, 20, 176};
        Instruction inst(POP);
        Argument arg1(0,10);
        Argument arg2(1,3);
        inst.addArgument(arg1);
        inst.addArgument(arg2);
        int i = 0;
        ByteSource source = [&i, &model](){ return model[i++]; };
        uint8_t bytesRead = 4;
        Instruction newInst = inst.decode(source, bytesRead);
        BOOST_CHECK_EQUAL(inst.getOperation(), newInst.getOperation());
        for (int i=0; i<operationArguments[inst.getOperation()]; i++){
            BOOST_CHECK_EQUAL(inst.getArgument(i).getType(), newInst.getArgument(i).getType());
            BOOST_CHECK_EQUAL(inst.getArgument(i).getValue(), newInst.getArgument(i).getValue());
        }
    }

    BOOST_AUTO_TEST_CASE( testADD ) {
        uint8_t model[4] = {40, 0, 20, 176};
        Instruction inst(ADD);
        Argument arg1(0,10);
        Argument arg2(1,3);
        inst.addArgument(arg1);
        inst.addArgument(arg2);
        int i = 0;
        ByteSource source = [&i, &model](){ return model[i++]; };
        uint8_t bytesRead = 4;
        Instruction newInst = inst.decode(source, bytesRead);
        BOOST_CHECK_EQUAL(inst.getOperation(), newInst.getOperation());
        for (int i=0; i<operationArguments[inst.getOperation()]; i++){
            BOOST_CHECK_EQUAL(inst.getArgument(i).getType(), newInst.getArgument(i).getType());
            BOOST_CHECK_EQUAL(inst.getArgument(i).getValue(), newInst.getArgument(i).getValue());
        }
    }

    BOOST_AUTO_TEST_CASE( testSUB ) {
        uint8_t model[4] = {48, 0, 20, 176};
        Instruction inst(SUB);
        Argument arg1(0,10);
        Argument arg2(1,3);
        inst.addArgument(arg1);
        inst.addArgument(arg2);
        int i = 0;
        ByteSource source = [&i, &model](){ return model[i++]; };
        uint8_t bytesRead = 4;
        Instruction newInst = inst.decode(source, bytesRead);
        BOOST_CHECK_EQUAL(inst.getOperation(), newInst.getOperation());
        for (int i=0; i<operationArguments[inst.getOperation()]; i++){
            BOOST_CHECK_EQUAL(inst.getArgument(i).getType(), newInst.getArgument(i).getType());
            BOOST_CHECK_EQUAL(inst.getArgument(i).getValue(), newInst.getArgument(i).getValue());
        }
    }

    BOOST_AUTO_TEST_CASE( testAND ) {
        uint8_t model[4] = {56, 0, 20, 176};
        Instruction inst(AND);
        Argument arg1(0,10);
        Argument arg2(1,3);
        inst.addArgument(arg1);
        inst.addArgument(arg2);
        int i = 0;
        ByteSource source = [&i, &model](){ return model[i++]; };
        uint8_t bytesRead = 4;
        Instruction newInst = inst.decode(source, bytesRead);
        BOOST_CHECK_EQUAL(inst.getOperation(), newInst.getOperation());
        for (int i=0; i<operationArguments[inst.getOperation()]; i++){
            BOOST_CHECK_EQUAL(inst.getArgument(i).getType(), newInst.getArgument(i).getType());
            BOOST_CHECK_EQUAL(inst.getArgument(i).getValue(), newInst.getArgument(i).getValue());
        }
    }

    BOOST_AUTO_TEST_CASE( testOR ) {
        uint8_t model[4] = {64, 0, 20, 176};
        Instruction inst(OR);
        Argument arg1(0,10);
        Argument arg2(1,3);
        inst.addArgument(arg1);
        inst.addArgument(arg2);
        int i = 0;
        ByteSource source = [&i, &model](){ return model[i++]; };
        uint8_t bytesRead = 4;
        Instruction newInst = inst.decode(source, bytesRead);
        BOOST_CHECK_EQUAL(inst.getOperation(), newInst.getOperation());
        for (int i=0; i<operationArguments[inst.getOperation()]; i++){
            BOOST_CHECK_EQUAL(inst.getArgument(i).getType(), newInst.getArgument(i).getType());
            BOOST_CHECK_EQUAL(inst.getArgument(i).getValue(), newInst.getArgument(i).getValue());
        }
    }

    BOOST_AUTO_TEST_CASE( testXOR ) {
        uint8_t model[4] = {72, 0, 20, 176};
        Instruction inst(XOR);
        Argument arg1(0,10);
        Argument arg2(1,3);
        inst.addArgument(arg1);
        inst.addArgument(arg2);
        int i = 0;
        ByteSource source = [&i, &model](){ return model[i++]; };
        uint8_t bytesRead = 4;
        Instruction newInst = inst.decode(source, bytesRead);
        BOOST_CHECK_EQUAL(inst.getOperation(), newInst.getOperation());
        for (int i=0; i<operationArguments[inst.getOperation()]; i++){
            BOOST_CHECK_EQUAL(inst.getArgument(i).getType(), newInst.getArgument(i).getType());
            BOOST_CHECK_EQUAL(inst.getArgument(i).getValue(), newInst.getArgument(i).getValue());
        }
    }

    BOOST_AUTO_TEST_CASE( testNOT ) {
        uint8_t model[4] = {80, 0, 20, 176};
        Instruction inst(NOT);
        Argument arg1(0,10);
        Argument arg2(1,3);
        inst.addArgument(arg1);
        inst.addArgument(arg2);
        int i = 0;
        ByteSource source = [&i, &model](){ return model[i++]; };
        uint8_t bytesRead = 4;
        Instruction newInst = inst.decode(source, bytesRead);
        BOOST_CHECK_EQUAL(inst.getOperation(), newInst.getOperation());
        for (int i=0; i<operationArguments[inst.getOperation()]; i++){
            BOOST_CHECK_EQUAL(inst.getArgument(i).getType(), newInst.getArgument(i).getType());
            BOOST_CHECK_EQUAL(inst.getArgument(i).getValue(), newInst.getArgument(i).getValue());
        }
    }

    BOOST_AUTO_TEST_CASE( testSHR ) {
        uint8_t model[4] = {88, 0, 20, 176};
        Instruction inst(SHR);
        Argument arg1(0,10);
        Argument arg2(1,3);
        inst.addArgument(arg1);
        inst.addArgument(arg2);
        int i = 0;
        ByteSource source = [&i, &model](){ return model[i++]; };
        uint8_t bytesRead = 4;
        Instruction newInst = inst.decode(source, bytesRead);
        BOOST_CHECK_EQUAL(inst.getOperation(), newInst.getOperation());
        for (int i=0; i<operationArguments[inst.getOperation()]; i++){
            BOOST_CHECK_EQUAL(inst.getArgument(i).getType(), newInst.getArgument(i).getType());
            BOOST_CHECK_EQUAL(inst.getArgument(i).getValue(), newInst.getArgument(i).getValue());
        }
    }

    BOOST_AUTO_TEST_CASE( testSHL ) {
        uint8_t model[4] = {96, 0, 20, 176};
        Instruction inst(SHL);
        Argument arg1(0,10);
        Argument arg2(1,3);
        inst.addArgument(arg1);
        inst.addArgument(arg2);
        int i = 0;
        ByteSource source = [&i, &model](){ return model[i++]; };
        uint8_t bytesRead = 4;
        Instruction newInst = inst.decode(source, bytesRead);
        BOOST_CHECK_EQUAL(inst.getOperation(), newInst.getOperation());
        for (int i=0; i<operationArguments[inst.getOperation()]; i++){
            BOOST_CHECK_EQUAL(inst.getArgument(i).getType(), newInst.getArgument(i).getType());
            BOOST_CHECK_EQUAL(inst.getArgument(i).getValue(), newInst.getArgument(i).getValue());
        }
    }

    BOOST_AUTO_TEST_CASE( testINC ) {
        uint8_t model[4] = {104, 0, 20, 176};
        Instruction inst(INC);
        Argument arg1(0,10);
        Argument arg2(1,3);
        inst.addArgument(arg1);
        inst.addArgument(arg2);
        int i = 0;
        ByteSource source = [&i, &model](){ return model[i++]; };
        uint8_t bytesRead = 4;
        Instruction newInst = inst.decode(source, bytesRead);
        BOOST_CHECK_EQUAL(inst.getOperation(), newInst.getOperation());
        for (int i=0; i<operationArguments[inst.getOperation()]; i++){
            BOOST_CHECK_EQUAL(inst.getArgument(i).getType(), newInst.getArgument(i).getType());
            BOOST_CHECK_EQUAL(inst.getArgument(i).getValue(), newInst.getArgument(i).getValue());
        }
    }

    BOOST_AUTO_TEST_CASE( testDEC ) {
        uint8_t model[4] = {112, 0, 20, 176};
        Instruction inst(DEC);
        Argument arg1(0,10);
        Argument arg2(1,3);
        inst.addArgument(arg1);
        inst.addArgument(arg2);
        int i = 0;
        ByteSource source = [&i, &model](){ return model[i++]; };
        uint8_t bytesRead = 4;
        Instruction newInst = inst.decode(source, bytesRead);
        BOOST_CHECK_EQUAL(inst.getOperation(), newInst.getOperation());
        for (int i=0; i<operationArguments[inst.getOperation()]; i++){
            BOOST_CHECK_EQUAL(inst.getArgument(i).getType(), newInst.getArgument(i).getType());
            BOOST_CHECK_EQUAL(inst.getArgument(i).getValue(), newInst.getArgument(i).getValue());
        }
    }

    BOOST_AUTO_TEST_CASE( testCMP ) {
        uint8_t model[4] = {120, 0, 20, 176};
        Instruction inst(CMP);
        Argument arg1(0,10);
        Argument arg2(1,3);
        inst.addArgument(arg1);
        inst.addArgument(arg2);
        int i = 0;
        ByteSource source = [&i, &model](){ return model[i++]; };
        uint8_t bytesRead = 4;
        Instruction newInst = inst.decode(source, bytesRead);
        BOOST_CHECK_EQUAL(inst.getOperation(), newInst.getOperation());
        for (int i=0; i<operationArguments[inst.getOperation()]; i++){
            BOOST_CHECK_EQUAL(inst.getArgument(i).getType(), newInst.getArgument(i).getType());
            BOOST_CHECK_EQUAL(inst.getArgument(i).getValue(), newInst.getArgument(i).getValue());
        }
    }

    BOOST_AUTO_TEST_CASE( testTEST ) {
        uint8_t model[4] = {128, 0, 20, 176};
        Instruction inst(TEST);
        Argument arg1(0,10);
        Argument arg2(1,3);
        inst.addArgument(arg1);
        inst.addArgument(arg2);
        int i = 0;
        ByteSource source = [&i, &model](){ return model[i++]; };
        uint8_t bytesRead = 4;
        Instruction newInst = inst.decode(source, bytesRead);
        BOOST_CHECK_EQUAL(inst.getOperation(), newInst.getOperation());
        for (int i=0; i<operationArguments[inst.getOperation()]; i++){
            BOOST_CHECK_EQUAL(inst.getArgument(i).getType(), newInst.getArgument(i).getType());
            BOOST_CHECK_EQUAL(inst.getArgument(i).getValue(), newInst.getArgument(i).getValue());
        }
    }

    BOOST_AUTO_TEST_CASE( testJMP ) {
        uint8_t model[4] = {136, 0, 20, 176};
        Instruction inst(JMP);
        Argument arg1(0,10);
        Argument arg2(1,3);
        inst.addArgument(arg1);
        inst.addArgument(arg2);
        int i = 0;
        ByteSource source = [&i, &model](){ return model[i++]; };
        uint8_t bytesRead = 4;
        Instruction newInst = inst.decode(source, bytesRead);
        BOOST_CHECK_EQUAL(inst.getOperation(), newInst.getOperation());
        for (int i=0; i<operationArguments[inst.getOperation()]; i++){
            BOOST_CHECK_EQUAL(inst.getArgument(i).getType(), newInst.getArgument(i).getType());
            BOOST_CHECK_EQUAL(inst.getArgument(i).getValue(), newInst.getArgument(i).getValue());
        }
    }

    BOOST_AUTO_TEST_CASE( testJE ) {
        uint8_t model[4] = {144, 0, 20, 176};
        Instruction inst(JE);
        Argument arg1(0,10);
        Argument arg2(1,3);
        inst.addArgument(arg1);
        inst.addArgument(arg2);
        int i = 0;
        ByteSource source = [&i, &model](){ return model[i++]; };
        uint8_t bytesRead = 4;
        Instruction newInst = inst.decode(source, bytesRead);
        BOOST_CHECK_EQUAL(inst.getOperation(), newInst.getOperation());
        for (int i=0; i<operationArguments[inst.getOperation()]; i++){
            BOOST_CHECK_EQUAL(inst.getArgument(i).getType(), newInst.getArgument(i).getType());
            BOOST_CHECK_EQUAL(inst.getArgument(i).getValue(), newInst.getArgument(i).getValue());
        }
    }

    BOOST_AUTO_TEST_CASE( testJNE ) {
        uint8_t model[4] = {152, 0, 20, 176};
        Instruction inst(JNE);
        Argument arg1(0,10);
        Argument arg2(1,3);
        inst.addArgument(arg1);
        inst.addArgument(arg2);
        int i = 0;
        ByteSource source = [&i, &model](){ return model[i++]; };
        uint8_t bytesRead = 4;
        Instruction newInst = inst.decode(source, bytesRead);
        BOOST_CHECK_EQUAL(inst.getOperation(), newInst.getOperation());
        for (int i=0; i<operationArguments[inst.getOperation()]; i++){
            BOOST_CHECK_EQUAL(inst.getArgument(i).getType(), newInst.getArgument(i).getType());
            BOOST_CHECK_EQUAL(inst.getArgument(i).getValue(), newInst.getArgument(i).getValue());
        }
    }

    BOOST_AUTO_TEST_CASE( testCALL ) {
        uint8_t model[4] = {160, 0, 20, 176};
        Instruction inst(CALL);
        Argument arg1(0,10);
        Argument arg2(1,3);
        inst.addArgument(arg1);
        inst.addArgument(arg2);
        int i = 0;
        ByteSource source = [&i, &model](){ return model[i++]; };
        uint8_t bytesRead = 4;
        Instruction newInst = inst.decode(source, bytesRead);
        BOOST_CHECK_EQUAL(inst.getOperation(), newInst.getOperation());
        for (int i=0; i<operationArguments[inst.getOperation()]; i++){
            BOOST_CHECK_EQUAL(inst.getArgument(i).getType(), newInst.getArgument(i).getType());
            BOOST_CHECK_EQUAL(inst.getArgument(i).getValue(), newInst.getArgument(i).getValue());
        }
    }

    BOOST_AUTO_TEST_CASE( testRET ) {
        uint8_t model[4] = {168, 0, 20, 176};
        Instruction inst(RET);
        Argument arg1(0,10);
        Argument arg2(1,3);
        inst.addArgument(arg1);
        inst.addArgument(arg2);
        int i = 0;
        ByteSource source = [&i, &model](){ return model[i++]; };
        uint8_t bytesRead = 4;
        Instruction newInst = inst.decode(source, bytesRead);
        BOOST_CHECK_EQUAL(inst.getOperation(), newInst.getOperation());
        for (int i=0; i<operationArguments[inst.getOperation()]; i++){
            BOOST_CHECK_EQUAL(inst.getArgument(i).getType(), newInst.getArgument(i).getType());
            BOOST_CHECK_EQUAL(inst.getArgument(i).getValue(), newInst.getArgument(i).getValue());
        }
    }

    BOOST_AUTO_TEST_CASE( testType2And3 ) {
        uint8_t model[5] = {12, 8, 63, 193, 0};
        Instruction inst(MOV);
        Argument arg1(2,0x041F);
        Argument arg2(3,4);
        inst.addArgument(arg1);
        inst.addArgument(arg2);
        int i = 0;
        ByteSource source = [&i, &model](){ return model[i++]; };
        uint8_t bytesRead = 4;
        Instruction newInst = inst.decode(source, bytesRead);
        BOOST_CHECK_EQUAL(inst.getOperation(), newInst.getOperation());
        for (int i=0; i<operationArguments[inst.getOperation()]; i++){
            BOOST_CHECK_EQUAL(inst.getArgument(i).getType(), newInst.getArgument(i).getType());
            BOOST_CHECK_EQUAL(inst.getArgument(i).getValue(), newInst.getArgument(i).getValue());
        }
    }

BOOST_AUTO_TEST_SUITE_END()
