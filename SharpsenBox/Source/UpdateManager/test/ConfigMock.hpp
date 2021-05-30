#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QString>
#include <QUrl>
#include <string>
#include "AppInfoParser.hpp"
#include "UpdateManager.hpp"
#include "UpdateInfo.hpp"
#include "ComponentRegister.hpp"
#include "IConfig.hpp"
#include <filesystem>


namespace sb {
	class ConfigMock : public IConfig {
	public:
		void update() final {};
		void init() final {};
		std::string getName() final { return TYPENAME(ConfigMock); }

		ConfigMock() final {}
		~ConfigMock() final {};
		void insertGame(Game& game) final {}
		bool gameExists(int id) final { return false; }
		Game& getGame(int id) final { return game_; }
		std::string gamePageDir(int id) final { return ""; }
		QString& getVer() final { return str_; }
		void setVer(QString ver) final {}

		std::filesystem::path& getDownloadDir() final { return curr; }
		std::filesystem::path getConfigJson() final { return curr; }
		std::filesystem::path getConfigJsonFileName() final { return curr; }
		std::filesystem::path getLauncherAppInfoUrl()final { return curr; }
		std::filesystem::path getGameInfoRepository()final { return curr; }

		bool installed(int id) final { return false; }
		QString gameInfoDir(int id) final { return "."; }
		QUrl defaultInstallDir() final { return "."; }
		void setDownloadSpeed(qint32 dp) final {}
		qint32 getDownloadSpeed()final { return 1; }
		void setGameAutoCheck(int id, bool val)final {}
		bool getGameAutoCheck(int id) final { return false; }
		QString getGameName(int id)final { return ""; }
		QString  getConfigJsonUrl()final { return ""; }
		int getGameId()final { return 1; }
		QString getGamePresentationUrl(int id) final { return ""; }
		QString getPresentationFile(int id)final { return ""; }
		int getDefaultGameId()final { return 1; }
		QString getCurrentDirectory()final { return "."; }
	private:
		std::filesystem::path curr = "./jsons";
		Game game_;
		QString str_;
	};
}
