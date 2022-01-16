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



void tui::arbitrary_report(Database& db, const std::vector<Item>& items) {
	std::string day_1;
	Component day_1_input = Input(&day_1, "     ");
	std::string month_1;
	Component month_1_input = Input(&month_1, "     ");
	std::string year_1;
	Component year_1_input = Input(&year_1, "     ");

	std::string day_2;
	Component day_2_input = Input(&day_2, "     ");
	std::string month_2;
	Component month_2_input = Input(&month_2, "     ");
	std::string year_2;
	Component year_2_input = Input(&year_2, "     ");

	int i_day_1, i_month_1, i_year_1;
	int i_day_2, i_month_2, i_year_2;

	int depth = 0;

	auto cancel_button = Button("ODUSTANI", [&] { supervisor_interface(db); });
	auto generate_button = Button("GENERISI IZVJESTAJ", [&] {
		bool check = true;
		try {
			if (day_1.length() > 2 || month_1.length() > 2 || year_1.length() > 4 ||
				day_1.length() > 2 || month_1.length() > 2 || year_1.length() > 4)
				throw "Error";
			i_day_1 = std::stoi(day_1);
			i_month_1 = std::stoi(month_1);
			i_year_1 = std::stoi(year_1);
			i_day_2 = std::stoi(day_2);
			i_month_2 = std::stoi(month_2);
			i_year_2 = std::stoi(year_2);
		} catch (...) {
			depth = 1; // nepravilan unos, ne moze se konvertovati u int
			check = false;
		}
		if (check) {
			if (util::is_valid_date(i_day_1, i_month_1, i_year_1, db.current_date_time()) &&
				util::is_valid_date(i_day_2, i_month_2, i_year_2, db.current_date_time())) {

				std::string temp_1 = std::to_string(i_year_1);
				std::string temp_2 = std::to_string(i_year_2);

				temp_1 += (i_month_1 < 10 ? "0" : "") + std::to_string(i_month_1) +
						  (i_day_1 < 10 ? "0" : "") + std::to_string(i_day_1);
				temp_2 += (i_month_2 < 10 ? "0" : "") + std::to_string(i_month_2) +
						  (i_day_2 < 10 ? "0" : "") + std::to_string(i_day_2);

				int start_date = std::stoi(temp_1);
				int end_date = std::stoi(temp_1);

				db.write_report(items, start_date, end_date);
				supervisor_interface(db);
			} else if (!util::compare_date(i_day_1, i_month_1, i_year_1, i_day_2, i_month_2,
										   i_year_2)) {
				depth = 3; // prvi datum veci od drugog
			} else
				depth = 2; // nevalidan datum
		}
	});

	auto component =
		Container::Vertical({cancel_button, generate_button, day_1_input, month_1_input,
							 year_1_input, day_2_input, month_2_input, year_2_input});

	auto renderer = Renderer(component, [&] {
		return vbox(
			{vbox({vbox({center(hbox(text("PROIZVOLJNI IZVJESTAJ IZVJESTAJ"))),
						 center(hbox(text("Izaberi pocetni i krajni datum") | underlined))}) |
					   borderLight,
				   center(hbox(hbox(ftxui::text("Pocetni dan: "), day_1_input->Render()) |
								   size(WIDTH, LESS_THAN, 40) | color(blue) | borderLight,
							   hbox(ftxui::text("Krajnji dan: "), day_2_input->Render()) |
								   size(WIDTH, LESS_THAN, 40) | color(blue) | borderLight) |
						  size(HEIGHT, EQUAL, 3) | vcenter),
				   separatorDouble(),

				   center(hbox(hbox(ftxui::text("Pocetni mjesec: "), month_1_input->Render()) |
								   size(WIDTH, LESS_THAN, 40) | color(blue) | borderLight,
							   hbox(ftxui::text("Krajnji mjesec: "), month_2_input->Render()) |
								   size(WIDTH, LESS_THAN, 40) | color(blue) | borderLight) |
						  size(HEIGHT, EQUAL, 3) | vcenter),
				   separatorDouble(),

				   center(hbox(hbox(ftxui::text("Pocetna godina: "), year_1_input->Render()) |
								   size(WIDTH, LESS_THAN, 40) | color(blue) | borderLight,
							   hbox(ftxui::text("Krajnja godina: "), year_2_input->Render()) |
								   size(WIDTH, LESS_THAN, 40) | color(blue) | borderLight) |
						  size(HEIGHT, EQUAL, 3) | vcenter),
				   separatorDouble(),

				   hbox({center(generate_button->Render()) | size(WIDTH, EQUAL, 100) |
							 ftxui::color(bright_green),
						 center(cancel_button->Render()) | size(WIDTH, EQUAL, 100) |
							 ftxui::color(red)}) |
					   borderRounded

			 }) | border | size(WIDTH, EQUAL, 150) | vcenter | hcenter

			});
	});

	auto on_agree = [&]() { depth = 0; };

	auto depth_1_container = Container::Horizontal({Button("U REDU", [&] { on_agree(); })});
	auto depth_2_container = Container::Horizontal({Button("U REDU", [&] { on_agree(); })});
	auto depth_3_container = Container::Horizontal({Button("U REDU", [&] { on_agree(); })});

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
	auto depth_3_renderer = Renderer(depth_3_container, [&] {
		return vbox({
				   text("Krajnji datum je veci od pocetnog"),
				   separator(),
				   center(hbox(depth_3_container->Render())),
			   }) |
			   border;
	});

	auto main_container =
		Container::Tab({renderer, depth_1_renderer, depth_2_renderer, depth_3_renderer}, &depth);
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
