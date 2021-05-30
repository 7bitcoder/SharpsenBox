#include <iostream>
#include <string>
#include "Dialog.hpp"
#include "IComponent.hpp"
#include "IGameManager.hpp"


namespace sb {
	Dialog::Dialog() {}
	Q_INVOKABLE void Dialog::dialog(bool value) {
		value_ = value;
		bool dontHide = false;
		if(callback_ && type_ != Type::INFO)
			dontHide = callback_(value);
		if (!dontHide) {
			resetCallback();
			type_ = NONE;
		}
		showDialog();
	}

	void Dialog::resetCallback() {
		callback_ = nullptr;
	}
}