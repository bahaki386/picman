#include "PixelMatrix.h"
#include <iostream>


int main(int argc, char** argv) {
	PixelMatrix input{};
	auto fr=input.readBmpFile("Lenna.bmp");
	if (!fr) return -1;
	std::cout << input.getWidth() << "," << input.getHeight() <<"\n";
	std::cout << input.showHistgram();
	auto fw = false;
	fw  = input.averageFilter().medianFilter().gaussianFilter().laplacianFilter().makeBmpFile("gauusian.bmp");
	if (!fw) return -2;
	std::cout << fr << "," << fw << "\n";
	return 0;
}