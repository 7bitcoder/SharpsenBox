#include "Config.hpp"
#include <QElapsedTimer>
#include <QJsonDocument>
#include <QJsonObject>
#include <QFile>


namespace cf {

	Config::Config() {
		if (!std::filesystem::exists(configJson_))
			;//problem
		QString val;
		QFile file;
		file.setFileName(configJson_.string().c_str());
		file.open(QIODevice::ReadOnly | QIODevice::Text);
		val = file.readAll();
		file.close();
		QJsonDocument d = QJsonDocument::fromJson(val.toUtf8());
		QJsonObject sett2 = d.object();
		version_ = d["Ver"].toString();
	}
	void Config::init() {
	}

	std::string Config::getName() {
		return TYPENAME(Config);
	}
	//
	//qint64 AppUpdateChecker::getProgress() const {
	//	return progress_;
	//}
}