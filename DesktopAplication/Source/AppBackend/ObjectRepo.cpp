#include "Dialog.hpp"
#include "InstalationManager.hpp"
#include "Config.hpp"
#include "LoadingBar.hpp"
#include "GameManager.hpp"
#include "ObjectRepo.hpp"

namespace bc {
	namespace {
		template<class T> T& get() {
			static T ob;
			return ob;
		};
	}
	cf::IConfig& ObjectsRepository::getConfig() { return get<cf::Config>(); }
	dl::IDialog& ObjectsRepository::getDialog() { return get<dl::Dialog>(); }
	lb::ILoadingBar& ObjectsRepository::getLoadingBar() { return get<lb::LoadingBar>(); }
	gm::IGameManager& ObjectsRepository::getGameManager() { return get<gm::GameManager>(); }
	im::IInstalationManager& ObjectsRepository::getInstalationManager() { return get<im::InstalationManager>(); }
}