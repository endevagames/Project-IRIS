#pragma once

namespace NURU
{
	typedef enum class MouseCode : uint16_t
	{
		// From glfw3.h
		Button0                = 0,
		Button1                = 1,
		Button2                = 2,
		Button3                = 3,
		Button4                = 4,
		Button5                = 5,
		Button6                = 6,
		Button7                = 7,

		ButtonLast             = Button7,
		ButtonLeft             = Button0,
		ButtonRight            = Button1,
		ButtonMiddle           = Button2
	} Mouse;
	
	inline std::ostream& operator<<(std::ostream& os, MouseCode mouseCode)
	{
		os << static_cast<int32_t>(mouseCode);
		return os;
	}
}

#define NURU_MOUSE_BUTTON_0      ::NURU::Mouse::Button0
#define NURU_MOUSE_BUTTON_1      ::NURU::Mouse::Button1
#define NURU_MOUSE_BUTTON_2      ::NURU::Mouse::Button2
#define NURU_MOUSE_BUTTON_3      ::NURU::Mouse::Button3
#define NURU_MOUSE_BUTTON_4      ::NURU::Mouse::Button4
#define NURU_MOUSE_BUTTON_5      ::NURU::Mouse::Button5
#define NURU_MOUSE_BUTTON_6      ::NURU::Mouse::Button6
#define NURU_MOUSE_BUTTON_7      ::NURU::Mouse::Button7
#define NURU_MOUSE_BUTTON_LAST   ::NURU::Mouse::ButtonLast
#define NURU_MOUSE_BUTTON_LEFT   ::NURU::Mouse::ButtonLeft
#define NURU_MOUSE_BUTTON_RIGHT  ::NURU::Mouse::ButtonRight
#define NURU_MOUSE_BUTTON_MIDDLE ::NURU::Mouse::ButtonMiddle