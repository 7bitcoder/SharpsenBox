#include "AppInfoParser.hpp"
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "IConfig.hpp"
#include "UpdateInfo.hpp"

namespace sb {
	void AppInfoParser::Run() {
		try {
			QString val;
			QFile file;
			//open SharpsenBoxConfig file
			auto &config = Component<IConfig>::Get();
			auto path =  config.CombinePath({config.GetDownloadDir(), parseInfoFileName});
			file.setFileName(path);
			file.open(QIODevice::ReadOnly | QIODevice::Text);
			val = file.readAll();
			auto ghj = val.toStdString();
			file.close();
			QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());

			versionToUpdate_ = d["Ver"].toString();
			auto& actualVersion = UpdateInfo->getActualVersion();

			auto tt = actualVersion.toStdString();
			auto gg = versionToUpdate_.toStdString();

			auto fileListUrl = d["FileList"].toString();
			pathFiles_.push_back({ fileListUrl, "FileList.json" });
			if (UpdateInfo->getFullInstall()) {
				needUpdate_ = true;
			} else if (versionToUpdate_ != actualVersion) { //need Update
				needUpdate_ = true;
				getPathUrls(d["Versioning"].toObject());
			} else { //app is up to date
				needUpdate_ = false;
			}
			//} catch (std::exception& e) {

		} catch (...) {
			Error("Unexpected error ocured while reading Update file");
		}
	}
	void AppInfoParser::getPathUrls(const QJsonObject& pathList) {
		for (auto it = pathList.begin(); it != pathList.end(); it++) {
			auto ff = it.key().toStdString();
			auto& actualVer = UpdateInfo->getActualVersion();
			if (it.key() <= actualVer)
				continue;
			pathFiles_.push_back({ it->toString() , "Path-" + it.key() + ".json" });
		}
	}

	void AppInfoParser::Reset() {
		needUpdate_ = false;
		pathFiles_.clear();
	}
}