#include <iostream>
#include <string>
#include "Dialog.hpp"
#include "IComponent.hpp"
#include "IGameManager.hpp"


namespace dl {
	Dialog::Dialog() {}
	Q_INVOKABLE void Dialog::dialog(bool value) {
		value_ = value;
		show_ = false;
		showDialogChanged();
		if(callback_)
			callback_(value);
		resetCallback();
	}

	void Dialog::resetCallback() {
		callback_ = nullptr;
	}
}