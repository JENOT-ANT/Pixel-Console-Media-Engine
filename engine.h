// |Game Engine / GUI| -> API library

// Copyleft !(c) Public Domain with restrictions* (read more at the begining of source file or in documentation)

/*

coded by <ANT/JENOT/Antoni Świtaj>

	0 1 0
	0 0 1  <Hackers' Community>
	1 1 1



Copyright:
!(c) Public Domain with restrictions* below

*RESTRICTIONS:

> This |code / software / whatever You call it|
can be used only for purposes that are ETHICAL, MORAL, lawful and don't harm anyone and anything!

> If You are against those rules (You don't agree with them), You are not allowed
to use this |code / software / whatever You call it|!

0. Remember -> use it wisely :)
1. You do not need to provide any attribution (in other words, it's not mandatory), but I would really appreciate if you provided them (like: engine coded by ANT or something like that).
2. Contents done by You via this software is your :)
3. Just freedom.

WARNING!!!
NO WARRANTY:
You Are using this |code / software / whatever You call it|
on Your own RESPONSIBILITY!
There is no WARRANTY!!! (NOTHING, really! Don't even think about it, please...)

[If Your device crushes, don't tell me I haven't warned You, sorry...]

*/

#pragma once

#include <thread>
#include <string>
#include <fstream>
#include <vector>

#include <Windows.h>


namespace egn { // |Game Engine / GUI| API namspace

	const int BUTTONS_GUI_LIMIT		= 20;
	const int TEXTS_GUI_LIMIT		= 15;
	const int MAX_BUFFER_SIZE		= 230400;
	const int MAX_FONT_SIZE_X		= 5;
	const int MAX_FONT_SIZE_Y		= 5;
	const int MAX_SMALL_IMAGE_SIZE	= 625;
	//const int MAX_MIDIUM_IMAGE_SIZE	= 2500;
	//const int MAX_HUDGE_IMAGE_SIZE	= 10000;

	const int LEFT_MOUSE_KEY = 0;
	const int RIGHT_MOUSE_KEY = 1;
	const int MOUSE_WHEEL_UP = 2;
	const int MOUSE_WHEEL_DOWN = 3;
	const long long INTERCEPT_MOUSE_WHEEL_UP = 0x780000;//7864320
	const long long INTERCEPT_MOUSE_WHEEL_DOWN = 0xFF880000;//4287102976

	const int KEY_ESCAPE = 27;

	const int FULL_COLOR = 0;

	const int ALPHA		= -1;

	const int BLACK		= 0;
	const int D_BLUE	= 16;
	const int GREEN		= 32;
	const int L_BLUE	= 48;
	const int RED		= 64;
	const int PINK		= 80;
	const int SAND		= 96;
	const int WHITE		= 112;

	const int GRAY		= 128;
	const int BLUE		= 144;
	const int L_GREEN	= 160;
	const int L_L_BLUE	= 176;
	const int L_RED		= 192;
	const int L_PINK	= 208;
	const int YELLOW	= 224;
	const int L_WHITE	= 240;

	const int COLOR_AMOUNT = 17;
	int COLORS[COLOR_AMOUNT] = {ALPHA, BLACK, D_BLUE, GREEN, L_BLUE, RED, PINK, SAND, WHITE, GRAY, BLUE, L_GREEN, L_L_BLUE, L_RED, L_PINK, YELLOW, L_WHITE};

	const float C3 = (float)130.8128;
	const float D3 = (float)146.8324;
	const float E3 = (float)164.8138;
	const float F3 = (float)174.6141;
	const float G3 = (float)195.9977;
	//const float A3	= ;
	//const float H3	= ;
	//const float C4	= ;
	//const float D4	= ;
	//const float E4	= ;
	//const float F4	= ;
	//const float G4	= ;
	//const float A4	= ;
	//const float H4	= ;
	//const float C5	= ;


	struct Pair {
		int x;
		int y;
	};


	struct Picture {
		int* color_map;
		Pair SIZE = { 0, 0 };

	private:
		void load_image_size(std::fstream* image_file) {
			std::string line;

			std::getline(*image_file, line);
			SIZE.x = std::atoi(line.c_str());
			std::getline(*image_file, line);
			SIZE.y = std::atoi(line.c_str());
		}

	public:
		void set_size(Pair image_size) {
			//this function is used to set image size buffer by hand
			SIZE = image_size;
			color_map = new int[SIZE.y * SIZE.x];
		}

