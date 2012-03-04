#include "mmstools/mmsconverter_test.h"

CPPUNIT_TEST_SUITE_REGISTRATION(MMSConverterTest);

void MMSConverterTest::setUp() {
}

void MMSConverterTest::tearDown() {
}

void MMSConverterTest::testConstructorException() {
	MMSConverter conv("invalid_codepage");
}

void MMSConverterTest::testConvert() {
	// TODO: find some testcase
}

void MMSConverterTest::testConvertException() {
	MMSConverter conv("cn");
	conv.convert("it", "aaa");
}
