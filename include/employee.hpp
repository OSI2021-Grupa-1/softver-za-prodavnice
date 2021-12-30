#pragma once
#include "user.hpp"

class Employee : public User {
  private:

  public:
	void sell_items();
	void create_receipt();
};
