#include <iostream>
#include <stdio.h>
#include <string>
#include "Peril.h"

int main(int argc, char *args[]) {
	Peril peril;
	peril.InitSDL();
	peril.InitPeril();
	std::string fn(args[1]);
	peril.LoadMap(fn);
	peril.gameover = 0;
	while (!peril.gameover) {
		peril.Delay(5);
		peril.GetInput();
		peril.DoLines();
		peril.DrawSDL();
	}
	peril.QuitSDL();
}
