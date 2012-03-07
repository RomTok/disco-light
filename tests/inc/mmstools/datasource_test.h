#ifndef DATASOURCE_TEST_H
#define DATASOURCE_TEST_H

/**
 * @file datasource_test.h
 *
 * Header file for unit tests of DataSource class.
 *
 * @ingroup cppunit
 */

#include <cppunit/extensions/HelperMacros.h>

class DataSourceTest: public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(DataSourceTest);
	CPPUNIT_TEST(testConstructor);
	CPPUNIT_TEST(testSetter);
	CPPUNIT_TEST_SUITE_END();

public:
	/**
	 * Not used.
	 */
	void setUp();

	/**
	 * Not used.
	 */
	void tearDown();

	/**
	 * Tests DataSource::DataSource constructor.
	 *
	 * It creates a DataSource instance which initializes private
	 * attributes which are then checked by calling the getter-methods.
	 */
	void testConstructor();

	/**
	 * Tests DataSource setter/getter methods.
	 *
	 * It sets all attributes by using their setter-method and compares
	 * the values by calling the getter-methods.
	 */
	void testSetter();
};

#endif  // DATASOURCE_TEST_H
