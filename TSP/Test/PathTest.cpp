#ifdef TESTING

#include "../Core/Path.h"

#include <array>
#include <gtest/gtest.h>

namespace
{

struct Int
{
	Int() = default;
	constexpr Int(int i)
		:m_i(i)
	{}

	operator bool() const { return m_i != std::numeric_limits<int>::lowest(); }

	int m_i = std::numeric_limits<int>::lowest();
};

float distance(Int first, Int second)
{
	return static_cast<float>(std::abs(first.m_i - second.m_i));
}

using IntPath = Path<Int>;
}

struct PathTestFixture : public ::testing::Test
{
};

TEST_F(PathTestFixture, test_operator_bool)
{
	IntPath empty;
	ASSERT_FALSE(empty);

	// NOTE: converts to size_t without the cast
	IntPath notEmpty(Int(1));
	ASSERT_TRUE(notEmpty);
}

TEST_F(PathTestFixture, test_totalDistance)
{
	// NOTE: converts to size_t without the cast
	IntPath p((Int)0);
	ASSERT_EQ(p.totalDistance(), 0.0f);

	p.push_back(5);
	ASSERT_EQ(p.totalDistance(), 5.0f);

	p.push_back(2);
	ASSERT_EQ(p.totalDistance(), 8.0f);

	p.push_back(-2);
	ASSERT_EQ(p.totalDistance(), 12.0f);

	std::array<Int, 4> a {0, 5, 8, 12};
	IntPath newP;
	newP.push_back(a.begin(), a.end());

	ASSERT_EQ(p.totalDistance(), newP.totalDistance());
}

#endif