#include <iostream>
#include <random>
#include "time.h"
#include "entity.h"
#define PI 3.14159265358979323846

SDL_Window* displayWindow;

float randomNumber(float min, float max) { //got this concept online with a small modification
	float randNum = ((float)rand()) / (float)RAND_MAX;
	float range = max - min;
	while ((randNum * range) + min < 0.5 && (randNum * range) + min > -0.5) {
		randNum = ((float)rand()) / (float)RAND_MAX;
	}
	return (randNum * range) + min;
}

GLuint LoadTexture(const char *image_path, GLuint imageFormat) {
	SDL_Surface *surface = IMG_Load(image_path);

	GLuint textureID;
	glGenTextures(1, &textureID);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glTexImage2D(GL_TEXTURE_2D, 0, 4, surface->w, surface->h, 0, GL_RGBA, GL_UNSIGNED_BYTE, surface->pixels);

	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	SDL_FreeSurface(surface);
	return textureID;
}

void Setup() {
	SDL_Init(SDL_INIT_VIDEO | SDL_INIT_JOYSTICK);
	displayWindow = SDL_CreateWindow("My Game", SDL_WINDOWPOS_CENTERED, SDL_WINDOWPOS_CENTERED, 800, 600, SDL_WINDOW_OPENGL);
	SDL_GLContext context = SDL_GL_CreateContext(displayWindow);
	SDL_GL_MakeCurrent(displayWindow, context);
	glViewport(0, 0, 800, 600);
	glMatrixMode(GL_PROJECTION);
	glOrtho(-1.33, 1.33, -1.0, 1.0, -1.0, 1.0);

	srand(time(NULL));
}

void Render(Entity& paddle1, Entity& paddle2, Entity& pb3, Entity& player1Win, Entity& player2Win, int& p1Pt, int& p2Pt) {
	glClearColor(1.0f, 1.0f, 1.0f, 0.0f);
	glClear(GL_COLOR_BUFFER_BIT);
	glMatrixMode(GL_MODELVIEW);

	//drawing line in the middle
	glLoadIdentity();
	GLfloat midLine[] = { -0.01f, 1.0f, -0.01f, -1.0f, 0.01f, -1.0f, 0.01f, 1.0f };
	glVertexPointer(2, GL_FLOAT, 0, midLine);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat midColor[] = { 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f, 0.65f };
	glColorPointer(3, GL_FLOAT, 0, midColor);
	glEnableClientState(GL_COLOR_ARRAY);

	glDrawArrays(GL_QUADS, 0, 4);

	glDisableClientState(GL_VERTEX_ARRAY);
	glDisableClientState(GL_COLOR_ARRAY);

	if (p1Pt == 7) {
		player1Win.Draw();
	}
	else if (p2Pt == 7) {
		player2Win.Draw();
	}
	else {
		pb3.Draw();
		paddle1.Draw();
		paddle2.Draw();
	}

	SDL_GL_SwapWindow(displayWindow);
}

bool Process(SDL_Event& event) {
	while (SDL_PollEvent(&event)) {
		if (event.type == SDL_QUIT || event.type == SDL_WINDOWEVENT_CLOSE) {
			return true;
		}
	}
	return false;
}

