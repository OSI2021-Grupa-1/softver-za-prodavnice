#include "softver-za-prodavnice/tui.hpp"

void tui::login_interface(Database& db) {
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
				std::vector<User> temp_usr_data = db.get_user_data();
				temp_usr_data[db.find_user(correct_name)].increase_num_logins();
				db.set_user_data(temp_usr_data);
				db.write_users_to_file(db.get_pahts().get_path(
					"korisnici")); // ne radi, ali potrebno sacekati da se implementuje ispravno
								   // citanje podataka iz fajlova

				if (current_user.get_number_of_logins() <= 30) {
					if (current_user.get_position() == "sef") {
						supervisor_interface(db);
					} else {
						employee_interface(db);
					}
				} else {

					change_password(db,current_user, true, login_interface);
				}
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
			correct_name = name; // mozada potrebno pozivati izvan uslova
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

void tui::change_password(Database& db,User &user, bool quitable, std::function<void(Database&)> caller) {
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
				db.change_password(user.get_username(), entered_password);
				db.write_users_to_file(db.get_pahts().get_path("korisnici"));
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
		ftxui::Container::Vertical({password_input, password_confirmation, confirm_button,
									ftxui::Maybe(cancel_button, &quitable)});

	auto renderer = ftxui::Renderer(component, [&] {
		ftxui::Color input_color = light_gray;

		entered_password = new_password;
		entered_confimation = confirmed_password;

		if (db.is_password_valid(entered_password) && entered_password == entered_confimation) {
			input_color = bright_green;
		}

		return ftxui::vbox(
				   {center(bold(ftxui::text("PROMJENA LOZINKE")) | vcenter |
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
								center(center(cancel_button->Render())) | size(WIDTH, EQUAL, 12) |
									ftxui::color(red)))}) |
			   hcenter | color(light_gray);
	});

	screen.Loop(renderer);
}

void tui::supervisor_interface(Database& db) {
	using namespace ftxui;
	auto screen = ftxui::ScreenInteractive::TerminalOutput();

	auto employee_button = ftxui::Button("PREGLED RADNIKA", [&] { employee_overview(db); });
	auto items_button = ftxui::Button("PREGLED ARTIKALA", [&] { items_overview(db); });
	auto backup_button = ftxui::Button("SIGURNOSNA KOPIJA", [&] { create_backup(db); });
	auto report_button = ftxui::Button("IZVJESTAJ", [&] { report_interface(db); });

	auto logout_button = ftxui::Button("ODJAVA", [&] { login_interface(db); });

	auto component = ftxui::Container::Vertical(
		{items_button, report_button, employee_button, backup_button, logout_button});

	auto renderer = ftxui::Renderer(component, [&] {
		return ftxui::vbox({center(bold(ftxui::text(db.get_current_user().get_username())) |
								   vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(white)) |
								borderHeavy | size(WIDTH, EQUAL, 100),
							center(ftxui::vbox({center(items_button->Render()) |
													size(WIDTH, EQUAL, 100) | ftxui::color(blue),
												center(report_button->Render()) |
													size(WIDTH, EQUAL, 100) | ftxui::color(blue),
												center(employee_button->Render()) |
													size(WIDTH, EQUAL, 100) | ftxui::color(blue),
												center(backup_button->Render()) |
													size(WIDTH, EQUAL, 100) | ftxui::color(blue),
												center(logout_button->Render()) |
													size(WIDTH, EQUAL, 50) | ftxui::color(red)}) |
								   borderRounded | size(WIDTH, EQUAL, 100) | center)}) |
			   hcenter | color(light_gray);
	});
	screen.Loop(renderer);
}

void tui::employee_interface(Database& db) {
	auto log_out_button = Button("ODJAVA", [&db] { tui::login_interface(db); });
	auto sell_items_button = Button("PRODAJA ARTIKALA", [&db] {
		selling_items_interface(db);
	}); // funkcija za prodaju izaziva mutex error
	auto logged_in_user = center(bold(text(db.get_current_user().get_username())));

	auto screen = ScreenInteractive::TerminalOutput();

	auto component = Container::Vertical({log_out_button, sell_items_button});

	auto renderer = ftxui::Renderer(component, [&] {
		return vbox({
				   vbox({
					   center(bold(text(db.get_current_user().get_username()))),
					   separator(),
					   vbox({center(hbox(center(sell_items_button->Render())))}) |
						   size(HEIGHT, EQUAL, 3),
					   vbox({
						   center(hbox(center(log_out_button->Render())) | color(red)),
					   }) | center |
						   size(HEIGHT, EQUAL, 3),
				   }) | size(WIDTH, EQUAL, 150) |
					   borderHeavy | vcenter | hcenter,
			   }) |
			   vcenter;
	});

	screen.Loop(renderer);
}

