#include "core.h"


void Core::CreateWindow(std::string title, int in_width, int in_height){
    Core::window = SDL_CreateWindow(title.c_str(), SDL_WINDOWPOS_UNDEFINED, SDL_WINDOWPOS_UNDEFINED, 400, 400, SDL_WINDOW_SHOWN);

    // Renderer is required at the moment (and we don't have a vsync option), so we will create it here anyway
    Core::CreateRenderer();
}

void Core::CreateRenderer(){
    renderer = SDL_CreateRenderer(window, -1, SDL_RENDERER_ACCELERATED);
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
	SDL_SetRelativeMouseMode(SDL_TRUE);
}

void Core::GetInput() {
	SDL_Event event;
	if (SDL_PollEvent(&event)) {
		switch(event.type) {
			case SDL_QUIT: this->gameover = 1; break;
			case SDL_KEYDOWN:
				if (event.key.keysym.sym == SDLK_w) {this->up = 1;}
                                if (event.key.keysym.sym == SDLK_s) {this->down = 1;}
                                if (event.key.keysym.sym == SDLK_a) {this->left = 1;}
                                if (event.key.keysym.sym == SDLK_d) {this->right = 1;}
                                if(event.key.keysym.sym == SDLK_ESCAPE){this->gameover = 1;}
			break;
                        case SDL_KEYUP:
                                if (event.key.keysym.sym == SDLK_w) {this->up = 0;}
                                if (event.key.keysym.sym == SDLK_s) {this->down = 0;}
                                if (event.key.keysym.sym == SDLK_a) {this->left = 0;}
                                if (event.key.keysym.sym == SDLK_d) {this->right = 0;}
                        break;
		}
	}
	if (this->up == 1) {this->MoveUp();}
	if (this->down == 1) {this->MoveDown();}
	if (this->left == 1) {this->MoveLeft();}
	if (this->right == 1) {this->MoveRight();}
	SDL_GetRelativeMouseState(&x, &y);
	this->player.angle += x * 0.03;
}

void Core::Delay(int a){
    SDL_Delay(a);      // filledPolygonRGBA(Peril::render
}

void Core::DrawLine(int x1, int y1, int x2, int y2){
    SDL_SetRenderDrawColor(renderer, 0, 0, 0, 0);
    SDL_RenderDrawLine(renderer, x1, y1, x2, y2);
}

void Core::Redraw(){
	 SDL_RenderPresent(this->renderer);
    SDL_SetRenderDrawColor(this->renderer, 0xFF, 0xFF, 0xFF, 0xFF);
	SDL_RenderClear(this->renderer);
   
}
