#ifndef __DSEED_IO_INPUT_H__
#define __DSEED_IO_INPUT_H__

namespace dseed::io
{
	enum class keys : uint8_t
	{
		back = 0x08, tab = 0x09,
		clear = 0x0C, enter = 0x0D,
		shift = 0x10, ctrl = 0x11, menu = 0x12, pause = 0x13, capslock = 0x14, kana = 0x15, hangul = kana,
		junja = 0x17, final = 0x18, hanja = 0x19, kanji = hanja,
		escape = 0x1B, esc = escape, space = 0x20, page_up = 0x21, page_down = 0x22, end = 0x23, home = 0x24,
		left = 0x25, up = 0x26, right = 0x27, down = 0x28, print_screen = 0x2C, insert = 0x2D, del = 0x2E,
		n0 = '0', n1 = '1', n2 = '2', n3 = '3', n4 = '4', n5 = '5', n6 = '6', n7 = '7', n8 = '8', n9 = '9',
		a = 'A', b = 'B', c = 'C', d = 'D', keys_e = 'E', f = 'F', g = 'G', h = 'H', i = 'I', j = 'J',
		k = 'K', l = 'L', m = 'M', n = 'N', keys_o = 'O', p = 'P', q = 'Q', r = 'R', s = 'S', t = 'T',
		u = 'U', v = 'V', w = 'W', x = 'X', keys_y = 'Y', z = 'Z',
		left_win = 0x5B, left_command = left_win, right_win = 0x5C, right_command = right_win, context_menu = 0x5D, sleep = 0x5F,
		numpad0 = 0x60, numpad1 = 0x61, numpad2 = 0x62, numpad3 = 0x63, numpad4 = 0x64, numpad5 = 0x65,
		numpad6 = 0x66, numpad7 = 0x67, numpad8 = 0x68, numpad9 = 0x69,
		multiply = 0x6A, add = 0x6B, separate = 0x6C, subtract = 0x6D, decimal = 0x6E, divide = 0x6F,
		f1 = 0x70, f2 = 0x71, f3 = 0x72, f4 = 0x73, f5 = 0x74, f6 = 0x75, f7 = 0x76,
		f8 = 0x77, f9 = 0x78, f10 = 0x79, f11 = 0x7A, f12 = 0x7B,
		num_lock = 0x90, scroll_lock = 0x91,
		left_shift = 0xA0, right_shift = 0xA1, left_ctrl = 0xA2, right_ctrl = 0xA3, left_menu = 0xA4, right_menu = 0xA5,
		browser_back = 0xA6, browser_forward = 0xA7, browser_refresh = 0xA8, browser_stop = 0xA9,
		volume_mute = 0xAD, volume_down = 0xAE, volume_up = 0xAF,
		media_next = 0xB0, media_previous = 0xB1, media_stop = 0xB2, media_play_pause = 0xB3,
		semicolon = 0xBA, plus = 0xBB, comma = 0xBC, minus = 0xBD, period = 0xBE, slash = 0xBF, apostrophe = 0xC0,
		left_bracket = 0xDB, backslash = 0xDC, right_bracket = 0xDD, quote = 0xDE,
	};

	enum class mousebuttons : int8_t
	{
		no_button = 0,
		left = 1,
		right = 2,
		middle = 4,
		x_button1 = 8,
		x_button2 = 16,
	};

	enum class gamepadbuttons : int32_t
	{
		no_button = 0,

		a = 1,
		b = 2,
		x = 4,
		y = 8,

		dpad_left = 16,
		dpad_right = 32,
		dpad_up = 64,
		dpad_down = 128,

		back = 256,
		start = 512,

		left_bumper = 1024,
		right_bumper = 2048,

		left_thumbstick = 4096,
		right_thumbstick = 8192,
	};

	enum class touchstate : int8_t
	{
		released,
		pressed,
		moved,
	};

	struct keyboard_state
	{
	public:
		bool key_states[256];

	public:
		inline bool key_down(keys key) noexcept { return key_states[(int)key]; }
		inline bool key_up(keys key) noexcept { return !key_states[(int)key]; }
	};

	struct mouse_state
	{
	public:
		point2i position;
		float2 wheel;
		mousebuttons buttons;
		bool is_relative;

	public:
		inline mouse_state()
			: position(int32_min, int32_min), wheel(0, 0)
			, buttons(mousebuttons::no_button)
			, is_relative(false)
		{ }
	};

	struct mouse_feedback
	{
	public:
		bool lock_in_window;
		point2i move_position;
		bool set_relative;
		bool hide_cursor;
	};

	struct gamepad_state
	{
	public:
		float2 left_thumbstick, right_thumbstick;
		float left_trigger, right_trigger;
		gamepadbuttons buttons;
	};

	struct gamepad_feedback
	{
	public:
		float left_motor_speed, right_motor_speed;
	};

	struct touchpointer
	{
		int32_t pointerId;
		point2i position;
		touchstate state;
	};

	struct touchpanel_state
	{
	public:
		touchpointer pointers[256];
		size_t pointerCount;
	};

	struct accelerometer_state
	{
	public:
		float3 accelerometer;
	};

	struct gyroscope_state
	{
		float3 gyroscope;
	};

	struct gps_state
	{
		double longitude, latitude;
	};

	using inputdevicetype = int32_t;
	constexpr inputdevicetype inputdevicetype_unknown = -1;
	constexpr inputdevicetype inputdevicetype_keyboard = 0;
	constexpr inputdevicetype inputdevicetype_mouse = 1;
	constexpr inputdevicetype inputdevicetype_gamepad = 2;
	constexpr inputdevicetype inputdevicetype_touchpanel = 3;
	constexpr inputdevicetype inputdevicetype_accelerometer = 4;
	constexpr inputdevicetype inputdevicetype_gyroscope = 5;
	constexpr inputdevicetype inputdevicetype_gps = 6;

	using playerindex = int32_t;
	constexpr playerindex playerindex_player1 = 0;
	constexpr playerindex playerindex_player2 = 1;
	constexpr playerindex playerindex_player3 = 2;
	constexpr playerindex playerindex_player4 = 3;
	constexpr playerindex playerindex_player5 = 4;
	constexpr playerindex playerindex_player6 = 5;
	constexpr playerindex playerindex_player7 = 6;
	constexpr playerindex playerindex_player8 = 7;

	class DSEEDEXP inputdevice : public object
	{
	public:
		virtual inputdevicetype type() = 0;

	public:
		virtual error_t state(void* state, size_t state_size, playerindex playerindex = playerindex_player1) = 0;
		virtual error_t send_feedback(const void* feedback, size_t feedback_size, playerindex playerindex = playerindex_player1) = 0;
	};
}

#endif