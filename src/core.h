#ifndef _CORE_h
#define _CORE_h 1
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

class Core{
    public:
        Core();
        void CreateWindow(std::string, int, int);
        void DrawLine(int, int, int, int);
        void Delay(int);
        void Redraw();

        void GetInput();

        unsigned int GetTicksSinceLastCall();

        const int get_width();
        const int get_height();

        int gameover;
    private:

        unsigned int gameticks;

        void CreateRenderer();
        SDL_Window* window;


        int width;
        int height;

    protected:
        SDL_Renderer* renderer;
        int up;
        int down;
        int left;
        int right;



        int x;
        int y;

	struct Velocity {
		double x;
		double y;
	};

        struct Player {
			double x;
			double y;
			double angle;
			Velocity velocity;
			double pcos() {
				double r = round(cos(angle));
				return r;
			}
			double psin() {
				double r = round(sin(angle));
				return r;
			}
		};
        Player player;

        virtual void MoveBy(double, double){};
        virtual void Move(int, int, int, int){};

};

#endif // _CORE_h
