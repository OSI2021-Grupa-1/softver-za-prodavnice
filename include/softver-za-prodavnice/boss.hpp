#pragma once
#include "user.hpp"
#include "report.hpp"

class Boss : public User {
  private:
	
  public:
	void list_of_employees();
	void create_account();
	void delete_account();
	void list_of_items();
	void create_item();
	void update_item();
	void delete_item();
	void generate_back_up();

	void generate_report(); // UI za izbor izvjestaja, funkcija koja vodi do pregleda opcija za izvjestaj
	Report generate_daily_report();
	Report generate_weekly_report();
	Report generate_monthly_report();
	Report generate_yearly_report();
	Report generate_arbitrarily_report();
};
