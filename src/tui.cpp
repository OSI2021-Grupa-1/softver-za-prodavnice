#include "softver-za-prodavnice/tui.hpp"

void tui::login(const Database& db) {
	using namespace ftxui;
	std::string name{};
	std::string password{};
	// posto input stalno resetuje ove stringove, potrebno ih je cuvati nakon svake provjere kako bi
	// se mogli unijeti u fajl
	std::string correct_name{};
	std::string correct_password{};

	std::string welcome_message{"SISTEM ZA UPRAVLJANJE PRODAVNICOM"};

	ftxui::Component name_input = ftxui::Input(&name, "korisnicko ime");

	ftxui::InputOption password_option;
	password_option.password = true;
	ftxui::Component password_input = ftxui::Input(&password, "lozinka", password_option);

	auto screen = ftxui::ScreenInteractive::TerminalOutput();

	auto log_in_button = ftxui::Button("PRIJAVI SE", [&] {
		if (db.find_user(correct_name) >= 0) {
			welcome_message = "ISPRAVNO IME";

			if (db.is_password_correct(correct_name, correct_password)) {
				welcome_message = "USPJESNA PRIJAVA";
			}
		}
	});

	auto exit_button = ftxui::Button("IZLAZ", [] { exit(0); });

	auto component =
		ftxui::Container::Vertical({name_input, password_input, log_in_button, exit_button});

	auto renderer = ftxui::Renderer(component, [&] {
		// korisnicko ime ce postati zeleno, ako je validno
		//bilo bi dobro i ostale boje ovako definisati, da ne budu samo magicni brojevi
		ftxui::Color input_color = light_gray;
		if (db.find_user(name) >= 0) {
			correct_name = name;
			input_color = {bright_green};
			// lozinku je dovoljno provjeravati samo u slucaju da je korisnicko ime tacno
			correct_password = password;
		}
		return ftxui::vbox(
				   {center(bold(ftxui::text(welcome_message)) | vcenter | size(HEIGHT, EQUAL, 5) |
						   ftxui::color(white)) |
						borderHeavy | size(WIDTH, EQUAL, 150),

					ftxui::vbox({center(ftxui::hbox(
						ftxui::text(""), name_input->Render() | size(WIDTH, LESS_THAN, 80) |
											 ftxui::color(input_color)))}) |
						ftxui::borderRounded,
					center(hbox(ftxui::text(""), password_input->Render() |
													 size(WIDTH, LESS_THAN, 80) |
													 ftxui::color(light_gray))) |
						ftxui::borderRounded | vcenter,
					center(hbox(center(log_in_button->Render()) | size(WIDTH, EQUAL, 12) |
									ftxui::color(bright_green),
								center(center(exit_button->Render())) | size(WIDTH, EQUAL, 12) |
									ftxui::color(red)))}) |
			   hcenter | color(light_gray);
	});

	screen.Loop(renderer);
}
