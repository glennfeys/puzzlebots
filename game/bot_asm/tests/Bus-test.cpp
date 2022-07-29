#define BOOST_TEST_DYN_LINK
#include <boost/test/unit_test.hpp>
#include "../Bus.h"
#include "../BotModule.h"
#include "../Memory.h"
#include "../VisionInput.h"
#include "../DecisionOutput.h"

BOOST_AUTO_TEST_SUITE(BusTest)

    BOOST_AUTO_TEST_CASE( testWrite ) {
        Bus bus;
        std::shared_ptr<Memory> memory (new Memory(MEM_START, 1024));
        bus.addBotModule(memory);

        bus.writeOneByte(5, MEM_START);
        BOOST_CHECK_EQUAL(bus.readOneByte(MEM_START), 5);
        BOOST_CHECK_EQUAL(memory->readOneByte(MEM_START), 5);
    }

    BOOST_AUTO_TEST_CASE( testSize ) {
        Bus bus;
        std::shared_ptr<Memory> memory (new Memory(MEM_START, 2));
        bus.addBotModule(memory);
        
        uint16_t bytes = (20 << 8) | 21;
        bus.writeTwoBytes(bytes, MEM_START);
        BOOST_CHECK_EQUAL(bus.readTwoBytes(MEM_START), (20 << 8) | 21);
    }

    BOOST_AUTO_TEST_CASE( multipleBotModules ) {
        Bus bus;
        std::shared_ptr<VisionInput> vision (new VisionInput(IO_VISION_START, IO_VISION_SIZE));
        std::shared_ptr<DecisionOutput> decision (new DecisionOutput(IO_DECISION_START, IO_DECISION_SIZE));
        std::shared_ptr<Memory> memoryFsm (new Memory(MEM_FSM_START, MEM_FSM_SIZE));
        std::shared_ptr<Memory> memory (new Memory(MEM_START, 1024));
        bus.addBotModule(vision);
        bus.addBotModule(decision);
        bus.addBotModule(memoryFsm);
        bus.addBotModule(memory);

        bus.writeTwoBytes(5022, MEM_FSM_START);
        BOOST_CHECK_EQUAL(memoryFsm->readTwoBytes(MEM_FSM_START), 5022);

        bus.writeOneByte(26, MEM_START + 500);
        BOOST_CHECK_EQUAL(memory->readOneByte(MEM_START + 500), 26);

        bus.writeOneByte(1, IO_DECISION_START + 1);
        BOOST_CHECK_EQUAL(decision->popDecision(), 1);
    }

    BOOST_AUTO_TEST_CASE( decisionOuput ) {
        Bus bus;
        std::shared_ptr<DecisionOutput> decision (new DecisionOutput(IO_DECISION_START, IO_DECISION_SIZE));
        bus.addBotModule(decision);
        bus.writeTwoBytes((1 << 8) | 2, IO_DECISION_START + 1);

        BOOST_CHECK(decision->isDecisionAvailable());
        BOOST_CHECK_EQUAL(decision->getDecisionCount(), 2);
        BOOST_CHECK_EQUAL(decision->popDecision(), 1);

        BOOST_CHECK(decision->isDecisionAvailable());
        BOOST_CHECK_EQUAL(decision->getDecisionCount(), 1);
        BOOST_CHECK_EQUAL(decision->popDecision(), 2);

        BOOST_CHECK(!decision->isDecisionAvailable());
        BOOST_CHECK_EQUAL(decision->getDecisionCount(), 0);
    }

    BOOST_AUTO_TEST_CASE( visionInput ) {
        Bus bus;
        std::shared_ptr<VisionInput> vision (new VisionInput(IO_VISION_START, IO_VISION_SIZE));
        bus.addBotModule(vision);
        vision->setVisionBitMask((1 << 24) | (2 << 16) | (3 << 8) | 4);

        BOOST_CHECK_EQUAL(bus.readOneByte(IO_VISION_START), 1);
        BOOST_CHECK_EQUAL(bus.readOneByte(IO_VISION_START + 1), 2);
        BOOST_CHECK_EQUAL(bus.readOneByte(IO_VISION_START + 2), 3);
        BOOST_CHECK_EQUAL(bus.readOneByte(IO_VISION_START + 3), 4);

        BOOST_CHECK_EQUAL(bus.readTwoBytes(IO_VISION_START), (1 << 8) | 2);
        BOOST_CHECK_EQUAL(bus.readTwoBytes(IO_VISION_START + 1), (2 << 8) | 3);
        BOOST_CHECK_EQUAL(bus.readTwoBytes(IO_VISION_START + 2), (3 << 8) | 4);
    }

    BOOST_AUTO_TEST_CASE( busTestExceptions ) {
        Bus bus;
        std::shared_ptr<Memory> memory_1 (new Memory(MEM_START, 1024));
        std::shared_ptr<Memory> memory_2 (new Memory(MEM_START + 512, 1024));
        std::shared_ptr<Memory> memory_3 (new Memory(MEM_START - 512, 1024));
        std::shared_ptr<Memory> memory_4 (new Memory(MEM_START + 256 , 512));
        std::shared_ptr<Memory> memory_5 (new Memory(0, 128));

        BOOST_CHECK_THROW(bus.readOneByte(MEM_START), BusError);
        bus.addBotModule(memory_1);
        bus.readOneByte(MEM_START);
        BOOST_CHECK_THROW(bus.addBotModule(memory_2), BusError);
        BOOST_CHECK_THROW(bus.addBotModule(memory_3), BusError);
        BOOST_CHECK_THROW(bus.addBotModule(memory_4), BusError);
        BOOST_CHECK_THROW(bus.addBotModule(memory_5), BusError);
    }

    BOOST_AUTO_TEST_CASE( exceptionsModules ) {
        Bus bus;
        std::shared_ptr<VisionInput> vision (new VisionInput(IO_VISION_START, IO_VISION_SIZE));
        std::shared_ptr<DecisionOutput> decision (new DecisionOutput(IO_DECISION_START, IO_DECISION_SIZE));
        std::shared_ptr<Memory> memoryFsm (new Memory(MEM_FSM_START, MEM_FSM_SIZE));
        bus.addBotModule(vision);
        bus.addBotModule(decision);
        bus.addBotModule(memoryFsm);
        
        BOOST_CHECK_THROW(bus.writeOneByte(1, IO_VISION_START), ModuleError);
        BOOST_CHECK_THROW(bus.writeTwoBytes(1, IO_VISION_START), ModuleError);
        BOOST_CHECK_THROW(vision->readOneByte(0), ModuleError);
        
        BOOST_CHECK_THROW(bus.readOneByte(IO_DECISION_START), ModuleError);
        BOOST_CHECK_THROW(bus.readTwoBytes(IO_DECISION_START), ModuleError);
        BOOST_CHECK_THROW(decision->writeOneByte(1, 0), ModuleError);

        BOOST_CHECK_THROW(memoryFsm->readOneByte(0), ModuleError);
        BOOST_CHECK_THROW(memoryFsm->writeOneByte(1, MEM_START), ModuleError);
    }
BOOST_AUTO_TEST_SUITE_END()