void Update (float& paddle1Y, float& paddle2Y, bool& scored, float elapsed, const Uint8* keys, Entity& paddle1, Entity& paddle2, Entity& pb3, int& p1Pt, int& p2Pt) {
	float p1Y = paddle1.getY();
	float p2Y = paddle2.getY();
	float bX = pb3.getX();
	float bY = pb3.getY();

	paddle2Y = 0.0;
	paddle1Y = 0.0;

	if (scored == true) {
		if (keys[SDL_SCANCODE_SPACE]) {
			scored = false;
			pb3.setDire(randomNumber(-1.5f, 1.5f), randomNumber(-1.5f, 1.5f));
		}
	}
	if (keys[SDL_SCANCODE_W]) {
		paddle1Y = 1.5;
	}
	if (keys[SDL_SCANCODE_S]) {
		paddle1Y = -1.5;
	}
	if (keys[SDL_SCANCODE_UP]) {
		paddle2Y = 1.5;
	}
	if (keys[SDL_SCANCODE_DOWN]) {
		paddle2Y = -1.5;
	}

	if (p1Pt < 7 || p2Pt < 7) {
		//paddle logic
		if (paddle1.getY() + (paddle1Y * elapsed) > -0.85 && paddle1.getY() + (paddle1Y * elapsed) < .85)
			paddle1.setY(p1Y += paddle1Y * elapsed);
		if (paddle2.getY() + (paddle2Y * elapsed) > -0.85 && paddle2.getY() + (paddle2Y * elapsed) < .85)
			paddle2.setY(p2Y += paddle2Y * elapsed);

		//check if game started
		if (scored == false) {
			//ball logic
			if (pb3.getY() > 0.95) {
				if (pb3.getDireY() > 0) //in case ball overshoots into the wall to prevent ball from "sliding" on surface
					pb3.setDire(pb3.getDireX(), -(pb3.getDireY()));
			}
			else if (pb3.getY() < -0.95) {
				if (pb3.getDireY() < 0) //in case ball overshoots into the wall to prevent ball from "sliding" on surface
					pb3.setDire(pb3.getDireX(), -(pb3.getDireY()));
			}

			//paddle collision
			if ((pb3.getX() + 0.05f > paddle2.getX() - 0.05f) && (pb3.getY() + 0.05f > paddle2.getY() - 0.15f && pb3.getY() - 0.05f < paddle2.getY() + 0.15f)) {
				if (pb3.getDireX() > 0) //in case ball overshoots into the paddle to prevent ball from "sliding" on surface
					pb3.setDire(-(pb3.getDireX()), pb3.getDireY());
			}
			else if ((pb3.getX() - 0.05f < paddle1.getX() + 0.05f) && (pb3.getY() + 0.05f > paddle1.getY() - 0.15f && pb3.getY() - 0.05f < paddle1.getY() + 0.15f)) {
				if (pb3.getDireX() < 0) //in case ball overshoots into the paddle to prevent ball from "sliding" on surface
					pb3.setDire(-(pb3.getDireX()), pb3.getDireY());
			}
			else if (pb3.getX() > 1.23) { //if ball passes paddle, score
				bX = 0;
				bY = 0;
				pb3.setDire(randomNumber(-1.5f, 1.5f), randomNumber(-1.5f, 1.5f));
				scored = true;
				++p1Pt;
			}
			else if (pb3.getX() < -1.23) {
				bX = 0;
				bY = 0;
				pb3.setDire(randomNumber(-1.5f, 1.5f), randomNumber(-1.5f, 1.5f));
				scored = true;
				++p2Pt;
			}
		}
		else if (scored == true) {
			pb3.setDire(0.0f, 0.0f);
		}

		pb3.setX(bX + pb3.getDireX() * pb3.getSpeed() * elapsed);
		pb3.setY(bY + pb3.getDireY() * pb3.getSpeed() * elapsed);
	}
}

int main(int argc, char *argv[])
{
	Setup();

	Entity paddle1(-1.28f, 0.0f, 0.0f, LoadTexture("paddle.png", GL_BGRA), 0.1f, 0.3f);
	Entity paddle2(1.28f, 0.0f, 0.0f, LoadTexture("paddle.png", GL_BGRA), 0.1f, 0.3f);
	Entity pb3(0.0f, 0.0f, 0.0f, LoadTexture("pokeball.jpg", GL_BGRA), 0.1f, 0.1f, 1.0f, randomNumber(-1.5f, 1.5f), randomNumber(-1.5f, 1.5f));
	Entity player1Win(0.0f, 0.0f, 0.0f, LoadTexture("p1win.png", GL_RGBA), 0.7f, 0.5f, 0.0f, 0.0f, 0.0f);
	Entity player2Win(0.0f, 0.0f, 0.0f, LoadTexture("p2win.png", GL_RGBA), 0.7f, 0.5f, 0.0f, 0.0f, 0.0f);

	bool done = false; 
	bool scored = true;

	SDL_Event event;

	float lastFrameTicks = 0.0f;
	float paddle2Y = 0.0;
	float paddle1Y = 0.0;

	int p1Pt = 0;
	int p2Pt = 0;

	const Uint8 *keys = SDL_GetKeyboardState(NULL);

	while (!Process(event )) {

		float ticks = (float)SDL_GetTicks() / 1000.0f;
		float elapsed = ticks - lastFrameTicks;
		lastFrameTicks = ticks;

		Update(paddle1Y, paddle2Y, scored, elapsed, keys, paddle1, paddle2, pb3, p1Pt, p2Pt);

		Render(paddle1, paddle2, pb3, player1Win, player2Win, p1Pt, p2Pt);
	}

	SDL_Quit();
	return 0;
}