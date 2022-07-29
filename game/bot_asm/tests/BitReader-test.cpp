#define BOOST_TEST_DYN_LINK
#include "../BitHelper.h"
#include <boost/test/unit_test.hpp>
#include <cstdint>

BOOST_AUTO_TEST_SUITE(BitReaderTest)

BOOST_AUTO_TEST_CASE(test1) {
    uint8_t a[] = {0xD3, 0x60, 0xA0};
    uint8_t i = 0;
    ByteSource source = [&i, a]() { return a[i++]; };
    BitReader<4> br(source);
    uint8_t one = br.readBits<uint8_t>(3);
    uint8_t two = br.readBits<uint8_t>(4);
    uint16_t three = br.readBits<uint16_t>(12);
    BOOST_CHECK_EQUAL(one, 6);
    BOOST_CHECK_EQUAL(two, 9);
    BOOST_CHECK_EQUAL(three, 2821);
}

BOOST_AUTO_TEST_CASE(testFull) {
    uint8_t a[] = {255};
    uint8_t i = 0;
    ByteSource source = [&i, a]() { return a[i++]; };
    BitReader<4> br(source);
    uint8_t one = br.readBits<uint8_t>(8);
    BOOST_CHECK_EQUAL(one, 255);
}

BOOST_AUTO_TEST_CASE(testEmpty) {
    uint8_t a[] = {255};
    uint8_t i = 0;
    ByteSource source = [&i, a]() { return a[i++]; };
    BitReader<4> br(source);
    uint8_t one = br.readBits<uint8_t>(0);
    BOOST_CHECK_EQUAL(one, 0);
}

BOOST_AUTO_TEST_SUITE_END()
