#pragma once

#include "config.hpp"
#include "database.hpp"
#include "item.hpp"
#include "user.hpp"
#include "util.hpp"

#include "ftxui/component/captured_mouse.hpp"
#include "ftxui/component/component.hpp"
#include "ftxui/component/component_base.hpp"
#include "ftxui/component/component_options.hpp"
#include "ftxui/component/screen_interactive.hpp"
#include "ftxui/dom/elements.hpp"
#include "ftxui/dom/node.hpp"
#include "ftxui/screen/box.hpp"
#include "ftxui/screen/color.hpp"
#include "ftxui/screen/screen.hpp"
#include "ftxui/screen/terminal.hpp"
#include "ftxui/util/ref.hpp"

namespace tui {
inline ftxui::Color purple = {93, 39, 93};
inline ftxui::Color red = {177, 62, 83};
inline ftxui::Color orange = {239, 125, 87};
inline ftxui::Color yellow = {255, 205, 117};
inline ftxui::Color bright_green = {167, 240, 112};
inline ftxui::Color dark_green = {56, 183, 100};
inline ftxui::Color teal = {37, 113, 121};
inline ftxui::Color dark_blue = {41, 54, 111};
inline ftxui::Color blue = {59, 93, 201};
inline ftxui::Color light_blue = {65, 166, 246};
inline ftxui::Color cyan = {115, 239, 247};
inline ftxui::Color white = {244, 244, 244};
inline ftxui::Color light_gray = {148, 176, 194};
inline ftxui::Color gray = {86, 108, 134};
inline ftxui::Color dark_gray = {51, 60, 87};

void login(Database& db);
void change_password(Database& db, bool quitable, std::function<void(Database&)> caller);
}; // namespace tui
