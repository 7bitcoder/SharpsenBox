
namespace im {
	class UpdateInfo;

	struct IInstalationManager {
		virtual ~IInstalationManager() {};

		virtual void downloadStatus(qint64 progress, qint64 total, double speed) = 0;

		virtual void installStatus(qint64 progress) = 0;

		virtual UpdateInfo& getUpdateInfo() = 0;
	};
}