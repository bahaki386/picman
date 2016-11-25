#pragma once
#include<fstream>
#include<sstream>
#include<algorithm>
#include<numeric>
#include"PixelMatrixConstants.h"



///<summary>
///画像データのクラス
///デフォルトコンストラクタで構築
///コピー・ムーブに関して通常のセマンティクスを持つ
///</summary>
class PixelMatrix
{
	std::vector<std::vector<Pixel>> mat;//画像本体の輝度行列
	std::vector<size_t> hist;//ヒストグラム
	std::vector<char> header_raw;//ヘッダ部バイト列
	BMPHEADER header;//画像ヘッダ部
	int
		width,//画像の幅
		height;//画像高さ
	size_t
		header_size,//ヘッダ部サイズ
		padding_size;//パディングサイズ
	bool
		flagh;//高さ方向判別用フラグ

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
	std::vector<std::vector<Pixel>>  detThreshold(const int );
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
	
	
	
	
	/// <summary>
	/// 注目画素に8-近傍オペレータを適用する
	/// </summary>
	/// <param name="x">注目画素のX座標</param>
	/// <param name="y">注目画素のY座標</param>
	/// <param name="opr">適用するオペレータ</param>
	/// <param name="d">方向</param>
	/// <returns>
	/// オペレータ適用後の注目画素の値
	/// </returns>
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
	Pixel calcMedianOfNeighbor(const int x, const int y,const int size);
	
	///<summary>
	///*thisのヒストグラムを生成しthis->histに格納する
	///</summary>
	void makeHistgram(void);

	
};
