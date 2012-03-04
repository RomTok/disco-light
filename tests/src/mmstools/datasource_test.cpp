#include "mmstools/datasource_test.h"
#include "mmstools/datasource.h"

CPPUNIT_TEST_SUITE_REGISTRATION(DataSourceTest);

void DataSourceTest::setUp() {
}

void DataSourceTest::tearDown() {
}

void DataSourceTest::testConstructor() {
	const std::string dbms = "dbms";
	const std::string dbName = "dbName";
	const std::string address = "address";
	const unsigned int port = 1234;
	const std::string user = "user";
	const std::string passwd = "passwd";

	DataSource ds(dbms, dbName, address, port, user, passwd);

	CPPUNIT_ASSERT_EQUAL(dbms, ds.getDBMS());
	CPPUNIT_ASSERT_EQUAL(dbName, ds.getDatabaseName());
	CPPUNIT_ASSERT_EQUAL(address, ds.getAddress());
	CPPUNIT_ASSERT_EQUAL(port, ds.getPort());
	CPPUNIT_ASSERT_EQUAL(user, ds.getUser());
	CPPUNIT_ASSERT_EQUAL(passwd, ds.getPassword());
}

void DataSourceTest::testSetter() {
	const std::string dbms = "dbms";
	const std::string dbName = "dbName";
	const std::string address = "address";
	const unsigned int port = 1234;
	const std::string user = "user";
	const std::string passwd = "passwd";

	DataSource ds("", "");

	ds.setDBMS(dbms);
	CPPUNIT_ASSERT_EQUAL(dbms, ds.getDBMS());

	ds.setDatabaseName(dbName);
	CPPUNIT_ASSERT_EQUAL(dbName, ds.getDatabaseName());

	ds.setAddress(address);
	CPPUNIT_ASSERT_EQUAL(address, ds.getAddress());

	ds.setPort(port);
	CPPUNIT_ASSERT_EQUAL(port, ds.getPort());

	ds.setUser(user);
	CPPUNIT_ASSERT_EQUAL(user, ds.getUser());

	ds.setPassword(passwd);
	CPPUNIT_ASSERT_EQUAL(passwd, ds.getPassword());
}
