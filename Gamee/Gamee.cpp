#include <SDL.h>
#include <SDL_image.h>
#include <SDL_ttf.h>
#include <iostream>
#include <string>

using namespace std;

const int SCREEN_WIDTH = 1440;
const int SCREEN_HEIGHT = 900;

SDL_Window* win = NULL;
SDL_Renderer* ren = NULL;
SDL_Texture* background = NULL, * image = NULL, *aimI = NULL, *fuelI = NULL, *gameover = NULL;



// Генерируем рандомное число между значениями min и max
// Предполагается, что функцию srand() уже вызывали
int getRandomNumber(int min, int max)
{
	static const double fraction = 1.0 / (static_cast<double>(RAND_MAX) + 1.0);
	// Равномерно распределяем рандомное число в нашем диапазоне
	return static_cast<int>(rand() * fraction * (max - min + 1) + min);
}

SDL_Texture* LoadTexture(const std::string& file, SDL_Renderer* ren) {

	SDL_Texture* texture = IMG_LoadTexture(ren, file.c_str());
	if (!texture) {
		std::cout << IMG_GetError();
	}
	return texture;
}

void ApplySurface(int x, int y, SDL_Texture* tex, SDL_Renderer* rend) {
	SDL_Rect pos;
	pos.x = x;
	pos.y = y;
	SDL_QueryTexture(tex, NULL, NULL, &pos.w, &pos.h);
	SDL_RenderCopy(rend, tex, NULL, &pos);
}

int init() {

	if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
		std::cout << "Can't init: " << SDL_GetError() << std::endl;
		system("pause");
		return false;
	}

	int flags = IMG_INIT_PNG;
	if (!(IMG_Init(flags) & flags)) {
		std::cout << "Can't init: " << SDL_GetError() << std::endl;
		system("pause");
		return false;
	}

	win = SDL_CreateWindow("Game!",
		SDL_WINDOWPOS_CENTERED,
		SDL_WINDOWPOS_CENTERED,
		SCREEN_WIDTH,
		SCREEN_HEIGHT,
		SDL_WINDOW_SHOWN);
	if (win == NULL) {
		std::cout << "SDL_CreateWindow Error: " << SDL_GetError() << std::endl;
		system("pause");
		return false;
	}

	ren = SDL_CreateRenderer(win, -1, SDL_RENDERER_ACCELERATED | SDL_RENDERER_PRESENTVSYNC);
	if (ren == NULL) {
		std::cout << "SDL_CreateRenderer Error: " << SDL_GetError() << std::endl;
		system("pause");
		return false;
	}

	return true;
}

int load() {
	background = LoadTexture("list.png", ren);
	image = LoadTexture("tankUP.png", ren);
	gameover = LoadTexture("gameover.jpg", ren);
	aimI = LoadTexture("aim.png", ren);
	fuelI = LoadTexture("fueli.png", ren);

	if (!background | !image | !aimI | !fuelI) {
		return false;
	}
	return true;
}



/**
* Отобразить SDL_Texture на SDL_Renderer на координатах x, y, с масштабированием
* @param tex Текстура для отображения
* @param ren Рендерер
* @param x Координаты
* @param y
* @param w Фактический размер при отрисовке
* @param h
*/
void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y, int w, int h)
{
	SDL_Rect dst;
	dst.x = x;
	dst.y = y;
	dst.w = w;
	dst.h = h;
	SDL_RenderCopy(ren, tex, NULL, &dst);
}

/**
* Отрисовать SDL_Texture на SDL_Renderer на координатах x, y, без масштабирования
* @param tex Текстура
* @param ren Рендерер
* @param x Коодринаты
* @param y
*/
void renderTexture(SDL_Texture* tex, SDL_Renderer* ren, int x, int y)
{
	int w, h;
	SDL_QueryTexture(tex, NULL, NULL, &w, &h);
	renderTexture(tex, ren, x, y, w, h);
}

