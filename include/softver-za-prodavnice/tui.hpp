#pragma once

#include <iomanip>
#include <memory>
#include <sstream>
#include "config.hpp"
#include "database.hpp"
#include "item.hpp"
#include "user.hpp"
#include "util.hpp"

#include <ftxui/dom/table.hpp>
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

using namespace ftxui;

namespace tui {

void login_interface(Database& db);
void change_password(Database& db, User& user, bool quitable,
					 std::function<void(Database&)> caller);

void employee_interface(Database& db);
void selling_items_interface(Database& db);
void supervisor_interface(Database& db);
void employee_overview(Database& db);
void create_employee_interface(Database& db);
void items_overview(Database& db);
void report_interface(Database& db);
void create_item_interface(Database& db);
}; // namespace tui