		void load(std::string file_path) {
			std::fstream image_file;
			std::string line;
			std::string number;
			int pixel_index = 0;

			image_file.open(file_path, std::ios::in);

			load_image_size(&image_file);
			color_map = new int[SIZE.y * SIZE.x];

			for (int i = 0; i < SIZE.y; i++) {
				std::getline(image_file, line);
				number = "";

				for (int i = 0; i <= line.size(); i++) {
					if (line[i] == '\t' or i == line.size()) {
						color_map[pixel_index] = std::atoi(number.c_str());
						number = "";
						pixel_index++;
					}
					else {
						number += line[i];
					}
					//if (line[i] != '\t') {
					//	color_map[pixel_index] = (int)line[i];
					//}
				}
			}

			image_file.close();
		}

	};


	struct Small_image {
		//this is not recomended image structure type, mayby use Picture structure instead?
	public:
		int color_map[MAX_SMALL_IMAGE_SIZE];
		Pair SIZE = { 0, 0 };

	private:
		void clear_color_map() {
			for (int i = 0; i < MAX_SMALL_IMAGE_SIZE; i++) {
				color_map[i] = 0;
			}
		}

	public:
		void load(std::string file_path) {
			std::fstream image_file;
			std::string line;
			std::string number;
			int j;
			clear_color_map();

			image_file.open(file_path, std::ios::in);

			// first line should be x image size
			std::getline(image_file, line);
			if (line != "") {
				SIZE.x = std::atoi(line.c_str());

				j = 0;
				number = "";
				line = "";

				while (std::getline(image_file, line)) {

					for (int i = 0; i <= line.size(); i++) { // i <= line.size()-1 + 1 (last time not to add anything to value, just to save last(line.size() - 1) value in the color map)
						if (line[i] == ' ' or i == line.size()) {
							color_map[j] = std::atoi(number.c_str());
							number = "";
							j++;
						}
						else {
							number += line[i];
						}
					}
					SIZE.y++;
				}
			}
			image_file.close();
		}
	};

	struct Font {
		//private:

	public:
		int letter_amount;
		//char letters[255];
		bool font_letters[256][MAX_FONT_SIZE_Y][MAX_FONT_SIZE_X];
		Pair SIZE;

	private:
		Pair convert_font_bit_index_to_2D(int font_bit_index) {
			return { font_bit_index % (SIZE.y), (int)(font_bit_index / (SIZE.x)) };
		}

		void clear_font_letters() {
			for (int i = 0; i < 256; i++) {
				for (int j = 0; j < MAX_FONT_SIZE_Y; j++) {
					for (int k = 0; k < MAX_FONT_SIZE_X; k++) {
						font_letters[i][j][k] = false;
					}
				}
			}
		}

	public:
		void load(std::string file_path) {
			std::fstream font_file;
			std::string line;
			char expected_char;
			//char normal_letter;
			bool reading_font_letter;
			int letter_index;
			int font_bit_index;
			Pair font_bit_index_2D;

			clear_font_letters();
			font_file.open(file_path);

			std::getline(font_file, line);
			SIZE.x = std::atoi(line.c_str());
			std::getline(font_file, line);
			SIZE.y = std::atoi(line.c_str());

			expected_char = '#';
			reading_font_letter = false;
			letter_index = 0;
			font_bit_index = 0;

			while (std::getline(font_file, line)) {
				for (int i = 0; i < line.size(); i++) {
					if (reading_font_letter == true) {
						font_bit_index_2D = convert_font_bit_index_to_2D(font_bit_index);

						if (line[i] == '0') {
							font_letters[letter_index][font_bit_index_2D.y][font_bit_index_2D.x] = false;
							font_bit_index++;
						}
						else if (line[i] == '1') {
							font_letters[letter_index][font_bit_index_2D.y][font_bit_index_2D.x] = true;
							font_bit_index++;
						}
						else if (line[i] == '}' or font_bit_index >= SIZE.x * SIZE.y) {
							reading_font_letter = false;
							expected_char = '#';
							break;
						}
					}
					else if (line[i] == expected_char) {
						if (line[i] == '#' and i < line.size() - 1) {
							if ((int)line[i + 1] <= 255) {
								letter_index = (int)line[i + 1];
								//letters[letter_index] = line[i + 1];
								expected_char = '{';
							}
						}
						else if (line[i] == '{') {
							reading_font_letter = true;
							font_bit_index = 0;
						}
					}
				}
			}

			font_file.close();
		}
	};

	class Textbox {
	
	public:
		std::string TEXT;
		Pair POSITION = { 0, 0 };
		int TEXT_COLOR;
		int BACKGROUND_COLOR;
		
