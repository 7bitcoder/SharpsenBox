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
		bc::Component < gm::IGameManager>::get().uninstall(value);
	}
}