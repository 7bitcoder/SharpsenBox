#include <iostream>
#include <string>
#include "Button.hpp"
#include "AppBackend.hpp"

namespace bc {
	Button::Button() {
		textArea = bc::Backend::engine->rootObjects().value(0)->findChild<QObject*>("textInput", Qt::FindChildrenRecursively);
		qmlRegisterType<Button>("Button", 1, 0, "Button");
	}
	void Button::buttonClicked(const QString& in) { textArea->setProperty("text", QString(value) + " hehehehe"); }
}