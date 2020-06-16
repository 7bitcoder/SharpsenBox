#pragma once
#include <QObject>
#include <QQmlApplicationEngine>
#include <QDebug>
#include <QThread>
#include <QNetworkAccessManager>
#include "IQmlObject.hpp"

namespace ge {
	class GameExecutor : public bc::IQmlObject {
		Q_OBJECT
	public:
		static GameExecutor& getObject() {
			static GameExecutor uc;
			return uc;
		}

		// implementation IQmlObject
		void update() override {};
		std::string getName() override;
		void init() override {};


		Q_INVOKABLE void execute(int id);
	private:
		virtual ~GameExecutor() {};
		GameExecutor() {};
		//public slots:
	};
}
