#include "softver-za-prodavnice/tui.hpp"

void tui::login() {
	using namespace ftxui;
	std::string name{};
	std::string password{};

	ftxui::Component name_input = ftxui::Input(&name, "korisnicko ime");

	ftxui::InputOption password_option;
	password_option.password = true;
	ftxui::Component password_input = ftxui::Input(&password, "lozinka", password_option);

	auto screen = ftxui::ScreenInteractive::TerminalOutput();

	auto log_in_button =
		ftxui::Button("PRIJAVI SE", [] { /*provjera podataka i pozivanje metode za odlazak na
							   intefejs radnika/sefa nakon detekcije vrste naloga*/
		});

	auto exit_button = ftxui::Button("IZLAZ", [] { exit(0); });

	auto component =
		ftxui::Container::Vertical({name_input, password_input, log_in_button, exit_button});

	auto renderer = ftxui::Renderer(component, [&] {
		return ftxui::vbox(
				   {center(bold(ftxui::text("SISTEM ZA UPRAVLJANJE PRODAVNICOM")) | vcenter |
						   size(HEIGHT, EQUAL, 5) | ftxui::color(ftxui::Color(244, 244, 244))) |
						borderHeavy | size(WIDTH, EQUAL, 150),

					ftxui::vbox({center(ftxui::hbox(
						ftxui::text(""), name_input->Render() | size(WIDTH, LESS_THAN, 80) |
											 ftxui::color(ftxui::Color(86, 108, 134))))}) |
						ftxui::borderRounded,
					center(hbox(ftxui::text(""), password_input->Render() |
													 size(WIDTH, LESS_THAN, 80) |
													 ftxui::color(ftxui::Color(86, 108, 134)))) |
						ftxui::borderRounded | vcenter,
					center(hbox(center(log_in_button->Render()) | size(WIDTH, EQUAL, 12) |
									ftxui::color(ftxui::Color(167, 240, 112)),
								center(center(exit_button->Render())) | size(WIDTH, EQUAL, 12) |
									ftxui::color(ftxui::Color(177, 62, 83))))}) |
			   hcenter | color(ftxui::Color(148, 176, 194));
	});

	screen.Loop(renderer);
}
