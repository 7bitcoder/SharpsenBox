#pragma once
#include <iostream>
#include <QObject>
#include <QThread>
#include <QDebug>
#include <filesystem>
#include "Game.hpp"
#include <unordered_set>
#include "IRunnable.hpp"

namespace sb {
	class GameFileRemover : public IRunnable {
	public:
		void removeFiles(Game& game, std::unordered_set<QString>& toRemove) { game_ = &game; toRemove_ = &toRemove; };
		virtual ~GameFileRemover() {};
		GameFileRemover() = default;

		// ImElement implementation
		void Run() override;
		void Reset() override;
	private:
		Game* game_;
		std::unordered_set<QString>* toRemove_;
	};
}
