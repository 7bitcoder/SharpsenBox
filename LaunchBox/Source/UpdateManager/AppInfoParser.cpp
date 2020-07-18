#include "AppInfoParser.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "IConfig.hpp"
#include "UpdateInfo.hpp"

namespace im {
	bool AppInfoParser::run() {
		try {
			QString val;
			QFile file;
			//open LaunchBoxConfig file
			auto path = bc::Component<cf::IConfig>::get().getDownloadDir() / parseInfoFileName;
			file.setFileName(path.generic_string().c_str());
			file.open(QIODevice::ReadOnly | QIODevice::Text);
			val = file.readAll();
			auto& ghj = val.toStdString();
			file.close();
			QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());

			versionToUpdate_ = d["Ver"].toString();
			auto& actualVersion = updateInfo_->getActualVersion();

			auto fileListUrl = d["FileList"].toString();
			pathFiles_.push_back({ fileListUrl.toStdString(), "FileList.json" });
			if (updateInfo_->getFullInstall()) {
				needUpdate_ = true;
			} else if (versionToUpdate_ != actualVersion) { //need update
				needUpdate_ = true;
				getPathUrls(d["Versioning"].toObject());
			} else { //app is up to date
				needUpdate_ = false;
			}
			//} catch (std::exception& e) {

		} catch (...) {
			errorStr_ = "Unexpected error ocured while reading update file";
			return false;
		}
		return true;
	}
	void AppInfoParser::getPathUrls(QJsonObject& pathList) {
		for (auto it = pathList.begin(); it != pathList.end(); it++) {
			auto& ff = it.key().toStdString();
			auto& actualVer = updateInfo_->getActualVersion();
			if (it.key() <= actualVer)
				continue;
			pathFiles_.push_back({ it->toString().toStdString() , "Path-" + it.key().toStdString() + ".json" });
		}
	}

	void AppInfoParser::reset() {
		needUpdate_ = false;
		pathFiles_.clear();
	}
}