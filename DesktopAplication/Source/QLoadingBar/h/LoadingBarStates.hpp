#pragma once
namespace lb {
	enum  State : int {
		NONE = -1, CHECKING = 0, DOWNLOADING, INSTALLING, PAUSE, ERRORD, STOPPED, COMPLEET
	};
	enum  VisibleState : int {
		HIDDEN = 0, SHOWED, MINIMALIZED
	};
}