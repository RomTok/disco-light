#ifndef MMSCONVERTER_TEST_H
#define MMSCONVERTER_TEST_H

/**
 * @file mmsconverter_test.h
 *
 * Header file for unit tests of MMSConverter class.
 *
 * @ingroup cppunit
 */

#include <cppunit/extensions/HelperMacros.h>

#include "mmstools/mmsconverter.h"

class MMSConverterTest: public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(MMSConverterTest);
	CPPUNIT_TEST_EXCEPTION(testConstructorException, MMSConverterError);
	CPPUNIT_TEST(testConvert);
	CPPUNIT_TEST_EXCEPTION(testConvertException, MMSConverterError);
	CPPUNIT_TEST_SUITE_END();

public:
	/**
	 * Creates in instance of MMSConverter with ISO-8859-1 codepage.
	 */
	void setUp();

	/**
	 * Deletes the MMSConverter instance.
	 */
	void tearDown();

	/**
	 * Tests exception thrown by MMSConverter::MMSConverter() constructor.
	 *
	 * It creates an instance of MMSConverter with an invalid codepage,
	 * which must throw an MMSConverterError exception.
	 */
	void testConstructorException();

	/**
	 * Tests MMSConverter::convert() method.
	 *
	 * Checks if latin1 character 'ö' is correctly converted
	 * to UTF-8 0xc3b6.
	 */
	void testConvert();

	/**
	 * Tests exceptions thrown by MMSConverter::convert() method.
	 *
	 * It calls MMSConverter::convert() with another codepage than ISO-8859-1,
	 * which must throw MMSConverterError exception.
	 */
	void testConvertException();

private:
	MMSConverter *conv;		/**< Converter used for MMSConverter::convert() tests. */
};

#endif  // MMSCONVERTER_TEST_H
