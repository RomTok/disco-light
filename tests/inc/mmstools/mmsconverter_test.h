#ifndef MMSCONVERTER_TEST_H
#define MMSCONVERTER_TEST_H

#include <cppunit/extensions/HelperMacros.h>

#include "mmstools/mmsconverter.h"

class MMSConverterTest: public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(MMSConverterTest);
	CPPUNIT_TEST_EXCEPTION(testConstructorException, MMSConverterError);
	CPPUNIT_TEST(testConvert);
	CPPUNIT_TEST_EXCEPTION(testConvertException, MMSConverterError);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testConstructorException();
	void testConvert();
	void testConvertException();
};

#endif  // MMSCONVERTER_TEST_H
