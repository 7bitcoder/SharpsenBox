#include <iostream>
#include <QObject>
#include <QThread>

namespace bb {
	class Uninstaller : public  QThread {
		void uninstall(int id);
		void run() override;
	};
}
