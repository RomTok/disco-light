/**
 * @file mmsconverter_test.cpp
 *
 * Unit tests of MMSConverter class.
 *
 * @ingroup cppunit
 */

#include "mmstools/mmsconverter_test.h"

CPPUNIT_TEST_SUITE_REGISTRATION(MMSConverterTest);

void MMSConverterTest::setUp() {
	this->conv = new MMSConverter("ISO-8859-1");
}

void MMSConverterTest::tearDown() {
	delete this->conv;
}

void MMSConverterTest::testConstructorException() {
	MMSConverter invalid_conv("invalid_codepage");
}

void MMSConverterTest::testConvert() {
	const char input[2] = {0xf6, 0x00}; // 'ö'
	const std::string out = this->conv->convert("ISO-8859-1", std::string(input));

	CPPUNIT_ASSERT_EQUAL(out.c_str()[0] & 0xff, 0xc3);
	CPPUNIT_ASSERT_EQUAL(out.c_str()[1] & 0xff, 0xb6);
}

void MMSConverterTest::testConvertException() {
	this->conv->convert("it", "aaa");
}
