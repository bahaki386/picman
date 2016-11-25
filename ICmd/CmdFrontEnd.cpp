#include "PixelMatrix.h"
#include <iostream>


int main(int argc, char** argv) {
	PixelMatrix input{};
	auto fr=input.readBmpFile("Text.bmp");
	if (!fr) return -1;
	auto fh = std::ofstream("hist.txt");
	std::cout << input.getWidth() << "," << input.getHeight() <<"\n";
	std::cout << input.showHistgram();
	fh << input.showHistgram();
	auto fw = true;
	fw &= input.averageFilter().makeBmpFile("average.bmp");
	fw &= input.medianFilter().makeBmpFile("median.bmp");
	fw &= input.gaussianFilter().makeBmpFile("gaussian.bmp");
	fw &= input.binalize(input.detThreshold(3)).makeBmpFile("binalize.bmp");
	fw &= input.prewittFilter().makeBmpFile("prewitt.bmp");
	fw &= input.sobelFilter().makeBmpFile("sobel.bmp");
	fw &= input.laplacianFilter().makeBmpFile("lapalcian.bmp");
	if (!fw) return -2;
	std::cout << fr << "," << fw << "\n";
	return 0;
}