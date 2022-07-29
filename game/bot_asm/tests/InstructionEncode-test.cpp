#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "../CPUInstruction.h"

using namespace CPUInstruction;

BOOST_AUTO_TEST_SUITE(InstructionEncodeTest)

    BOOST_AUTO_TEST_CASE( testNOP ) {
        Instruction ip(NOP);
        uint8_t arr[1] = {0};
        int i = 0;
        ByteSink sink = [&i, &arr](uint8_t byte){ arr[i++] = byte; };
        ip.encode(sink);
        uint8_t model[1] = {0};
        for (int i=0; i<1; i++){
            BOOST_CHECK_EQUAL(model[i],arr[i]);
        }
    }

    BOOST_AUTO_TEST_CASE( testMOV ) {
        Instruction ip(MOV);
        Argument arg1(0,10);
        Argument arg2(1,3);
        ip.addArgument(arg1);
        ip.addArgument(arg2);
        uint8_t arr[4] = {0,0,0,0};
        int i = 0;
        ByteSink sink = [&i, &arr](uint8_t byte){ arr[i++] = byte; };
        ip.encode(sink);
        uint8_t model[4] = {8, 0, 20, 176};
        for (int i=0; i<4; i++){
            BOOST_CHECK_EQUAL(model[i],arr[i]);
        }
    }

    BOOST_AUTO_TEST_CASE( testMOVB ) {
        Instruction ip(MOVB);
        Argument arg1(0,10);
        Argument arg2(1,3);
        ip.addArgument(arg1);
        ip.addArgument(arg2);
        uint8_t arr[4] = {0,0,0,0};
        int i = 0;
        ByteSink sink = [&i, &arr](uint8_t byte){ arr[i++] = byte; };
        ip.encode(sink);
        uint8_t model[4] = {16, 0, 20, 176};
        for (int i=0; i<4; i++){
            BOOST_CHECK_EQUAL(model[i],arr[i]);
        }
    }

    BOOST_AUTO_TEST_CASE( testPUSH ) {
        Instruction ip(PUSH);
        Argument arg1(1,3);
        ip.addArgument(arg1);
        uint8_t arr[2] = {0,0};
        int i = 0;
        ByteSink sink = [&i, &arr](uint8_t byte){ arr[i++] = byte; };
        ip.encode(sink);
        uint8_t model[2] = {26, 192};
        for (int i=0; i<2; i++){
            BOOST_CHECK_EQUAL(model[i],arr[i]);
        }
    }

    BOOST_AUTO_TEST_CASE( testPOP ) {
        Instruction ip(POP);
        Argument arg1(1,3);
        ip.addArgument(arg1);
        uint8_t arr[2] = {0,0};
        int i = 0;
        ByteSink sink = [&i, &arr](uint8_t byte){ arr[i++] = byte; };
        ip.encode(sink);
        uint8_t model[2] = {34, 192};
        for (int i=0; i<2; i++){
            BOOST_CHECK_EQUAL(model[i],arr[i]);
        }
    }

    BOOST_AUTO_TEST_CASE( testADD ) {
        Instruction ip(ADD);
        Argument arg1(0,10);
        Argument arg2(1,3);
        ip.addArgument(arg1);
        ip.addArgument(arg2);
        uint8_t arr[4] = {0,0,0,0};
        int i = 0;
        ByteSink sink = [&i, &arr](uint8_t byte){ arr[i++] = byte; };
        ip.encode(sink);
        uint8_t model[4] = {40, 0, 20, 176};
        for (int i=0; i<4; i++){
            BOOST_CHECK_EQUAL(model[i],arr[i]);
        }
    }
    BOOST_AUTO_TEST_CASE( testSUB ) {
        Instruction ip(SUB);
        Argument arg1(0,10);
        Argument arg2(1,3);
        ip.addArgument(arg1);
        ip.addArgument(arg2);
        uint8_t arr[4] = {0,0,0,0};
        int i = 0;
        ByteSink sink = [&i, &arr](uint8_t byte){ arr[i++] = byte; };
        ip.encode(sink);
        uint8_t model[4] = {48, 0, 20, 176};
        for (int i=0; i<4; i++){
            BOOST_CHECK_EQUAL(model[i],arr[i]);
        }
    }
    BOOST_AUTO_TEST_CASE( testAND ) {
        Instruction ip(AND);
        Argument arg1(0,10);
        Argument arg2(1,3);
        ip.addArgument(arg1);
        ip.addArgument(arg2);
        uint8_t arr[4] = {0,0,0,0};
        int i = 0;
        ByteSink sink = [&i, &arr](uint8_t byte){ arr[i++] = byte; };
        ip.encode(sink);
        uint8_t model[4] = {56, 0, 20, 176};
        for (int i=0; i<4; i++){
            BOOST_CHECK_EQUAL(model[i],arr[i]);
        }
    }
    BOOST_AUTO_TEST_CASE( testOR ) {
        Instruction ip(OR);
        Argument arg1(0,10);
        Argument arg2(1,3);
        ip.addArgument(arg1);
        ip.addArgument(arg2);
        uint8_t arr[4] = {0,0,0,0};
        int i = 0;
        ByteSink sink = [&i, &arr](uint8_t byte){ arr[i++] = byte; };
        ip.encode(sink);
        uint8_t model[4] = {64, 0, 20, 176};
        for (int i=0; i<4; i++){
            BOOST_CHECK_EQUAL(model[i],arr[i]);
        }
    }
    BOOST_AUTO_TEST_CASE( testXOR ) {
        Instruction ip(XOR);
        Argument arg1(0,10);
        Argument arg2(1,3);
        ip.addArgument(arg1);
        ip.addArgument(arg2);
        uint8_t arr[4] = {0,0,0,0};
        int i = 0;
        ByteSink sink = [&i, &arr](uint8_t byte){ arr[i++] = byte; };
        ip.encode(sink);
        uint8_t model[4] = {72, 0, 20, 176};
        for (int i=0; i<4; i++){
            BOOST_CHECK_EQUAL(model[i],arr[i]);
        }
    }
    BOOST_AUTO_TEST_CASE( testNOT ) {
        Instruction ip(NOT);
        Argument arg1(1,3);
        ip.addArgument(arg1);
        uint8_t arr[2] = {0,0};
        int i = 0;
        ByteSink sink = [&i, &arr](uint8_t byte){ arr[i++] = byte; };
        ip.encode(sink);
        uint8_t model[2] = {82, 192};
        for (int i=0; i<2; i++){
            BOOST_CHECK_EQUAL(model[i],arr[i]);
        }
    }

    BOOST_AUTO_TEST_CASE( testSHR ) {
        Instruction ip(SHR);
        Argument arg1(0,10);
        Argument arg2(1,3);
        ip.addArgument(arg1);
        ip.addArgument(arg2);
        uint8_t arr[4] = {0,0,0,0};
        int i = 0;
        ByteSink sink = [&i, &arr](uint8_t byte){ arr[i++] = byte; };
        ip.encode(sink);
        uint8_t model[4] = {88, 0, 20, 176};
        for (int i=0; i<4; i++){
            BOOST_CHECK_EQUAL(model[i],arr[i]);
        }
    }
    BOOST_AUTO_TEST_CASE( testSHL ) {
        Instruction ip(SHL);
        Argument arg1(0,10);
        Argument arg2(1,3);
        ip.addArgument(arg1);
        ip.addArgument(arg2);
        uint8_t arr[4] = {0,0,0,0};
        int i = 0;
        ByteSink sink = [&i, &arr](uint8_t byte){ arr[i++] = byte; };
        ip.encode(sink);
        uint8_t model[4] = {96, 0, 20, 176};
        for (int i=0; i<4; i++){
            BOOST_CHECK_EQUAL(model[i],arr[i]);
        }
    }
    BOOST_AUTO_TEST_CASE( testINC ) {
        Instruction ip(INC);
        Argument arg1(1,3);
        ip.addArgument(arg1);
        uint8_t arr[2] = {0,0};
        int i = 0;
        ByteSink sink = [&i, &arr](uint8_t byte){ arr[i++] = byte; };
        ip.encode(sink);
        uint8_t model[2] = {106, 192};
        for (int i=0; i<2; i++){
            BOOST_CHECK_EQUAL(model[i],arr[i]);
        }
    }
    BOOST_AUTO_TEST_CASE( testDEC ) {
        Instruction ip(DEC);
        Argument arg1(1,3);
        ip.addArgument(arg1);
        uint8_t arr[2] = {0,0};
        int i = 0;
        ByteSink sink = [&i, &arr](uint8_t byte){ arr[i++] = byte; };
        ip.encode(sink);
        uint8_t model[2] = {114, 192};
        for (int i=0; i<2; i++){
            BOOST_CHECK_EQUAL(model[i],arr[i]);
        }
    }
    BOOST_AUTO_TEST_CASE( testCMP ) {
        Instruction ip(CMP);
        Argument arg1(0,10);
        Argument arg2(1,3);
        ip.addArgument(arg1);
        ip.addArgument(arg2);
        uint8_t arr[4] = {0,0,0,0};
        int i = 0;
        ByteSink sink = [&i, &arr](uint8_t byte){ arr[i++] = byte; };
        ip.encode(sink);
        uint8_t model[4] = {120, 0, 20, 176};
        for (int i=0; i<4; i++){
            BOOST_CHECK_EQUAL(model[i],arr[i]);
        }
    }
    BOOST_AUTO_TEST_CASE( testTEST ) {
        Instruction ip(TEST);
        Argument arg1(0,10);
        Argument arg2(1,3);
        ip.addArgument(arg1);
        ip.addArgument(arg2);
        uint8_t arr[4] = {0,0,0,0};
        int i = 0;
        ByteSink sink = [&i, &arr](uint8_t byte){ arr[i++] = byte; };
        ip.encode(sink);
        uint8_t model[4] = {128, 0, 20, 176};
        for (int i=0; i<4; i++){
            BOOST_CHECK_EQUAL(model[i],arr[i]);
        }
    }
    BOOST_AUTO_TEST_CASE( testJMP ) {
        Instruction ip(JMP);
        Argument arg1(1,3);
        ip.addArgument(arg1);
        uint8_t arr[2] = {0,0};
        int i = 0;
        ByteSink sink = [&i, &arr](uint8_t byte){ arr[i++] = byte; };
        ip.encode(sink);
        uint8_t model[2] = {138, 192};
        for (int i=0; i<2; i++){
            BOOST_CHECK_EQUAL(model[i],arr[i]);
        }
    }
    BOOST_AUTO_TEST_CASE( testJE ) {
        Instruction ip(JE);
        Argument arg1(1,3);
        ip.addArgument(arg1);
        uint8_t arr[2] = {0,0};
        int i = 0;
        ByteSink sink = [&i, &arr](uint8_t byte){ arr[i++] = byte; };
        ip.encode(sink);
        uint8_t model[2] = {146, 192};
        for (int i=0; i<2; i++){
            BOOST_CHECK_EQUAL(model[i],arr[i]);
        }
    }
    BOOST_AUTO_TEST_CASE( testJNE ) {
        Instruction ip(JNE);
        Argument arg1(1,3);
        ip.addArgument(arg1);
        uint8_t arr[2] = {0,0};
        int i = 0;
        ByteSink sink = [&i, &arr](uint8_t byte){ arr[i++] = byte; };
        ip.encode(sink);
        uint8_t model[2] = {154, 192};
        for (int i=0; i<2; i++){
            BOOST_CHECK_EQUAL(model[i],arr[i]);
        }
    }
    BOOST_AUTO_TEST_CASE( testCALL ) {
        Instruction ip(CALL);
        Argument arg1(1,3);
        ip.addArgument(arg1);
        uint8_t arr[2] = {0,0};
        int i = 0;
        ByteSink sink = [&i, &arr](uint8_t byte){ arr[i++] = byte; };
        ip.encode(sink);
        uint8_t model[2] = {162, 192};
        for (int i=0; i<2; i++){
            BOOST_CHECK_EQUAL(model[i],arr[i]);
        }
    }
    BOOST_AUTO_TEST_CASE( testRET ) {
        Instruction ip(RET);
        uint8_t arr[1] = {0};
        int i = 0;
        ByteSink sink = [&i, &arr](uint8_t byte){ arr[i++] = byte; };
        ip.encode(sink);
        uint8_t model[1] = {168};
        for (int i=0; i<1; i++){
            BOOST_CHECK_EQUAL(model[i],arr[i]);
        }
    }
    BOOST_AUTO_TEST_CASE( testarg2and3 ) {
        Instruction ip(MOV);
        Argument arg1(2,0x041F);
        Argument arg2(3,4);
        arg2.setDisplacement(16);
        ip.addArgument(arg1);
        ip.addArgument(arg2);
        uint8_t arr[5] = {0,0,0,0,0};
        int i = 0;
        ByteSink sink = [&i, &arr](uint8_t byte){ arr[i++] = byte; };
        ip.encode(sink);
        uint8_t model[5] = {12, 8, 63, 193, 0};
        for (int i=0; i<4; i++){
            BOOST_CHECK_EQUAL(model[i],arr[i]);
        }
    }

    BOOST_AUTO_TEST_CASE( encodeDecode ) {
        Instruction ip(MOV);
        Argument arg1(2,0x041F);
        Argument arg2(3,4);
        arg2.setDisplacement(16);
        ip.addArgument(arg1);
        ip.addArgument(arg2);
        uint8_t arr[5] = {0,0,0,0,0};
        int i = 0;
        ByteSink sink = [&i, &arr](uint8_t byte){ arr[i++] = byte; };
        ip.encode(sink);
        uint8_t model[5] = {12, 8, 63, 193, 0};
        i = 0;
        ByteSource source = [&i, &arr](){ return arr[i++]; };
        uint8_t bytesRead = 0;
        Instruction newInst = ip.decode(source, bytesRead);
    }

BOOST_AUTO_TEST_SUITE_END()
