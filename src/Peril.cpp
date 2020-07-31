#include <iostream>
#include <stdio.h>
#include <fstream>
#include <math.h>
#include <vector>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>
#include "Peril.h"

//int Peril::FNcross(int x1, int y1, double x2, double y2) {
//	int a = (x1*y2)-(y1*x2);
//	return a;
//}

//void Intersect(int x1, int y1, int x2, int y2, double x3, double y3, int x4, int y4, int& x, int& y) {
//	x = Peril::FNcross(x1, y1, x2, y2);
//	y = Peril::FNcross(x3, y3, x4, y4);
//	int det = Peril::FNcross(x1-x2, y1-y2, x3-x4, y3-y4);
//	if (det == 0) {det = 1;}
//	x = Peril::FNcross(x, x1-x2, y, x3-x4) / det;
//	y = Peril::FNcross(x, y1-y2, y, y3-y4) / det;
//}

void Peril::TransformLine(Peril::Line line, Peril::Player players, Peril::Line &tlin)  {
	Peril::Line tline = line;
	tline.x1 = line.x1 - players.x;
	tline.y1 = line.y1 - players.y;
	tline.x2 = line.x2 - players.x;
	tline.y2 = line.y2 - players.y;
	tline.z1 = tline.x1 * players.pcos() + tline.y1 * players.psin();
	tline.z2 = tline.x2 * players.pcos() + tline.y2 * players.psin();
	tline.x1 = tline.x1 * players.psin() - tline.y1 * players.pcos();
	tline.x2 = tline.x2 * players.psin() - tline.y2 * players.pcos();
	if (tline.z1 <= 0) {
		tline.x1 = (1-tline.z1)*(tline.x2-tline.x1)/((tline.z2-tline.z1)+0.5)+tline.x1;
		tline.z1 = 1;
	}
	if (tline.z2 <= 0) {
                tline.x2 = (1-tline.z2)*(tline.x1-tline.x2)/((tline.z1-tline.z2)+0.5)+tline.x2;
		tline.z2 = 1;
	}
	if (tline.z1 <= 0) {tline.z1 = 1;}
	if (tline.z2 <= 0) {tline.z2 = 1;}
	tlin = tline;
}

void Peril::InitSDL() {
	Peril::window = SDL_CreateWindow("Peril", SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, Peril::SCREEN_SIZE, Peril::SCREEN_SIZE, SDL_WINDOW_SHOWN);
	Peril::renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
	SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_SetRelativeMouseMode(SDL_TRUE);
};

void Peril::QuitSDL() {
	SDL_Quit();
}

void Peril::InitPeril() {
	Peril::player.x = 0;
	Peril::player.y = 0;
	Peril::player.angle = 0;
}

void Peril::LoadMap(std::string filename) {
	std::ifstream map (filename);
	std::string line;
	std::string tline;
        int lnum = 0;
        int pstart = 0;
        int kstart = 0;
	if (map.is_open()) {
		while (getline(map, line)) {
			lnum = line.find(",");
			tline = line;
			tline.resize(lnum);
			line.erase(0, lnum+1);
			if (pstart == 1) {
				if (kstart == 0) {
					kstart = 1;
					Peril::templine.x1 = std::stoi(tline);
					Peril::templine.y1 = std::stoi(line);
				}
				else {
					kstart = 0;
                                        Peril::templine.x2 = std::stoi(tline);
                                        Peril::templine.y2 = std::stoi(line);
					Peril::lines.push_back(Peril::templine);
				}
			}
			if (pstart == 0) {
				Peril::player.x = std::stoi(tline);
				Peril::player.y = std::stoi(line);
				pstart = 1;
			}
		}
		map.close();
	}
        for (int i=0; i<Peril::lines.size(); i++) {
                TransformLine(Peril::lines[i], Peril::player, Peril::templine);
                Peril::tlines.push_back(Peril::templine);
        }
}

void Peril::Delay(int a) {
	SDL_Delay(a);
}

void Peril::MoveUp() {
	Peril::player.x += Peril::player.pcos();
	Peril::player.y += Peril::player.psin();
}
void Peril::MoveDown() {
	Peril::player.x -= Peril::player.pcos();
	Peril::player.y -= Peril::player.psin();
}
void Peril::MoveRight() {
	Peril::player.x -= Peril::player.psin();
	Peril::player.y -= Peril::player.pcos();
}
void Peril::MoveLeft() {
	Peril::player.x += Peril::player.psin();
	Peril::player.y += Peril::player.pcos();
}

