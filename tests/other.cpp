#include <gtest/gtest.h>

#include <cxxmidi/event.hpp>
#include <cxxmidi/message.hpp>
#include <cxxmidi/track.hpp>

TEST(Other, NoexceptMoveCtors) {
  // for the best std::vector performance we expect following classes to be
  // noexcept move constructible
  EXPECT_TRUE(std::is_nothrow_move_constructible<cxxmidi::Message>::value);
  EXPECT_TRUE(std::is_nothrow_move_assignable<cxxmidi::Message>::value);
  EXPECT_TRUE(std::is_nothrow_move_constructible<cxxmidi::Event>::value);
  EXPECT_TRUE(std::is_nothrow_move_assignable<cxxmidi::Event>::value);
  EXPECT_TRUE(std::is_nothrow_move_constructible<cxxmidi::Track>::value);
  EXPECT_TRUE(std::is_nothrow_move_assignable<cxxmidi::Track>::value);
}
