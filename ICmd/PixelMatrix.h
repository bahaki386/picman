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
	///this->mat[y][x]の8-近傍にオペレータoprを適用する
	Operator transpose(const Operator& opr);
	
	///<summary>
	///注目画素に8-近傍オペレータを適用する
	///</summary>
	///<param name="x">
	///注目画素のX座標
	///</param>
	///<param name="y">
	///注目画素のY座標
	///</param>
	///<param name="opr">
	///適用するオペレータ
	///</param>
	///<param name="d">
	///方向
	///</param>
	///<returns>
	///オペレータ適用後の注目画素の値
	///</returns>
	int applyOperatorOnPixel(const int x, const int y, const Operator& opr, Dim d);
	
	///<summary>
	///8-近傍の中央値を得る
	///</summary>
	///<param name="x">
	///注目画素のX座標
	///</param>
	///<param name="y">
	///注目画素のY座標
	///</param>
	Pixel calcMedianOfNeighbor(const int x, const int y);
	
	///<summary>
	///*thisのヒストグラムを生成しthis->histに格納する
	///</summary>
	void makeHistgram(void);

	void parseHeader(const char* rawHeader);
	void parseBody(const char* rawBody);
	char* makeRawHeader(void);
	char* makeRawBody(void);
	
	///<summary>
	///data[i]から1byte取り出してint8_t型の整数に変換する
	///</summary>
	inline int8_t mkint8_t(const char* data, int& i) {
		return((int8_t)data[i]);
	}

	///data[i]から2byte取り出してint16_t型の整数に変換する
	inline int16_t mkint16_t(const char* data, int& i) {
		return(((int16_t)data[++i]) + (((int16_t)data[i]) << 8));
	}

	///data[i]から4byte取り出してint32_t型の整数に変換する
	inline int32_t mkint32_t(const char* data, int& i) {
		return( ((int32_t)data[++i]) + (((int32_t)data[++i]) << 8) + (((int32_t)data[++i]) << 16) + (((int32_t)data[i]) << 24));
	}

};

	