#ifndef _CORE_h
#define _CORE_h 1
#include <iostream>
#include <string>
#include <SDL2/SDL.h>
#include <SDL2/SDL2_gfxPrimitives.h>

class Core{
    public:
        Core(){SDL_Init(SDL_INIT_EVERYTHING);};
        void CreateWindow(std::string, int, int);
        void DrawLine(int, int, int, int);
        void Delay(int);
        void Redraw();

        void GetInput();

        const int get_width();
        const int get_height();

        int gameover;
    private:
        void CreateRenderer();
        SDL_Window* window;
        SDL_Renderer* renderer;

        int width;
        int height;

    protected:
        
        int up;
        int down;
        int left;
        int right;

        

        int x;
        int y;

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
        Player player;

        virtual void MoveUp(){};
        virtual void MoveDown(){};
        virtual void MoveLeft(){};
        virtual void MoveRight(){};


};

#endif // _CORE_h