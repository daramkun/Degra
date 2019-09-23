#ifndef __DSEED_HID_H__
#define __DSEED_HID_H__

namespace dseed
{
	enum keys_t : uint8_t
	{
		keys_back = 0x08, keysab = 0x09,
		keys_clear = 0x0C, keys_return = 0x0D, keys_enter = keys_return,
		keys_shift = 0x10, keys_ctrl = 0x11, keys_menu = 0x12, keys_pause = 0x13, keys_capslock = 0x14, keys_kana = 0x15, keys_hangul = keys_kana,
		keys_junja = 0x17, keys_final = 0x18, keys_hanja = 0x19, keys_kanji = keys_hanja,
		keys_escape = 0x1B, keys_esc = keys_escape, keys_space = 0x20, keys_page_up = 0x21, keys_page_down = 0x22, keys_end = 0x23, keys_home = 0x24,
		keys_left = 0x25, keys_up = 0x26, keys_right = 0x27, keys_down = 0x28, keys_print_screen = 0x2C, keys_insert = 0x2D, keys_delete = 0x2E,
		keys_0 = '0', keys_1 = '1', keys_2 = '2', keys_3 = '3', keys_4 = '4', keys_5 = '5', keys_6 = '6', keys_7 = '7', keys_8 = '8', keys_9 = '9',
		keys_a = 'A', keys_b = 'B', keys_c = 'C', keys_d = 'D', keys_e = 'E', keys_f = 'F', keys_g = 'G', keys_h = 'H', keys_i = 'I', keys_j = 'J',
		keys_k = 'K', keys_l = 'L', keys_m = 'M', keys_n = 'N', keys_o = 'O', keys_p = 'P', keys_q = 'Q', keys_r = 'R', keys_s = 'S', keys = 'T',
		keys_u = 'U', keys_v = 'V', keys_w = 'W', keys_x = 'X', keys_y = 'Y', keys_z = 'Z',
		keys_left_win = 0x5B, keys_left_command = keys_left_win, keys_right_win = 0x5C, keys_right_command = keys_right_win, keys_context_menu = 0x5D, keys_sleep = 0x5F,
		keys_numpad0 = 0x60, keys_numpad1 = 0x61, keys_numpad2 = 0x62, keys_numpad3 = 0x63, keys_Numpad4 = 0x64, keys_Numpad5 = 0x65,
		keys_numpad6 = 0x66, keys_numpad7 = 0x67, keys_numpad8 = 0x68, keys_numpad9 = 0x69,
		keys_multiply = 0x6A, keys_add = 0x6B, keys_separate = 0x6C, keys_subtract = 0x6D, keys_decimal = 0x6E, keys_divide = 0x6F,
		keys_f1 = 0x70, keys_f2 = 0x71, keys_f3 = 0x72, keys_f4 = 0x73, keys_f5 = 0x74, keys_f6 = 0x75, keys_f7 = 0x76,
		keys_f8 = 0x77, keys_f9 = 0x78, keys_f10 = 0x79, keys_f11 = 0x7A, keys_f12 = 0x7B,
		keys_num_lock = 0x90, keys_scroll_lock = 0x91,
		keys_left_shift = 0xA0, keys_right_shift = 0xA1, keys_left_ctrl = 0xA2, keys_right_ctrl = 0xA3, keys_left_menu = 0xA4, keys_right_menu,
		keys_browser_back = 0xA6, keys_browser_forward = 0xA7, keys_browser_refresh = 0xA8, keys_browser_stop = 0xA9,
		keys_volume_mute = 0xAD, keys_volume_down = 0xAE, keys_volume_up = 0xAF,
		keys_media_next = 0xB0, keys_media_previous = 0xB1, keys_media_stop = 0xB2, keys_media_play_pause = 0xB3,
		keys_semicolon = 0xBA, keys_plus = 0xBB, keys_comma = 0xBC, keys_minus = 0xBD, keys_period = 0xBE, keys_slash = 0xBF, keys_Apostrophe = 0xC0,
		keys_left_bracket = 0xDB, keys_backslash = 0xDC, keys_right_bracket = 0xDD, keys_quote = 0xDE,
	};

	enum mousebuttons_t : int8_t
	{
		mousebuttons_no_button = 0,
		mousebuttons_left = 1,
		mousebuttons_right = 2,
		mousebuttons_middle = 4,
		mousebuttons_x_button1 = 8,
		mousebuttons_x_button2 = 16,
	};

	enum gamepadbuttons_t : int32_t
	{
		gamepadbuttons_no_button = 0,

		gamepadbuttons_a = 1,
		gamepadbuttons_b = 2,
		gamepadbuttons_x = 4,
		gamepadbuttons_y = 8,

		gamepadbuttons_dpad_left = 16,
		gamepadbuttons_dpad_right = 32,
		gamepadbuttons_dpad_up = 64,
		gamepadbuttons_dpad_down = 128,

		gamepadbuttons_back = 256,
		gamepadbuttons_start = 512,

		gamepadbuttons_left_bumper = 1024,
		gamepadbuttons_right_bumper = 2048,

		gamepadbuttons_left_thumbstick = 4096,
		gamepadbuttons_right_thumbstick = 8192,
	};

	enum touchstate_t : int8_t
	{
		touchstate_released,
		touchstate_pressed,
		touchstate_moved,
	};

	struct keyboard_state
	{
	public:
		bool key_states[256];

	public:
		inline bool key_down (keys_t key) noexcept { return key_states[(int)key]; }
		inline bool key_up (keys_t key) noexcept { return !key_states[(int)key]; }
	};

	struct mouse_state
	{
	public:
		point2i position;
		float2 wheel;
		mousebuttons_t buttons;
		bool is_relative;

	public:
		inline mouse_state ()
			: position (int32_min, int32_min), wheel (0, 0)
			, buttons (mousebuttons_no_button)
			, is_relative (false)
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
		gamepadbuttons_t buttons;
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
		touchstate_t state;
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

	using inputdevice_t = int32_t;
	constexpr inputdevice_t inputdevice_unknown = -1;
	constexpr inputdevice_t inputdevice_keyboard = 0;
	constexpr inputdevice_t inputdevice_mouse = 1;
	constexpr inputdevice_t inputdevice_gamepad = 2;
	constexpr inputdevice_t inputdevice_touchpanel = 3;
	constexpr inputdevice_t inputdevice_accelerometer = 4;
	constexpr inputdevice_t inputdevice_gyroscope = 5;
	constexpr inputdevice_t inputdevice_gps = 6;

	using playerindex_t = int32_t;
	constexpr playerindex_t playerindex_player1 = 0;
	constexpr playerindex_t playerindex_player2 = 1;
	constexpr playerindex_t playerindex_player3 = 2;
	constexpr playerindex_t playerindex_player4 = 3;
	constexpr playerindex_t playerindex_player5 = 4;
	constexpr playerindex_t playerindex_player6 = 5;
	constexpr playerindex_t playerindex_player7 = 6;
	constexpr playerindex_t playerindex_player8 = 7;

	class DSEEDEXP inputdevice : public object
	{
	public:
		virtual inputdevice_t type () = 0;

	public:
		virtual error_t state (void* state, size_t state_size, playerindex_t playerindex = playerindex_player1) = 0;
		virtual error_t send_feedback (const void* feedback, size_t feedback_size, playerindex_t playerindex = playerindex_player1) = 0;
	};
}

#endif