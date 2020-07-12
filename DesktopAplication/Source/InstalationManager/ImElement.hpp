#pragma once
#include "IComponent.hpp"
#include "InstalationManager.hpp"

namespace im {
	struct ImElement : bc::IInitializable {

		virtual bool run() = 0;
		virtual void reset() = 0;

		QString getErrorStr() {
			return errorStr_;
		};

		void init() override {
			im_ = &InstalationManager::getObject();
			updateInfo_ = &im_->getUpdateInfo();
		};

		InstalationManager* im_ = nullptr;
		UpdateInfo* updateInfo_ = nullptr;
		QString errorStr_;
	};
}