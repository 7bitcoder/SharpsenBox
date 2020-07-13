#pragma once
namespace upd {
	enum State : int {
		NONE = 0, DOWNLOADING, INSTALLING, ENDED, ERROR, NO_UPDATE_FOUND, UPDATING_GAME_PAGES
	};
}