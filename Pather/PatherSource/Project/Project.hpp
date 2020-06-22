#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <filesystem>
#include "IQmlObject.hpp"
#include "TreeModel.hpp"
#include "Packer.hpp"
#include<unordered_map>



namespace pr {

	struct File {
		QString path;
		qint64 size;
		QString sha;
	};
	class Project : public bc::IQmlObject {
		Q_OBJECT
	public:
		static Project& getObject() {
			static Project uc;
			return uc;
		}

		// implementation IQmlObject
		void update() override {};
		void init() override {};
		std::string getName() override { return TYPENAME(Project); };

		Q_PROPERTY(QString version WRITE setVer READ getVer);

		Q_INVOKABLE void setVer(QString str) {
			version_ = str;
		}

		Q_INVOKABLE QString getVer() {
			return version_;
		}

		Q_INVOKABLE void setGameDir(QString str) {
			AppDir_ = str.toStdString();
			auto& tree = dt::TreeModel::getObject();
			tree.setRoot(AppDir_);
			tree.init(false);
		}

		Q_INVOKABLE void setProjectDir(QString str) { projectDir = str.toStdString(); }
		Q_INVOKABLE void setGameName(QString str) { gameName = str.toStdString(); }
		Q_INVOKABLE void setProjectName(QString str) { projectName = str.toStdString(); }

		Q_INVOKABLE void setUpProject() { generate(); }
		Q_INVOKABLE void generatePatch() { save(); }
		void save();
		void generate();
		void insertFileData(std::filesystem::path file);
		bool newProject() { return newProject_; }
		void insertData(dt::TreeItem* item);
		QHash<QString, File>& getLastStatus() { return lastStatus_; }
		Q_INVOKABLE void loadProject(QString dir);
	signals:
		void verChanged();
	private:
		bool newProject_ = true;
		QFile file_;
		std::filesystem::path rootDir_;
		QJsonDocument doc_;
		QJsonObject* rootObject_;

		QString version_;
		std::filesystem::path AppDir_;
		std::filesystem::path projectDir;
		std::string gameName;
		std::string projectName;

		bb::Packer packer_;
		QHash<QString, File> lastStatus_;
	};
}
