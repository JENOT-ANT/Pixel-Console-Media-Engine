#include "engine.h"

using namespace egn;

const WORD BASIC_FREQUENCY = 20;
const WORD MENU_FREQUENCY = 120;

const Pair DISPLAY_SIZE = { 80, 80 };
const Pair MAX_PICTURE_SIZE = { DISPLAY_SIZE.x % 101 - 50, DISPLAY_SIZE.y % 101 - 50 };
const Pair BUTTON_SIZE = { 24, 7 };
const Pair TEXT_SIZE = { 28, 8 };

const std::string FILE_PATH = "Your-Picture.pict";

Display display;
Devices input;
Font font;

class Canva{
public:
	Picture picture;
	int scale;
	void prepare(int fill_color = WHITE) {
		int buffer_size = picture.SIZE.x * picture.SIZE.y;
		
		for (int i = 0; i < buffer_size; i++) {
			picture.color_map[i] = fill_color;
		}
	}

	void create(int canva_scale) {
		scale = canva_scale;
	}
	void save(std::string file_name) {
		std::fstream picture_file;
		int index = 0;

		picture_file.open(file_name, std::ios::out);
		
		picture_file << picture.SIZE.x << '\n' << picture.SIZE.y << '\n';
		
		for (int i = 0; i < picture.SIZE.y; i++) {
			for (int j = 0; j < picture.SIZE.x; j++) {
				picture_file << picture.color_map[index];
				if (j != picture.SIZE.x - 1) {
					picture_file << '\t';
				}
				index++;
			}
			picture_file << '\n';
		}

		picture_file.close();
	}
	void load(std::string file_name) {
		picture.load(file_name);
	}
	void render() {
		display.draw_picture(&picture, { 0, 0 }, scale);
	 }
};


class Brush {
public:
	Pair position;
	int color;
	int size;
private:

	int get_buffer_index_position(Pair image_size, int scale) {
		return position.y / scale * image_size.x + position.x / scale;
	}

public:
	void init(Pair brush_position, int brush_color, int brush_size) {
		color = brush_color;
		position = brush_position;
		size = brush_size;
	}
	
	void draw(int* color_map, Pair image_size, int scale) {
		color_map[get_buffer_index_position(image_size, scale)] = color;
	}
	
	void set_position(Pair accurate_position, int scale) {
		position = { (accurate_position.x / scale) * scale, (accurate_position.y / scale) * scale };
	}

	void render(int scale, Picture* picture, int background_color = BLACK, int opposite_color = WHITE, int opposite_color_2 = L_PINK) {
		int buffer_position;
		int render_color;
		scale--;

		if (color != ALPHA) {
			display.draw_rectangle(position, { position.x + (size * scale), position.y + (size * scale)}, color);
		}
		else {
			buffer_position = get_buffer_index_position(picture->SIZE, scale + 1);
			
			if (picture->color_map[buffer_position] == background_color) {
				render_color = opposite_color;
			}
			else if (picture->color_map[buffer_position] == ALPHA) {
				render_color = opposite_color_2;
			}
			else {
				render_color = background_color;
			}
			
			display.draw_rectangle({ position.x + (scale + 1) / 2, position.y }, { position.x + (size * scale), position.y + (size * scale) - (scale + 1) / 2 }, render_color);
			display.draw_rectangle({ position.x, position.y + (scale + 1) / 2 }, { position.x + (size * scale) - (scale + 1) / 2, position.y + (size * scale) }, render_color);
		}
	}
};


bool confirmation_loop() {
	bool loop_on;
	GUI_page gui;

	gui.init();
	gui.add_text("OVERWRITTE FILE?", { (int)(DISPLAY_SIZE.x / 2 - 8 * (MAX_FONT_SIZE_X + 1)), (int)(DISPLAY_SIZE.y / 10) }, SAND);
	gui.add_button({ 3 * (MAX_FONT_SIZE_X + 1), MAX_FONT_SIZE_Y + 2 }, { (int)(DISPLAY_SIZE.x / 2 - 4 * (MAX_FONT_SIZE_X + 1)), (int)(DISPLAY_SIZE.y / 10 + MAX_FONT_SIZE_Y + 4) }, GRAY, "YES", GREEN);
	gui.add_button({ 2 * (MAX_FONT_SIZE_X + 1), MAX_FONT_SIZE_Y + 2 }, { (int)(DISPLAY_SIZE.x / 2 + 1), (int)(DISPLAY_SIZE.y / 10 + MAX_FONT_SIZE_Y + 4) }, GRAY, "NO", RED);


	display.clear();
	display.draw_GUI_page(&gui, &font);
	display.render();

	loop_on = true;
	while (loop_on){
		Sleep(MENU_FREQUENCY);
		gui.update_buttons_state(input.get_mouse_position(), input.get_mouse_key());

		if (gui.buttons[0].get_state() == true) {
			loop_on = false;
		}
		else if (gui.buttons[1].get_state() == true) {
			loop_on = false;
			return false;
		}
	}
	return true;
}

