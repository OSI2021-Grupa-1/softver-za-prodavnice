#include <cstddef>
#include <cstdint>
#include <string>
#include "ftxui/component/component.hpp"
#include "ftxui/dom/elements.hpp"
#include "softver-za-prodavnice/item.hpp"
#include "softver-za-prodavnice/tui.hpp"
#include "softver-za-prodavnice/util.hpp"

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

void tui::report_interface(Database& db) {

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

	std::string filter_value{};
	std::vector<std::string> filter_options = {"sve", "kolicina veca", "kolicina manja",
											   "cijena veca", "cijena manja"};
	ftxui::Component value_input = ftxui::Input(&filter_value, "filter vrijednost");
	ftxui::Component quantity_input = ftxui::Input(&quantity, "nova kolicina");

	int selected = 0;
	int last_selected = 0;
	auto item_filter = Radiobox(&filter_options, &selected);
	auto message = ftxui::text("Filter:");

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

	std::vector<CheckboxState> states(db.get_item_data().size());

	auto items = Container::Vertical({});
	for (int i = 0; i < db.get_item_data().size(); ++i) {
		states[i].checked = false;
		std::stringstream str;
		str << "  " << std::left << std::setw(10) << db.get_item_data()[i].get_barcode()
			<< std::setw(21) << db.get_item_data()[i].get_name();

		items->Add(Checkbox(str.str(), &states[i].checked));
	}
	auto filter = Container::Vertical({item_filter, value_input});

	auto cancel_button = Button("ODUSTANI", [&] { supervisor_interface(db); });
	auto daily_button = Button("DNEVNI IZVJESTAJ", [&] { daily_report(db, selected_items); });
	auto weekly_button = Button("SEDMICNI IZVJESTAJ", [&] { weekly_report(db, selected_items); });
	auto monthly_button = Button("MJESECNI IZVJESTAJ", [&] { monthly_report(db, selected_items); });
	auto yearly_button = Button("GODISNJI IZVJESTAJ", [&] { yearly_report(db, selected_items); });
	auto arbitrary_button =
		Button("PROIZVOLJNI IZVJESTAJ", [&] { arbitrary_report(db, selected_items); });

	auto component =
		Container::Vertical({cancel_button, daily_button, weekly_button, monthly_button,
							 yearly_button, arbitrary_button, items, items, filter, filter_button});

	auto renderer = Renderer(component, [&] {
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

		items->DetachAllChildren();

		for (int i = 0; i < number_of_items; ++i) {
			std::stringstream ss;
			ss << "  " << std::left << std::setw(10) << items_for_display[i].get_barcode()
			   << std::setw(21) << items_for_display[i].get_name() << std::setw(8)
			   << items_for_display[i].get_price() << std::setw(8)
			   << items_for_display[i].get_quantity();
			std::string display_string = ss.str();
			items->Add(Checkbox(display_string, &states[i].checked));
		}

		return vbox(
			{vbox({center(bold(text("SPISAK ARTIKALA"))), separator(),
				   ftxui::hbox(
					   {ftxui::vbox({
							text("      SIFRA     ARTIKAL              CIJENA  KOLICINA"),
							ftxui::separatorHeavy(),
							items->Render() | vscroll_indicator | frame | border |
								size(HEIGHT, LESS_THAN, 8),
						}),
						ftxui::vbox({center(bold(text("Filter: "))), separator(), filter->Render(),
									 filter_button->Render() | ftxui::size(HEIGHT, EQUAL, 3)}) |
							border}),
				   hbox({center(daily_button->Render()) | size(HEIGHT, EQUAL, 3) | vcenter}) |
					   hcenter,
				   hbox({center(weekly_button->Render()) | size(HEIGHT, EQUAL, 3) | vcenter}) |
					   hcenter,
				   hbox({center(monthly_button->Render()) | size(HEIGHT, EQUAL, 3) | vcenter}) |
					   hcenter,
				   hbox({center(yearly_button->Render()) | size(HEIGHT, EQUAL, 3) | vcenter}) |
					   hcenter,
				   hbox({center(arbitrary_button->Render()) | size(HEIGHT, EQUAL, 3) | vcenter}) |
					   hcenter,
				   hbox({center(cancel_button->Render()) | size(HEIGHT, EQUAL, 3) | color(red) |
						 vcenter}) |
					   hcenter

			 }) |
			 border | size(WIDTH, EQUAL, 150) | vcenter | hcenter});
	});

	auto screen = ftxui::ScreenInteractive::TerminalOutput();

	screen.Loop(renderer);
}

void tui::arbitrary_report(Database& db, const std::vector<Item>& items) {}