		void create_textbox(std::string textbox_text, Pair textbox_position, int textbox_text_color = WHITE, int textbox_background_color = ALPHA) {
			TEXT = textbox_text;
			POSITION = textbox_position;
			TEXT_COLOR = textbox_text_color;
			BACKGROUND_COLOR = textbox_background_color;
		}
	};

	class Button {
	private:
		bool state;

	public:
		Pair SIZE = { 10, 10 };
		Pair POSITION = { 0, 0 };
		int COLOR = WHITE;
		std::string TEXT;
		int TEXT_COLOR;

	private:
		void release_button() {
			state = false;
		}

	public:
		bool mouse_over(Pair mouse_position) {
			if ((mouse_position.x >= POSITION.x and mouse_position.x <= POSITION.x + SIZE.x - 1) and (mouse_position.y >= POSITION.y and mouse_position.y <= POSITION.y + SIZE.y - 1)) {
				return true;
			}
			else {
				return false;
			}
		}

		void create_button(Pair button_size, Pair button_position, int button_color, std::string button_text, int text_color) {
			SIZE = button_size;
			POSITION = button_position;
			COLOR = button_color;
			TEXT = button_text;
			TEXT_COLOR = text_color;

			state = false;
		}

		void update_state(Pair mouse_position, int mouse_key) {
			if (mouse_key == LEFT_MOUSE_KEY) {
				if (mouse_over(mouse_position)) {
					state = true;
				}
				else {
					state = false;
				}
			}
		}

		bool get_state() {
			bool output = state;
			release_button();
			return output;
		}

	
	};

	class Inputbox {
	public:
		Button box;
		bool focus;

		void init(Pair inputbox_size, Pair input_box_position, int input_box_color, int text_color) {
			box.create_button(inputbox_size, input_box_position, input_box_color,"\0", text_color);
		}

		void update_state(Pair mouse_position, int mouse_key) {
			if (mouse_key == LEFT_MOUSE_KEY) {
				if (box.mouse_over(mouse_position) == true) {
					focus = true;
				}
				else {
					focus = false;
				}
			}
		}

		void add_input(char letter) {
			box.TEXT += letter;
		}
	};

	class GUI_page {

	public:
		Button buttons[BUTTONS_GUI_LIMIT];
		int buttons_amount;
		
		Textbox texts[TEXTS_GUI_LIMIT];
		int texts_amount;

	public:
		void init() {
			buttons_amount = 0;
			texts_amount = 0;
		}

		void update_buttons_state(Pair mouse_position, int mouse_key) {
			for (int i = 0; i < buttons_amount; i++) {
				buttons[i].update_state(mouse_position, mouse_key);
			}
		}

		void add_text(std::string textbox_text, Pair textbox_position, int textbox_text_color = WHITE, int textbox_background_color = ALPHA) {
			texts[texts_amount].create_textbox(textbox_text, textbox_position, textbox_text_color, textbox_background_color);
			texts_amount++;
		}

		void add_button(Pair button_size, Pair button_position, int button_color, std::string button_text, int text_color) {

			if (buttons_amount < BUTTONS_GUI_LIMIT) {
				buttons[buttons_amount].create_button(button_size, button_position, button_color, button_text, text_color);
				buttons_amount++;
			}
		}
	};

	// -----DISPLAY-----

	class Display {

		// atributtes:
	private:
		HANDLE handle;
		DWORD written_chars;
		WCHAR char_buffer[MAX_BUFFER_SIZE];
		WORD attribute_buffer[MAX_BUFFER_SIZE];
		SMALL_RECT screen_rectangle;
		COORD screen_coordinates;
		int* to_clear_indexs;
		int clear_indexes_amount;
		//bool update;

	public:
		Pair SIZE;
		int RESOLUTION;

		// methods:

	private:
		int convert_2D_to_buffer_coordinates(Pair position, Pair limit) {
			if (position.x < limit.x and position.y < limit.y and position.x >= 0 and position.y >= 0) {
				return position.y * SIZE.x + position.x;
			}
			else {
				return -1;
			}
		}

	public:
		void reset_window_size() {
			std::string command = "mode con: cols=";
			command += std::to_string((long double)screen_coordinates.X);
			command += " lines=";
			command += std::to_string((long double)screen_coordinates.Y);

			SetConsoleWindowInfo(handle, (BOOL)true, &screen_rectangle);
			SetConsoleScreenBufferSize(handle, screen_coordinates);

			SetConsoleActiveScreenBuffer(handle);
			system(command.c_str());
			//system("mode con: cols=100 lines=100");
			//SetConsoleWindowInfo(handle, (BOOL)true, &screen_rectangle);
		}

