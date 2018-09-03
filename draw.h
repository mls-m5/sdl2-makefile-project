/*
 * draw.h
 *
 *  Created on: 19 aug 2014
 *      Author: mattias
 */

#ifndef DRAW_H_
#define DRAW_H_



struct vec {
	vec(double x, double y, double z): x(x), y(y), z(z) {}
	vec(double x, double y): x(x), y(y), z(0) {}
	double x; double y; double z;
};

enum DrawStyle {
	DRAW_STYLE_FILLED,
	DRAW_STYLE_LINES
};

bool initDrawModule(double width, double height);
void QuitDrawModule();

void modelTransform(unsigned int pointer, vec p, double a, double scaleX, double scaleY);
void resetTransform(unsigned int poniter);
void setCam(vec p, double a);
void drawSquare(class Vec p, double a, double scaleX, double scaleY, DrawStyle);
void setDimensions(double width, double height);

#endif /* DRAW_H_ */
