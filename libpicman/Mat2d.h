#pragma once
#include "stdafx.h"
template<typename T>
class Mat2d
{
private:
	size_t xsize, ysize;
	std::valarray<T> mat;
public:
	Mat2d(size_t, size_t);
	Mat2d(size_t,size_t,std::valarray<T>);
	Mat2d() :xsize(0),ysize(0),mat(0){}
	const T& at(size_t x,size_t y) const;
	void assign(size_t, size_t);
	T& at(size_t x,size_t y);
	size_t size(void);
};
template<typename T>
Mat2d<T>::Mat2d(size_t x, size_t y) :xsize(x), ysize(y), mat(xsize*ysize) {}

template<typename T>
Mat2d<T>::Mat2d(size_t x, size_t y, std::valarray<T> m) : xsize(x), ysize(y), mat(m) { if (x*y != m.size())throw std::invalid_argument("Mat2d:check"); }

template<typename T>
const T & Mat2d<T>::at(size_t x, size_t y) const
{
	if ((y*xsize + x) > xsize*ysize) throw std::out_of_range("Mat2d:Read:RangeError");
	return (mat[y*xsize + x]);
}

template<typename T>
void Mat2d<T>::assign(size_t x, size_t y)
{
	xsize = x;
	ysize = y;
	mat = std::valarray<T>(x*y);
	return;
}

template<typename T>
T & Mat2d<T>::at(size_t x, size_t y)
{
	if ((y*xsize + x) > xsize*ysize) throw std::out_of_range("Mat2d:Write:RangeError");
	return (mat[y*xsize + x]);
}

template<typename T>
size_t Mat2d<T>::size(void)
{
	return xsize;
}