		void init_buffer(Pair size) {
			SIZE = size;
			RESOLUTION = SIZE.x * SIZE.y;

			to_clear_indexs = new int[RESOLUTION * 2];//*2 for safty to not overflow this buffer
			clear_indexes_amount = 0;
			//update = false;

			handle = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 3, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);

			//std::system("mode con: cols=120 lines=120");

			screen_rectangle = { 0, 0, (SHORT)(SIZE.x - 1), (SHORT)(SIZE.y - 1) };
			screen_coordinates = { (SHORT)(SIZE.x), (SHORT)(SIZE.y) };

			reset_window_size();
			//SetConsoleWindowInfo(handle, (WINBOOL)true, &screen_rectangle);
			//SetConsoleScreenBufferSize(handle, screen_coordinates);

			//SetConsoleActiveScreenBuffer(handle);

			//SetConsoleDisplayMode(handle, CONSOLE_WINDOWED_MODE, &screen_coordinates);


			for (int i = 0; i < RESOLUTION; i++) {
				char_buffer[i] = (char)0;
			}
			WriteConsoleOutputCharacter(handle, char_buffer, (DWORD)RESOLUTION, { 0, 0 }, &written_chars);
		}

		void clear(int color = 0) {
			//for (int i = 0; i < RESOLUTION; i++) {
			//	attribute_buffer[i] = (WORD)color;
			//}
			
			for (int i = 0; i < clear_indexes_amount; i++) {
				attribute_buffer[to_clear_indexs[i]] = (WORD)color;
			}
			clear_indexes_amount = 0;
			
		}

		void draw_point(Pair position, int color, int background_color = BLACK) {
			
			//if (position.x >= 0 and position.y >= 0) {
			
			int screen_buffer_index = convert_2D_to_buffer_coordinates(position, { SIZE.x, SIZE.y });

			if (screen_buffer_index != -1) {
				if (color != ALPHA and attribute_buffer[screen_buffer_index] != (WORD)color) { // -1 color is transparent and -1 index mean that pixel is outside the buffer
					if (color != background_color and attribute_buffer[screen_buffer_index] == background_color) {
						to_clear_indexs[clear_indexes_amount] = screen_buffer_index;
						clear_indexes_amount++;
					}
					attribute_buffer[screen_buffer_index] = (WORD)color;
					//update = true;
				}
			}
			//}
		}

		void draw_rectangle(Pair up_row_position, Pair down_row_position, int color) {
			for (int y = up_row_position.y; y <= down_row_position.y; y++) {
				for (int x = up_row_position.x; x <= down_row_position.x; x++) {
					draw_point({ x, y }, color);
				}
			}
		}

		void draw_image(Pair up_row_position, Pair size, int* texture, int scale = 1) {
			Pair down_row_position = { up_row_position.x + (size.x * scale), up_row_position.y + (size.y * scale) };
			int i = 0;
			for (int y = up_row_position.y; y < down_row_position.y; y += scale) {
				for (int x = up_row_position.x; x < down_row_position.x; x += scale) {
					//draw_point({ x, y }, texture[i]);
					draw_rectangle({ x, y }, {x + scale - 1, y + scale - 1}, texture[i]);
					i++;
				}
			}
		}

		void draw_picture(Picture* picture, Pair up_row_position, int scale = 1) {
			draw_image(up_row_position, picture->SIZE, picture->color_map, scale);
		}

		void draw_letter(Pair position, char letter, Font* font, int background, int color) {
			if (letter != '\0') {
				for (int i = 0; i < font->SIZE.y; i++) {
					for (int j = 0; j < font->SIZE.x; j++) {
						if (font->font_letters[(int)letter][i][j] == true) {
							draw_point({ position.x + j, position.y + i }, color);
						}
						else {
							draw_point({ position.x + j, position.y + i }, background);
						}
					}
				}
			}
		}

		void draw_text(Pair position, std::string text, Font* font, int background, int color) {
			for (int i = 0; i < text.size(); i++) {
				draw_letter({ position.x + (font->SIZE.x + 1) * i, position.y }, text[i], font, background, color);
			}
		}

		void draw_textbox(Textbox* textbox, Font* font) {
			draw_text(textbox->POSITION, textbox->TEXT, font, textbox->BACKGROUND_COLOR, textbox->TEXT_COLOR);
		}

