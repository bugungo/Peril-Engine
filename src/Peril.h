#ifndef PERIL_H
#define PERIL_H
#include <SDL2/SDL.h>
#include <string>
#include <vector>

class Peril {
	private:
		struct Player {
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
		};
		struct Line {
			int x1;
			int y1;
			int x2;
			int y2;
			int z1;
			int z2;
		};
		SDL_Window* window;
		SDL_Renderer* renderer;
		std::vector<Line> lines;
		std::vector<Line> tlines;
		Line templine;
		Player player;
		int FNcross(int, int, double, double);
		void Intersect(int, int, int ,int, double, double, int, int, int&, int&);
		void TransformLine(Line, Player, Line&);
		void MoveUp();
		void MoveDown();
		void MoveLeft();
		void MoveRight();
		int up = 0;
		int down = 0;
		int left = 0;
		int right = 0;
		int x;
		int y;
	public:
		const int SCREEN_SIZE = 400;
		int gameover = 0;
		void InitSDL();
		void QuitSDL();
		void InitPeril();
		void LoadMap(std::string);
		void Delay(int);
		void GetInput();
		void DoLines();
		void DrawSDL();
};

#endif
