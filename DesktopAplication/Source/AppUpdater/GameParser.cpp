#include "GameParser.hpp"
#include "Config.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>

namespace upd {
	void GameParser::parse() {
		start();
	}
	
	void GameParser::run() {
		QString val;
		QFile file;
		//open LaunchBoxConfig file
		auto path = cf::Config::getObject().getDownloadDir() / "Games.json";
		file.setFileName(path.generic_string().c_str());
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		val = file.readAll();
		auto& ghj = val.toStdString();
		file.close();
		QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
		for (auto& game : d.object()) {
			auto& gameObject = game.toObject();
			auto id = std::stoi(gameObject["Id"].toString().toStdString());
			if (!cf::Config::getObject().gameExists(id)) {
				//create new
				cf::Game game;
				game.id = id;
				game.version = gameObject["Ver"].toString();
				game.autoCheck = game.installed = game.shortcut = false;
				game.execPath = gameObject["GameExecPath"].toString();
				game.presentationUrl = gameObject["PresentationUrl"].toString();
				//game.PresentationVer = gameObject["PresentationVer"].toString(); for update purpose
				game.appInfoUrl = gameObject["AppInfoUrl"].toString();
				game.PresentationPackUrl = gameObject["PresentationPackUrl"].toString();
				game.PresentationQml = gameObject["PresentationQml"].toString();

				cf::Config::getObject().insertGame(game);
			}
			auto& hadGame = cf::Config::getObject().getGame(id);
			auto& presetationVer = gameObject["PresentationVer"].toString();
			hadGame.presentationUrl = gameObject["PresentationUrl"].toString();
			if (hadGame.presentationUrl.isEmpty() && hadGame.PresentationVer != presetationVer) {
				auto& url = gameObject["PresentationPackUrl"].toString().toStdString();
				auto& fileName = hadGame.name.toStdString() + ".zip";
				std::filesystem::path destination = cf::Config::getObject().gameInfoDirRel(id);
				files_.push_back({ url, fileName , destination });
				toUpdate_.push_back({id, presetationVer });
			}
		}
		parseEnded();
	}

	void GameParser::updateGamesInfo() {
		for (auto& data : toUpdate_) {
			cf::Config::getObject().getGame(data.first).PresentationVer = data.second;
		}
	}

	void GameParser::reset() {
		files_.clear();
		toUpdate_.clear();
	}
}