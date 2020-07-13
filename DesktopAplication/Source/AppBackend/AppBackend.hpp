#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <unordered_map>
#include "IComponent.hpp"
#include "IAppBackend.hpp"

// Backend class - registers all objects that will be exported to qml

namespace bc {
	class Backend : IBackend {
	public:
		static IBackend& getBackend() {
			static Backend bc;
			return bc;
		};

		void init(QQmlApplicationEngine* eng);

		cf::IConfig& getConfig();
		dl::IDialog& getDialog();
		lb::ILoadingBar& getLoadingBar();
		gm::IGameManager& getGameManager();
		im::IInstalationManager& getInstalationManager();

	private:
		Backend();
		~Backend();
		void qmlRegisterObject(IQmlComponent& object);

		void registerObject(IComponent& object);

		void registerObjects();
		void initializeObjects();
		QQmlApplicationEngine* engine;
		std::vector<IComponent*> objects_;

		// components
		std::unique_ptr< cf::Config> config_;
		std::unique_ptr< dl::Dialog> dialog_;
		std::unique_ptr< lb::LoadingBar> lodingBar_;
		std::unique_ptr< gm::GameManager> gameManager_;
		std::unique_ptr< im::InstalationManager> installationManager_;
	};
}