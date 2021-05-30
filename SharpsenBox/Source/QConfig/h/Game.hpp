#pragma once
#include <filesystem>
#include <string>
#include <QString>
#include <unordered_map>
namespace sb {

	struct AppPack {
		std::filesystem::path url;
		std::string fileName;
		std::filesystem::path destination;
	};

	class Game {
	public:
		int id;
		bool installed;
		bool shortcut;
		bool autoCheck;
		QString name;
		QString version;
		QString appInfoUrl;
		QString fileName;
		QString gameDir;
		QString execPath;
		QString shortcutPath;
		QString presentationUrl;
		QString PresentationQml;
		QString PresentationPackUrl;
		QString PresentationVer;
		std::unordered_map<QString, int> sha;

		bool updateChecked = false;
		bool operator==(const Game& g) {
			return id == g.id;
		}
	};
}