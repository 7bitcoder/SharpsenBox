#pragma once
#include <memory>

namespace cf {
	struct IConfig;
}
namespace lb {
	struct ILoadingBar;
}
namespace im {
	struct IInstalationManager;
}
namespace dl {
	struct IDialog;
}
namespace gm {
	struct IGameManager;
}
namespace bc {
	class ObjectsRepository {
	public:
		static ObjectsRepository& getRepo() {
			static ObjectsRepository ob;
			return ob;
		}
		cf::IConfig& getConfig();
		dl::IDialog& getDialog();
		lb::ILoadingBar& getLoadingBar();
		gm::IGameManager& getGameManager();
		im::IInstalationManager& getInstalationManager();
	private:
		ObjectsRepository() = default;
		~ObjectsRepository() = default;
	};
}