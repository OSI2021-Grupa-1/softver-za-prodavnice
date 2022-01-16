#include "softver-za-prodavnice/tui.hpp"
#include <cstddef>
#include <cstdint>
#include <cstdlib>
#include <string>
#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"
#include "softver-za-prodavnice/item.hpp"
#include "softver-za-prodavnice/util.hpp"

using namespace ftxui;

namespace {
ftxui::Color purple = {93, 39, 93};
ftxui::Color red = {177, 62, 83};
ftxui::Color orange = {239, 125, 87};
ftxui::Color yellow = {255, 205, 117};
ftxui::Color bright_green = {167, 240, 112};
ftxui::Color dark_green = {56, 183, 100};
ftxui::Color teal = {37, 113, 121};
ftxui::Color dark_blue = {41, 54, 111};
ftxui::Color blue = {59, 93, 201};
ftxui::Color light_blue = {65, 166, 246};
ftxui::Color cyan = {115, 239, 247};
ftxui::Color white = {244, 244, 244};
ftxui::Color light_gray = {148, 176, 194};
ftxui::Color gray = {86, 108, 134};
ftxui::Color dark_gray = {51, 60, 87};
} // namespace

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
	auto quit_app = screen.ExitLoopClosure();
	auto log_in_button = ftxui::Button("PRIJAVI SE", [&] {
		if (db.find_user(correct_name) >= 0) {
			if (db.is_password_correct(correct_name, correct_password)) {
				welcome_message = "USPJESNA PRIJAVA";

				// welcome_message = db.get_user_data()[db.find_user(correct_name)].get_position();
				std::vector<User> temp_usr_data = db.get_user_data();
				temp_usr_data[db.find_user(correct_name)].increase_num_logins();
				User current_user = db.get_user_data()[db.find_user(correct_name)];
				db.set_current_user(current_user);
				db.set_user_data(temp_usr_data);
				db.write_users_to_file(db.get_paths().get_path(
					"korisnici")); // ne radi, ali potrebno sacekati da se implementuje ispravno
								   // citanje podataka iz fajlova

				if (current_user.get_number_of_logins() <= 30) {
					if (current_user.get_position() == "sef") {
						supervisor_interface(db);
					} else {
						employee_interface(db);
					}
				} else {

					change_password(db, current_user, true, login_interface);
				}
			}
		}
	});
	// Exit ne radi za linux, samo zaledi terminal
	auto exit_button = ftxui::Button("IZLAZ", [&] { exit(0); });

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

