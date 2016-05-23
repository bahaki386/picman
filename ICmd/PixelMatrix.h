#pragma once
#include"PixelMatrixConstants.h"

class PixelMatrix
{
	std::vector<std::vector<Pixel>> mat;
	std::vector<size_t> hist;
	std::vector<char> header_raw;
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
	bool readFile(const char* fileName);

	bool makeBmpFile(const char* fileName);
	bool writeFile(const char* fileName);

	PixelMatrix apllyFilter(const Operator& opr, const bool dim);

	PixelMatrix averageFilter(void);
	PixelMatrix prewittFilter(void);
	PixelMatrix sobelFilter(void);
	PixelMatrix laplacianFilter(void);
	PixelMatrix medianFilter(void);

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
	
	BMPHEADER header;
	///this->mat[y][x]��8-�ߖT�ɃI�y���[�^opr��K�p����
	Operator transpose(const Operator& opr);
	
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
	Pixel calcMedianOfNeighbor(const int x, const int y);
	
	///<summary>
	///*this�̃q�X�g�O�����𐶐���this->hist�Ɋi�[����
	///</summary>
	void makeHistgram(void);

	void parseHeader(const char* rawHeader);
	void parseBody(const char* rawBody);
	char* makeRawHeader(void);
	char* makeRawBody(void);
	
	///<summary>
	///data[i]����1byte���o����int8_t�^�̐����ɕϊ�����
	///</summary>
	inline int8_t mkint8_t(const char* data, int& i) {
		return((int8_t)data[i]);
	}

	///data[i]����2byte���o����int16_t�^�̐����ɕϊ�����
	inline int16_t mkint16_t(const char* data, int& i) {
		return(((int16_t)data[++i]) + (((int16_t)data[i]) << 8));
	}

	///data[i]����4byte���o����int32_t�^�̐����ɕϊ�����
	inline int32_t mkint32_t(const char* data, int& i) {
		return( ((int32_t)data[++i]) + (((int32_t)data[++i]) << 8) + (((int32_t)data[++i]) << 16) + (((int32_t)data[i]) << 24));
	}

};

	