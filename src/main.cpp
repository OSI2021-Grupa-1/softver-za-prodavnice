#include <memory>
#include <string>

#include "ftxui/component/captured_mouse.hpp"	  // for ftxui
#include "ftxui/component/component.hpp"		  // for Input, Renderer, Vertical
#include "ftxui/component/component_base.hpp"	  // for ComponentBase
#include "ftxui/component/component_options.hpp"  // for InputOption
#include "ftxui/component/screen_interactive.hpp" // for Component, ScreenInteractive
#include "ftxui/dom/elements.hpp" // for text, hbox, separator, Element, operator|, vbox, border
#include "ftxui/util/ref.hpp"	  // for ref

#include "softver-za-prodavnice/user.hpp"

int main() {
	std::string name{};
	std::string password{};

	ftxui::Component name_input = ftxui::Input(&name, "Name");

	ftxui::InputOption password_option;
	password_option.password = true;
	ftxui::Component password_input = ftxui::Input(&password, "password", password_option);

	auto component = ftxui::Container::Vertical({
		name_input,
		password_input,
	});

	auto renderer = ftxui::Renderer(component, [&] {
		return ftxui::vbox({
				   ftxui::text("Hello " + name + '!'),
				   ftxui::separator(),
				   ftxui::hbox(ftxui::text(" Name : "), name_input->Render()),
				   hbox(ftxui::text(" Password   : "), password_input->Render()),
			   }) |
			   ftxui::border;
	});

	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	screen.Loop(renderer);
}