void tui::daily_report(Database& db, const std::vector<Item>& items) {
	std::string day;
	Component day_input = Input(&day, "     ");
	std::string month;
	Component month_input = Input(&month, "     ");
	std::string year;
	Component year_input = Input(&year, "     ");

	int i_day, i_month, i_year;
	int depth = 0;

	auto cancel_button = Button("ODUSTANI", [&] { supervisor_interface(db); });
	auto generate_button = Button("GENERISI IZVJESTAJ", [&] {
		bool check = true;
		try {
			if (day.length() > 2 || month.length() > 2 || year.length() > 4) throw "Error";
			i_day = std::stoi(day);
			i_month = std::stoi(month);
			i_year = std::stoi(year);
		} catch (... /* const std::exception& exc */) {
			depth = 1; // nepravilan unos, ne moze se konvertovati u int
			check = false;
		}
		if (check) {
			if (util::is_valid_date(i_day, i_month, i_year, db.current_date_time())) {
				std::string temp = std::to_string(i_year);

				temp += (i_month < 10 ? "0" : "") + std::to_string(i_month) +
						(i_day < 10 ? "0" : "") + std::to_string(i_day);

				int date = std::stoi(temp);
				db.create_report(items, date, date);
				supervisor_interface(db);
			} else
				depth = 2; // nevalidan datum
		}
	});

	auto component =
		Container::Vertical({cancel_button, generate_button, day_input, month_input, year_input});

	auto renderer = Renderer(component, [&] {
		return vbox({vbox({vbox({center(hbox(text("DNEVNI IZVJESTAJ"))),
								 center(hbox(text("Izaberi datum") | underlined))}) |
							   borderLight,
						   center(hbox(ftxui::text("Dan: "), day_input->Render()) |
								  size(WIDTH, LESS_THAN, 80) | color(blue)) |
							   size(HEIGHT, EQUAL, 3) | vcenter,
						   separatorDouble(),
						   center(hbox(ftxui::text("Mjesec: "), month_input->Render()) |
								  size(WIDTH, LESS_THAN, 80) | color(blue)) |
							   size(HEIGHT, EQUAL, 3) | vcenter,
						   separatorDouble(),
						   center(hbox(ftxui::text("Godina: "), year_input->Render()) |
								  size(WIDTH, LESS_THAN, 80) | color(blue)) |
							   size(HEIGHT, EQUAL, 3) | vcenter,
						   separatorDouble(),
						   hbox({center(generate_button->Render()) | size(WIDTH, EQUAL, 100) |
									 ftxui::color(bright_green),
								 center(cancel_button->Render()) | size(WIDTH, EQUAL, 100) |
									 ftxui::color(red)}) |
							   borderRounded

					 }) |
					 border | size(WIDTH, EQUAL, 150) | vcenter | hcenter

		});
	});

	auto on_agree = [&]() { depth = 0; };

	auto depth_1_container = Container::Horizontal({Button("U REDU", [&] { on_agree(); })});
	auto depth_2_container = Container::Horizontal({Button("U REDU", [&] { on_agree(); })});

	auto depth_1_renderer = Renderer(depth_1_container, [&] {
		return vbox({
				   text("Nepravilan unos datuma"),
				   separator(),
				   center(hbox(depth_1_container->Render())),
			   }) |
			   border;
	});
	auto depth_2_renderer = Renderer(depth_2_container, [&] {
		return vbox({
				   text("Uneseni datum ne postoji"),
				   separator(),
				   center(hbox(depth_2_container->Render())),
			   }) |
			   border;
	});

	auto main_container = Container::Tab({renderer, depth_1_renderer, depth_2_renderer}, &depth);
	auto main_renderer = Renderer(main_container, [&] {
		Element document = renderer->Render();

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
		}
		return document;
	});

	auto screen = ftxui::ScreenInteractive::TerminalOutput();

	screen.Loop(main_renderer);
}

