#include <QString>
#include <QFile>
#include <QJsonDocument>
#include <QJsonObject>
#include <filesystem>
#include "IQmlObject.hpp"
#include "TreeModel.hpp"
#include<unordered_map>



namespace pr {

	struct {

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

		Q_INVOKABLE void setGameDir(QString str) {
			gameDir_ = str.toStdString();
			auto& tree = dt::TreeModel::getObject();
			tree.setRoot(gameDir_);
			tree.init(false);
		}

		Q_INVOKABLE void setProjectDir(QString str) { projectDir = str.toStdString(); }
		Q_INVOKABLE void setGameName(QString str) { gameName = str.toStdString(); }
		Q_INVOKABLE void setProjectName(QString str) { projectName = str.toStdString(); }

		Q_INVOKABLE void setUpProject() { generate(); }
		void save();
		void generate();
		void insertFileData(std::filesystem::path file);
		bool newProject() { return newProject_; }
		void insertData(dt::TreeItem* item);
	private:
		bool newProject_ = true;
		QFile file_;
		std::filesystem::path rootDir_;
		QJsonDocument doc_;
		QJsonObject* rootObject_;


		std::filesystem::path gameDir_;
		std::filesystem::path projectDir;
		std::string gameName;
		std::string projectName;
	};
}
