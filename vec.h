#ifndef __Vec__
#define __Vec__

#include <math.h>
//#include <LinearMath/btVector3.h>
#include "common.h" //for PI

class Vec
{
	public:
		double x, y, z;

		Vec (): x(0), y(0), z(0) {};
		Vec (double nx, double ny, double nz):x(nx), y(ny), z(nz) {}
		Vec (double nx, double ny): x(nx), y(ny), z(0) {}
		Vec (const Vec& v): x(v.x), y(v.y), z(v.z) {}


		Vec operator+=(Vec v){
			x += v.x;
			y += v.y;
			z += v.z;
			return *this;
		}

		Vec operator-=(Vec v){
			x -= v.x;
			y -= v.y;
			z -= v.z;
			return *this;
		}


		Vec operator*=(double t){
			x *= t;
			y *= t;
			z *= t;
			return *this;
		}

		Vec operator/=(double t){
			x /= t;
			y /= t;
			z /= t;
			return *this;
		}

		Vec operator*(double t){
			return Vec(x *t, y*t, z*t);
		}
		Vec operator/(double t){
			return Vec(x /t, y/t, z/t);
		}

		double operator *(Vec v2){
			return x * v2.x +
				y * v2.y +
				z + v2.z;
		}

		Vec operator -(Vec v){
			return Vec(
				x - v.x,
				y - v.y,
				z - v.z);
		}

		Vec operator +(Vec v){
			return Vec(
				x + v.x,
				y + v.y,
				z + v.z);
		}

		bool operator== (Vec v){
			return x == v.x && y == v.y && z == v.z;
		}

		double abs(){
			return sqrt(x*x + y*y + z*z);
		}

		double abs2(){
			return x*x + y*y + z*z;
		}

		Vec normalize(){
			* this /= abs();
			return *this;
		}

		Vec cross(Vec v){
			return Vec(
				y * v.z - z * v.y,
				z * v.x - x * v.z,
				x * v.y - y * v.x
			);
		}

		double angle(double a){
			double angle = atan2(x,y) + a;

			redo1:
			if (angle < pi){
				angle += pi2;
				goto redo1;
			}

			redo2:
			if (angle > pi){
				angle -= pi2;
				goto redo2;
			}
			return angle;
		}

		double angle(){
			return atan2(x,y);
		}
};


namespace map{

	class mapVec{
	public:
		mapVec(): x(0), y(0){}
		mapVec(int nx, int ny): x(nx), y(ny){}
		mapVec(Vec v, int size){
			conv(v, size);
		}
		int x, y;

		void conv(Vec v, int size){
			x = v.x + .5 - (double) size / 2.;
			y = v.y + .5 - (double) size / 2.;
		}

		Vec toVec(int size) const{
			return Vec( (double) x + (double) size / 2. - .5,
						(double) y + (double) size / 2. - .5);
		}


		mapVec operator+(mapVec v){
			return mapVec(
				x + v.x,
				y + v.y);
		}

		mapVec operator-(mapVec v){
			return mapVec(
				x - v.x,
				y - v.y);
		}
		bool operator ==(mapVec v){
			if (v.x == x && v.y == y){
				return true;
			}
			return false;
		}
		bool operator !=(mapVec v){
			if (v.x == x && v.y == y){
				return false;
			}
			return true;
		}
	};
}


template <class Ar>
void serialize (Ar ar, Vec& v){
	ar & v.x;
	ar & v.y;
	ar & v.z;
}

#endif
