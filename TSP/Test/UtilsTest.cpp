#ifdef TESTING
#include "../Utils.h"
#include <gtest/gtest.h>

namespace test
{
struct IsEqualTestFixture : public ::testing::Test
{};

TEST_F(IsEqualTestFixture, test_is_equal)
{
	ASSERT_TRUE(isEqual(1.0f, 1.0f));
	ASSERT_TRUE(isEqual(0.0f, 0.0f));
	ASSERT_TRUE(isEqual(0.001f, 0.001f));
	ASSERT_TRUE(isEqual(-1.0f, -1.0f));

	ASSERT_FALSE(isEqual(0.001f, 0.0f));
	ASSERT_FALSE(isEqual(-0.001f, 0.0f));
	ASSERT_FALSE(isEqual(0.999f, 1.0f));
	ASSERT_FALSE(isEqual(-0.999f, -1.0f));
	ASSERT_FALSE(isEqual(-1.0, 1.0f));
}

}

#endif // TESTING
