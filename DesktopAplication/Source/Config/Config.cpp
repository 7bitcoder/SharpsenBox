#include "Config.hpp"
#include <QElapsedTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>


namespace cf {

	void Config::init() {

		if (!std::filesystem::exists(ConfigJson))
			;//problem
		QString val;
		QFile file;
		file.setFileName(ConfigJson.string().c_str());
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		val = file.readAll();
		file.close();
		QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
		QJsonObject sett2 = d.object();
		version = d["Ver"].toString();
	}

	std::string Config::getName() {
		return TYPENAME(Config);
	}
	//
	//qint64 AppUpdateChecker::getProgress() const {
	//	return progress_;
	//}
}