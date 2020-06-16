#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <filesystem>

namespace gn {
	class Generator {
	public:
		void generate(std::filesystem::path dir);
		void insertFileData(std::filesystem::path file);
	private:
		QFile file_;
		std::filesystem::path rootDir_;
		QJsonDocument doc_;
		QJsonObject rootObject_;
	};
}
