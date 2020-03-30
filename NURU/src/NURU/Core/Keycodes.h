  
#pragma once

namespace NURU
{
	typedef enum class KeyCode : uint16_t
	{
		// From glfw3.h
		Space               = 32,
		Apostrophe          = 39, /* ' */
		Comma               = 44, /* , */
		Minus               = 45, /* - */
		Period              = 46, /* . */
		Slash               = 47, /* / */

		D0                  = 48, /* 0 */
		D1                  = 49, /* 1 */
		D2                  = 50, /* 2 */
		D3                  = 51, /* 3 */
		D4                  = 52, /* 4 */
		D5                  = 53, /* 5 */
		D6                  = 54, /* 6 */
		D7                  = 55, /* 7 */
		D8                  = 56, /* 8 */
		D9                  = 57, /* 9 */

		Semicolon           = 59, /* ; */
		Equal               = 61, /* = */

		A                   = 65,
		B                   = 66,
		C                   = 67,
		D                   = 68,
		E                   = 69,
		F                   = 70,
		G                   = 71,
		H                   = 72,
		I                   = 73,
		J                   = 74,
		K                   = 75,
		L                   = 76,
		M                   = 77,
		N                   = 78,
		O                   = 79,
		P                   = 80,
		Q                   = 81,
		R                   = 82,
		S                   = 83,
		T                   = 84,
		U                   = 85,
		V                   = 86,
		W                   = 87,
		X                   = 88,
		Y                   = 89,
		Z                   = 90,

		LeftBracket         = 91,  /* [ */
		Backslash           = 92,  /* \ */
		RightBracket        = 93,  /* ] */
		GraveAccent         = 96,  /* ` */

		World1              = 161, /* non-US #1 */
		World2              = 162, /* non-US #2 */

		/* Function keys */
		Escape              = 256,
		Enter               = 257,
		Tab                 = 258,
		Backspace           = 259,
		Insert              = 260,
		Delete              = 261,
		Right               = 262,
		Left                = 263,
		Down                = 264,
		Up                  = 265,
		PageUp              = 266,
		PageDown            = 267,
		Home                = 268,
		End                 = 269,
		CapsLock            = 280,
		ScrollLock          = 281,
		NumLock             = 282,
		PrintScreen         = 283,
		Pause               = 284,
		F1                  = 290,
		F2                  = 291,
		F3                  = 292,
		F4                  = 293,
		F5                  = 294,
		F6                  = 295,
		F7                  = 296,
		F8                  = 297,
		F9                  = 298,
		F10                 = 299,
		F11                 = 300,
		F12                 = 301,
		F13                 = 302,
		F14                 = 303,
		F15                 = 304,
		F16                 = 305,
		F17                 = 306,
		F18                 = 307,
		F19                 = 308,
		F20                 = 309,
		F21                 = 310,
		F22                 = 311,
		F23                 = 312,
		F24                 = 313,
		F25                 = 314,

		/* Keypad */
		KP0                 = 320,
		KP1                 = 321,
		KP2                 = 322,
		KP3                 = 323,
		KP4                 = 324,
		KP5                 = 325,
		KP6                 = 326,
		KP7                 = 327,
		KP8                 = 328,
		KP9                 = 329,
		KPDecimal           = 330,
		KPDivide            = 331,
		KPMultiply          = 332,
		KPSubtract          = 333,
		KPAdd               = 334,
		KPEnter             = 335,
		KPEqual             = 336,

		LeftShift           = 340,
		LeftControl         = 341,
		LeftAlt             = 342,
		LeftSuper           = 343,
		RightShift          = 344,
		RightControl        = 345,
		RightAlt            = 346,
		RightSuper          = 347,
		Menu                = 348
	} Key;

	inline std::ostream& operator<<(std::ostream& os, KeyCode keyCode)
	{
		os << static_cast<int32_t>(keyCode);
		return os;
	}
}

