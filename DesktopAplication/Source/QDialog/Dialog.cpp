#include <iostream>
#include <string>
#include "Dialog.hpp"
#include "IComponent.hpp"
#include "IGameManager.hpp"

namespace bc {
	dl::IDialog* bc::Get<dl::IDialog>::component_ = nullptr;
}
namespace dl {
	Dialog::Dialog() : bc::Get<IDialog>(this) {}
	Q_INVOKABLE void Dialog::dialog(bool value) {
		value_ = value;
		show_ = false;
		showDialogChanged();
		bc::Get < gm::IGameManager>::get().uninstall(value);
	}
}