void quit() {
	SDL_DestroyTexture(background);
	SDL_DestroyRenderer(ren);
	SDL_DestroyTexture(gameover);
	SDL_DestroyTexture(image);
	SDL_DestroyTexture(aimI);
	SDL_DestroyWindow(win);

	SDL_Quit();
	IMG_Quit();
	TTF_Quit();
}

int main(int argc, char** argv) {
	bool run = true;
	bool gameoverb = false;
	SDL_Event e;
	SDL_Rect tank;
	SDL_Rect aim;
	SDL_Rect fuel;

	int fx = 0;
	int fy = 0;
	int quantity = 0;
	int score = 0;
	fuel.x = fx;
	fuel.y = fy;

	int ax = 705;
	int ay = 450;
	int speed_aim = 1;
	aim.x = ax;
	aim.y = ay;

	int x = 705;
	int y = 720;
	int speed_tank = 15;
	tank.x = x;
	tank.y = y;

	bool point = true;

	if (!init()) {
		quit();
		return false;
	}

	if (!load()) {
		quit();
		return false;
	}

	while (run) {

		if (point == true) {
			fuel.x = fx = getRandomNumber(1, 80) * 15;
			fuel.y = fy = getRandomNumber(1, 50) * 15;

			point = false;
		}

		if (ax > x) {
			aim.x = ax -= speed_aim;
		}
		if (ax < x) {
			aim.x = ax += speed_aim;
		}
		if (ay < y) {
			aim.y = ay += speed_aim;
		}
		if (ay > y) {
			aim.y = ay -= speed_aim;
		}

		while (SDL_PollEvent(&e) != 0) {
			if (e.type == SDL_QUIT) {
				run = false;
			}

			if (e.type == SDL_KEYDOWN) {

				if (e.key.keysym.sym == SDLK_RIGHT) {
					tank.x = x += 15;
				}
				if (e.key.keysym.sym == SDLK_LEFT) {
					tank.x = x -= 15;
				}
				if (e.key.keysym.sym == SDLK_DOWN) {
					tank.y = y += 15;
				}
				if (e.key.keysym.sym == SDLK_UP) {
					tank.y = y -= 15;
				}
				if (e.key.keysym.sym == SDLK_ESCAPE) {
					run = false;
				}
			}
		}
		if (x + 90 == 1440) {
			tank.x = x -= speed_tank;
		}
		if (x == 0) {
			tank.x = x += speed_tank;
		}
		if (y + 75 == 885) {
			tank.y = y -= speed_tank;
		}
		if (y == 0) {
			tank.y = y += speed_tank;
		}

		if (x + 45 >= ax && x + 45 <= ax + 90 && y + 60 <= ay + 90 && y + 60 >= ay) {
			run = false;
			gameoverb = true;
		}

		if (x + 45 <= fx + 50 && x + 45 >= fx && y + 35 <= fy + 60 && y + 35 >= fy) {
			score += 1;
			quantity += 1;
			point = true;
		}
		if (quantity == 10) {
			speed_aim += 1;
			quantity = 0;
		}

		SDL_RenderClear(ren);

		renderTexture(background, ren, 0, 0, 1440, 900);

		int fW, fH;
		SDL_QueryTexture(fuelI, NULL, NULL, &fW, &fH);
		renderTexture(fuelI, ren, fx, fy, 50, 60);

		int aW, aH;
		SDL_QueryTexture(aimI, NULL, NULL, &aW, &aH);
		renderTexture(aimI, ren, ax, ay, 90, 90);

		int iW, iH;
		SDL_QueryTexture(image, NULL, NULL, &iW, &iH);
		renderTexture(image, ren, x, y, 90, 90);

		SDL_RenderPresent(ren);

	}

	while (gameoverb)
	{
		SDL_RenderClear(ren);

		renderTexture(gameover, ren, 0, 0, 1440, 900);

		SDL_RenderPresent(ren);

		SDL_Delay(3000);

		gameoverb = false;
	}

	quit();

	return true;
}