void tui::change_password(Database& db, User& user, bool quitable,
						  std::function<void(Database&)> caller) {
	using namespace ftxui;
	std::string new_password{};		   // prvi unos, koji se resetuje pri svakom novom upisu
	std::string entered_password{};	   // unos koji cuva stanje new_password i koji ce se slati
									   // funkciji za promjenu lozinke
	std::string confirmed_password{};  // lozinka koja se unosi radi sigurnosne provjere/potvrde
	std::string entered_confimation{}; // cuva se potvrda

	std::string welcome_message = "PROMJENA LOZINKE";

	ftxui::InputOption password_option;
	password_option.password = true;
	ftxui::Component password_input = ftxui::Input(&new_password, "nova lozinka", password_option);
	ftxui::Component password_confirmation =
		ftxui::Input(&confirmed_password, "potvrdite lozinku", password_option);

	auto screen = ftxui::ScreenInteractive::TerminalOutput();
	// treba proivjeriti da li se treba vratiti na prethodni ekran ili nastaviti dalje, to se moze
	// preko onog bool, treba resetovati broj prijava
	auto confirm_button = ftxui::Button("PRIHVATI", [&] {
		if (db.is_password_valid(entered_password)) {
			if (entered_password == entered_confimation) {
				welcome_message = "LOZINKA USPJESNO PROMJENJENA";

				db.change_password(user.get_username(), entered_password);
				db.reset_attempts(user.get_username());

				db.write_users_to_file(db.get_paths().get_path("korisnici"));

				if (db.get_current_user().get_position() == "sef") {
					supervisor_interface(db);
				} else {
					employee_interface(db);
				}
			}
		} else {
			welcome_message = "UNESENA LOZINKA NIJE VALIDNA";
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
				   {center(bold(ftxui::text(welcome_message)) | vcenter | size(HEIGHT, EQUAL, 5) |
						   ftxui::color(white)) |
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
	auto backup_button = ftxui::Button("SIGURNOSNA KOPIJA", [&] { db.backup(); });
	auto report_button = ftxui::Button("IZVJESTAJ", [&] { report_interface(db); });

	auto logout_button = ftxui::Button("ODJAVA", [&] { login_interface(db); });

	auto component = ftxui::Container::Vertical(
		{items_button, report_button, employee_button, backup_button, logout_button});

	auto renderer = ftxui::Renderer(component, [&] {
		return ftxui::vbox({center(bold(ftxui::text(db.get_current_user().get_username())) |
								   vcenter | size(HEIGHT, EQUAL, 5) | ftxui::color(white)) |
								borderHeavy | size(WIDTH, EQUAL, 100),
							center(ftxui::vbox({center(items_button->Render()) |
													size(WIDTH, EQUAL, 100) | ftxui::color(yellow),
												center(report_button->Render()) |
													size(WIDTH, EQUAL, 100) | ftxui::color(yellow),
												center(employee_button->Render()) |
													size(WIDTH, EQUAL, 100) | ftxui::color(yellow),
												center(backup_button->Render()) |
													size(WIDTH, EQUAL, 100) | ftxui::color(yellow),
												center(logout_button->Render()) |
													size(WIDTH, EQUAL, 100) | ftxui::color(red)}) |
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

	int depth = 0; // 1 za nema na stanju, 2 za neispravan unos kolicine

	auto add_item_button = Button("DODAJ ARTIKAL NA RACUN", [&] {
		bool item_available = false;
		try {
			d_quantity = std::stod(quantity);
			item_available = db.check_item_availability(list[selected][0], d_quantity);
		} catch (const std::length_error& exc) {
			depth = 1;
		} catch (const std::invalid_argument& exc) {
			depth = 2;
		}
		if (item_available) {
			db.update_quantity_by_id(items_copy, list[selected][0], -d_quantity);
			Item new_it = db.find_item_by_barcode(list[selected][0]);
			bool update = false;
			for (auto& [item, quantity] : sold_items) {
				if (item.get_barcode() == list[selected][0]) {
					item.set_quantity(item.get_quantity() + d_quantity);
					update = true;
				}
			}
			if (!update) {
				sold_items.push_back(std::make_pair(new_it, d_quantity));
			}
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

	auto depth_0_container = Container::Vertical(
		{radiobox, quantity_input, add_item_button, generate_receipt_button, cancel_button});

	auto depth_0_renderer = Renderer(depth_0_container, [&] {
		return ftxui::vbox({
				   hbox(center(text(db.get_current_user().get_username()))),
				   separator(),
				   hbox(center(text("     SIFRA   ARTIKAL              CIJENA"))),
				   radiobox->Render() | vscroll_indicator | frame | size(HEIGHT, LESS_THAN, 20) |
					   border,
				   center(hbox(ftxui::text(" Kolicina artikla:  ") | size(WIDTH, LESS_THAN, 20),
							   quantity_input->Render() | ftxui::color(light_gray))) |
					   ftxui::borderRounded | vcenter | size(WIDTH, EQUAL, 50),
				   center(hbox(center(add_item_button->Render()))),
				   center(hbox(center(generate_receipt_button->Render()))),
				   center(hbox(center(cancel_button->Render()))),

			   }) |
			   center | borderHeavy;
	});

	auto on_agree = [&]() { depth = 0; };

	auto depth_1_container = Container::Horizontal({Button("U REDU", [&] { on_agree(); })});
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
		{depth_0_renderer, depth_1_renderer, depth_2_renderer, depth_3_renderer}, &depth);
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

void tui::employee_overview(Database& db) {
	std::vector<User> selected_users{};
	std::vector<User> user_data = db.get_user_data();
	std::size_t number_of_workers = db.get_user_data().size();
	struct CheckboxState {
		bool checked;
	};
	bool password_editable = false;
	auto create_button = ftxui::Button("NOVI NALOG", [&] { create_employee_interface(db); });
	auto edit_button = ftxui::Button("PROMJENA LOZINKE", [&] {
		change_password(db, selected_users[0], true, employee_overview);
	});

	auto delete_button = ftxui::Button("IZBRISI NALOGE", [&] {
		bool should_logout = false;
		User boss{};
		// ukoliko postoji samo jedan sef, on se mora ukloniti iz vektora za brisanje jer baza
		// podataka u svakom trenutku mora imati bar jednog sefa
		if (db.number_of_bosses() == 1) { // brise sefa iz koleckije za brisanje
			auto temp = db.get_user_data();
			for (auto& user : temp) {
				if (user.get_position() == "sef") {
					boss = user;
				}
			}
			auto it = std::find(selected_users.begin(), selected_users.end(), boss);
			if (it != selected_users.end()) {

				selected_users.erase(it);
			}
		}
		db.delete_users(selected_users); // izbrise korisnike
		auto it = std::find(selected_users.begin(), selected_users.end(), db.get_current_user());
		if (it != selected_users.end()) {
			login_interface(db); // ako je trenutni korisnik izbrisao sam sebe, vraca na pocetni
		}
		selected_users = {};
	});
	auto back_button = ftxui::Button("NAZAD", [&] { supervisor_interface(db); });

	std::vector<CheckboxState> states(number_of_workers);
	auto items = Container::Vertical({});
	for (int i = 0; i < number_of_workers; ++i) {
		states[i].checked = false;
		items->Add(Checkbox(user_data[i].get_username() + "    " + user_data[i].get_position(),
							&states[i].checked));
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
		if (selected_users.size() == 1) {
			password_editable = true;
		} else {
			password_editable = false;
		}
		return vbox(
			{center(bold(text("PREGLED RADNIKA"))), separator(),
			 items->Render() | vscroll_indicator | frame | size(HEIGHT, LESS_THAN, 40) | border,
			 vbox({center(create_button->Render()) | ftxui::color(yellow),
				   center(delete_button->Render()) | ftxui::color(yellow),
				   center(edit_button->Render()) | ftxui::color(yellow),
				   center(back_button->Render()) | ftxui::color(red)

			 }) | border});
	});

	auto screen = ScreenInteractive::TerminalOutput();
	screen.Loop(renderer);
}

void tui::create_employee_interface(Database& db) {
	using namespace ftxui;
	std::string name{};
	std::string password{};
	std::string password_confirm{};
	// posto input stalno resetuje ove stringove, potrebno ih je cuvati nakon svake provjere kako bi
	// se mogli unijeti u fajl
	std::string correct_name{};
	std::string correct_password{};
	std::string correct_new_password{};

	std::string welcome_message{"SISTEM ZA UPRAVLJANJE PRODAVNICOM"};

	ftxui::Component name_input = ftxui::Input(&name, "korisnicko ime");

	ftxui::InputOption password_option;
	password_option.password = true;
	ftxui::Component password_input = ftxui::Input(&password, "lozinka", password_option);

	ftxui::Component password_confirmation_input =
		ftxui::Input(&password_confirm, "potvrdite lozinku", password_option);
	auto screen = ftxui::ScreenInteractive::TerminalOutput();

	std::vector<std::string> entries = {"Radnik", "Sef"};
	int selected = 0;
	auto user_type = Radiobox(&entries, &selected);

	auto cancel_button = ftxui::Button("ODUSTANI", [&] { employee_overview(db); });
	auto confirm_button = ftxui::Button("SACUVAJ", [&] {
		if (db.find_user(name) == -1) {
			std::vector<User> temp = db.get_user_data();
			std::string position = (selected == 0) ? "radnik" : "sef";
			temp.push_back({correct_name, correct_password, position, 0});
			auto pos = db.find_user("admin");
			db.set_user_data(temp);

			if (pos != -1 && db.number_of_bosses() > 1) {
				std::vector<User> removal;
				removal.push_back(temp[pos]);
				db.delete_users(removal);
			}
			db.write_users_to_file(db.get_paths().get_path("korisnici"));
			employee_overview(db);
		}
	});
	bool valid_account = false;

	auto components =
		Container::Vertical({name_input, password_input, password_confirmation_input, cancel_button,
							 user_type, Maybe(confirm_button, &valid_account)});

	auto renderer = Renderer(components, [&] {
		if (name.size() > 0) {
			correct_name = name;
		}
		if (db.is_password_valid(password)) {
			correct_password = password;
			if (password_confirm == correct_password) {
				valid_account = true;
			} else {
				valid_account = false;
			}
		}
		return vbox(
			{center(bold(text("NOVI NALOG"))), separator(),
			 vbox({center(name_input->Render()) | ftxui::color(yellow), separatorDouble(),
				   center(password_input->Render()) | ftxui::color(yellow), separatorDouble(),
				   center(password_confirmation_input->Render()) | ftxui::color(yellow),
				   separatorDouble(),
				   vbox({center(bold(text("Vrsta naloga: "))) | color(yellow),

						 center(user_type->Render()) | ftxui::color(yellow)}),
				   separatorDouble(),

				   hbox({center(confirm_button->Render()) | size(WIDTH, EQUAL, 100) |
							 ftxui::color(bright_green),
						 center(cancel_button->Render()) | size(WIDTH, EQUAL, 100) |
							 ftxui::color(red)}) |
					   borderRounded

			 }) | border});
	});

	screen.Loop(renderer);
}

void tui::items_overview(Database& db) {
	struct CheckboxState {
		bool checked;
	};

	std::string quantity{};
	double new_quantity{};

	std::vector<Item> selected_items{};
	std::vector<Item> item_data = db.get_item_data();

	std::vector<Item> filter_items{};
	std::vector<Item> items_for_display = item_data;

	size_t number_of_items = items_for_display.size();

	// filter
	std::vector<std::string> filter_options = {"sve", "kolicina veca", "kolicina manja",
											   "cijena veca", "cijena manja"};
	std::string filter_value{}; // posto je string, morace se konvertovati u int kasnije
	ftxui::Component value_input = ftxui::Input(&filter_value, "filter vrijednost");
	ftxui::Component quantity_input = ftxui::Input(&quantity, "nova kolicina");

	int selected = 0;
	int last_selected = 0;
	auto item_filter = Radiobox(&filter_options, &selected);
	auto message = ftxui::text("Filter:");

	bool item_editable = false;
	auto create_button = ftxui::Button("NOVI ARTIKAL", [&] { create_item_interface(db); });
	auto edit_button = ftxui::Button("IZMIJENI KOLICINU", [&] {
		db.change_quantity(std::move(selected_items[0].get_barcode()), new_quantity);
		for (auto& item : items_for_display) {
			if (item.get_barcode() == selected_items[0].get_barcode()) {
				item.set_quantity(new_quantity);
			}
		}
		db.write_items_to_file(db.get_paths().get_path("artikli_na_stanju"));
	});
	auto filter_button = ftxui::Button("primijeni", [&] {
		double comparator{};
		if (filter_value == "") {
			comparator = 0;
		} else if (std::ranges::all_of(filter_value.begin(), filter_value.end(),
									   [](char c) { return isdigit(c) != 0; }) &&
				   std::stod(filter_value) > -1) {
			comparator = std::stod(filter_value);
		}
		switch (selected) {
		case 0: items_for_display = db.get_item_data(); break;
		case 1: items_for_display = db.filter(util::greater_quantity, comparator); break;
		case 2: items_for_display = db.filter(util::lesser_quantity, comparator); break;
		case 3: items_for_display = db.filter(util::greater_price, comparator); break;
		case 4: items_for_display = db.filter(util::lesser_price, comparator); break;
		}
		last_selected = selected;
	});

	std::vector<CheckboxState> states(number_of_items);
	auto items = Container::Vertical({});

	auto delete_button = ftxui::Button("IZBRISI ARTIKLE", [&] {
		db.delete_items(selected_items);
		for (auto item : selected_items) {
			auto it = std::find(items_for_display.begin(), items_for_display.end(), item);
			if (it != items_for_display.end()) {
				items_for_display.erase(it);
			}
		}
		for (auto& st : states) {
			st.checked = false;
		}
		selected_items = {};
	});
	auto back_button = ftxui::Button("NAZAD", [&] { supervisor_interface(db); });

	// legenda
	std::stringstream title_stream;
	title_stream << "  " << std::left << std::setw(11) << " sifra " << std::setw(30) << " naziv "
				 << std::setw(6) << " cijena " << std::setw(6) << " kolicina ";
	auto legend = ftxui::text(title_stream.str());

	for (int i = 0; i < number_of_items; ++i) {
		states[i].checked = false;
		std::stringstream ss;
		ss << "  " << std::left << std::setw(8) << items_for_display[i].get_barcode()
		   << std::setw(21) << items_for_display[i].get_name() << std::setw(6)
		   << items_for_display[i].get_price() << std::setw(6)
		   << items_for_display[i].get_quantity();
		std::string display_string = ss.str();
		items->Add(Checkbox(display_string, &states[i].checked));
	}

	auto filter = Container::Vertical({item_filter, value_input});
	auto components = Container::Vertical({create_button, delete_button, back_button, items, filter,
										   filter_button, Maybe(edit_button, &item_editable),
										   Maybe(quantity_input, &item_editable)});

	auto renderer = Renderer(components, [&] {
		// osvjezava se kolekcija izabranih artikala
		number_of_items = items_for_display.size();
		for (size_t i = 0; i < number_of_items; ++i) {
			auto it = std::find(selected_items.begin(), selected_items.end(), items_for_display[i]);
			if (states[i].checked) {
				if (it == selected_items.end()) {
					selected_items.push_back(items_for_display[i]);
				}
			} else {
				if (it != selected_items.end()) {
					selected_items.erase(it);
				}
			}
		}

		// ponovo se popunjava spisak za prikaz
		items->DetachAllChildren();
		for (int i = 0; i < number_of_items; ++i) {
			std::stringstream ss;
			ss << "  " << std::left << std::setw(11) << items_for_display[i].get_barcode()
			   << std::setw(30) << items_for_display[i].get_name() << std::setw(6)
			   << items_for_display[i].get_price() << std::setw(6)
			   << items_for_display[i].get_quantity();
			std::string display_string = ss.str();
			items->Add(Checkbox(display_string, &states[i].checked));
		}

		// moguce je mijenjati kolicinu samo jednog artikla
		if (selected_items.size() == 1) {
			item_editable = true;
		} else {
			item_editable = false;
		}

		if (item_editable) {
			if (quantity == "") {
				new_quantity = 0;
			} else if (std::ranges::all_of(quantity.begin(), quantity.end(),
										   [](char c) { return isdigit(c) != 0; }) &&
					   std::stod(quantity) > -1) {
				new_quantity = std::stod(quantity);
			}
		}

		return ftxui::vbox(
			{center(bold(text("PREGLED ARTIKALA"))), separator(),
			 ftxui::hbox(
				 {ftxui::vbox({
					  legend,
					  ftxui::separatorHeavy(),
					  items->Render() | vscroll_indicator | frame | border,
				  }),
				  ftxui::vbox({center(bold(text("Filter: "))), separator(), filter->Render(),
							   filter_button->Render() | ftxui::size(HEIGHT, EQUAL, 3)}) |
					  border}),

			 vbox({center(create_button->Render()) | ftxui::color(yellow),
				   center(delete_button->Render()) | ftxui::color(yellow),
				   center(
					   ftxui::hbox({center(edit_button->Render()) | ftxui::color(yellow),
									center(bold(text("  "))), center(quantity_input->Render())}) |
					   border),
				   center(back_button->Render()) | ftxui::color(red)

			 }) | border});
	});

	auto screen = ScreenInteractive::TerminalOutput();

	screen.Loop(renderer);
}

void tui::create_item_interface(Database& db) {

	std::string name;
	Component name_input = Input(&name, "     ");
	std::string barcode;
	Component barcode_input = Input(&barcode, "     ");
	std::string price;
	Component price_input = Input(&price, "     ");
	std::string quantity;
	Component quantity_input = Input(&quantity, "     ");

	double d_price;
	double d_quantity;

	auto cancel_button = Button("ODUSTANI", [&] { items_overview(db); });

	int depth = 0;

	auto create_button = Button("KREIRAJ ARTIKAL", [&] {
		bool barcode_exists = false;
		bool cont = true;
		try {
			d_quantity = std::stod(quantity);
			d_price = std::stod(price);
			barcode_exists = db.is_contained(barcode);
		} catch (const std::invalid_argument) {
			depth = 1; // pogresan unos cijene ili kolicine (sadrzi slovo)
			cont = false;
		}
		if (cont) {
			if (barcode.length() != 8) depth = 2; // duzina barkoda se razlikuje od 8
			else if (barcode_exists)
				depth = 3; // barkod vec postoji
			else {
				std::vector<Item> temp = db.get_item_data();
				Item new_item(barcode, name, d_price, d_quantity);
				temp.push_back(new_item);

				db.set_item_data(temp);
				db.write_items_to_file(db.get_paths().get_path("artikli_na_stanju"));
				items_overview(db);
			}
		}
	});

	auto component = Container::Vertical(
		{name_input, barcode_input, price_input, quantity_input, cancel_button, create_button});

	auto depth_0_renderer = Renderer(component, [&] {
		return vbox(
			{vbox({center(hbox(ftxui::text("Naziv artikla: "), name_input->Render()) |
						  size(WIDTH, LESS_THAN, 80) | color(blue)) |
					   size(HEIGHT, EQUAL, 3) | vcenter,
				   separatorDouble(),
				   center(hbox(ftxui::text("Sifra artikla: "), barcode_input->Render()) |
						  size(WIDTH, LESS_THAN, 80) | color(blue)) |
					   size(HEIGHT, EQUAL, 3) | vcenter,
				   separatorDouble(),
				   center(hbox(ftxui::text("Cijena artikla: "), price_input->Render()) |
						  size(WIDTH, LESS_THAN, 80) | color(blue)) |
					   size(HEIGHT, EQUAL, 3) | vcenter,
				   separatorDouble(),
				   center(hbox(ftxui::text("Kolicina artikla: "), quantity_input->Render()) |
						  size(WIDTH, LESS_THAN, 80) | color(blue)) |
					   size(HEIGHT, EQUAL, 3) | vcenter,
				   separatorDouble(),
				   hbox({center(create_button->Render()) | size(WIDTH, EQUAL, 100) |
							 ftxui::color(bright_green),
						 center(cancel_button->Render()) | size(WIDTH, EQUAL, 100) |
							 ftxui::color(red)}) |
					   borderRounded}) |
			 border | size(WIDTH, EQUAL, 150) | center});
	});

	auto on_agree = [&]() { depth = 0; };

	auto depth_1_container = Container::Horizontal({Button("U REDU", [&] { on_agree(); })});
	auto depth_2_container = Container::Horizontal({Button("U REDU", [&] { on_agree(); })});
	auto depth_3_container = Container::Horizontal({Button("U REDU", [&] { on_agree(); })});

	auto depth_1_renderer = Renderer(depth_1_container, [&] {
		return vbox({
				   text("Nepravilan unos cijene ili kolicine"),
				   separator(),
				   center(hbox(depth_1_container->Render())),
			   }) |
			   border;
	});
	auto depth_2_renderer = Renderer(depth_2_container, [&] {
		return vbox({
				   text("Sifra artikla mora da sadrzi 8 karaktera"),
				   separator(),
				   center(hbox(depth_2_container->Render())),
			   }) |
			   border;
	});
	auto depth_3_renderer = Renderer(depth_3_container, [&] {
		return vbox({
				   text("Sifra artikla vec postoji"),
				   separator(),
				   center(hbox(depth_3_container->Render())),
			   }) |
			   border;
	});

	auto main_container = Container::Tab(
		{depth_0_renderer, depth_1_renderer, depth_2_renderer, depth_3_renderer}, &depth);

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
