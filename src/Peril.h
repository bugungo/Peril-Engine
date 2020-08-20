#ifndef PERIL_H
#define PERIL_H

#include <string>
#include <vector>

#include "core.h"

class Peril : public Core{
	private:
		/*struct Player {
			int x;
			int y;
			double angle;
			double pcos() {
				double r = round(cos(angle));
				return r;
			}
			double psin() {
				double r = round(sin(angle));
				return r;
			}
		};*/
		struct Line {
			int x1;
			int y1;
			int x2;
			int y2;
			int z1;
			int z2;
			int clr;
			int next;
		};
		struct DLine{
			int num;
			int dist;
		};
		//SDL_Window* window;
		//SDL_Renderer* renderer;
		std::vector<Line> lines;
		std::vector<Line> tlines;
		std::vector<Line> liness;
		std::vector<Line> linesss;
		std::vector<DLine> dlines;
		std::vector<int> dliness;
		std::vector<int> tempd;
		Line templine;
		DLine dline;
		//Player player;
		int FNcross(int, int, double, double);
		void Intersect(int, int, int ,int, double, double, int, int, int&, int&);
		bool DoIntersect(int, int, int, int, int, int, int ,int, int&, int&);
		void TransformLine(Line, Player, Line&);
		void MoveBy(double, double);
		void Move(int, int, int, int);
		int LineDistance(int, int, int, int);
		int up = 0;
		int down = 0;
		int left = 0;
		int right = 0;
		int x;
		int y;
	public:
		Peril(){
			Peril::player.x = 0.0;
			Peril::player.y = 0.0;
			Peril::player.angle = 0.0;
		};
		const int SCREEN_SIZE = 400;
		int gameover = 0;
		// - void InitSDL();
		// - void QuitSDL();
		void InitPeril();
		void LoadMap(std::string);
		// - void Delay(int);
		// - void GetInput();
		void DoLines();
		// - void DrawSDL();
};

#endif