void Peril::GetInput() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT: Peril::gameover = 1; break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_w) {Peril::up = 1;}
                                if (event.key.keysym.sym == SDLK_s) {Peril::down = 1;}
                                if (event.key.keysym.sym == SDLK_a) {Peril::left = 1;}
                                if (event.key.keysym.sym == SDLK_d) {Peril::right = 1;}
			break;
                        case SDL_KEYUP:
                                if (event.key.keysym.sym == SDLK_w) {Peril::up = 0;}
                                if (event.key.keysym.sym == SDLK_s) {Peril::down = 0;}
                                if (event.key.keysym.sym == SDLK_a) {Peril::left = 0;}
                                if (event.key.keysym.sym == SDLK_d) {Peril::right = 0;}
                        break;
		}
	}
	if (Peril::up == 1) {Peril::MoveUp();}
	if (Peril::down == 1) {Peril::MoveDown();}
	if (Peril::left == 1) {Peril::MoveLeft();}
	if (Peril::right == 1) {Peril::MoveRight();}
	SDL_GetRelativeMouseState(&x, &y);
	Peril::player.angle += x * 0.03;
	std::cout << player.angle << std::endl;
}

void Peril::DoLines() {
	for (int i=0; i<Peril::tlines.size(); i++) {
		TransformLine(Peril::lines[i], Peril::player, Peril::tlines[i]);
	}
	for (int i=0; i<Peril::tlines.size(); i++) {
	        int x1 = -tlines[i].x1 * 160 / tlines[i].z1;
	        int x2 = -tlines[i].x2 * 160 / tlines[i].z2;
		int y1a = (-(Peril::SCREEN_SIZE/2)) / tlines[i].z1;
		int y1b = (Peril::SCREEN_SIZE/2) / tlines[i].z1;
		int y2a = (-(Peril::SCREEN_SIZE/2)) / tlines[i].z2;
		int y2b = (Peril::SCREEN_SIZE/2) / tlines[i].z2;
		short int wallx[] = {(Peril::SCREEN_SIZE/2)+x2, (Peril::SCREEN_SIZE/2)+x1, (Peril::SCREEN_SIZE/2)+x2};
		short int wally[] = {(Peril::SCREEN_SIZE/2)+y2b, (Peril::SCREEN_SIZE/2)+y1a, (Peril::SCREEN_SIZE/2)+y2a};
                filledPolygonRGBA(Peril::renderer, wallx, wally, 3, 100, 100, 100, 255);
                short int wallx1[] = {(Peril::SCREEN_SIZE/2)+x1, (Peril::SCREEN_SIZE/2)+x2, (Peril::SCREEN_SIZE/2)+x1};
                short int wally1[] = {(Peril::SCREEN_SIZE/2)+y1b, (Peril::SCREEN_SIZE/2)+y2b, (Peril::SCREEN_SIZE/2)+y1a};
                filledPolygonRGBA(Peril::renderer, wallx1, wally1, 3, 100, 100, 100, 255);
		SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
                SDL_RenderDrawLine(renderer, ((SCREEN_SIZE/2)+x1), ((SCREEN_SIZE/2)+y1a), ((SCREEN_SIZE/2)+x2), ((SCREEN_SIZE/2)+y2a));
                SDL_RenderDrawLine(renderer, ((SCREEN_SIZE/2)+x1), ((SCREEN_SIZE/2)+y1b), ((SCREEN_SIZE/2)+x2), ((SCREEN_SIZE/2)+y2b));
                SDL_RenderDrawLine(renderer, ((SCREEN_SIZE/2)+x1), ((SCREEN_SIZE/2)+y1a), ((SCREEN_SIZE/2)+x1), ((SCREEN_SIZE/2)+y1b));
                SDL_RenderDrawLine(renderer, ((SCREEN_SIZE/2)+x2), ((SCREEN_SIZE/2)+y2a), ((SCREEN_SIZE/2)+x2), ((SCREEN_SIZE/2)+y2b));

	}
}

void Peril::DrawSDL() {
	SDL_RenderPresent(Peril::renderer);
	SDL_SetRenderDrawColor(Peril::renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(Peril::renderer);
	SDL_SetRenderDrawColor(Peril::renderer, 0, 0, 0, 0);
}
