/*
 * matrix.h
 *
 *  Created on: 3 sep. 2018
 *      Author: Mattias Larsson Sköld
 */

#pragma once

#include "vec.h"

template <class T>
class Matrix {
public:

	inline T *get() {
		return &x1;
	}

	inline T *get() const {
		return &x1;
	}

	inline T &at(int i) {
		return (&x1)[i];
	}

	inline T at(int i) const {
		return (&x1)[i];
	}

	inline T&at(int x, int y) {
		return get()[x + y * 4];
	}

	inline T&at(int x, int y) const {
		return get()[x + y * 4];
	}

	inline T &operator [](int i) {
		return at(i);
	}

	inline T operator [](int i) const {
		return at(i);
	}


	inline T &operator() (int x, int y) {
		return at(x, y);
	}

	operator T*() {
		return &x1;
	}

	void identity() {
		for (int i = 0; i < 16; ++i) {
			at(i) = (i % 5) == 0;
		}
	}

	void clear() {
		for (int i = 0; i < 16; ++i) {
			at(i) = 0;
		}
	}

	void fill(T value) {
		for (int i = 0; i < 16; ++i) {
			at(i) = value;
		}
	}

	void translate(T x, T y, T z) {
		x4 += x;
		y4 += y;
		z4 += z;
	}

	void translate(Vec p) {
		translate(p.x, p.y, p.z);
	}

	//Set the tranlation in absolute numbers
	void setTranslation(T x, T y, T z) {
		x4 = x;
		y4 = y;
		z4 = z;
	}

	void scale(T x, T y, T z) {
		x1 *= x; y1 *= y; z1 *= z;
		x2 *= x; y2 *= y; z2 *= z;
		x3 *= x; y3 *= y; z3 *= z;
	}

	void scale (T s) {
		scale(s, s, s);
	}


	Matrix operator * (const Matrix &m) const {
		throw "it does not work yet";
		Matrix returnMatrix;

		for (int x = 0; x < 4; ++x) for (int y = 0; y < 4; ++y) {
			returnMatrix(x, y) = 0;
			for (int i = 0; i < 4; ++i) {
				returnMatrix(x, y) += ((*this)[x, i] * m[i, y]);
			}
		}

		return returnMatrix;
	}

	Matrix &operator *=(const Matrix &m) {
		Matrix mTemp(*this * m);
		(*this) = mTemp;
		return *this;
	}


	Vec getRow(int num) {
		return Vec(at(0, num), at(1, num), at(2, num));
	}

	Vec getCol(int num) {
		return Vec(at(num, 0), at(num, 1), at(num, 2));
	}

	static Matrix Identity() {
		Matrix m;
		m.identity();
		return m;
	}

	static Matrix RotationX(double angle) {
		Matrix m = Identity();
		double c = cos(angle);
		double s = sin(angle);
		m.y2 = c; m.z2 = -s;
		m.y3 = s; m.z3 = c;
		return m;
	}


	static Matrix RotationY(double angle) {
		Matrix m = Identity();
		double c = cos(angle);
		double s = sin(angle);
		m.x1 = c; m.z1 = s;
		m.x3 = s; m.z3 = -c;
		return m;
	}


	static Matrix RotationZ(double angle) {
		Matrix m = Identity();
		double c = cos(angle);
		double s = sin(angle);
		m.x1 = c; m.y1 = -s;
		m.x2 = s; m.y2 = c;
		return m;
	}



	float x1, y1, z1, w1;
	float x2, y2, z2, w2;
	float x3, y3, z3, w3;
	float x4, y4, z4, w4;
};


typedef Matrix<float> Matrixf;
typedef Matrix<double> Matrixd;

