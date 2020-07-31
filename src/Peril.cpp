#include <iostream>
#include <stdio.h>
#include <fstream>
#include <math.h>
#include <vector>

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
	tline.z1 = tline.x1 * cos(players.angle) + tline.y1 * sin(players.angle);
	tline.z2 = tline.x2 * cos(players.angle) + tline.y2 * sin(players.angle);
	tline.x1 = tline.x1 * sin(players.angle) - tline.y1 * cos(players.angle);
	tline.x2 = tline.x2 * sin(players.angle) - tline.y2 * cos(players.angle);
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

void Peril::InitPeril() {
	
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

void Peril::MoveBy(int x, int y) {
	Peril::player.x += x;
	Peril::player.y += y;
}

void Peril::DoLines() {
	for (int i=0; i<Peril::tlines.size(); i++) {
		TransformLine(Peril::lines[i], Peril::player, Peril::tlines[i]);
	}
	for (int i=0; i<Peril::tlines.size(); i++) {
	        int x1 = -tlines[i].x1 * 200 / tlines[i].z1;
	        int x2 = -tlines[i].x2 * 200 / tlines[i].z2;
		int y1a = (-(Peril::SCREEN_SIZE*2)) / tlines[i].z1;
		int y1b = (Peril::SCREEN_SIZE*2) / tlines[i].z1;
		int y2a = (-(Peril::SCREEN_SIZE*2)) / tlines[i].z2;
		int y2b = (Peril::SCREEN_SIZE*2) / tlines[i].z2;

		// TODO: Eventually implement the following into Core.  Currently they are massively worked around
		short int wallx[] = {(Peril::SCREEN_SIZE/2)+x2, (Peril::SCREEN_SIZE/2)+x1, (Peril::SCREEN_SIZE/2)+x2};
		short int wally[] = {(Peril::SCREEN_SIZE/2)+y2b, (Peril::SCREEN_SIZE/2)+y1a, (Peril::SCREEN_SIZE/2)+y2a};
        filledPolygonRGBA(Peril::renderer, wallx, wally, 3, 100, 100, 100, 255);
        short int wallx1[] = {(Peril::SCREEN_SIZE/2)+x1, (Peril::SCREEN_SIZE/2)+x2, (Peril::SCREEN_SIZE/2)+x1};
        short int wally1[] = {(Peril::SCREEN_SIZE/2)+y1b, (Peril::SCREEN_SIZE/2)+y2b, (Peril::SCREEN_SIZE/2)+y1a};
    	filledPolygonRGBA(Peril::renderer, wallx1, wally1, 3, 100, 100, 100, 255);
		
                this->DrawLine(((SCREEN_SIZE/2)+x1), ((SCREEN_SIZE/2)+y1a), ((SCREEN_SIZE/2)+x2), ((SCREEN_SIZE/2)+y2a));
                this->DrawLine(((SCREEN_SIZE/2)+x1), ((SCREEN_SIZE/2)+y1b), ((SCREEN_SIZE/2)+x2), ((SCREEN_SIZE/2)+y2b));
                this->DrawLine(((SCREEN_SIZE/2)+x1), ((SCREEN_SIZE/2)+y1a), ((SCREEN_SIZE/2)+x1), ((SCREEN_SIZE/2)+y1b));
                this->DrawLine(((SCREEN_SIZE/2)+x2), ((SCREEN_SIZE/2)+y2a), ((SCREEN_SIZE/2)+x2), ((SCREEN_SIZE/2)+y2b));

	}
	gameover = Core::gameover;
}

void Peril::Move(int up, int down, int left, int right){
	float acceleration = 0;
	float vel1 = 0;
	float vel2 = 0;
	if (up == 1) {vel1 += cos(this->player.angle)*1.2; vel2 += sin(this->player.angle)*1.2;}
	if (down == 1) {vel1 -= cos(this->player.angle)*1.2; vel2 -= sin(this->player.angle)*1.2;}
	if (left == 1) {vel1 += sin(this->player.angle)*1.2; vel2 -= cos(this->player.angle)*1.2;}
	if (right == 1) {vel1 -= sin(this->player.angle)*1.2; vel2 += cos(this->player.angle)*1.2;}
	if (up == 1 || down == 1 || right == 1 || left == 1) {
		acceleration = 0.4;
	}
	this->player.velocity.x = this->player.velocity.x * (1-acceleration) + vel1 * acceleration;
	this->player.velocity.y = this->player.velocity.y * (1-acceleration) + vel2 * acceleration;
	std::cout << this->player.velocity.x << ", " << this->player.velocity.y << std::endl;
	this->MoveBy(vel1, vel2);
	SDL_GetRelativeMouseState(&x, &y);
	this->player.angle += x * 0.03;
}
