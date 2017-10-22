#pragma once
#include "stdafx.h"
#include "Mat2d.h"
using Operator = std::vector<Mat2d<double>>;
const Operator averageMatrix
{
	Mat2d<double>(3,3,std::valarray<double>{
		 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0 ,
		 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0 ,
		 1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0})
	
};
const Operator prewittMatrix{
	Mat2d<double>(3,3,std::valarray<double>
		{ 1.0, 0.0, -1.0 ,
		 1.0, 0.0, -1.0 ,
		 1.0, 0.0, -1.0 }
	),
	Mat2d<double>(3,3,std::valarray<double>
		{ 1.0, 1.0, 1 ,
		 0.0, 0.0, 0.0 ,
		 -1.0, -1.0, -1.0 }
		)
};
const Operator gaussianMatrix{
	
	Mat2d<double>(3,3,
		std::valarray<double>{
		1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0 ,
			2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0,
			1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0 
	}
	)
	
};
const Operator laplacianMatrix{
		Mat2d<double>(3,3,std::valarray<double>
	{ 1.0,  1.0,1.0,
	 1.0, -8.0,	1.0 ,
	 1.0,  1.0,	1.0 }
	)
};
const Operator sobelMatrix {
		Mat2d<double>(3,3,std::valarray<double>
		{ 1.0, 0.0, -1.0 ,
		 2.0, 0.0, -2.0 ,
		 1.0, 0.0, -1.0 }
	),
		Mat2d<double>(3,3,std::valarray<double>{
		1.0, 2.0, 1.0 ,
		 0.0, 0.0, 0.0 ,
		 -1.0, -2.0, -1.0 }
					  )
};

struct RGBQUAD{
	uint8_t	rgbBlue;
	uint8_t	rgbGreen;
	uint8_t	rgbRed;
	uint8_t	rgbReserved;

};
struct BITMAPFILEHEADER{
	uint16_t	bfType;
	uint32_t	bfSize;
	uint16_t	bfReserved1;
	uint16_t	bfReserved2;
	uint32_t	bfOffBits;
};
struct BITMAPINFOHEADER{
	uint32_t	bcSize;
	uint32_t	bcWidth;
	int32_t		bcHeight;
	uint16_t	bcPlanes;
	uint16_t	bcBitCount;
	uint32_t	biCompression;
	uint32_t	biSizeImage;
	uint32_t	biXPixPerMeter;
	uint32_t	biYPixPerMeter;
	uint32_t	biClrUsed;
	uint32_t	biCirImportamt;
};
struct BMPHEADER{
	BITMAPFILEHEADER File;
	BITMAPINFOHEADER Info;
	std::vector<RGBQUAD> Palette;
};
