#include "entity.h"

Entity::Entity(float x, float y, float rot, GLuint textID, float w, float h, float speed, float direX, float direY) : x(x), y(y), rotation(rot), 
	textureID(textID), width(w), height(h), speed(speed), direction_x(direX), direction_y(direY) {}

void Entity::setX(float newX) {
	x = newX;
}

void Entity::setY(float newY) {
	y = newY;
}

float Entity::getX() {
	return x;
}

float Entity::getY() {
	return y;
}

float Entity::getDireX() {
	return direction_x;
}

float Entity::getDireY() {
	return direction_y;
}

float Entity::getSpeed() {
	return speed;
}

void Entity::setDire(float newX, float newY) {
	direction_x = newX;
	direction_y = newY;
}

void Entity::Draw() {
	glEnable(GL_TEXTURE_2D);
	glBindTexture(GL_TEXTURE_2D, textureID);

	glMatrixMode(GL_MODELVIEW);

	glLoadIdentity();
	glTranslatef(x, y, 0.0);
	glRotatef(rotation, 0.0, 0.0, 1.0);

	GLfloat quad[] = { -width * 0.5f, height * 0.5f, -width * 0.5f, -height * 0.5f, width * 0.5f, -height * 0.5f, width * 0.5f, height * 0.5f };
	glVertexPointer(2, GL_FLOAT, 0, quad);
	glEnableClientState(GL_VERTEX_ARRAY);

	GLfloat quadUVs[] = { 0.0, 0.0, 0.0, 1.0, 1.0, 1.0, 1.0, 0.0 };
	glTexCoordPointer(2, GL_FLOAT, 0, quadUVs);
	glEnableClientState(GL_TEXTURE_COORD_ARRAY);

	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

	glDrawArrays(GL_QUADS, 0, 4);
	glDisable(GL_TEXTURE_2D);
}
float Entity::updatePosX() {
	return speed * direction_x;
}
float Entity::updatePosY() {
	return speed * direction_y;
}