#include <iostream>
#include "Button.hpp"
#include "AppBackend.hpp"

namespace bc {
	Button::Button() {

	}
	void Button::buttonClicked(const QString& in) { std::cout << "clicked\n"; }
}