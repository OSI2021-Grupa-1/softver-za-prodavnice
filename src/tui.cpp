#include "softver-za-prodavnice/tui.hpp"

void tui::login(Database& db) {
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
			if (db.is_password_correct(correct_name, correct_password)) {
				welcome_message = "USPJESNA PRIJAVA";
				User current_user = db.get_user_data()[db.find_user(correct_name)];
				db.set_current_user(current_user);
				// welcome_message = db.get_user_data()[db.find_user(correct_name)].get_position();

				db.get_user_data()[db.find_user(correct_name)].increase_num_logins();
				db.write_users_to_file(db.get_pahts().get_path(
					"korisnici")); // ne radi, ali potrebno sacekati da se implementuje ispravno
								   // citanje podataka iz fajlova
				change_password(db, true, login);
			}
		}
	});

	auto exit_button = ftxui::Button("IZLAZ", [] { exit(0); });

	auto component =
		ftxui::Container::Vertical({name_input, password_input, log_in_button, exit_button});

	auto renderer = ftxui::Renderer(component, [&] {
		// korisnicko ime ce postati zeleno, ako je validno
		// bilo bi dobro i ostale boje ovako definisati, da ne budu samo magicni brojevi
		ftxui::Color input_color = light_gray;
		if (db.find_user(name) >= 0) {
			correct_name = name; //mozada potrebno pozivati izvan uslova
			input_color = {bright_green};
			// lozinku je dovoljno provjeravati samo u slucaju da je korisnicko ime tacno
			correct_password = password;
		}
		return ftxui::vbox({center(bold(ftxui::text(welcome_message)) | vcenter |
								   size(HEIGHT, EQUAL, 5) | ftxui::color(white)) |
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
										center(center(exit_button->Render())) |
											size(WIDTH, EQUAL, 12) | ftxui::color(red)))}) |
			   hcenter | color(light_gray);
	});

	screen.Loop(renderer);
}

void tui::change_password(Database& db, bool quitable, std::function<void(Database&)> caller) {
	using namespace ftxui;
	std::string new_password{};		   // prvi unos, koji se resetuje pri svakom novom upisu
	std::string entered_password{};	   // unos koji cuva stanje new_password i koji ce se slati
									   // funkciji za promjenu lozinke
	std::string confirmed_password{};  // lozinka koja se unosi radi sigurnosne provjere/potvrde
	std::string entered_confimation{}; // cuva se potvrda

	ftxui::InputOption password_option;
	password_option.password = true;
	ftxui::Component password_input = ftxui::Input(&new_password, "nova lozinka", password_option);
	ftxui::Component password_confirmation =
		ftxui::Input(&confirmed_password, "potvrdite lozinku", password_option);

	auto screen = ftxui::ScreenInteractive::TerminalOutput();

	auto confirm_button = ftxui::Button("PRIHVATI", [&] {
		if (db.is_password_valid(entered_password)) {
			if (entered_password == entered_confimation) {
				db.change_password(db.get_current_user().get_username(), entered_password);
				caller(db);
			}
		}
	});
	auto cancel_button = ftxui::Button("ODUSTANI", [&] {
		if (quitable) {
			caller(db);
		}
	});

	auto component =
		ftxui::Container::Vertical({password_input, password_confirmation, confirm_button, ftxui::Maybe( cancel_button, &quitable)});

	auto renderer = ftxui::Renderer(component, [&] {
		ftxui::Color input_color = light_gray;

		entered_password = new_password;
		confirmed_password = confirmed_password;
		
		if (db.is_password_valid(entered_password) && entered_password == confirmed_password) {
			input_color = bright_green;
			}

		return ftxui::vbox({center(bold(ftxui::text("PROMJENA LOZINKE")) | vcenter |
								   size(HEIGHT, EQUAL, 5) | ftxui::color(white)) |
								borderHeavy | size(WIDTH, EQUAL, 150),

							ftxui::vbox({center(ftxui::hbox(
								ftxui::text(""), password_input->Render() | size(WIDTH, LESS_THAN, 80) |
													 ftxui::color(light_gray)))}) |
								ftxui::borderRounded,
							center(hbox(ftxui::text(""), password_confirmation->Render() |
															 size(WIDTH, LESS_THAN, 80) |
															 ftxui::color(input_color))) |
								ftxui::borderRounded | vcenter,
							center(hbox(center(confirm_button->Render()) | size(WIDTH, EQUAL, 12) |
											ftxui::color(bright_green),
										center(center(cancel_button->Render())) |
											size(WIDTH, EQUAL, 12) | ftxui::color(red)))}) |
			   hcenter | color(light_gray);
	});

	screen.Loop(renderer);

}
