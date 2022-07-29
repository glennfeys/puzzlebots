#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "../CPU.h"
#include "../CPUInstruction.h"
#include "../Memory.h"

using namespace CPUInstruction;

BOOST_AUTO_TEST_SUITE(CpuTest)  

    BOOST_AUTO_TEST_CASE( MOVtest1 ) {
        Instruction ip(MOV);
        Argument arg1(0,10);
        Argument arg2(1,3);
        ip.addArgument(arg1);
        ip.addArgument(arg2);
        int i = 0;
        Bus bus;
        std::shared_ptr<BotModule> memory (new Memory(MEM_START, 1024));
        bus.addBotModule(memory);
        ByteSink sink = [&i,&bus](uint8_t byte){ bus.writeOneByte(byte,0x041F + (i++)); };
        ip.encode(sink);
        CPU cpu(0x041F, 0x051F, bus);
        cpu.step();
        BOOST_CHECK_EQUAL(cpu.getRegisters()[3],10);
    }

    BOOST_AUTO_TEST_CASE( MOVtest2 ) {
        Instruction ip(MOV);
        Argument arg1(0,10);
        Argument arg2(2, MEM_START+666);
        Bus bus;
        std::shared_ptr<BotModule> memory (new Memory(MEM_START, 1024));
        bus.addBotModule(memory);
        bus.writeTwoBytes(12, MEM_START+666);
        ip.addArgument(arg1);
        ip.addArgument(arg2);
        int i = 0;
        ByteSink sink = [&i,&bus](uint8_t byte){ bus.writeOneByte(byte,0x041F + (i++)); };
        ip.encode(sink);
        CPU cpu(0x041F, 0x051F, bus);
        cpu.step();
        uint16_t debug = bus.readTwoBytes(MEM_START+666);
        BOOST_CHECK_EQUAL(debug,10);
    }

    BOOST_AUTO_TEST_CASE( MOVBtest ) {
        Instruction ip(MOVB);
        Argument arg1(0,10);
        Argument arg2(1,3);
        ip.addArgument(arg1);
        ip.addArgument(arg2);
        int i = 0;
        Bus bus;
        std::shared_ptr<BotModule> memory (new Memory(MEM_START, 1024));
        bus.addBotModule(memory);
        ByteSink sink = [&i,&bus](uint8_t byte){ bus.writeOneByte(byte,0x041F + (i++)); };
        ip.encode(sink);
        CPU cpu(0x041F, 0x051F, bus);
        cpu.step();
        BOOST_CHECK_EQUAL(cpu.getRegisters()[3],10);
    }

    BOOST_AUTO_TEST_CASE( PUSHPOPtest ) {
        Instruction ip(PUSH);
        Argument arg1(0,10);
        ip.addArgument(arg1);
        int i = 0;
        Bus bus;
        std::shared_ptr<BotModule> memory (new Memory(MEM_START, 1024));
        bus.addBotModule(memory);
        ByteSink sink = [&i,&bus](uint8_t byte){ bus.writeOneByte(byte,0x041F + (i++)); };
        ip.encode(sink);
        CPU cpu(0x041F, 0x051F, bus);
        cpu.step();
        uint16_t value = bus.readTwoBytes(cpu.getRegisters()[1]);
        BOOST_CHECK_EQUAL(value,10);

        Instruction ip1(POP);
        Argument arg11(1,3);
        ip1.addArgument(arg11);
        ByteSink sink2 = [&i,&bus](uint8_t byte){ bus.writeOneByte(byte,0x041F + 3 + (i++)); };
        ip1.encode(sink);

        cpu.step();
        BOOST_CHECK_EQUAL(cpu.getRegisters()[3],10);
    }

    BOOST_AUTO_TEST_CASE( ADDtest ) {
        Instruction ip(ADD);
        Argument arg1(0,10);
        ip.addArgument(arg1);
        Argument arg2(1,3);
        ip.addArgument(arg2);
        int i = 0;
        Bus bus;
        std::shared_ptr<BotModule> memory (new Memory(MEM_START, 1024));
        bus.addBotModule(memory);
        ByteSink sink = [&i,&bus](uint8_t byte){ bus.writeOneByte(byte,0x041F + (i++)); };
        ip.encode(sink);
        CPU cpu(0x041F, 0x051F, bus);
        cpu.getRegisters()[3] = 5;
        cpu.step();
        BOOST_CHECK_EQUAL(cpu.getRegisters()[3],15);
    }

    BOOST_AUTO_TEST_CASE( SUBtest ) {
        Instruction ip(SUB);
        Argument arg1(0,10);
        ip.addArgument(arg1);
        Argument arg2(1,3);
        ip.addArgument(arg2);
        int i = 0;
        Bus bus;
        std::shared_ptr<BotModule> memory (new Memory(MEM_START, 1024));
        bus.addBotModule(memory);
        ByteSink sink = [&i,&bus](uint8_t byte){ bus.writeOneByte(byte,0x041F + (i++)); };
        ip.encode(sink);
        CPU cpu(0x041F, 0x051F, bus);
        cpu.getRegisters()[3] = 5;
        cpu.step();
        BOOST_CHECK_EQUAL(cpu.getRegisters()[3],5);
    }

    BOOST_AUTO_TEST_CASE( ANDtest ) {
        Instruction ip(AND);
        Argument arg1(0,1);
        ip.addArgument(arg1);
        Argument arg2(1,3);
        ip.addArgument(arg2);
        int i = 0;
        Bus bus;
        std::shared_ptr<BotModule> memory (new Memory(MEM_START, 1024));
        bus.addBotModule(memory);
        ByteSink sink = [&i,&bus](uint8_t byte){ bus.writeOneByte(byte,0x041F + (i++)); };
        ip.encode(sink);
        CPU cpu(0x041F, 0x051F, bus);
        cpu.getRegisters()[3] = 0;
        cpu.step();
        BOOST_CHECK_EQUAL(cpu.getRegisters()[3],0);
    }

    BOOST_AUTO_TEST_CASE( ORtest ) {
        Instruction ip(OR);
        Argument arg1(0,1);
        ip.addArgument(arg1);
        Argument arg2(1,3);
        ip.addArgument(arg2);
        int i = 0;
        Bus bus;
        std::shared_ptr<BotModule> memory (new Memory(MEM_START, 1024));
        bus.addBotModule(memory);
        ByteSink sink = [&i,&bus](uint8_t byte){ bus.writeOneByte(byte,0x041F + (i++)); };
        ip.encode(sink);
        CPU cpu(0x041F, 0x051F, bus);
        cpu.getRegisters()[3] = 0;
        cpu.step();
        BOOST_CHECK_EQUAL(cpu.getRegisters()[3],1);
    }

    BOOST_AUTO_TEST_CASE( XORtest ) {
        Instruction ip(XOR);
        Argument arg1(0,1);
        ip.addArgument(arg1);
        Argument arg2(1,3);
        ip.addArgument(arg2);
        int i = 0;
        Bus bus;
        std::shared_ptr<BotModule> memory (new Memory(MEM_START, 1024));
        bus.addBotModule(memory);
        CPU cpu(0x041F, 0x051F, bus);
        ByteSink sink = [&i,&bus](uint8_t byte){ bus.writeOneByte(byte,0x041F + (i++)); };
        ip.encode(sink);
        cpu.getRegisters()[3] = 1;
        cpu.step();
        BOOST_CHECK_EQUAL(cpu.getRegisters()[3], 0);
    }

    BOOST_AUTO_TEST_CASE( NOTtest ) {
        Instruction ip(NOT);
        Argument arg1(1,3);
        ip.addArgument(arg1);
        int i = 0;
        Bus bus;
        std::shared_ptr<BotModule> memory (new Memory(MEM_START, 1024));
        bus.addBotModule(memory);
        ByteSink sink = [&i,&bus](uint8_t byte){ bus.writeOneByte(byte,0x041F + (i++)); };
        ip.encode(sink);
        CPU cpu(0x041F, 0x051F, bus);
        cpu.getRegisters()[3] = 0;
        cpu.step();
        BOOST_CHECK_EQUAL(cpu.getRegisters()[3], 1);
    }

    BOOST_AUTO_TEST_CASE( SHRtest ) {
        Instruction ip(SHR);
        Argument arg1(0,2);
        ip.addArgument(arg1);
        Argument arg2(1,3);
        ip.addArgument(arg2);
        int i = 0;
        Bus bus;
        std::shared_ptr<BotModule> memory (new Memory(MEM_START, 1024));
        bus.addBotModule(memory);
        ByteSink sink = [&i,&bus](uint8_t byte){ bus.writeOneByte(byte,0x041F + (i++)); };
        ip.encode(sink);
        CPU cpu(0x041F, 0x051F, bus);
        cpu.getRegisters()[3] = 4;
        cpu.step();
        BOOST_CHECK_EQUAL(cpu.getRegisters()[3], 1);
    }

    BOOST_AUTO_TEST_CASE( SHLtest ) {
        Instruction ip(SHL);
        Argument arg1(0,2);
        ip.addArgument(arg1);
        Argument arg2(1,3);
        ip.addArgument(arg2);
        int i = 0;
        Bus bus;
        std::shared_ptr<BotModule> memory (new Memory(MEM_START, 1024));
        bus.addBotModule(memory);
        ByteSink sink = [&i,&bus](uint8_t byte){ bus.writeOneByte(byte,0x041F + (i++)); };
        ip.encode(sink);
        CPU cpu(0x041F, 0x051F, bus);
        cpu.getRegisters()[3] = 4;
        cpu.step();
        BOOST_CHECK_EQUAL(cpu.getRegisters()[3], 16);
    }

    BOOST_AUTO_TEST_CASE( INCtest ) {
        Instruction ip(INC);
        Argument arg1(1,3);
        ip.addArgument(arg1);
        int i = 0;
        Bus bus;
        std::shared_ptr<BotModule> memory (new Memory(MEM_START, 1024));
        bus.addBotModule(memory);
        ByteSink sink = [&i,&bus](uint8_t byte){ bus.writeOneByte(byte,0x041F + (i++)); };
        ip.encode(sink);
        CPU cpu(0x041F, 0x051F, bus);
        cpu.getRegisters()[3] = 3;
        cpu.step();
        BOOST_CHECK_EQUAL(cpu.getRegisters()[3], 4);
    }

    BOOST_AUTO_TEST_CASE( DECtest ) {
        Instruction ip(DEC);
        Argument arg1(1,3);
        ip.addArgument(arg1);
        int i = 0;
        Bus bus;
        std::shared_ptr<BotModule> memory (new Memory(MEM_START, 1024));
        bus.addBotModule(memory);
        ByteSink sink = [&i,&bus](uint8_t byte){ bus.writeOneByte(byte,0x041F + (i++)); };
        ip.encode(sink);
        CPU cpu(0x041F, 0x051F, bus);
        cpu.getRegisters()[3] = 4;
        cpu.step();
        BOOST_CHECK_EQUAL(cpu.getRegisters()[3], 3);
    }

    BOOST_AUTO_TEST_CASE( CMPEQtest ) {
        Instruction ip(CMP);
        Argument arg1(0,2);
        ip.addArgument(arg1);
        Argument arg2(0,2);
        ip.addArgument(arg2);
        int i = 0;
        Bus bus;
        std::shared_ptr<BotModule> memory (new Memory(MEM_START, 1024));
        bus.addBotModule(memory);
        ByteSink sink = [&i,&bus](uint8_t byte){ bus.writeOneByte(byte,0x041F + (i++)); };
        ip.encode(sink);
        CPU cpu(0x041F, 0x051F, bus);
        cpu.step();
        BOOST_CHECK_EQUAL(cpu.getRegisters()[2], 0);
    }

    BOOST_AUTO_TEST_CASE( CMPNEQtest ) {
        Instruction ip(CMP);
        Argument arg1(0,1);
        ip.addArgument(arg1);
        Argument arg2(0,3);
        ip.addArgument(arg2);
        int i = 0;
        Bus bus;
        std::shared_ptr<BotModule> memory (new Memory(MEM_START, 1024));
        bus.addBotModule(memory);
        ByteSink sink = [&i,&bus](uint8_t byte){ bus.writeOneByte(byte,0x041F + (i++)); };
        ip.encode(sink);
        CPU cpu(0x041F, 0x051F, bus);
        cpu.step();
        BOOST_CHECK_EQUAL(cpu.getRegisters()[2], 2);
    }

    BOOST_AUTO_TEST_CASE( TESTtest ) {
        Instruction ip(TEST);
        Argument arg1(0,1);
        ip.addArgument(arg1);
        Argument arg2(0,1);
        ip.addArgument(arg2);
        int i = 0;
        Bus bus;
        std::shared_ptr<BotModule> memory (new Memory(MEM_START, 1024));
        bus.addBotModule(memory);
        ByteSink sink = [&i,&bus](uint8_t byte){ bus.writeOneByte(byte,0x041F + (i++)); };
        ip.encode(sink);
        CPU cpu(0x041F, 0x051F, bus);
        cpu.step();
        BOOST_CHECK_EQUAL(cpu.getRegisters()[2], 1);
    }

    BOOST_AUTO_TEST_CASE( JMPtest ) {
        Instruction ip(JMP);
        Argument arg1(0,0x077F);
        ip.addArgument(arg1);
        int i = 0;
        Bus bus;
        std::shared_ptr<BotModule> memory (new Memory(MEM_START, 1024));
        bus.addBotModule(memory);
        ByteSink sink = [&i,&bus](uint8_t byte){ bus.writeOneByte(byte,0x041F + (i++)); };
        ip.encode(sink);
        CPU cpu(0x041F, 0x051F, bus);
        cpu.step();
        BOOST_CHECK_EQUAL(cpu.getRegisters()[0], 0x077F);
    }

    BOOST_AUTO_TEST_CASE( JEtestEQ ) {
        Instruction ip(JE);
        Argument arg1(0,0x077F);
        ip.addArgument(arg1);
        int i = 0;
        Bus bus;
        std::shared_ptr<BotModule> memory (new Memory(MEM_START, 1024));
        bus.addBotModule(memory);
        ByteSink sink = [&i,&bus](uint8_t byte){ bus.writeOneByte(byte,0x041F + (i++)); };
        ip.encode(sink);
        CPU cpu(0x041F, 0x051F, bus);
        cpu.getRegisters()[2] = 0;
        cpu.step();
        BOOST_CHECK_EQUAL(cpu.getRegisters()[0], 0x077F);
    }

    BOOST_AUTO_TEST_CASE( JEtestNEQ ) {
        Instruction ip(JE);
        Argument arg1(0,0x077F);
        ip.addArgument(arg1);
        int i = 0;
        Bus bus;
        std::shared_ptr<BotModule> memory (new Memory(MEM_START, 1024));
        bus.addBotModule(memory);
        ByteSink sink = [&i,&bus](uint8_t byte){ bus.writeOneByte(byte,0x041F + (i++)); };
        ip.encode(sink);
        CPU cpu(0x041F, 0x051F, bus);
        cpu.getRegisters()[2] = 1;
        cpu.step();
        BOOST_CHECK_EQUAL(cpu.getRegisters()[0], 0x0422);
    }

    BOOST_AUTO_TEST_CASE( JNEtestEQ ) {
        Instruction ip(JNE);
        Argument arg1(0,0x077F);
        ip.addArgument(arg1);
        int i = 0;
        Bus bus;
        std::shared_ptr<BotModule> memory (new Memory(MEM_START, 1024));
        bus.addBotModule(memory);
        ByteSink sink = [&i,&bus](uint8_t byte){ bus.writeOneByte(byte,0x041F + (i++)); };
        ip.encode(sink);
        CPU cpu(0x041F, 0x051F, bus);
        cpu.getRegisters()[2] = 0;
        cpu.step();
        BOOST_CHECK_EQUAL(cpu.getRegisters()[0], 0x0422);
    }

    BOOST_AUTO_TEST_CASE( JNEtestNEQ ) {
        Instruction ip(JNE);
        Argument arg1(0,0x077F);
        ip.addArgument(arg1);
        int i = 0;
        Bus bus;
        std::shared_ptr<BotModule> memory (new Memory(MEM_START, 1024));
        bus.addBotModule(memory);
        ByteSink sink = [&i,&bus](uint8_t byte){ bus.writeOneByte(byte,0x041F + (i++)); };
        ip.encode(sink);
        CPU cpu(0x041F, 0x051F, bus);
        cpu.getRegisters()[2] = 1;
        cpu.step();
        BOOST_CHECK_EQUAL(cpu.getRegisters()[0], 0x077F);
    }

    BOOST_AUTO_TEST_CASE( CALLtest ) {
        Instruction ip(CALL);
        Argument arg1(0,0x077F);
        ip.addArgument(arg1);
        int i = 0;
        Bus bus;
        std::shared_ptr<BotModule> memory (new Memory(MEM_START, 1024));
        bus.addBotModule(memory);
        ByteSink sink = [&i,&bus](uint8_t byte){ bus.writeOneByte(byte,0x041F + (i++)); };
        ip.encode(sink);
        CPU cpu(0x041F, 0x051F, bus);
        cpu.step();
        BOOST_CHECK_EQUAL(cpu.getRegisters()[0], 0x077F);
        BOOST_CHECK_EQUAL(cpu.getRegisters()[1], 0x0422);
    }

    BOOST_AUTO_TEST_CASE( RETtest ) {
        Instruction ip(RET);
        int i = 0;
        Bus bus;
        std::shared_ptr<BotModule> memory (new Memory(MEM_START, 1024));
        bus.addBotModule(memory);
        ByteSink sink = [&i,&bus](uint8_t byte){ bus.writeOneByte(byte,0x041F + (i++)); };
        ip.encode(sink);
        CPU cpu(0x041F, 0x051F, bus);
        cpu.getRegisters()[1] = 0x077F;
        cpu.step();
        BOOST_CHECK_EQUAL(cpu.getRegisters()[0], 0x077F);
        BOOST_CHECK_EQUAL(cpu.getRegisters()[1], 0x0781);
    }

BOOST_AUTO_TEST_SUITE_END()