void tui::selling_items_interface(Database& db) {
	struct CheckboxState {
		bool checked;
	};

	std::vector<std::pair<Item, double>> sold_items;
	std::vector<CheckboxState> states(db.get_item_data().size());
	auto container = Container::Vertical({});
	auto screen = ScreenInteractive::TerminalOutput();

	for (int i = 0; db.get_item_data().size(); ++i) {
		states[i].checked = false;
		container->Add(Checkbox(db.get_item_data()[i].get_name(), &states[i].checked));
	}

	auto renderer = Renderer(container, [&] {
		return container->Render() | vscroll_indicator | frame | size(HEIGHT, LESS_THAN, 100) |
			   size(WIDTH, EQUAL, 150) | center | border;
	});
}

void tui::employee_overview(Database& db) {
	std::vector<User> selected_users{};
	std::vector<User> user_data = db.get_user_data();
	std::size_t number_of_workers = db.get_user_data().size();
	struct CheckboxState {
		bool checked;
	};
	bool password_editable = false;
	auto create_button = ftxui::Button("NOVI NALOG", [&] { employee_overview(db); });
	auto edit_button = ftxui::Button("PROMJENA LOZINKE", [&] {
		change_password(db, selected_users[0], true, employee_overview);
	});

	auto delete_button = ftxui::Button("IZBRISI NALOGE", [&] { db.delete_users(selected_users); });
	auto back_button = ftxui::Button("NAZAD", [&] { supervisor_interface(db); });

	std::vector<CheckboxState> states(number_of_workers);
	auto items = Container::Vertical({});
	for (int i = 0; i < number_of_workers; ++i) {
		states[i].checked = false;
		items->Add(Checkbox(user_data[i].get_username(), &states[i].checked));
	}

	auto components = Container::Vertical(
		{create_button, delete_button, back_button, items, Maybe(edit_button, &password_editable)});

	auto renderer = Renderer(components, [&] {
		user_data = db.get_user_data();
		number_of_workers = db.get_user_data().size();
		for (size_t i = 0; i < number_of_workers; ++i) {
			if (states[i].checked) {
				auto it = std::find(selected_users.begin(), selected_users.end(), user_data[i]);
				if (it == selected_users.end()) {
					selected_users.push_back(user_data[i]);
				}
			} else {
				auto it = std::find(selected_users.begin(), selected_users.end(), user_data[i]);
				if (it != selected_users.end()) {
					selected_users.erase(it);
				}
			}
		}

		items->DetachAllChildren();
		for (int i = 0; i < number_of_workers; ++i) {
			// states[i].checked = false;
			items->Add(Checkbox(user_data[i].get_username(), &states[i].checked));
		}
		if (selected_users.size()) {
			password_editable = true;
		} else {
			password_editable = false;
		}
		return vbox(
			{center(bold(text("SPISAK RADNIKA"))), separator(),
			 items->Render() | vscroll_indicator | frame | size(HEIGHT, LESS_THAN, 40) | border,
			 vbox({center(create_button->Render()) | size(WIDTH, EQUAL, 100) | ftxui::color(blue),
				   center(delete_button->Render()) | size(WIDTH, EQUAL, 100) | ftxui::color(blue),
				   center(edit_button->Render()) | size(WIDTH, EQUAL, 100) | ftxui::color(blue),
				   center(back_button->Render()) | size(WIDTH, EQUAL, 100) | ftxui::color(red)

			 }) | border});
	});

	auto screen = ScreenInteractive::TerminalOutput();
	screen.Loop(renderer);
}
void tui::items_overview(Database& db) {}
void tui::create_backup(Database& db) {}
void tui::report_interface(Database& db) {}
