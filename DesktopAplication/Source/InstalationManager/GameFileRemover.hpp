#pragma once
#include <iostream>
#include <QObject>
#include <QThread>
#include <QDebug>
#include <filesystem>
#include "Config.hpp"
#include <unordered_set>
#include "InstalationManager.hpp"

namespace im {
	class GameFileRemover : public ImElement {
		Q_OBJECT
	public:
		void removeFiles(cf::Game& game, std::unordered_set<QString>& toRemove) { game_ = &game; toRemove_ = &toRemove;  start(); };
		virtual ~GameFileRemover() {};
		GameFileRemover() = default;

	signals:
		void removeComplete();
	public slots:
	private:
		cf::Game* game_;
		std::unordered_set<QString>* toRemove_;
	};
}
