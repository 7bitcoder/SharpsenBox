#include "IConfig.hpp"
#include "Game.hpp"
#include "GameParser.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace sb {
	void GameParser::Run() {
		Reset();
		try {
			QString val;
			QFile file;
			//open SharpsenBoxConfig file
			auto& config = Component<IConfig>::Get();
			auto path = config.CombinePath({config.GetDownloadDir(), "Games.json"});
			file.setFileName(path);
			file.open(QIODevice::ReadOnly | QIODevice::Text);
			val = file.readAll();
			auto ghj = val.toStdString();
			file.close();
			QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
			for (auto game : d.object()) {
				auto gameObject = game.toObject();
				auto id = std::stoi(gameObject["Id"].toString().toStdString());
				if (!config.GameExists(id)) {
					//create new
					Game game;
					game.Id = id;
					game.Version = gameObject["Ver"].toString();
					game.UpdateAutoCheck = game.IsInstalled = game.HasShortcut = false;
					game.ExecutablePath = gameObject["GameExecPath"].toString();
					game.PresentationUrl = gameObject["PresentationUrl"].toString();
					//game.PresentationVer = gameObject["PresentationVer"].toString();
					game.GameInfoUrl = gameObject["AppInfoUrl"].toString();
					game.PresentationPackUrl = gameObject["PresentationPackUrl"].toString();
					game.PresentationQml = gameObject["PresentationQml"].toString();

					config.AddNewGame(game);
				}
				auto& hadGame = config.GetGame(id);
				auto presetationVer = gameObject["PresentationVer"].toString();
				hadGame.PresentationUrl = gameObject["PresentationUrl"].toString();
				
				if (hadGame.PresentationUrl.isEmpty() && hadGame.PresentationVer != presetationVer) {
					auto url = gameObject["PresentationPackUrl"].toString();
					auto fileName = hadGame.Title + ".zip";
					auto destination = config.GamePageDir(id);
					files_.push_back({ url, fileName , destination });
					toUpdate_.push_back({ id, presetationVer });
				}
			}
		} catch (...) {
			Error("Unexpected error ocured while processing gamePages Update");
		}
	}

	void GameParser::updateGamesInfo() {
		for (auto& data : toUpdate_) {
			Component<IConfig>::Get().GetGame(data.first).PresentationVer = data.second;
		}
	}

	void GameParser::Reset() {
		files_.clear();
		toUpdate_.clear();
	}
}
