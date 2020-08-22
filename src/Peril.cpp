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
	x = FNcross(FNcross(x1, y1, x2, y2), (x1)-(x2), FNcross(x3, y3, x4, y4), (x3)-(x4)) / (FNcross((x1)-(x2), (y1)-(y2), (x3)-(x4), (y3)-(y4)) + 0.1);
	y = FNcross(FNcross(x1, y1, x2, y2), (y1)-(y2), FNcross(x3, y3, x4, y4), (y3)-(y4)) / (FNcross((x1)-(x2), (y1)-(y2), (x3)-(x4), (y3)-(y4)) + 0.1);
}

bool Peril::DoIntersect(int x1, int y1, int x2, int y2, int x3, int y3, int x4, int y4, int& ix, int& iy) {
	int tem;
	Intersect(x1, y1, x2, y2, x3, y3, x4, y4, ix, iy);
	if (x1 > x2) {
		tem = x1;
		x1 = x2;
		x2 = tem;
	}
	if (y1 > y2) {
		tem = y1;
		y1 = y2;
		y2 = tem;
	}
	if (x3 > x4) {
		tem = x3;
		x3 = x4;
		x4 = tem;
	}
	if (y3 > y4) {
		tem = y3;
		y3 = y4;
		y4 = tem;
	}
	if (ix >= x1 && ix <= x2 && iy >= y1 && iy <= y2 && ix >= x3 && ix <= x4 && iy >= y3 && iy <= y4) {
		return true;
	}
	else {
		return false;
	}
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
        	float nearz = 1e-4f, farz = 5, nearside = 1e-5f, farside = 20.f;
//		float nearz = 0, farz = 5, nearside = 0, farside = 20;
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

void Peril::MoveBy(double x, double y) {

}

void Peril::DoLines() {
	liness.clear();
	dlines.clear();
	dliness.clear();
	tempd.clear();
	for (int i=0; i<Peril::lines.size(); i++) {
		Peril::dline.dist = LineDistance(Peril::player.x, Peril::player.y, (lines[i].x1+lines[i].x2)/2, (lines[i].y1+lines[i].y2)/2);
		Peril::dline.num = i;
		Peril::dliness.push_back(Peril::dline.dist);
		Peril::dlines.push_back(Peril::dline);
	}
	sort(Peril::dliness.begin(), Peril::dliness.end());
	reverse(Peril::dliness.begin(), Peril::dliness.end());
	for (int i=0; i<Peril::dlines.size(); i++) {
		for (int j=0; j<Peril::dliness.size(); j++) {
			if (Peril::dlines[j].dist == Peril::dliness[i]) {
				int dnum = Peril::dlines[j].num;
				int lcheck = 0;
				for (int k=0; k<Peril::tempd.size(); k++) {
					if (dnum == tempd[k]) {
						lcheck = 1;
					}
				}
				if (lcheck == 0) {
	                                Peril::tempd.push_back(dnum);
	                               	Peril::liness.push_back(Peril::lines[dnum]);
					Peril::lines[dnum].clr = Peril::dlines[dnum].dist;
				}
			}
		}
	}
	for (int i=0; i<Peril::lines.size(); i++) {
		Peril::lines[i].clr = 70;
	}
//	Peril::linesss = Peril::liness;
	Peril::linesss.clear();
	Peril::tempd.clear();
	for (int i=0; i<Peril::liness.size(); i++) {
		int ix = 0;
		int iy = 0;
		int ix1 = 0;
		int iy1 = 0;
		int chek = 0;
		int tnum = 0;
		for (int j=0; j<Peril::liness.size(); j++) {
			int check1 = 0;
			int check2 = 0;
			int check3 = 0;
			if (i != j) {
			if (DoIntersect(Peril::liness[i].x1, Peril::liness[i].y1, Peril::liness[i].x2, Peril::liness[i].y2, Peril::player.x, Peril::player.y, Peril::liness[j].x1, Peril::liness[j].y1, ix, iy)) {
				check1 = 1;
				chek = 1;
			}
			if (DoIntersect(Peril::liness[i].x1, Peril::liness[i].y1, Peril::liness[i].x2, Peril::liness[i].y2, Peril::player.x, Peril::player.y, Peril::liness[j].x2, Peril::liness[j].y2, ix1, iy1)) {
				check2 = 1;
				chek = 1;
			}
			if (check1 == 1 || check2 == 1) {
				//if j is blocked by i
				for (int k=0; k<Peril::tempd.size(); k++) {
					if (Peril::tempd[k] == j) {
						tnum = 1;
					}
				}
				if (tnum == 0) {
					Peril::linesss.push_back(Peril::liness[j]);
					Peril::tempd.push_back(j);
				}
			}
			}
		}
		if (chek == 0) {
			//if i didnt block any other walls
			Peril::linesss.push_back(Peril::liness[i]);
			Peril::tempd.push_back(i);
		}
	}
        for (int i=0; i<Peril::liness.size(); i++) {
                TransformLine(Peril::linesss[i], Peril::player, Peril::tlines[i]);
        }
//	int tnext = 0;
	for (int tnext=0; tnext<Peril::liness.size(); tnext++) {
//		if (i != 0 && i != Peril::liness.size()-1) {
//			tnext = Peril::tlines[tnext].next;
//		}
//		else {
//			tnext = i;
//		}
	        int x1 = -tlines[tnext].x1 * 160 / tlines[tnext].z1;
	        int x2 = -tlines[tnext].x2 * 160 / tlines[tnext].z2;
		int y1a = (-(Peril::SCREEN_SIZE*2)) / tlines[tnext].z1;
		int y1b = (Peril::SCREEN_SIZE*2) / tlines[tnext].z1;
		int y2a = (-(Peril::SCREEN_SIZE*2)) / tlines[tnext].z2;
		int y2b = (Peril::SCREEN_SIZE*2) / tlines[tnext].z2;

		// TODO: Eventually implement the following into Core.  Currently they are massively worked around

		// Is casting everything to short int really the _best_ way to do this?
		short int wallx[] = {static_cast<short int>((Peril::SCREEN_SIZE/2)+x2), static_cast<short int>((Peril::SCREEN_SIZE/2)+x1), static_cast<short int>((Peril::SCREEN_SIZE/2)+x2)};
		short int wally[] = {static_cast<short int>((Peril::SCREEN_SIZE/2)+y2b), static_cast<short int>((Peril::SCREEN_SIZE/2)+y1a), static_cast<short int>((Peril::SCREEN_SIZE/2)+y2a)};
	        filledPolygonRGBA(Peril::renderer, wallx, wally, 3, Peril::lines[tnext].clr, Peril::lines[tnext].clr, Peril::lines[tnext].clr, 255);
	        short int wallx1[] = {static_cast<short int>((Peril::SCREEN_SIZE/2)+x1), static_cast<short int>((Peril::SCREEN_SIZE/2)+x2), static_cast<short int>((Peril::SCREEN_SIZE/2)+x1)};
	        short int wally1[] = {static_cast<short int>((Peril::SCREEN_SIZE/2)+y1b), static_cast<short int>((Peril::SCREEN_SIZE/2)+y2b), static_cast<short int>((Peril::SCREEN_SIZE/2)+y1a)};
	    	filledPolygonRGBA(Peril::renderer, wallx1, wally1, 3, Peril::lines[tnext].clr, Peril::lines[tnext].clr, Peril::lines[tnext].clr, 255);

                this->DrawLine(((SCREEN_SIZE/2)+x1), ((SCREEN_SIZE/2)+y1a), ((SCREEN_SIZE/2)+x2), ((SCREEN_SIZE/2)+y2a));
                this->DrawLine(((SCREEN_SIZE/2)+x1), ((SCREEN_SIZE/2)+y1b), ((SCREEN_SIZE/2)+x2), ((SCREEN_SIZE/2)+y2b));
                this->DrawLine(((SCREEN_SIZE/2)+x1), ((SCREEN_SIZE/2)+y1a), ((SCREEN_SIZE/2)+x1), ((SCREEN_SIZE/2)+y1b));
                this->DrawLine(((SCREEN_SIZE/2)+x2), ((SCREEN_SIZE/2)+y2a), ((SCREEN_SIZE/2)+x2), ((SCREEN_SIZE/2)+y2b));

	}
	gameover = Core::gameover;
}

void Peril::Move(int up, int down, int left, int right){
	float acceleration = 0;
	float vel1 = 0.0;
	float vel2 = 0.0;
	if (up == 1) {vel1 += cos(this->player.angle)*1.2; vel2 += sin(this->player.angle)*1.2;}
	if (down == 1) {vel1 -= cos(this->player.angle)*1.2; vel2 -= sin(this->player.angle)*1.2;}
	if (left == 1) {vel1 += sin(this->player.angle)*1.2; vel2 -= cos(this->player.angle)*1.2;}
	if (right == 1) {vel1 -= sin(this->player.angle)*1.2; vel2 += cos(this->player.angle)*1.2;}

        std::cout << "up: " << up << ", down: " << down <<", left: " << left << ", right: " << right <<std::endl;

        if (up == 1 || down == 1 || right == 1 || left == 1) {
		acceleration = 0.4;
	}
	this->player.velocity.x = this->player.velocity.x * (1-acceleration) + vel1 * acceleration;
	this->player.velocity.y = this->player.velocity.y * (1-acceleration) + vel2 * acceleration;

        std::cout << "vel1: " << vel1 << ", vel2 " << vel2 << std::endl;
	//this->MoveBy(vel1, vel2);


        float timestep = static_cast<float>(this->GetTicksSinceLastCall()) / 20.f;
        Peril::player.x += vel1 * timestep;
        Peril::player.y += vel2 * timestep;
        std::cout << "Moved: " << player.x << ", " << player.y << std::endl;

        SDL_GetRelativeMouseState(&x, &y);
	this->player.angle += x * 0.03;
	if (this->player.angle > 6.3 || this->player.angle < -6.3) {
		this->player.angle = 0;
	}
}
