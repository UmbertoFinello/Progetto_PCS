#ifndef __TEST_H
#define __TEST_H

#include <gtest/gtest.h>

#include "Delaunay_class.hpp"

using namespace testing;

TEST(TestEmpty, TestEmpty)
{
  ProjectLibrary::Empty empty;
  ASSERT_NO_THROW(empty.Show());
}

#endif // __TEST_H
