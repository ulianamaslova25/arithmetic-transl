#include "transl.h"

#include <gtest.h>

TEST(ArithmeticTranslyator, CanCreate)
{
    ASSERT_NO_THROW(ArithmeticTranslyator a("2.35+53"));
}

TEST(ArithmeticTranslyator, SimpleAddition) {
    ArithmeticTranslyator a("2+3");
    ASSERT_EQ(a.calculate(), 5);
}

TEST(ArithmeticTranslyator, SimpleSubtraction) {
    ArithmeticTranslyator a("5-2");
    ASSERT_EQ(a.calculate(), 3);
}

TEST(ArithmeticTranslyator, SimpleMultiplication) {
    ArithmeticTranslyator a("4*2");
    ASSERT_EQ(a.calculate(), 8);
}

TEST(ArithmeticTranslyator, SimpleDivision) {
    ArithmeticTranslyator a("10/2");
    ASSERT_EQ(a.calculate(), 5);
}

TEST(ArithmeticTranslyator, ComplexExpression) {
    ArithmeticTranslyator a("2+3*4-1/2");
    ASSERT_EQ(a.calculate(), 13.5);
}

TEST(ArithmeticTranslyator, Parentheses) {
    ArithmeticTranslyator a("(2+3)*4");
    ASSERT_EQ(a.calculate(), 20);
}

TEST(ArithmeticTranslyator, NestedParentheses) {
    ArithmeticTranslyator a("((2+3)*4)-1");
    ASSERT_EQ(a.calculate(), 19);
}


TEST(ArithmeticTranslyator, FloatingPointNumbers) {
    ArithmeticTranslyator a("2.5+3.5");
    ASSERT_EQ(a.calculate(), 6);
}

TEST(ArithmeticTranslyator, EmptyParentheses) {
    ASSERT_ANY_THROW(ArithmeticTranslyator a("23+()55+5"));
}


TEST(ArithmeticTranslyator, MismatchedParentheses) {
    ASSERT_ANY_THROW(ArithmeticTranslyator a("(2+3"));
    ASSERT_ANY_THROW(ArithmeticTranslyator a("2+3)"));
    ASSERT_ANY_THROW(ArithmeticTranslyator a(")2+3("));
}


TEST(ArithmeticTranslyator, StartsWithOperator) {
    ASSERT_ANY_THROW(ArithmeticTranslyator a("+2+3"));
    ASSERT_ANY_THROW(ArithmeticTranslyator a("*2+3"));
}

TEST(ArithmeticTranslyator, EndsWithOperator) {
    ASSERT_ANY_THROW(ArithmeticTranslyator a("2+3+"));
    ASSERT_ANY_THROW(ArithmeticTranslyator a("2+3*"));
}

TEST(ArithmeticTranslyator, InvalidFloat1) {
    ASSERT_ANY_THROW(ArithmeticTranslyator a("2.+3"));
}
TEST(ArithmeticTranslyator, InvalidFloat2) {
    ASSERT_ANY_THROW(ArithmeticTranslyator a("2.3.4+5"));
}

TEST(ArithmeticTranslyator, DivisionByZero) {
    ArithmeticTranslyator a("5/0");
    ASSERT_THROW(a.calculate(), std::runtime_error);
}

TEST(ArithmeticTranslyator, EmptyString) {
    ASSERT_ANY_THROW(ArithmeticTranslyator a(""));
}


TEST(ArithmeticTranslyator, WhitespaceHandling) {
    ArithmeticTranslyator a("   2    + 3  ");
    ASSERT_EQ(a.calculate(), 5);
}

TEST(ArithmeticTranslyator, LongExpression1) {
    ArithmeticTranslyator a("10 + 2 * 5 - 3 / 2 + (4 * (2 + 1))");
    ASSERT_EQ(a.calculate(), 30.5);
}

TEST(ArithmeticTranslyator, LongInvalidFloat)
{
    ASSERT_ANY_THROW(ArithmeticTranslyator a("1.2.3 + 4 + 5 + 6 + 7.8.9"));
}





