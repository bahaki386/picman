#pragma once
#include<fstream>
#include<sstream>
#include<algorithm>
#include<numeric>
#include"PixelMatrixConstants.h"



///<summary>
///�摜�f�[�^�̃N���X
///�f�t�H���g�R���X�g���N�^�ō\�z
///�R�s�[�E���[�u�Ɋւ��Ēʏ�̃Z�}���e�B�N�X������
///</summary>
class PixelMatrix
{
	std::vector<std::vector<Pixel>> mat;//�摜�{�̂̋P�x�s��
	std::vector<size_t> hist;//�q�X�g�O����
	std::vector<char> header_raw;//�w�b�_���o�C�g��
	BMPHEADER header;//�摜�w�b�_��
	int
		width,//�摜�̕�
		height;//�摜����
	size_t
		header_size,//�w�b�_���T�C�Y
		padding_size;//�p�f�B���O�T�C�Y
	bool
		flagh;//�����������ʗp�t���O

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
	/// ���ډ�f��8-�ߖT�I�y���[�^��K�p����
	/// </summary>
	/// <param name="x">���ډ�f��X���W</param>
	/// <param name="y">���ډ�f��Y���W</param>
	/// <param name="opr">�K�p����I�y���[�^</param>
	/// <param name="d">����</param>
	/// <returns>
	/// �I�y���[�^�K�p��̒��ډ�f�̒l
	/// </returns>
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