// From glfw3.h
#define NURU_KEY_SPACE           ::NURU::Key::Space
#define NURU_KEY_APOSTROPHE      ::NURU::Key::Apostrophe    /* ' */
#define NURU_KEY_COMMA           ::NURU::Key::Comma         /* , */
#define NURU_KEY_MINUS           ::NURU::Key::Minus         /* - */
#define NURU_KEY_PERIOD          ::NURU::Key::Period        /* . */
#define NURU_KEY_SLASH           ::NURU::Key::Slash         /* / */
#define NURU_KEY_0               ::NURU::Key::D0
#define NURU_KEY_1               ::NURU::Key::D1
#define NURU_KEY_2               ::NURU::Key::D2
#define NURU_KEY_3               ::NURU::Key::D3
#define NURU_KEY_4               ::NURU::Key::D4
#define NURU_KEY_5               ::NURU::Key::D5
#define NURU_KEY_6               ::NURU::Key::D6
#define NURU_KEY_7               ::NURU::Key::D7
#define NURU_KEY_8               ::NURU::Key::D8
#define NURU_KEY_9               ::NURU::Key::D9
#define NURU_KEY_SEMICOLON       ::NURU::Key::Semicolon     /* ; */
#define NURU_KEY_EQUAL           ::NURU::Key::Equal         /* = */
#define NURU_KEY_A               ::NURU::Key::A
#define NURU_KEY_B               ::NURU::Key::B
#define NURU_KEY_C               ::NURU::Key::C
#define NURU_KEY_D               ::NURU::Key::D
#define NURU_KEY_E               ::NURU::Key::E
#define NURU_KEY_F               ::NURU::Key::F
#define NURU_KEY_G               ::NURU::Key::G
#define NURU_KEY_H               ::NURU::Key::H
#define NURU_KEY_I               ::NURU::Key::I
#define NURU_KEY_J               ::NURU::Key::J
#define NURU_KEY_K               ::NURU::Key::K
#define NURU_KEY_L               ::NURU::Key::L
#define NURU_KEY_M               ::NURU::Key::M
#define NURU_KEY_N               ::NURU::Key::N
#define NURU_KEY_O               ::NURU::Key::O
#define NURU_KEY_P               ::NURU::Key::P
#define NURU_KEY_Q               ::NURU::Key::Q
#define NURU_KEY_R               ::NURU::Key::R
#define NURU_KEY_S               ::NURU::Key::S
#define NURU_KEY_T               ::NURU::Key::T
#define NURU_KEY_U               ::NURU::Key::U
#define NURU_KEY_V               ::NURU::Key::V
#define NURU_KEY_W               ::NURU::Key::W
#define NURU_KEY_X               ::NURU::Key::X
#define NURU_KEY_Y               ::NURU::Key::Y
#define NURU_KEY_Z               ::NURU::Key::Z
#define NURU_KEY_LEFT_BRACKET    ::NURU::Key::LeftBracket   /* [ */
#define NURU_KEY_BACKSLASH       ::NURU::Key::Backslash     /* \ */
#define NURU_KEY_RIGHT_BRACKET   ::NURU::Key::RightBracket  /* ] */
#define NURU_KEY_GRAVE_ACCENT    ::NURU::Key::GraveAccent   /* ` */
#define NURU_KEY_WORLD_1         ::NURU::Key::World1        /* non-US #1 */
#define NURU_KEY_WORLD_2         ::NURU::Key::World2        /* non-US #2 */

