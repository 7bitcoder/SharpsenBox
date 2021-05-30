#include "AppInfoParser.hpp"
#include "UpdateManager.hpp"
#include "UpdateInfo.hpp"
#include "IComponent.hpp"
#include "ConfigMock.hpp"

namespace sb {
	class UpdateManagerMock : public IUpdateManager {
	public:
		UpdateManagerMock() {}
		~UpdateManagerMock() {};
		bool installMainApp(QString version, std::filesystem::path appInfoUrl, std::filesystem::path gamesRepoUrl) { return true; };
		bool updateMainApp(QString version, std::filesystem::path appInfoUrl, std::filesystem::path gamesRepoUrl) { return true; };
		bool installGame(Game& game, const QString& gamePath, bool shortcut) { return false; };
		bool updateGame(Game& game) { return false; };

		void downloadStatus(qint64 progress, qint64 total, double speed) {};

		void installStatus(qint64 progress) {};

		UpdateInfo& getUpdateInfo() { return ui_; };


		void paused() {};
		void resumed() {};
		void stop() {};
	private:

		UpdateInfo ui_;
	};
}