bool new_file_creator_loop(Canva* canva) {
	GUI_page gui;
	int key;
	int mouse_key;
	bool loop_on;
	Pair cursor_position;
	Pair canva_size = {20, 20};
	char conversion_buffer[4];
	bool update = true;

	gui.init();
	gui.add_text("IMAGE SIZE", { (int)(DISPLAY_SIZE.x / 2 - TEXT_SIZE.x), (int)(DISPLAY_SIZE.y / 10) });

	gui.add_text("X: ", { (int)(DISPLAY_SIZE.x / 2 - TEXT_SIZE.x), (int)(DISPLAY_SIZE.y / 10 + TEXT_SIZE.y) });
	gui.add_button({ MAX_FONT_SIZE_X + 1, MAX_FONT_SIZE_Y + 2 }, { (int)(DISPLAY_SIZE.x / 2), (int)(DISPLAY_SIZE.y / 10 + TEXT_SIZE.y) }, SAND, "<", L_BLUE);
	gui.add_text("00", { (int)(DISPLAY_SIZE.x / 2 + MAX_FONT_SIZE_X + 2), (int)(DISPLAY_SIZE.y / 10 + TEXT_SIZE.y) });
	gui.add_button({ MAX_FONT_SIZE_X + 1, MAX_FONT_SIZE_Y + 2 }, { (int)(DISPLAY_SIZE.x / 2 + 3 * (MAX_FONT_SIZE_X + 2)), (int)(DISPLAY_SIZE.y / 10 + TEXT_SIZE.y) }, SAND, ">", L_BLUE);

	gui.add_text("Y: ", { (int)(DISPLAY_SIZE.x / 2 - TEXT_SIZE.x), (int)(DISPLAY_SIZE.y / 10 + 2 * TEXT_SIZE.y) });
	gui.add_button({ MAX_FONT_SIZE_X + 1, MAX_FONT_SIZE_Y + 2}, { (int)(DISPLAY_SIZE.x / 2), (int)(DISPLAY_SIZE.y / 10 + 2 * TEXT_SIZE.y) }, SAND, "<", L_BLUE);
	gui.add_text("00", { (int)(DISPLAY_SIZE.x / 2 + MAX_FONT_SIZE_X + 2), (int)(DISPLAY_SIZE.y / 10 + 2 * TEXT_SIZE.y) });
	gui.add_button({ MAX_FONT_SIZE_X + 1, MAX_FONT_SIZE_Y + 2}, { (int)(DISPLAY_SIZE.x / 2 + 3 * (MAX_FONT_SIZE_X + 2)), (int)(DISPLAY_SIZE.y / 10 + 2 * TEXT_SIZE.y) }, SAND, ">", L_BLUE);

	gui.add_button({ 2 * (MAX_FONT_SIZE_X + 1), (MAX_FONT_SIZE_Y + 2) }, { (int)(DISPLAY_SIZE.x / 2), (int)(DISPLAY_SIZE.y / 10 + 4 * TEXT_SIZE.y) }, GREEN, "OK", YELLOW);


	loop_on = true;
	
	while (loop_on){
		Sleep(MENU_FREQUENCY);
		key = input.get_key();
		mouse_key = input.get_mouse_key();
		cursor_position = input.get_mouse_position();

		if (key == 27) {
			loop_on = false;
			return false;
		}
		
		gui.update_buttons_state(cursor_position, mouse_key);

		if (gui.buttons[0].get_state() == true and canva_size.x > 1) {
			canva_size.x--;
			update = true;
		}
		else if (gui.buttons[1].get_state() == true and canva_size.x < MAX_PICTURE_SIZE.x) {
			canva_size.x++;
			update = true;
		}
		else if (gui.buttons[2].get_state() == true and canva_size.y > 1) {
			canva_size.y--;
			update = true;
		}
		else if (gui.buttons[3].get_state() == true and canva_size.y < MAX_PICTURE_SIZE.y) {
			canva_size.y++;
			update = true;
		}
		else if (gui.buttons[4].get_state() == true) {
			loop_on = false;
			canva->picture.set_size(canva_size);
			canva->prepare();
		}


		if (update) {
			update = false;
			
			_itoa_s(canva_size.x, conversion_buffer, 10);
			if (canva_size.x < 10) {
				conversion_buffer[1] = (char)0;
			}
			else {
				conversion_buffer[2] = (char)0;
			}
			gui.texts[2].TEXT = std::string(conversion_buffer);


			_itoa_s(canva_size.y, conversion_buffer, 10);
			if (canva_size.y < 10) {
				conversion_buffer[1] = (char)0;
			}
			else {
				conversion_buffer[2] = (char)0;
			}
			gui.texts[4].TEXT = std::string(conversion_buffer);
			
			display.clear();

			display.draw_GUI_page(&gui, &font);
			display.render();
		}
	}
	return true;
}