/* Function keys */
#define NURU_KEY_ESCAPE          ::NURU::Key::Escape
#define NURU_KEY_ENTER           ::NURU::Key::Enter
#define NURU_KEY_TAB             ::NURU::Key::Tab
#define NURU_KEY_BACKSPACE       ::NURU::Key::Backspace
#define NURU_KEY_INSERT          ::NURU::Key::Insert
#define NURU_KEY_DELETE          ::NURU::Key::Delete
#define NURU_KEY_RIGHT           ::NURU::Key::Right
#define NURU_KEY_LEFT            ::NURU::Key::Left
#define NURU_KEY_DOWN            ::NURU::Key::Down
#define NURU_KEY_UP              ::NURU::Key::Up
#define NURU_KEY_PAGE_UP         ::NURU::Key::PageUp
#define NURU_KEY_PAGE_DOWN       ::NURU::Key::PageDown
#define NURU_KEY_HOME            ::NURU::Key::Home
#define NURU_KEY_END             ::NURU::Key::End
#define NURU_KEY_CAPS_LOCK       ::NURU::Key::CapsLock
#define NURU_KEY_SCROLL_LOCK     ::NURU::Key::ScrollLock
#define NURU_KEY_NUM_LOCK        ::NURU::Key::NumLock
#define NURU_KEY_PRINT_SCREEN    ::NURU::Key::PrintScreen
#define NURU_KEY_PAUSE           ::NURU::Key::Pause
#define NURU_KEY_F1              ::NURU::Key::F1
#define NURU_KEY_F2              ::NURU::Key::F2
#define NURU_KEY_F3              ::NURU::Key::F3
#define NURU_KEY_F4              ::NURU::Key::F4
#define NURU_KEY_F5              ::NURU::Key::F5
#define NURU_KEY_F6              ::NURU::Key::F6
#define NURU_KEY_F7              ::NURU::Key::F7
#define NURU_KEY_F8              ::NURU::Key::F8
#define NURU_KEY_F9              ::NURU::Key::F9
#define NURU_KEY_F10             ::NURU::Key::F10
#define NURU_KEY_F11             ::NURU::Key::F11
#define NURU_KEY_F12             ::NURU::Key::F12
#define NURU_KEY_F13             ::NURU::Key::F13
#define NURU_KEY_F14             ::NURU::Key::F14
#define NURU_KEY_F15             ::NURU::Key::F15
#define NURU_KEY_F16             ::NURU::Key::F16
#define NURU_KEY_F17             ::NURU::Key::F17
#define NURU_KEY_F18             ::NURU::Key::F18
#define NURU_KEY_F19             ::NURU::Key::F19
#define NURU_KEY_F20             ::NURU::Key::F20
#define NURU_KEY_F21             ::NURU::Key::F21
#define NURU_KEY_F22             ::NURU::Key::F22
#define NURU_KEY_F23             ::NURU::Key::F23
#define NURU_KEY_F24             ::NURU::Key::F24
#define NURU_KEY_F25             ::NURU::Key::F25

/* Keypad */
#define NURU_KEY_KP_0            ::NURU::Key::KP0
#define NURU_KEY_KP_1            ::NURU::Key::KP1
#define NURU_KEY_KP_2            ::NURU::Key::KP2
#define NURU_KEY_KP_3            ::NURU::Key::KP3
#define NURU_KEY_KP_4            ::NURU::Key::KP4
#define NURU_KEY_KP_5            ::NURU::Key::KP5
#define NURU_KEY_KP_6            ::NURU::Key::KP6
#define NURU_KEY_KP_7            ::NURU::Key::KP7
#define NURU_KEY_KP_8            ::NURU::Key::KP8
#define NURU_KEY_KP_9            ::NURU::Key::KP9
#define NURU_KEY_KP_DECIMAL      ::NURU::Key::KPDecimal
#define NURU_KEY_KP_DIVIDE       ::NURU::Key::KPDivide
#define NURU_KEY_KP_MULTIPLY     ::NURU::Key::KPMultiply
#define NURU_KEY_KP_SUBTRACT     ::NURU::Key::KPSubtract
#define NURU_KEY_KP_ADD          ::NURU::Key::KPAdd
#define NURU_KEY_KP_ENTER        ::NURU::Key::KPEnter
#define NURU_KEY_KP_EQUAL        ::NURU::Key::KPEqual

#define NURU_KEY_LEFT_SHIFT      ::NURU::Key::LeftShift
#define NURU_KEY_LEFT_CONTROL    ::NURU::Key::LeftControl
#define NURU_KEY_LEFT_ALT        ::NURU::Key::LeftAlt
#define NURU_KEY_LEFT_SUPER      ::NURU::Key::LeftSuper
#define NURU_KEY_RIGHT_SHIFT     ::NURU::Key::RightShift
#define NURU_KEY_RIGHT_CONTROL   ::NURU::Key::RightControl
#define NURU_KEY_RIGHT_ALT       ::NURU::Key::RightAlt
#define NURU_KEY_RIGHT_SUPER     ::NURU::Key::RightSuper
#define NURU_KEY_MENU            ::NURU::Key::Menu