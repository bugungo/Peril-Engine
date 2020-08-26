#include <iostream>
#include <stdio.h>
#include <fstream>
#include <vector>
#include <string>
#include <SDL2/SDL.h>

const int SCREEN_SIZE = 400;
const int LINE_SIZE = 10;

int LineDistance(int x, int y, int x2, int y2) {
        int a = sqrt(((x2-x)*(x2-x))+((y2-y)*(y2-y)));
        return a;
}

int Grid(int a) {
        int b = 0;
        for (int i=0; i<a; i = i+10) {
                b++;
        }
      b--;
	b = b*10;
        return b;
}

int main(int argc, char *args[]) {
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;

	window = SDL_CreateWindow("PMaker", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_SIZE, SCREEN_SIZE, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

	struct Line {
		int x1;
		int y1;
		int x2;
		int y2;
	};

	Line templine;
	std::vector<Line> lines;

        std::ifstream map;
	std::string line;
	std::string tline;
	int lnum = 0;
	int px, py;
	int pstart = 0;
	int start = 0;
        map.open(args[1]);
        if (map.is_open()) { 
		while (getline(map, line)) {
			lnum = line.find(",");
			tline = line;
			tline.resize(lnum);
			line.erase(0, lnum+1);		
			if (pstart == 0) {
					px = std::stoi(tline);
					py = std::stoi(line);
			}
			if (pstart == 1) {
				if (start == 0) {
					start = 1;
					templine.x1 = std::stoi(tline);
					templine.y1 = std::stoi(line);
				}
				else {
					templine.x2 = std::stoi(tline);
					templine.y2 = std::stoi(line);
					lines.push_back(templine);
					start = 0;
				}
			}
			pstart = 1;
		}
		map.close();
        }

	SDL_Event event;
	int gameover = 0;
	int lstart = 0;
	int dogrid = 1;
	int x, y;
	int mx, my;

	while (!gameover) {
		SDL_Delay(5);
		if (SDL_PollEvent(&event)) {
			if (event.type == SDL_QUIT) {gameover = 1;}
			if (event.type == SDL_KEYDOWN) {
				switch (event.key.keysym.sym) {
					case SDLK_o:
						std::cout << "Saved Map: " << args[1] << std::endl;
						std::ofstream save (args[1]);
						if (save.is_open()) {
							save << px << "," << py << std::endl;
							for (int i=0; i<lines.size(); i++) {
								save << lines[i].x1 << "," << lines[i].y1 << std::endl;
								save << lines[i].x2 << "," << lines[i].y2 << std::endl;
							}
							save.close();
						}
					break;
//					case SDLK_g:
//						if (dogrid == 0) {
//							std::cout << "Grid Overlay: On" << std::endl;
//							dogrid = 1;
//						}
//						else {
//							std::cout << "Grid Overlay: Off" << std::endl;
//							dogrid = 0;
//						}
//					break;
				}
			}
			if (event.type == SDL_MOUSEBUTTONDOWN) {
				if (event.button.button == SDL_BUTTON_LEFT) {
					if (lstart == 0) {
						templine.x1 = Grid(event.button.x);
						templine.y1 = Grid(event.button.y);
						templine.x2 = 0;
						templine.y2 = 0;
						lstart = 1;
					}
					else {
						lstart = 0;
						templine.x2 = Grid(event.button.x);
						templine.y2 = Grid(event.button.y);
						lines.push_back(templine);
					}
				}
				if (event.button.button == SDL_BUTTON_RIGHT) {
					px = event.button.x;
					py = event.button.y;
				}
			}
		}

		if (lstart == 1) {
			SDL_GetMouseState(&mx, &my);
			templine.x2 = mx;
			templine.y2 = my;
			SDL_RenderDrawLine(renderer, templine.x1, templine.y1, templine.x2, templine.y2);
		}

		if (dogrid == 1) {
			for (int i=0; i<SCREEN_SIZE; i += 10) {
				for (int j=0; j<SCREEN_SIZE; j += 10) {
					SDL_RenderDrawPoint(renderer, i, j);
				}
			}
		}

		for (int i=0; i<lines.size(); i++) {
			SDL_RenderDrawLine(renderer, lines[i].x1, lines[i].y1, lines[i].x2, lines[i].y2);
		}

		SDL_RenderDrawPoint(renderer, px, py);
		SDL_RenderPresent(renderer);
		SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	}
	SDL_Quit();
}
