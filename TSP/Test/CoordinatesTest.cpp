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
bool isEqualRadian(float value, float radian)
{
}

template <class T>
void testToRadians()
{
	T zero{ 0.0f };
	float radian = zero.toRadian();
	ASSERT_EQ(radian, 0.0f);

	T forty_five{ 45.0f, 0.0f, T::getPositiveDirection() };
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
	testValues<Latitude>();
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
	testValues<Longitude>();
}


/// Coordinate Tests ////

}


#endif // TESTING
