#include <iostream>
#include <stdio.h>
#include <fstream>
#include <math.h>
#include <vector>
#include <algorithm>

#include "Peril.h"

int Peril::FNcross(int x1, int y1, double x2, double y2) {
	int a = (x1*y2)-(y1*x2);
	return a;
}

void Peril::Intersect(int x1, int y1, int x2, int y2, double x3, double y3, int x4, int y4, int& x, int& y) {
//	x = Peril::FNcross(x1, y1, x2, y2);
//	y = Peril::FNcross(x3, y3, x4, y4);
//	int det = Peril::FNcross(x1-x2, y1-y2, x3-x4, y3-y4);
//	if (det == 0) {det = 1;}
//	x = Peril::FNcross(x, x1-x2, y, x3-x4) / det;
//	y = Peril::FNcross(x, y1-y2, y, y3-y4) / det;
	x = FNcross(FNcross(x1, y1, x2, y2), (x1)-(x2), FNcross(x3, y3, x4, y4), (x3)-(x4)) / (FNcross((x1)-(x2), (y1)-(y2), (x3)-(x4), (y3)-(y4)) + 0.5);
	y = FNcross(FNcross(x1, y1, x2, y2), (y1)-(y2), FNcross(x3, y3, x4, y4), (y3)-(y4)) / (FNcross((x1)-(x2), (y1)-(y2), (x3)-(x4), (y3)-(y4)) + 0.5);	
}

int Peril::LineDistance(int x, int y, int x2, int y2) {
	int a = sqrt(((x2-x)*(x2-x))+((y2-y)*(y2-y)));
	return a;
}

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
//	if (tline.z1 <= 0) {
//		tline.x1 = (1-tline.z1)*(tline.x2-tline.x1)/((tline.z2-tline.z1)+0.5)+tline.x1;
//		tline.z1 = 1;
//	}
//	if (tline.z2 <= 0) {
//              tline.x2 = (1-tline.z2)*(tline.x1-tline.x2)/((tline.z1-tline.z2)+0.5)+tline.x2;
//		tline.z2 = 1;
//	}
	if (tline.z1 <= 0 || tline.z2 <= 0) {
//        	float nearz = 1e-4f, farz = 5, nearside = 1e-5f, farside = 20.f;
		float nearz = 0.0001, farz = 5, nearside = 0.00001, farside = 20;
		int ix = 0;
		int iy = 0;
		int ix2 = 0;
		int iy2 = 0;
		Intersect(tline.x1, tline.z1, tline.x2, tline.z2, -nearside, nearz, -farside, farz, ix, iy);
		Intersect(tline.x1, tline.z1, tline.x2, tline.z2, nearside, nearz, farside, farz, ix2, iy2);
		if (tline.z1 < nearz) {
			if (iy > 0) {
				tline.x1 = ix; tline.z1 = iy;
			}
			else {
				tline.x1 = ix2; tline.z1 = iy2;
			}
		}
		if (tline.z2 < nearz) {
			if (iy > 0) {
				tline.x2 = ix; tline.z2 = iy;
			}
			else {
				tline.x2 = ix2; tline.z2 = iy2;
			}
		}
		if (tline.z1 == 0) {
			tline.z1 = 1;
		}
		if (tline.z2 == 0) {
			tline.z2 = 1;
		}
	}
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
	liness.clear();
	dlines.clear();
	dliness.clear();
	for (int i=0; i<Peril::lines.size(); i++) {
		Peril::dline.dist = LineDistance(Peril::player.x, Peril::player.y, (lines[i].x1+lines[i].x2)/2, (lines[i].y1+lines[i].y2)/2);
		Peril::dline.num = i;
		Peril::dliness.push_back(Peril::dline.dist);
		Peril::dlines.push_back(Peril::dline);
	}
	sort(Peril::dliness.begin(), Peril::dliness.end());
	reverse(Peril::dliness.begin(), Peril::dliness.end());
	for (int i=0; i<Peril::dlines.size(); i++) {
		for (int j=0; j<Peril::dlines.size(); j++) {
			if (Peril::dlines[j].dist == Peril::dliness[i]) {
				int ix = 0;
				int iy = 0;
				int dnum = Peril::dlines[j].num;
                                Peril::liness.push_back(Peril::lines[dnum]);
			}
		}
	}
        int ix = 0;
        int iy = 0;
	for (int i=1; i<Peril::liness.size(); i++) {
		Intersect(Peril::liness[i].x1, Peril::liness[i].y1, Peril::liness[i].x2, Peril::liness[i].y2, Peril::player.x, Peril::player.y, (Peril::liness[Peril::liness.size()-1].x1+Peril::liness[Peril::liness.size()-1].x2)/2, (Peril::liness[Peril::liness.size()-1].y1+Peril::liness[Peril::liness.size()-1].y2)/2, ix, iy);
		std::cout << ix << ", " << iy << std::endl;
		if (ix > Peril::liness[i].x1 && ix < Peril::liness[i].x2 && iy > Peril::liness[i].y1 && iy < Peril::liness[i].y2) {
//		if (ix > 0 && iy > 0) {
			Peril::liness.erase(Peril::liness.begin()+i);
		}
	}
	for (int i=0; i<Peril::tlines.size(); i++) {
		TransformLine(Peril::liness[i], Peril::player, Peril::tlines[i]);
	}
	for (int i=0; i<Peril::tlines.size(); i++) {
	        int x1 = -tlines[i].x1 * 160 / tlines[i].z1;
	        int x2 = -tlines[i].x2 * 160 / tlines[i].z2;
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
	this->MoveBy(vel1, vel2);
	SDL_GetRelativeMouseState(&x, &y);
	this->player.angle += x * 0.03;
	if (this->player.angle > 6.3 || this->player.angle < -6.3) {
		this->player.angle = 0;
	}
}
