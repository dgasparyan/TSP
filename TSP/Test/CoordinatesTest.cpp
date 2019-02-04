#ifdef TESTING

#include "../Coordinates.h"
#include "../Utils.h"

#include <gtest/gtest.h>
#include <array>

namespace Test
{
class CoordinateTestFixture : public ::testing::Test
{};

namespace
{
template <class T>
void testValid()
{
	T invalid;
	ASSERT_FALSE(invalid.isValid());

	T valid(45.0f, 0.0f);
	ASSERT_TRUE(valid.isValid());

	T less_than_min(T::MIN - 1.0f);
	ASSERT_FALSE(less_than_min.isValid());

	T greater_than_max(T::MAX + 1.0f);
	ASSERT_FALSE(greater_than_max.isValid());

}

template <class T>
void testValues()
{
	std::vector<T> positive_values;
	positive_values.emplace_back(0.0f, 120.0f);
	positive_values.emplace_back(1.0f, 60.0f);
	positive_values.emplace_back(1.5f, 30.0f);
	positive_values.emplace_back(2.0f, 0.0f);
	positive_values.emplace_back(-2.0f, 0.0f, T::getNegativeDirection());
	positive_values.emplace_back(2.0f);
	T val {2.0f};
	for (auto v : positive_values)
	{
		bool is_eq = v == val;
		if (!is_eq)
		{
			std::cout << "Error: " << v << " " << val << std::endl;
			ASSERT_TRUE(false);
		}
	}

	std::vector<T> negative_values;
	negative_values.emplace_back(0.0f, 120.0f, T::getNegativeDirection());
	negative_values.emplace_back(1.0f, 60.0f, T::getNegativeDirection());
	negative_values.emplace_back(1.5f, 30.0f, T::getNegativeDirection());
	negative_values.emplace_back(2.0f, 0.0f, T::getNegativeDirection());
	negative_values.emplace_back(-2.0f, 0.0f, T::getPositiveDirection());
	T n_val{ -2.0f };
	for (auto v : negative_values)
	{
		bool is_eq = v == n_val;
		if (!is_eq)
		{
			std::cout << "Error: " << v << " " << n_val << std::endl;
			ASSERT_TRUE(false);
		}
	}

}

template <class T>
void isEqualRadian(float value, float radian)
{
	T t(value);
	if (!isEqual(t.toRadians(), radian))
	{
		std::cout << "Error while calculating radian! " << t  << " " << t.toRadians() 
				  << " Expected Radian = " << radian << std::endl;
		ASSERT_TRUE(false);
	}
}

template <class T>
void testToRadians()
{
	isEqualRadian<T>(0.0f, 0.0f);
	isEqualRadian<T>(45.0f, 0.785f);
	isEqualRadian<T>(90.0f, 1.57f);
	isEqualRadian<T>(-45.0f, -0.785f);
	isEqualRadian<T>(-90.0f, -1.57f);
}

// law of consines is known to have rounding errors
const float EPS = 50.0f;

void testDistance(Coordinate f, Coordinate s, int expected)
{
	if (!isEqual(distance(f, s), (float)expected, EPS))
	{
		std::cout << "Errorcalculating distance, expected " << expected << " while the value was " << distance(f, s) << std::endl;
		ASSERT_TRUE(false);
	}
}

}
 
///   Latitude tests  ////
TEST_F(CoordinateTestFixture, test_latitude_valid)
{
	testValid<Latitude>();
}

TEST_F(CoordinateTestFixture, test_latitude_values)
{
	testValues<Latitude>();
}

TEST_F(CoordinateTestFixture, test_latitude_to_radians)
{
	testToRadians<Latitude>();
}

///		Longitude tests ////
TEST_F(CoordinateTestFixture, test_longitude_valid)
{
	testValid<Longitude>();
}

TEST_F(CoordinateTestFixture, test_longitude_values)
{
	testValues<Longitude>();
}

TEST_F(CoordinateTestFixture, test_longitude_to_radians)
{
	testToRadians<Longitude>();
	isEqualRadian<Longitude>(180.0f, 3.14f);
	isEqualRadian<Longitude>(-180.0f, -3.14f);
}

/// Coordinate Tests ////
TEST_F(CoordinateTestFixture, test_coordinate_isValid)
{
	Coordinate c(Latitude(0.0f), Longitude(0.0f));
	ASSERT_TRUE(c.isValid());

	Coordinate invalidLatitude(Latitude(), Longitude(0.0f));
	ASSERT_FALSE(invalidLatitude.isValid());

	Coordinate invalidLongitude(Latitude(0.0f), Longitude());
	ASSERT_FALSE(invalidLongitude.isValid());
}


TEST_F(CoordinateTestFixture, test_distance_same_point)
{
	Coordinate f(Latitude(45.0f), Longitude(45.0f));
	Coordinate s(Latitude(45.0f), Longitude(45.0f));

	ASSERT_TRUE(isEqual(distance(f, s), 0.0f));
}

// NOTE: expected values are taken from https://www.movable-type.co.uk/scripts/latlong.html

TEST_F(CoordinateTestFixture, test_distance_nw)
{
	Coordinate point(Latitude(30.0f, 0.0f, Latitude::Direction::North), Longitude(45.0f, 0.0f, Longitude::Direction::West));

	Coordinate NW(Latitude(15.0f, 0.0f, Latitude::Direction::North), Longitude(80.0f, 0.0f, Longitude::Direction::West));
	testDistance(point, NW, 3942);

	Coordinate NE(Latitude(15.0f, 0.0f, Latitude::Direction::North), Longitude(80.0f, 0.0f, Longitude::Direction::East));
	testDistance(point, NE, 12290);

	Coordinate SW(Latitude(15.0f, 0.0f, Latitude::Direction::South), Longitude(80.0f, 0.0f, Longitude::Direction::West));
	testDistance(point, SW, 6253);

	Coordinate SE(Latitude(15.0f, 0.0f, Latitude::Direction::South), Longitude(80.0f, 0.0f, Longitude::Direction::East));
	testDistance(point, SE, 14180);
}

TEST_F(CoordinateTestFixture, test_distance_ne_nw)
{
	Coordinate point(Latitude(23.0f, 10.0f, Latitude::Direction::North), Longitude(110.0f, 15.0f, Longitude::Direction::East));

	Coordinate NW(Latitude(14.0f, 23.0f, Latitude::Direction::North), Longitude(74.0f, 12.0f, Longitude::Direction::West));
	testDistance(point, NW, 15810);

	Coordinate NE(Latitude(14.0f, 23.0f, Latitude::Direction::North), Longitude(74.0f, 12.0f, Longitude::Direction::East));
	testDistance(point, NE, 3908);

	Coordinate SW(Latitude(14.0f, 23.0f, Latitude::Direction::South), Longitude(74.0f, 12.0f, Longitude::Direction::West));
	testDistance(point, SW, 18930);

	Coordinate SE(Latitude(14.0f, 23.0f, Latitude::Direction::South), Longitude(74.0f, 12.0f, Longitude::Direction::East));
	testDistance(point, SE, 5728);
}

// TODO: add SW and SE tests in similar fashion

TEST_F(CoordinateTestFixture, test_distance_longitude_diff_overflow)
{
	Coordinate f(Latitude(45.0f), Longitude(-90.0f));
	Coordinate s(Latitude(45.0f), Longitude(120.0f));
	testDistance(f, s, 9580);
}


}


#endif // TESTING
