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

					change_password(db, true, login_interface);
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

	std::vector<std::string> entries;
	int selected = 0;
	auto list = db.items_table();
	auto items_copy = db.get_item_data();


	for (int i = 0; i < db.get_item_data().size(); ++i) {
		std::stringstream str;
		str << "  " << std::left << std::setw(8) << list[i][0] << std::setw(21) << list[i][1]
			<< std::setw(6) << list[i][2];
		std::string inp = str.str();
		entries.push_back(inp);
	}
	auto radiobox = Menu(&entries, &selected);

	std::string quantity;
	ftxui::Component quantity_input = ftxui::Input(&quantity, "     ");

	double d_quantity = 0;
	std::vector<std::pair<Item, double>> sold_items;

	int depth = 0; //1 za nema na stanju, 2 za neispravan unos kolicine

	auto add_item_button = Button("DODAJ ARTIKAL NA RACUN", [&] {
		bool item_available = false;
		try {
			d_quantity = std::stod(quantity);
			item_available = db.check_item_availability(list[selected][0], d_quantity);
		} catch (const std::length_error& exc){
			depth = 1;
		} catch (const std::invalid_argument& exc) {
			depth = 2;
		}
		if (item_available) {
			db.update_quantity_by_id(items_copy, list[selected][0], -d_quantity);
			Item new_it = db.find_item_by_barcode(list[selected][0]);
			int i;
			for (i = 0; i < sold_items.size(); i++) {
				auto item = sold_items[i].first;
				if (item == new_it) break;
			}
			if (i == sold_items.size()) sold_items.push_back(std::make_pair(new_it, d_quantity));
			else
				sold_items[i].first.set_quantity(sold_items[i].first.get_quantity() + d_quantity);
		}
	});

	auto generate_receipt_button = Button("GENERISI RACUN", [&] {
		if (sold_items.size() == 0) {
			depth = 3;
		} else {
			db.set_item_data(items_copy);
			std::string current_time = db.current_date_time();
			db.generate_receipt(sold_items, current_time);

			std::vector<Item> for_another_func;
			for (auto item : sold_items) {
				item.first.set_quantity(item.second);
				for_another_func.push_back(item.first);
			}
			std::string date = current_time.substr(0, 10);
			std::replace(date.begin(), date.end(), '.', '/');
			db.write_sold_items_to_file(for_another_func, date);
			tui::employee_interface(db);
		}
	});
	auto cancel_button = ftxui::Button("ODUSTANI", [&] { employee_interface(db); });



	auto depth_0_container = Container::Vertical({radiobox, quantity_input, add_item_button, generate_receipt_button,
							 cancel_button});

	auto depth_0_renderer = Renderer(depth_0_container, [&] {
		//std::string inp;
		//for (int i = 0; i < sold_items.size(); i++) {
		//	auto item = sold_items[i];
		//	std::stringstream str;
		//	str << " " << std::left << std::setw(8) << item.first.get_barcode() << std::setw(21)
		//		<< item.first.get_name() << std::setw(9) << item.first.get_price() << std::setw(6)
		//		<< item.second << '\n';
		//	inp += str.str();
		//}
		return ftxui::vbox({
			hbox(center(text(db.get_current_user().get_username()))), separator(),
				// 0123456789012345678901234567890123456
				hbox(center(text("     SIFRA   ARTIKAL              CIJENA"))),
				radiobox->Render() | vscroll_indicator | frame | size(HEIGHT, LESS_THAN, 20) |
					border,
				center(hbox(ftxui::text(" Kolicina artikla:  ") | size(WIDTH, LESS_THAN, 20),
							quantity_input->Render() | ftxui::color(light_gray))) |
					ftxui::borderRounded | vcenter | size(WIDTH, EQUAL, 50),
				center(hbox(center(add_item_button->Render()))),
				center(hbox(center(generate_receipt_button->Render()))),
				center(hbox(center(cancel_button->Render()))),

				}) | center | borderHeavy;
	});


	auto on_agree = [&]() {
		depth = 0;
	};

	auto depth_1_container = Container::Horizontal({ Button("U REDU", [&]{on_agree();})});
	auto depth_2_container = Container::Horizontal({Button("U REDU", [&] { on_agree(); })});
	auto depth_3_container = Container::Horizontal({Button("U REDU", [&] { on_agree(); })});

	auto depth_1_renderer = Renderer(depth_1_container, [&] {
		return vbox({
				   text("Nedovoljno artikala na stanju"),
				   separator(),
				   center(hbox(depth_1_container->Render())),
			   }) |
			   border;
	});
	auto depth_2_renderer = Renderer(depth_2_container, [&] {
		return vbox({
				   text("Nepravilan unos kolicine"),
				   separator(),
				   center(hbox(depth_2_container->Render())),
			   }) |
			   border;
	});
	auto depth_3_renderer = Renderer(depth_3_container, [&] {
		return vbox({
				   text("Za generisanje racuna mora se dodati bar jedan artikal"),
				   separator(),
				   center(hbox(depth_3_container->Render())),
			   }) |
			   border;
	});

	  auto main_container = Container::Tab(
		{
			depth_0_renderer,
			depth_1_renderer,
			depth_2_renderer, 
			depth_3_renderer
		},
		&depth);
	auto main_renderer = Renderer(main_container, [&] {
		Element document = depth_0_renderer->Render();

		if (depth == 1) {
			document = dbox({
				document,
				depth_1_renderer->Render() | clear_under | center,
			});
		} else if (depth == 2) {
			document = dbox({
				document,
				depth_2_renderer->Render() | clear_under | center,
			});
		} else if (depth == 3) {
			document = dbox({
				document,
				depth_3_renderer->Render() | clear_under | center,
			});
		}
		return document;
	});

	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	screen.Loop(main_renderer);
}

void tui::employee_overview(Database& db) { std::vector<User> selected_users{};


	}
void tui::items_overview(Database& db) {}
void tui::create_backup(Database& db) {}
void tui::report_interface(Database& db) {}
