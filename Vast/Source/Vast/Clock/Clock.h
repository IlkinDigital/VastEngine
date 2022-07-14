#pragma once

namespace Vast {

	class Clock
	{
	public:
		static float EpochSeconds();
		static float EpochMilliseconds();
		static String DateTime();
		// DateTime in descending order y/m/d-h/m/s
		static String DateTimeDesc();
	};

}