bool menu_loop(Canva* canva) {
	int key;
	int mouse_key;
	//bool program_on;
	bool menu_on;

	Pair cursor_position;
	GUI_page main_menu;

	main_menu.init();
	main_menu.add_button(BUTTON_SIZE, { int((DISPLAY_SIZE.x / 2) - int(BUTTON_SIZE.x / 2)), int(DISPLAY_SIZE.y / 10) }, GRAY, "NEW", GREEN);
	main_menu.add_button(BUTTON_SIZE, { int((DISPLAY_SIZE.x / 2) - int(BUTTON_SIZE.x / 2)), int(DISPLAY_SIZE.y / 10) + BUTTON_SIZE.y + 1 }, GRAY, "SAVE", GREEN);
	main_menu.add_button(BUTTON_SIZE, { int((DISPLAY_SIZE.x / 2) - int(BUTTON_SIZE.x / 2)), int(DISPLAY_SIZE.y / 10) + 2 * (BUTTON_SIZE.y + 1) }, GRAY, "LOAD", GREEN);
	main_menu.add_button(BUTTON_SIZE, { int((DISPLAY_SIZE.x / 2) - int(BUTTON_SIZE.x / 2)), int(DISPLAY_SIZE.y / 10) + 3 * (BUTTON_SIZE.y + 1) }, GRAY, "EXIT", GREEN);
	

	menu_on = true;

	display.clear();

	display.draw_GUI_page(&main_menu, &font);
	display.render();

	while (menu_on) {
		Sleep(MENU_FREQUENCY);
		key = input.get_key();
		mouse_key = input.get_mouse_key();
		cursor_position = input.get_mouse_position();

		if (key == 27) {
			menu_on = false;
		}

		main_menu.update_buttons_state(cursor_position, mouse_key);
		if (main_menu.buttons[3].get_state() == true) {
			menu_on = false;
			return false;
		}
		else if(main_menu.buttons[0].get_state() == true) {
			if (new_file_creator_loop(canva) == true) {
				menu_on = false;
			}
		}
		else if (main_menu.buttons[1].get_state() == true) {
			if (confirmation_loop() == true) {
				canva->save(FILE_PATH);
				menu_on = false;
			}
			else {
				display.clear();

				display.draw_GUI_page(&main_menu, &font);
				display.render();
			}
		}
		else if (main_menu.buttons[2].get_state() == true) {
			canva->load(FILE_PATH);
			menu_on = false;
		}
	}
	return true;
}

