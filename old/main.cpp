#include <iostream>
#include <stdio.h>
#include <fstream>
#include <math.h>
#include <vector>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL_image.h>
#include <SDL2/SDL2_gfxPrimitives.h>

const int SCREEN_SIZE = 400;

int FNcross(int x1, int y1, double x2, double y2) {
	int a = (x1*y2)-(y1*x2);
	return a;
}

void Intersect(int x1, int y1, int x2, int y2, double x3, double y3, int x4, int y4, int& x, int& y) {
	x = FNcross(x1, y1, x2, y2);
	y = FNcross(x3, y3, x4, y4);
	int det = FNcross(x1-x2, y1-y2, x3-x4, y3-y4);
	if (det == 0) {det = 1;}
	x = FNcross(x, x1-x2, y, x3-x4) / det;
	y = FNcross(x, y1-y2, y, y3-y4) / det;
}

int main(int argc, char *args[]) {
	SDL_Window* window = NULL;
	SDL_Renderer* renderer = NULL;
	SDL_Rect temp;

	window = SDL_CreateWindow("Peril", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, SCREEN_SIZE, SCREEN_SIZE, SDL_WINDOW_SHOWN);
	renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_SetRelativeMouseMode(SDL_TRUE);

	struct Move {
		int x;
		int y;
	};
	struct Sprite {
		int x;
		int y;
		double angle;
		Move move;
		double pcos() {
			double r = round(cos(angle));
			return r;
		}
		double psin() {
			double r = round(sin(angle));
			return r;
		}
	};
	struct Line {
		int x1;
		int y1;
		int x2;
		int y2;
		int z1;
		int z2;
	};
	struct Wall {
		short int x[4] = {};
		short int y[4] = {};
	};
	Wall wall;
	Sprite player;
	player.x = SCREEN_SIZE/2;
	player.y = SCREEN_SIZE/2;
	player.angle = 0;
	player.move.x = 0;
	player.move.y = 0;
	Line templine;
	std::vector<Line> lines;
	std::vector<Line> tlines;

	std::string line;
	std::string tline;
	int lnum = 0;
	int pstart = 0;
	int lstar = 0;
	std::ifstream map (args[1]);
	if (map.is_open()) {
		while (getline(map, line)) {
			lnum = line.find(",");
			tline = line;
                        tline.resize(lnum);
                        line.erase(0, lnum+1);
			if (pstart == 1) {
				if (lstar == 0) {
					lstar = 1;
					templine.x1 = std::stoi(tline);
					templine.y1 = std::stoi(line);
				}
				else {
					lstar = 0;
					templine.x2 = std::stoi(tline);
					templine.y2 = std::stoi(line);
					lines.push_back(templine);
				}
			}
			if (pstart == 0) {
				player.x = std::stoi(tline);
				player.y = std::stoi(line);
				pstart = 1;
			}
		}
		map.close();
	}

        for (int i=0; i<lines.size(); i++) {
                templine.x1 = lines[i].x1 - player.x;
                templine.y1 = lines[i].y1 - player.y;
                templine.x2 = lines[i].x2 - player.x;
                templine.y2 = lines[i].y2 - player.y;
	        tlines.push_back(templine);
        }

	int gameover = 0;
	int ix1, ix2, iz1, iz2;
	int x;
	int y;
	SDL_Event event;

	while (!gameover) {
		SDL_Delay(5);
		if (player.angle < 0.1 && player.angle > -0.1) {player.angle = 0;}
		if (SDL_PollEvent(&event)) {
			switch(event.type) {
				case SDL_QUIT: gameover = 1; break;
				case SDL_KEYDOWN:
					if (event.key.keysym.sym == SDLK_w) {
						player.x += player.pcos(); player.y += player.psin();
					}
					if (event.key.keysym.sym == SDLK_s) {
						player.x -= player.pcos(); player.y -= player.psin();
					}
					if (event.key.keysym.sym == SDLK_d) {
						player.x += player.psin(); player.y += player.pcos();
					}
					if (event.key.keysym.sym == SDLK_a) {
						player.x -= player.psin(); player.y -= player.pcos();
					}
//					if (event.key.keysym.sym == SDLK_a) {player.angle -= 0.1;}
//					if (event.key.keysym.sym == SDLK_d) {player.angle += 0.1;}
				break;
			}
		}
		SDL_GetRelativeMouseState(&x,&y);
		player.angle += x * 0.03;
		for (int i=0; i<tlines.size(); i++) {
			tlines[i].x1 = lines[i].x1 - player.x;
			tlines[i].y1 = lines[i].y1 - player.y;
			tlines[i].x2 = lines[i].x2 - player.x;
			tlines[i].y2 = lines[i].y2 - player.y;
			tlines[i].z1 = tlines[i].x1 * cos(player.angle) + tlines[i].y1 * sin(player.angle);
			tlines[i].z2 = tlines[i].x2 * cos(player.angle) + tlines[i].y2 * sin(player.angle);
			tlines[i].x1 = tlines[i].x1 * sin(player.angle) - tlines[i].y1 * cos(player.angle);
			tlines[i].x2 = tlines[i].x2 * sin(player.angle) - tlines[i].y2 * cos(player.angle);
			Intersect(tlines[i].x1, tlines[i].z1, tlines[i].x2, tlines[i].z2, -0.0001, 0.0001, -10, 5, ix1, iz1);
			Intersect(tlines[i].x1, tlines[i].z1, tlines[i].x2, tlines[i].z2, 0.0001, 0.0001, 10, 5, ix2, iz2);
			if (tlines[i].z1 <= 0) {
				tlines[i].x1 = (1-tlines[i].z1)*(tlines[i].x2-tlines[i].x1)/((tlines[i].z2-tlines[i].z1)+0.5)+tlines[i].x1;
				tlines[i].z1 = 1;
			}
			if (tlines[i].z2 <= 0) {
                                tlines[i].x2 = (1-tlines[i].z2)*(tlines[i].x1-tlines[i].x2)/(tlines[i].z1-tlines[i].z2)+tlines[i].x2;
				tlines[i].z2 = 1;
			}
			int x1 = -tlines[i].x1 * 160 / tlines[i].z1;
			int x2 = -tlines[i].x2 * 160 / tlines[i].z2;
			if (tlines[i].z1 <= 0) {tlines[i].z1 = 1;}
			if (tlines[i].z2 <= 0) {tlines[i].z2 = 1;}
			int y1a = (-SCREEN_SIZE/2) / tlines[i].z1;
			int y1b = (SCREEN_SIZE/2) / tlines[i].z1;
			int y2a = (-SCREEN_SIZE/2) / tlines[i].z2;
			int y2b = (SCREEN_SIZE/2) / tlines[i].z2;	
			short int wallx[] = {(SCREEN_SIZE/2)+x2, (SCREEN_SIZE/2)+x1, (SCREEN_SIZE/2)+x2};
                        short int wally[] = {(SCREEN_SIZE/2)+y2b, (SCREEN_SIZE/2)+y1a, (SCREEN_SIZE/2)+y2a};
			filledPolygonRGBA(renderer, wallx, wally, 3, 100, 100, 100, 255);
                        short int wallx1[] = {(SCREEN_SIZE/2)+x1, (SCREEN_SIZE/2)+x2, (SCREEN_SIZE/2)+x1};
                        short int wally1[] = {(SCREEN_SIZE/2)+y1b, (SCREEN_SIZE/2)+y2b, (SCREEN_SIZE/2)+y1a};
                        filledPolygonRGBA(renderer, wallx1, wally1, 3, 100, 100, 100, 255);
			SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
			SDL_RenderDrawLine(renderer, ((SCREEN_SIZE/2)+x1), ((SCREEN_SIZE/2)+y1a), ((SCREEN_SIZE/2)+x2), ((SCREEN_SIZE/2)+y2a));
			SDL_RenderDrawLine(renderer, ((SCREEN_SIZE/2)+x1), ((SCREEN_SIZE/2)+y1b), ((SCREEN_SIZE/2)+x2), ((SCREEN_SIZE/2)+y2b));
			SDL_RenderDrawLine(renderer, ((SCREEN_SIZE/2)+x1), ((SCREEN_SIZE/2)+y1a), ((SCREEN_SIZE/2)+x1), ((SCREEN_SIZE/2)+y1b));
			SDL_RenderDrawLine(renderer, ((SCREEN_SIZE/2)+x2), ((SCREEN_SIZE/2)+y2a), ((SCREEN_SIZE/2)+x2), ((SCREEN_SIZE/2)+y2b));
		}
		SDL_RenderPresent(renderer);
                SDL_SetRenderDrawColor(renderer, 0xFF, 0xFF, 0xFF, 0xFF);
		SDL_RenderClear(renderer);
                SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);

	}
	SDL_Quit();
}
