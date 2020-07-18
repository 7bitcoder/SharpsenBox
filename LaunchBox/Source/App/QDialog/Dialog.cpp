#include <iostream>
#include <string>
#include "Dialog.hpp"
#include "IComponent.hpp"
#include "IGameManager.hpp"


namespace dl {
	Dialog::Dialog() {}
	Q_INVOKABLE void Dialog::dialog(bool value) {
		value_ = value;
		if(callback_ && type_ != Type::INFO)
			callback_(value);
		resetCallback();
		type_ = NONE;
		showDialog();
	}

	void Dialog::resetCallback() {
		callback_ = nullptr;
	}
}