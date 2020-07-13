#pragma once
#include <iostream>
#include <QObject>
#include <QThread>
#include <QDebug>
#include <filesystem>
#include "Game.hpp"
#include <unordered_set>
#include "IRunnable.hpp"

namespace im {
	class GameFileRemover : public IRunnable {
	public:
		void removeFiles(cf::Game& game, std::unordered_set<QString>& toRemove) { game_ = &game; toRemove_ = &toRemove; };
		virtual ~GameFileRemover() {};
		GameFileRemover() = default;

		// ImElement implementation
		bool run() override;
		void reset() override;
	private:
		cf::Game* game_;
		std::unordered_set<QString>* toRemove_;
	};
}
