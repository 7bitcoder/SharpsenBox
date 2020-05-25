#include <iostream>
#include <string>
#include "Button.hpp"
#include "AppBackend.hpp"

namespace bc {
	Button::Button() {
		}
	QString Button::sayHello() const { return string; }
	void Button::buttonClicked(const QString& in) { 
		string = std::to_string(++value).c_str();
		textChanged();
	}
}