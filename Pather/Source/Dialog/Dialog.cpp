#include <iostream>
#include <string>
#include "Dialog.hpp"
#include "AppBackend.hpp"

namespace dl {
	Q_INVOKABLE void Dialog::dialog(bool value) {
		value_ = value;
		show_ = false;
		showDialogChanged();
		dialogTriggered(value);
	}
}