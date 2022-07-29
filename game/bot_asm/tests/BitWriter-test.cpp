#define BOOST_TEST_DYN_LINK
#include "../BitHelper.h"
#include <boost/test/unit_test.hpp>

BOOST_AUTO_TEST_SUITE(BitWriterTest)

BOOST_AUTO_TEST_CASE(test1) {
    uint8_t a = 6;
    uint8_t b = 0x9;
    uint16_t c = 0xB05;
    BitWriter<10> bw;

    bw.writeBits(a, 3);
    BOOST_CHECK_EQUAL(bw.getByteCount(), 1);
    bw.writeBits(b, 4);
    BOOST_CHECK_EQUAL(bw.getByteCount(), 1);
    bw.writeBits(c, 12);
    BOOST_CHECK_EQUAL(bw.getByteCount(), 3);

    uint8_t i = 0;
    uint8_t arr[3] = {0, 0, 0};
    ByteSink sink = [&i, &arr](uint8_t byte) { arr[i++] = byte; };
    bw.toSink(sink);
    uint8_t model[3] = {0xD3, 0x60, 0xA0};
    for (int i = 0; i < 3; i++) {
        BOOST_CHECK_EQUAL(arr[i], model[i]);
    }
}

BOOST_AUTO_TEST_CASE(test2) {
    uint16_t a = 0xFFFF;
    BitWriter<10> bw;

    bw.writeBits(a, 16);
    BOOST_CHECK_EQUAL(bw.getByteCount(), 2);
    bw.writeBits(a, 8);
    BOOST_CHECK_EQUAL(bw.getByteCount(), 3);
    bw.writeBits(a, 4);
    BOOST_CHECK_EQUAL(bw.getByteCount(), 4);
    bw.writeBits(a, 16);
    BOOST_CHECK_EQUAL(bw.getByteCount(), 6);

    uint8_t i = 0;
    uint8_t arr[6] = {0};
    ByteSink sink = [&i, &arr](uint8_t byte) { arr[i++] = byte; };
    bw.toSink(sink);
    uint8_t model[6] = {255, 255, 255, 255, 255, 240};
    for (int i = 0; i < 6; i++) {
        BOOST_CHECK_EQUAL(arr[i], model[i]);
    }
}

BOOST_AUTO_TEST_CASE(test3) {
    uint16_t a = 32812;
    uint8_t b = 104;
    uint8_t c = 244;
    BitWriter<5> bw;

    bw.writeBits(a, 16);
    BOOST_CHECK_EQUAL(bw.getByteCount(), 2);
    bw.writeBits(b, 4);
    BOOST_CHECK_EQUAL(bw.getByteCount(), 3);
    bw.writeBits(c, 7);
    BOOST_CHECK_EQUAL(bw.getByteCount(), 4);

    uint8_t i = 0;
    uint8_t arr[4] = {0};
    ByteSink sink = [&i, &arr](uint8_t byte) { arr[i++] = byte; };
    bw.toSink(sink);
    uint8_t model[4] = {128, 44, 142, 128};
    for (int i = 0; i < 4; i++) {
        BOOST_CHECK_EQUAL(arr[i], model[i]);
    }
}

BOOST_AUTO_TEST_SUITE_END()
