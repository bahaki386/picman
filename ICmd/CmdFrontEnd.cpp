#include "PixelMatrix.h"
#include <iostream>


int main(int argc, char** argv) {
	PixelMatrix input{};
	auto fr=input.readBmpFile("Text.bmp");
	if (!fr) return -1;
	std::cout << input.getWidth() << "," << input.getHeight() <<"\n";
	std::cout << input.showHistgram();
	auto fw = true;
	auto s=input.gaussianFilter();
	auto p = s.prewittFilter();
	fw &= s.makeBmpFile("s.bmp");
	fw &= input.prewittFilter().makeBmpFile("prewitt.bmp");
	fw &= input.sobelFilter().makeBmpFile("sobel.bmp");
	fw &= input.laplacianFilter().makeBmpFile("lapalcian.bmp");
	fw &= p.makeBmpFile("s-prewitt.bmp");
	if (!fw) return -2;
	std::cout << fr << "," << fw << "\n";
	return 0;
}