		void draw_button(Button* button, Font* font) {
			draw_rectangle(button->POSITION, { button->POSITION.x + button->SIZE.x - 1, button->POSITION.y + button->SIZE.y - 1 }, button->COLOR);
			draw_text({ button->POSITION.x + 1, button->POSITION.y + 1 }, button->TEXT, font, ALPHA, button->TEXT_COLOR);
		}

		void draw_GUI_page(GUI_page* page, Font* font) {
			for (int i = 0; i < page->buttons_amount; i++) {
				draw_button(&page->buttons[i], font);
			}
			for (int i = 0; i < page->texts_amount; i++) {
				draw_textbox(&page->texts[i], font);
			}
		}


		void render() {
			WriteConsoleOutputAttribute(handle, attribute_buffer, (DWORD)RESOLUTION, { 0, 0 }, &written_chars);
		}

	};


	void intercept_input(HANDLE* handle, Pair* mouse_position, int* mouse_key, int* key, int time_to_update) {
		bool input_on = true;
		INPUT_RECORD input_info;
		DWORD read_input;

		while (input_on) {
			Sleep(time_to_update);
			ReadConsoleInput(*handle, &input_info, 1, &read_input);

			for (int i = 0; i < (int)read_input; i++) {
				if (input_info.EventType == MOUSE_EVENT) {
					*mouse_position = { (int)input_info.Event.MouseEvent.dwMousePosition.X, (int)input_info.Event.MouseEvent.dwMousePosition.Y };
					
					if (input_info.Event.MouseEvent.dwEventFlags == MOUSE_WHEELED) {
						if (input_info.Event.MouseEvent.dwButtonState == INTERCEPT_MOUSE_WHEEL_UP) {
							*mouse_key = MOUSE_WHEEL_UP;
						}
						else {
							*mouse_key = MOUSE_WHEEL_DOWN;
						}
					}
					else if (input_info.Event.MouseEvent.dwButtonState == FROM_LEFT_1ST_BUTTON_PRESSED and *mouse_key == -1) {
						*mouse_key = LEFT_MOUSE_KEY;
					}
					else if (input_info.Event.MouseEvent.dwButtonState == RIGHTMOST_BUTTON_PRESSED and *mouse_key == -1) {
						*mouse_key = RIGHT_MOUSE_KEY;
					}
				}
				else if (input_info.EventType == KEY_EVENT) {
					if ((bool)input_info.Event.KeyEvent.bKeyDown and *key == -1) {
						*key = (int)input_info.Event.KeyEvent.uChar.AsciiChar;
					}
				}
			}
		}
	}

	class Devices {

		// atributtes:
	private:
		HANDLE handle;
		Pair mouse_position = {0, 0};
		int mouse_key = -1;
		int key = -1;
		int time_to_update;

		std::thread input_thread;

		//public:

		// methods:
	private:
		void keyboard_release() {
			key = -1;
		}
		void mouse_key_release() {
			mouse_key = -1;
		}

	public:
		void init(int accuracy = 1) {
			handle = GetStdHandle(STD_INPUT_HANDLE);
			SetConsoleMode(handle, 16 | ENABLE_WINDOW_INPUT);
			time_to_update = accuracy;
			input_thread = std::thread(intercept_input, &handle, &mouse_position, &mouse_key, &key, time_to_update);
			mouse_position = { 0, 0 };
			mouse_key = -1;
			key = -1;
		}

		Pair get_mouse_position() {
			return mouse_position;
		}

		int get_mouse_key() {
			int output = mouse_key;
			mouse_key_release();
			return output;
		}

		int get_key() {
			int output = key;
			keyboard_release();
			return output;
		}

		void shut_down() {
			input_thread.detach();
		}
	};


	class Sound {


		void play() {

		}
		void load() {

		}
	};

	namespace tls {
		//namespace for tiles class and other future staff mayby
		
		const int MAX_TILES_AMOUNT = 10;
		//const int TILE_SIZE = 20;
		const std::string ASSETS_PATH = "./assets";
		const std::string IMAGES_PATH = ASSETS_PATH + "/images/";

		class Tile {

		public:
			std::string id;
			int frames;
			int frame_index;

		private:
			Picture* pictures;
			Display* display;

		public:
			void init(std::string name, std::vector<std::string>image_files_names, Display* screen) {
				id = name;
				frames = (int)image_files_names.size();

				pictures = new Picture[frames];

				for (int i = 0; i < frames; i++) {
					pictures[i].load(IMAGES_PATH + image_files_names[i] + ".pict");
				}

				display = screen;
				frame_index = 0;
			}

			void render(egn::Pair position, int scale) {
				display->draw_picture(&pictures[frame_index], position, scale);
			}
		};
	}
}
