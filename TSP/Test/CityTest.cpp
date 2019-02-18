#ifdef TESTING

#include "../Cities/City.h"
#include <gtest/gtest.h>
#include <unordered_set>

using namespace std::string_literals;

struct CityTestFixture : public ::testing::Test
{};

namespace
{

void testInvalidString(const std::string& str)
{
	try
	{
		City c = fromString(str);
	}
	catch (std::invalid_argument&)
	{
		return;
	}

	ASSERT_FALSE("should never reach here");
}

}

TEST_F(CityTestFixture, test_operator_bool)
{
	City c;
	ASSERT_FALSE(c);

	c = City("NewCity", Coordinate(15.0_lat, 0.0_lon));
	ASSERT_TRUE(c);
}

TEST_F(CityTestFixture, test_name_trimmed)
{
	City c("    NewCity\t\t		", Coordinate(15.0_lat, 0.0_lon));
	ASSERT_EQ(c.getName(), "NewCity"s);
}

TEST_F(CityTestFixture, test_coordinates)
{
	City c("NewCity", Coordinate(15.0_lat, 0.0_lon));
	ASSERT_EQ(c.getCoordinates(), Coordinate(15.0_lat, 0.0_lon));
}

TEST_F(CityTestFixture, test_from_string_valid)
{
	std::string city("Vancouver  ,49,13,123,6");
	City c = fromString(city);

	ASSERT_TRUE(c);
	ASSERT_EQ(c.getName(), "Vancouver"s);
	Coordinate cord(Latitude(49.0f, 13.0f, Latitude::Direction::North), Longitude(123.0f, 6.0f));
	ASSERT_EQ(cord, c.getCoordinates());
}

TEST_F(CityTestFixture, test_from_string_invalid_length)
{
	std::string city("Vancouver  ,49,13,123");
	City c = fromString(city);

	ASSERT_FALSE(c);
}

TEST_F(CityTestFixture, test_from_string_invalid_float)
{
	testInvalidString("Vancouver, INVALID, 49, 13, 123");
	testInvalidString("Vancouver, 49,INVALID,123,6");
	testInvalidString("Vancouver, 49,13,INVALID,6");
	testInvalidString("Vancouver, 49,13,123,INVALID");
}

TEST_F(CityTestFixture, test_equal)
{
	City c1("NewCity", Coordinate(15.0_lat, 0.0_lon));
	City c2("NEWCITY", Coordinate(25.0_lat, 10.0_lon));
	City c3("\t		NeWCITy    ", Coordinate(35.0_lat, 20.0_lon));
	ASSERT_EQ(c1, c2);
	ASSERT_EQ(c1, c3);
	ASSERT_EQ(c2, c3);
}

TEST_F(CityTestFixture, test_hash)
{
	City c1("NewCity", Coordinate(15.0_lat, 0.0_lon));
	City c2("NEWCITY", Coordinate(25.0_lat, 10.0_lon));
	City c3("\t		NeWCITy    ", Coordinate(35.0_lat, 20.0_lon));
	ASSERT_EQ(std::hash<City>()(c1), std::hash<City>()(c2));
	ASSERT_EQ(std::hash<City>()(c1), std::hash<City>()(c3));
	ASSERT_EQ(std::hash<City>()(c2), std::hash<City>()(c3));
}

TEST_F(CityTestFixture, test_hashset)
{
	City c1("NewCity", Coordinate(15.0_lat, 0.0_lon));
	City c2("NEWCITY", Coordinate(25.0_lat, 10.0_lon));
	City c3("\t		NeWCITy    ", Coordinate(35.0_lat, 20.0_lon));
	std::unordered_set<City> citySet;

	auto [f1, s1] = citySet.emplace(c1);
	ASSERT_TRUE(s1);

	auto [f2, s2] = citySet.emplace(c2);
	ASSERT_FALSE(s2);

	auto[f3, s3] = citySet.emplace(c3);
	ASSERT_FALSE(s3);
}



#endif // TESTING
