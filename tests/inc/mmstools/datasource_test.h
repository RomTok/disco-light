#ifndef DATASOURCE_TEST_H
#define DATASOURCE_TEST_H

#include <cppunit/extensions/HelperMacros.h>

class DataSourceTest: public CppUnit::TestFixture {
	CPPUNIT_TEST_SUITE(DataSourceTest);
	CPPUNIT_TEST(testConstructor);
	CPPUNIT_TEST(testSetter);
	CPPUNIT_TEST_SUITE_END();

public:
	void setUp();
	void tearDown();

	void testConstructor();
	void testSetter();
};

#endif  // DATASOURCE_TEST_H
