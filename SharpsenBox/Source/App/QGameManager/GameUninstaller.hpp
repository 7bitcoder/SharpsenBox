#include <iostream>
#include <QObject>
#include <QThread>
#include <QDebug>
#include <filesystem>

namespace sb
{
	class GameUninstaller : public QThread
	{
		Q_OBJECT
	public:
		void SetGameId(int id);
		void run() override;
		virtual ~GameUninstaller(){};
		GameUninstaller(){};

	signals:
		void UninstalationComplete(int id);
	public slots:
	private:
		int _GameId;
		QString _GameDir;
		bool _Shortcut;
		QString _ShortcutPath;
	};
}
