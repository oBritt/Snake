	
#include <iostream>
#include <vector>
#include <cmath>
#include <string>
#include <algorithm>
#include <chrono>
#include <thread>
#include <conio.h>
#include <cstdlib>
#include <windows.h>
#include <fstream>
#include "json.hpp"

using json = nlohmann::json;

using namespace std;

class head {
public:
	int vilocity = 1;
	string s;
	int x;
	int y;
};

class body {
public:
	int x;
	int y;
	string s = "o";
	bool move = false;
};

class eat{
public:
	int x;
	int y;
	string s = "*";
	bool isthere = 0;	
};

class sign{
public:
	int value = 1;
	int x;
	int y;
	string s = "<";
};

class Game {
public:

	eat appel;
	int width;
	int reload;
	head h;
	vector<body> bodies;
	vector<vector<string>> display;
	bool playing = true;
	sign si;
	int score = 0;
	bool stop = false;

	Game(int w = 25, int r = 300) {
		width = w;
		reload = r;

		vector<vector<string>> d(width, vector<string>(width));
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < width; j++) {
				d[i][j] = " ";
			}
		}
		head t;
		vector<body> b;
		display = d;
		
		h = t;
		h.x = width / 2;
		h.y = width / 2;
		bodies = b;

		int menuwidth = width;
		if (menuwidth % 2 == 1) { menuwidth--; }
		si.x = (menuwidth - 8) / 2 + 9;
		si.y = (menuwidth / 2 - 2);
	}


	void refill() {
		for (int i = 0; i < width; i++) {
			for (int j = 0; j < width; j++) {
				display[i][j] = " ";
			}
		}
	}
	void symbole() {
		if (h.vilocity == 1) {
			h.s = ">";
		}
		else if (h.vilocity == 2) {
			h.s = "^";
		}
		else if (h.vilocity == 3) {
			h.s = "<";
		}
		else {
			h.s = "v";
		}
	}

	void death() {
		vector<vector<int>> arr;
		for (body b : bodies) {
			if (b.x == h.x and b.y == h.y) {
				playing = false;
				break;
			}
		}

	}

	void move() {
		if (bodies.size() == 0) {
			display[h.y][h.x] = " ";
		}
		else {
			body b = bodies[bodies.size() - 1];
			if (b.move) {
				display[b.y][b.x] = " ";
			}
			for (int i = bodies.size() - 1; i > 0; i--) {
				bodies[i].x = bodies[i - 1].x;
				bodies[i].y = bodies[i - 1].y;
			}
			bodies[0].x = h.x;
			bodies[0].y = h.y;

		}
		if (h.vilocity == 1) {
			h.x += 1;
			if (h.x == width) {
				h.x = 0;
			}
		}
		else if (h.vilocity == 2) {
			h.y -= 1;
			if (h.y == -1) {
				h.y = width - 1;
			}
		}
		else if (h.vilocity == 3) {
			h.x -= 1;
			if (h.x == -1) {
				h.x = width - 1;
			}
		}
		else {
			h.y += 1;
			if (h.y == width) {
				h.y = 0;
			}
		}
	}

	void rool() {
		char key;
		if (_kbhit()) {
			key = _getch();


			if ((key == 'a' or key == 'A') and h.vilocity != 1) {
				h.vilocity = 3;
			}
			else if ((key == 'd' or key == 'D') and h.vilocity != 3) {
				h.vilocity = 1;
			}
			else if ((key == 'w' or key == 'W') and h.vilocity != 4) {
				h.vilocity = 2;
			}
			else if ((key == 's' or key == 'S') and h.vilocity != 2) {
				h.vilocity = 4;
			}
			else if (key == 27) {
				stop = true;
			}
		}
	}
	void spawn() {
		if (!appel.isthere) {
			srand(time(NULL));
			vector<vector<int>> arr;
			arr.push_back({ h.y, h.x });
			for (body b : bodies) {
				arr.push_back({ h.y, h.x });
			}
			while (true) {
				int xs = rand() % (width - 1);
				int ys = rand() % (width - 1);
				bool ch = true;
				for (int i = 0; i < arr.size(); i++) {
					if (arr[i][0] == ys and arr[i][1] == xs) {
						ch = false;
						break;
					}
				}
				if (ch) {
					appel.isthere = true;
					appel.x = xs;
					appel.y = ys;
					break;
				}
			}
		}
	}
	bool detect() {
		if (h.x == appel.x and h.y == appel.y) {
			appel.isthere = false;
			return true;
		}
		return false;
	}

	void add() {
		if (detect()) {
			body b;
			if (bodies.size() == 0) {
				b.x = h.x;
				b.y = h.y;
			}
			else {
				b.x = bodies[bodies.size() - 1].x;
				b.y = bodies[bodies.size() - 1].y;
			}
			bodies.push_back(b);
			score = bodies.size();
		}
	}
	void update() {
		if (bodies.size() > 0) {
			if (bodies[bodies.size() - 1].move == false) {
				bodies[bodies.size() - 1].move = true;
			}
		}
	}

	void put() {
		if (appel.isthere) {
			display[appel.y][appel.x] = appel.s;
		}
		for (body b : bodies) {
			display[b.y][b.x] = b.s;
		}
		display[h.y][h.x] = h.s;
	}

	void draw() {
		for (int i = 0; i < 5; i++) {
			cout << endl;
		}
		string prob = "          ";
		string up = "--";
		for (int i = 0; i < width; i++, up += "-") {}
		cout << prob << up << endl;
		for (int i = 0; i < width; i++) {
			cout << prob << '|';
			for (int j = 0; j < width; j++) {
				cout << display[i][j];
			}
			cout << '|' << endl;
		}
		cout << prob << up << endl;
		int p = (width - (6 + to_string(score).size())) / 2;
		string additionspaces = "";
		for (int i = 0; i != p; i++, additionspaces += " ") {}
		cout << prob << additionspaces << "Score:" << score;
	}

	void detect_press() {
		if (_kbhit()) {
			char key;
			key = _getch();
			if (key == 'w' or key == 'W') {
				display[si.y][si.x] = ' ';
				si.value -= 1;
				si.y -= 2;
				if (si.value == 0) {
					si.value = 3;
					si.y += 6;
				}
			}
			else if (key == 's' or key == 'S') {
				display[si.y][si.x] = ' ';
				si.value += 1;
				si.y += 2;
				if (si.value == 4) {
					si.value = 1;
					si.y -= 6;
				}
			}
			else if (key == 13) {
				order();
			}
		}
	}
	void putSign() {
		display[si.y][si.x] = si.s;
	}

	void order() {
		playing = true;
		if (si.value == 1) {
			h.x = width / 2;
			h.y = width / 2;
			bodies = {};
			appel.isthere = false;
			play();
		}
		else if (si.value == 2) {
			std::ifstream file("data.json");
			json data;
			file >> data;

			vector<int> vectorHead = data["v1"];
			vector<int> vectorAppel = data["v2"];
			vector<vector<int>> vectorB = data["v3"];

			h.y = vectorHead[0];
			h.x = vectorHead[1];
			h.vilocity = vectorHead[2];

			appel.y = vectorAppel[0];
			appel.x = vectorAppel[1];

			for (vector<int> B : vectorB) {
				body b;
				b.y = B[0];
				b.x = B[1];
				bodies.push_back(b);
			}
			score = bodies.size();
			play();
		}
		else {
			HWND hwnd = GetConsoleWindow();
			if (hwnd != NULL) {
				PostMessage(hwnd, WM_CLOSE, 0, 0);
			}
		}
	}

	void displayLose() {
		string s1 = "You have lost";
		string s2 = "Your score is " + to_string(score);
		int menu_width = width;
		if (menu_width % 2 == 1) {
			menu_width--;
		}
		for (int i = 0; i < 5; i++) {
			cout << endl;
		}
		string prob = "          ";
		string up = "--";
		for (int i = 0; i < menu_width; i++, up += "-") {}
		cout << prob << up << endl;
		for (int i = 0; i < menu_width; i++) {
			cout << prob << '|';
			if (i * 2 + 2 == menu_width) {
				for (int j = 0; j != s1.size(); j++) {
					display[i][j + (menu_width - s1.size()) / 2] = s1[j];
				}
			}
			if (i * 2 - 2 == menu_width) {
				for (int j = 0; j != s2.size(); j++) {
					display[i][j + (menu_width - s2.size()) / 2] = s2[j];
				}
			}


			for (int j = 0; j < menu_width; j++) {
				cout << display[i][j];
			}
			cout << '|' << endl;
		}
		cout << prob << up << endl;
	}

	void drawMenu() {
		int menu_width = width;
		if (menu_width % 2 == 1) {
			menu_width--;
		}
		string newGame = "New game";
		string Continue = "Continue";
		string close = "Close game";
		for (int i = 0; i < 5; i++) {
			cout << endl;
		}
		string prob = "          ";
		string up = "--";
		for (int i = 0; i < menu_width; i++, up += "-") {}
		cout << prob << up << endl;
		for (int i = 0; i < menu_width; i++) {
			cout << prob << '|';
			if (i * 2 + 4 == menu_width) {
				display[i][(menu_width - 8) / 2] = newGame;
				for (int j = (menu_width - 8) / 2 + 1; j != (menu_width - 8) / 2 + 8; j++) {
					display[i][j] = "";
				}
			}
			if (i * 2 == menu_width) {
				display[i][(menu_width - 8) / 2] = Continue;
				for (int j = (menu_width - 8) / 2 + 1; j != (menu_width - 8) / 2 + 8; j++) {
					display[i][j] = "";
				}
			}
			if (i * 2 - 4 == menu_width) {
				display[i][(menu_width - 9) / 2] = close;
				for (int j = (menu_width - 9) / 2 + 1; j != (menu_width - 9) / 2 + 10; j++) {
					display[i][j] = "";
				}
			}

			for (int j = 0; j < menu_width; j++) {
				cout << display[i][j];
			}
			cout << '|' << endl;
		}
		cout << prob << up << endl;

	}

	void outputingmenu() {
		while (true) {
			detect_press();
			putSign();
			drawMenu();
			std::this_thread::sleep_for(std::chrono::milliseconds(reload));
			system("cls");
		}
	}

	void play() {
		refill();
		stop = false;
		playing = true;
		while (playing and !stop) {
			update();
			spawn();
			rool();
			symbole();
			move();
			death();
			add();
			put();
			draw();
			std::this_thread::sleep_for(std::chrono::milliseconds(reload));
			system("cls");
		}
		if (not playing) {
			refill();
			h.x = width / 2;
			h.y = width / 2;
			displayLose();
			score = 0;
			bodies = {};
			while (true) {
				if (_kbhit()) {
					char key;
					key = _getch();
					if (key == 13) {
						break;
					}
				}
			}

		}
		if (stop) {
			vector<int> vectorHead = { h.y, h.x, h.vilocity };
			vector<int> vectorAppel = { appel.y, appel.x };
			vector<vector<int>> vectorb;
			for (body b : bodies) {
				vectorb.push_back({ b.y, b.x });
			}
			json data;
			data["v1"] = vectorHead;
			data["v2"] = vectorAppel;
			data["v3"] = vectorb;

			std::ofstream file("data.json");
			file << std::setw(4) << data << std::endl;
			file.close();
			bodies = {};
			score = 0;
			h.x = width / 2;
			h.y = width / 2;
		}
		refill();
	}

};

int main() {
	Game myGame(25, 200);
	myGame.outputingmenu();
	head h;
	h.x = 2;
}