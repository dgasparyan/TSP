#ifdef TESTING
#include "../Core/Utils.h"
#include <gtest/gtest.h>

namespace test
{
struct UtilTestFixture : public ::testing::Test
{};

TEST_F(UtilTestFixture, test_is_equal)
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

TEST_F(UtilTestFixture, test_trim_start)
{
	ASSERT_EQ("abcde", trimAndLowerCopy("abcde"));
	ASSERT_EQ("abcde", trimAndLowerCopy("  abcde"));
	ASSERT_EQ("abcde", trimAndLowerCopy("\t\tabcde"));
	ASSERT_EQ("", trimAndLowerCopy("     "));
}

TEST_F(UtilTestFixture, test_trim_end)
{
	ASSERT_EQ("abcde", trimAndLowerCopy("abcde"));
	ASSERT_EQ("abcde", trimAndLowerCopy("abcde   "));
	ASSERT_EQ("abcde", trimAndLowerCopy("abcde\t\t\t"));
	ASSERT_EQ("", trimAndLowerCopy("     "));
}

TEST_F(UtilTestFixture, test_trim)
{
	ASSERT_EQ("abcde", trimAndLowerCopy("abcde"));
	ASSERT_EQ("abcde", trimAndLowerCopy("  abcde"));
	ASSERT_EQ("abcde", trimAndLowerCopy("abcde   "));
	ASSERT_EQ("abcde", trimAndLowerCopy("  abcde   "));
	ASSERT_EQ("", trimAndLowerCopy("     "));
}

TEST_F(UtilTestFixture, test_trim_and_lower_copy)
{
	ASSERT_EQ("abcde", trimAndLowerCopy("abcde"));
	ASSERT_EQ("abcde", trimAndLowerCopy("  abcde   "));
	ASSERT_EQ("abcde", trimAndLowerCopy("   ABCDE      "));
	ASSERT_EQ("", trimAndLowerCopy("     "));
}

TEST_F(UtilTestFixture, test_trim_and_lower_equal)
{
	ASSERT_TRUE(trimAndLowerEqual("abcde", "abcde"));
	ASSERT_TRUE(trimAndLowerEqual("abcde", "  abcde   "));
	ASSERT_TRUE(trimAndLowerEqual("abcde", "   ABCDE      "));
	ASSERT_TRUE(trimAndLowerEqual("", "     "));
}

TEST_F(UtilTestFixture, test_split_string)
{
	std::vector<std::string> result_str{ "abc", "dfg", "hij", "klm" };
	std::string to_split("abc dfg hij klm");
	std::vector<std::string> v(4, "");
	splitString(to_split, " ", v.begin());
	ASSERT_EQ(v, result_str);

	to_split.append("   ");
	to_split.insert(0 , "  ");
	ASSERT_EQ(v, result_str);
}

}

#endif // TESTING