void tui::weekly_report(Database& db, const std::vector<Item>& items) {
	std::string day;
	Component day_input = Input(&day, "     ");
	std::string month;
	Component month_input = Input(&month, "     ");
	std::string year;
	Component year_input = Input(&year, "     ");

	int i_day, i_month, i_year;
	int depth = 0;

	auto cancel_button = Button("ODUSTANI", [&] { supervisor_interface(db); });
	auto generate_button = Button("GENERISI IZVJESTAJ", [&] {
		bool check = true;
		try {
			if (day.length() > 2 || month.length() > 2 || year.length() > 4) throw "error";
			i_day = std::stoi(day);
			i_month = std::stoi(month);
			i_year = std::stoi(year);
		} catch (...) {
			depth = 1; // nepravilan unos, ne moze se konvertovati u int
			check = false;
		}
		if (check) {
			if (util::is_valid_date(i_day, i_month, i_year, db.current_date_time())) {
				std::string temp = std::to_string(i_year);

				temp += (i_month < 10 ? "0" : "") + std::to_string(i_month) +
						(i_day < 10 ? "0" : "") + std::to_string(i_day);

				int curr_date = std::stoi(temp);

				int next_date = util::week_increase(i_day, i_month, i_year);
				db.create_report(items, curr_date, next_date);
				supervisor_interface(db);
			} else
				depth = 2; // nevalidan datum
		}
	});

	auto component =
		Container::Vertical({cancel_button, generate_button, day_input, month_input, year_input});

	auto renderer = Renderer(component, [&] {
		return vbox({vbox({vbox({center(hbox(text("SEDMICNI IZVJESTAJ"))),
								 center(hbox(text("Izaberi datum") | underlined))}) |
							   borderLight,
						   center(hbox(ftxui::text("Dan: "), day_input->Render()) |
								  size(WIDTH, LESS_THAN, 80) | color(blue)) |
							   size(HEIGHT, EQUAL, 3) | vcenter,
						   separatorDouble(),
						   center(hbox(ftxui::text("Mjesec: "), month_input->Render()) |
								  size(WIDTH, LESS_THAN, 80) | color(blue)) |
							   size(HEIGHT, EQUAL, 3) | vcenter,
						   separatorDouble(),
						   center(hbox(ftxui::text("Godina: "), year_input->Render()) |
								  size(WIDTH, LESS_THAN, 80) | color(blue)) |
							   size(HEIGHT, EQUAL, 3) | vcenter,
						   separatorDouble(),
						   hbox({center(generate_button->Render()) | size(WIDTH, EQUAL, 100) |
									 ftxui::color(bright_green),
								 center(cancel_button->Render()) | size(WIDTH, EQUAL, 100) |
									 ftxui::color(red)}) |
							   borderRounded

					 }) |
					 border | size(WIDTH, EQUAL, 150) | vcenter | hcenter

		});
	});

	auto on_agree = [&]() { depth = 0; };

	auto depth_1_container = Container::Horizontal({Button("U REDU", [&] { on_agree(); })});
	auto depth_2_container = Container::Horizontal({Button("U REDU", [&] { on_agree(); })});

	auto depth_1_renderer = Renderer(depth_1_container, [&] {
		return vbox({
				   text("Nepravilan unos datuma"),
				   separator(),
				   center(hbox(depth_1_container->Render())),
			   }) |
			   border;
	});
	auto depth_2_renderer = Renderer(depth_2_container, [&] {
		return vbox({
				   text("Uneseni datum ne postoji"),
				   separator(),
				   center(hbox(depth_2_container->Render())),
			   }) |
			   border;
	});

	auto main_container = Container::Tab({renderer, depth_1_renderer, depth_2_renderer}, &depth);
	auto main_renderer = Renderer(main_container, [&] {
		Element document = renderer->Render();

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
		}
		return document;
	});

	auto screen = ftxui::ScreenInteractive::TerminalOutput();

	screen.Loop(main_renderer);
}

void tui::monthly_report(Database& db, const std::vector<Item>& items) {

	std::string month;
	Component month_input = Input(&month, "     ");
	std::string year;
	Component year_input = Input(&year, "     ");

	int i_month, i_year;
	int depth = 0;

	auto cancel_button = Button("ODUSTANI", [&] { supervisor_interface(db); });
	auto generate_button = Button("GENERISI IZVJESTAJ", [&] {
		bool check = true;
		try {
			if (month.length() > 2 || year.length() > 4) throw "error";
			i_month = std::stoi(month);
			i_year = std::stoi(year);
		} catch (const std::exception& exc) {
			depth = 1; // nepravilan unos, ne moze se konvertovati u int
			check = false;
		}
		if (check) {
			if (util::is_valid_date(1, i_month, i_year, db.current_date_time())) {
				std::string temp = std::to_string(i_year) + std::to_string(i_month) + "01";
				int curr_date = std::stoi(temp);

				int last_day = util::number_of_days(i_month, i_year);
				std::string next =
					std::to_string(i_year) + std::to_string(i_month) + std::to_string(last_day);

				int next_date = std::stoi(next);
				db.create_report(items, curr_date, next_date);
				supervisor_interface(db);
			} else
				depth = 2; // nevalidan datum
		}
	});

	auto component = Container::Vertical({cancel_button, generate_button, month_input, year_input});

	auto renderer = Renderer(component, [&] {
		return vbox({vbox({vbox({center(hbox(text("MJESECNI IZVJESTAJ"))),
								 center(hbox(text("Izaberi mjesec i godinu") | underlined))}) |
							   borderLight,

						   center(hbox(ftxui::text("Mjesec: "), month_input->Render()) |
								  size(WIDTH, LESS_THAN, 80) | color(blue)) |
							   size(HEIGHT, EQUAL, 3) | vcenter,
						   separatorDouble(),
						   center(hbox(ftxui::text("Godina: "), year_input->Render()) |
								  size(WIDTH, LESS_THAN, 80) | color(blue)) |
							   size(HEIGHT, EQUAL, 3) | vcenter,
						   separatorDouble(),
						   hbox({center(generate_button->Render()) | size(WIDTH, EQUAL, 100) |
									 ftxui::color(bright_green),
								 center(cancel_button->Render()) | size(WIDTH, EQUAL, 100) |
									 ftxui::color(red)}) |
							   borderRounded

					 }) |
					 border | size(WIDTH, EQUAL, 150) | vcenter | hcenter

		});
	});

	auto on_agree = [&]() { depth = 0; };

	auto depth_1_container = Container::Horizontal({Button("U REDU", [&] { on_agree(); })});
	auto depth_2_container = Container::Horizontal({Button("U REDU", [&] { on_agree(); })});

	auto depth_1_renderer = Renderer(depth_1_container, [&] {
		return vbox({
				   text("Nepravilan unos datuma"),
				   separator(),
				   center(hbox(depth_1_container->Render())),
			   }) |
			   border;
	});
	auto depth_2_renderer = Renderer(depth_2_container, [&] {
		return vbox({
				   text("Uneseni datum ne postoji"),
				   separator(),
				   center(hbox(depth_2_container->Render())),
			   }) |
			   border;
	});

	auto main_container = Container::Tab({renderer, depth_1_renderer, depth_2_renderer}, &depth);
	auto main_renderer = Renderer(main_container, [&] {
		Element document = renderer->Render();

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
		}
		return document;
	});

	auto screen = ftxui::ScreenInteractive::TerminalOutput();

	screen.Loop(main_renderer);
}

