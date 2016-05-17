#include "PixelMatrix.h"
#include <iostream>


int main(int argc, char** argv) {
	PixelMatrix input{};
	auto fr=input.readBmpFile("Lenna.bmp");
	if (!fr) return -1;
	std::cout << input.getWidth() << "," << input.getHeight() <<"\n";
	std::cout << input.showHistgram();
	auto fw=input.prewittFilter().makeBmpFile("prewitt.bmp");
	fw &= input.medianFilter().makeBmpFile("median.bmp");
	fw &= input.averageFilter().makeBmpFile("average.bmp");
	if (!fw) return -2;
	std::cout << fr << "," << fw << "\n";
	return 0;
}