void main_loop() {
	bool program_on;
	int key;
	bool update = true;
	Pair last_brush_position;
	int mouse_key;
	Pair cursor_position = {0, 0};
	Brush brush{};
	Canva canva;
	int color_index = 4;
	GUI_page gui;
	GUI_page panel;
	bool mouse_over_panel = false;

	gui.init();
	panel.init();
	
	gui.add_button({ 2, 10 }, { DISPLAY_SIZE.x - 2, DISPLAY_SIZE.y / 2 - 5 }, L_PINK, "\0", ALPHA);

	panel.add_button({ 13, 52 }, { DISPLAY_SIZE.x - 13, DISPLAY_SIZE.y / 2 - 26 }, GREEN, "\0", ALPHA);
	for (int i = 0; i < COLOR_AMOUNT; i++) {
		panel.add_button({ 2, 2 }, { DISPLAY_SIZE.x - 4, DISPLAY_SIZE.y / 2 - 26 + i * 3}, COLORS[i], "\0", ALPHA);
	}
	
	panel.add_button({ MAX_FONT_SIZE_X + 2, MAX_FONT_SIZE_Y + 2 }, { DISPLAY_SIZE.x - 12, DISPLAY_SIZE.y / 2 - 25 }, GRAY, "+", BLACK);
	panel.add_button({ MAX_FONT_SIZE_X + 2, MAX_FONT_SIZE_Y + 2 }, { DISPLAY_SIZE.x - 12, DISPLAY_SIZE.y / 2 - 25 + MAX_FONT_SIZE_Y + 3 }, GRAY, "-", BLACK);
	
	canva.create(3);
	canva.picture.set_size({ 1, 1 });
	canva.prepare();

	brush.init({ 0, 0 }, GREEN, 1); //change brush size
	program_on = menu_loop(&canva);

	while (program_on) {
		Sleep(BASIC_FREQUENCY);
		key = input.get_key();
		mouse_key = input.get_mouse_key();
		//last_cursor_position = cursor_position;
		cursor_position = input.get_mouse_position();
		
		last_brush_position = brush.position;
		brush.set_position(cursor_position, canva.scale);


		if (key == 27) {
			program_on = menu_loop(&canva);
			update = true;
		}
		else if (key == (int)'c' or mouse_key == RIGHT_MOUSE_KEY) {
			if (color_index < 16) {
				color_index++;
			}
			else {
				color_index -= 16;
			}
			brush.color = COLORS[color_index];
			panel.buttons[0].COLOR = COLORS[color_index];
			update = true;
		}
		if (mouse_key == LEFT_MOUSE_KEY and cursor_position.x / canva.scale < canva.picture.SIZE.x and cursor_position.y / canva.scale < canva.picture.SIZE.y and mouse_over_panel == false) {
			brush.draw(canva.picture.color_map, canva.picture.SIZE, canva.scale);
			update = true;
		}
		else if (mouse_key == MOUSE_WHEEL_UP and canva.scale > 1) {
			canva.scale--;
			update = true;
		}
		else if (mouse_key == MOUSE_WHEEL_DOWN and canva.scale < 5) {
			canva.scale++;
			update = true;
		}
		
		if (gui.buttons[0].mouse_over(cursor_position) == true) {
			mouse_over_panel = true;
			update = true;
		}
		else if (brush.position.x != last_brush_position.x or brush.position.y != last_brush_position.y or mouse_key != -1) {
			update = true;
		}

		
		if (update == true) {
			update = false;

			display.clear();
			canva.render();

			if (brush.position.x < canva.picture.SIZE.x * canva.scale and brush.position.y < canva.picture.SIZE.y * canva.scale) {
				brush.render(canva.scale, &canva.picture);
			}
			
			if (mouse_over_panel == true) {
				panel.update_buttons_state(cursor_position, mouse_key);

				for (int i = 1; i <= COLOR_AMOUNT; i++) {
					if (panel.buttons[i].get_state() == true) {
						brush.color = COLORS[i - 1];
						panel.buttons[0].COLOR = COLORS[i - 1];
					}
				}
				if (panel.buttons[COLOR_AMOUNT + 1].get_state() == true and canva.scale < 5) {
					canva.scale++;
				}
				else if (panel.buttons[COLOR_AMOUNT + 2].get_state() == true and canva.scale > 1) {
					canva.scale--;
				}

				if (panel.buttons[0].mouse_over(cursor_position) == false) {
					mouse_over_panel = false;
				}
				display.draw_GUI_page(&panel, &font);
			}
			else {
				display.draw_GUI_page(&gui, &font);
			}
			display.render();
			//display.clear();
		}
	}
}


int main() {

	display.init_buffer(DISPLAY_SIZE);
	input.init();
	font.load("pixel-font.font");

	main_loop();

	input.shut_down();

	return 0;
}
