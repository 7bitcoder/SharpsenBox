#include <iostream>
#include "Button.hpp"

namespace bc {
	void Button::buttonClicked(const QString& in) { std::cout << "clicked\n"; }
}