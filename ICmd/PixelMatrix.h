#pragma once
#include<fstream>
#include<sstream>
#include<algorithm>
#include<numeric>
#include"PixelMatrixConstants.h"

class PixelMatrix
{
	std::vector<std::vector<Pixel>> mat;
	std::vector<size_t> hist;
	std::vector<char> header_raw;
	BMPHEADER header;
	int
		width,
		height;
	size_t
		header_size,
		padding_size;
	bool
		flagh;	

public:
	
	bool readBmpFile(const char* fileName);
	
	bool makeBmpFile(const char* fileName);
	
	PixelMatrix apllyFilter(const Operator& opr);

	PixelMatrix averageFilter(void);
	PixelMatrix prewittFilter(void);
	PixelMatrix sobelFilter(void);
	PixelMatrix laplacianFilter(void);
	PixelMatrix medianFilter(void);
	PixelMatrix gaussianFilter(void);

	PixelMatrix binalize(std::vector < std::vector<Pixel >>  threshold);
	PixelMatrix binalize(Pixel threshold);
	std::vector<std::vector<Pixel>>  detThreshold(void);
	Pixel detThreshold(Pixel,Pixel);
	inline const int getWidth(void) {
		return width;
	}

	inline const int getHeight(void) {
		return height;
	}

	std::string showHistgram(void);

	private:
	static constexpr int
		posOffsetSize = 0xa,
		posWidth = 0x12,
		posHeight = 0x16;
	
	
	
	
	///<summary>
	///���ډ�f��8-�ߖT�I�y���[�^��K�p����
	///</summary>
	///<param name="x">
	///���ډ�f��X���W
	///</param>
	///<param name="y">
	///���ډ�f��Y���W
	///</param>
	///<param name="opr">
	///�K�p����I�y���[�^
	///</param>
	///<param name="d">
	///����
	///</param>
	///<returns>
	///�I�y���[�^�K�p��̒��ډ�f�̒l
	///</returns>
	int applyOperatorOnPixel(const int x, const int y, const Operator& opr, Dim d);
	
	///<summary>
	///8-�ߖT�̒����l�𓾂�
	///</summary>
	///<param name="x">
	///���ډ�f��X���W
	///</param>
	///<param name="y">
	///���ډ�f��Y���W
	///</param>
	Pixel calcMedianOfNeighbor(const int x, const int y,const int size);
	
	///<summary>
	///*this�̃q�X�g�O�����𐶐���this->hist�Ɋi�[����
	///</summary>
	void makeHistgram(void);

	
};
