#pragma once //for header files

#include <SDL.h>
#include <SDL_opengl.h>
#include <SDL_image.h>

class Entity {
public:
	Entity(float, float, float, GLuint, float, float, float = 0, float = 0, float = 0);
	void Draw();
	void setY(float);
	void setX(float);
	void setDire(float, float);
	float getX();
	float getY();
	float getDireX();
	float getDireY();
	float getSpeed();
	float updatePosX();
	float updatePosY();
private:

	float x;
	float y;
	float rotation;

	GLuint textureID;

	float width;
	float height;

	float speed;
	float direction_x;
	float direction_y;
};