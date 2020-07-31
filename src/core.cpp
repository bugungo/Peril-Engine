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
	std::cout << this->player.angle << std::endl;
	SDL_Event event;
	float acceleration = 0;
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
	float vel1 = 0;
	float vel2 = 0;
	if (this->up == 1) {vel1 += cos(this->player.angle)*1.2; vel2 += sin(this->player.angle)*1.2;}
	if (this->down == 1) {vel1 -= cos(this->player.angle)*1.2; vel2 -= sin(this->player.angle)*1.2;}
	if (this->left == 1) {vel1 += sin(this->player.angle)*1.2; vel2 -= cos(this->player.angle)*1.2;}
	if (this->right == 1) {vel1 -= sin(this->player.angle)*1.2; vel2 += cos(this->player.angle)*1.2;}
	if (this->up == 1 || this->down == 1 || this->right == 1 || this->left == 1) {
		acceleration = 0.4;
	}
	this->player.velocity.x = this->player.velocity.x * (1-acceleration) + vel1 * acceleration;
	this->player.velocity.y = this->player.velocity.y * (1-acceleration) + vel2 * acceleration;
	std::cout << this->player.velocity.x << ", " << this->player.velocity.y << std::endl;
	this->MoveBy(vel1, vel2);
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
