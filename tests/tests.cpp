#include <cassert>
#include <fstream>
#include <iostream>
#include <gtest/gtest.h>
#include "softver-za-prodavnice/config.hpp"
#include "softver-za-prodavnice/util.hpp"

TEST(trim_whitespace, whitespace) {
	const std::string_view whitespace = "whitespace";

	std::string_view str1 = whitespace;
	EXPECT_EQ(whitespace, util::trim_whitespace(str1));

	str1 = "  whitespace";
	EXPECT_EQ(whitespace, util::trim_whitespace(str1));

	str1 = "whitespace   ";
	EXPECT_EQ(whitespace, util::trim_whitespace(str1));

	str1 = "  whitespace      ";
	EXPECT_EQ(whitespace, util::trim_whitespace(str1));

	str1 = "whitespace";
	EXPECT_NE(whitespace, util::trim_whitespace(str1));
}