void tui::yearly_report(Database& db, const std::vector<Item>& items) {
	std::string year;
	Component year_input = Input(&year, "     ");

	int i_year;
	int depth = 0;

	auto cancel_button = Button("ODUSTANI", [&] { supervisor_interface(db); });
	auto generate_button = Button("GENERISI IZVJESTAJ", [&] {
		bool check = true;
		try {
			if (year.length() > 4) throw "error";
			i_year = std::stoi(year);
		} catch (const std::exception& exc) {
			depth = 1; // nepravilan unos, ne moze se konvertovati u int
			check = false;
		}
		if (check) {
			if (util::is_valid_date(1, 1, i_year, db.current_date_time())) {
				std::string temp = std::to_string(i_year) + "01" + "01";
				int curr_date = std::stoi(temp);

				std::string next = std::to_string(i_year) + "12" + "31";
				int next_date = std::stoi(next);
				db.create_report(items, curr_date, next_date);
				supervisor_interface(db);
			} else
				depth = 2; // nevalidna godina
		}
	});

	auto component = Container::Vertical({cancel_button, generate_button, year_input});

	auto renderer = Renderer(component, [&] {
		return vbox({vbox({vbox({center(hbox(text("GODISNJI IZVJESTAJ"))),
								 center(hbox(text("Izaberi godinu") | underlined))}) |
							   borderLight,

						   center(hbox(ftxui::text("Godina: "), year_input->Render()) |
								  size(WIDTH, LESS_THAN, 80) | color(blue)) |
							   size(HEIGHT, EQUAL, 3) | vcenter,
						   separatorDouble(),
						   hbox({center(generate_button->Render()) | size(WIDTH, EQUAL, 100) |
									 ftxui::color(bright_green),
								 center(cancel_button->Render()) | size(WIDTH, EQUAL, 100) |
									 ftxui::color(red)}) |
							   borderRounded

					 }) |
					 border | size(WIDTH, EQUAL, 150) | vcenter | hcenter

		});
	});

	auto on_agree = [&]() { depth = 0; };

	auto depth_1_container = Container::Horizontal({Button("U REDU", [&] { on_agree(); })});
	auto depth_2_container = Container::Horizontal({Button("U REDU", [&] { on_agree(); })});

	auto depth_1_renderer = Renderer(depth_1_container, [&] {
		return vbox({
				   text("Nepravilan unos datuma"),
				   separator(),
				   center(hbox(depth_1_container->Render())),
			   }) |
			   border;
	});
	auto depth_2_renderer = Renderer(depth_2_container, [&] {
		return vbox({
				   text("Uneseni datum ne postoji"),
				   separator(),
				   center(hbox(depth_2_container->Render())),
			   }) |
			   border;
	});

	auto main_container = Container::Tab({renderer, depth_1_renderer, depth_2_renderer}, &depth);
	auto main_renderer = Renderer(main_container, [&] {
		Element document = renderer->Render();

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
		}
		return document;
	});

	auto screen = ftxui::ScreenInteractive::TerminalOutput();

	screen.Loop(main_renderer);
}
