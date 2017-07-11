#ifndef ACTIONMANAGER_H
#define ACTIONMANAGER_H

#include "InputManager.h"

class ActionManager{

	public:

		static bool LeftArrowAction();

		static bool RightArrowAction();

		static bool UpArrowAction();

		static bool DownArrowAction();

		static bool StartAction();

		static bool EscapeAction();

		static bool RightShoulderAction();

		static bool LeftShoulderAction();

	private:

		ActionManager();

};

#endif
