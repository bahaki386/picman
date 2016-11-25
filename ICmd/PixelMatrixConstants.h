#pragma once
#include<vector>
#include<cstdint>
#include<fstream>
#include<sstream>
#include<algorithm>
typedef uint8_t Pixel;

enum class Dim { X, Y, N };

using Operator = std::vector<std::vector<std::vector<double>>>;
static const Operator
averageMatrix = {
	{
		{ 1.0/9.0, 1.0/9.0, 1.0/9.0 },
		{ 1.0/9.0, 1.0/9.0, 1.0/9.0 },
		{ 1.0/9.0, 1.0/9.0, 1.0/9.0 }
	}
},
prewittMatrix = {
	{
		{ 1, 0, -1 },
		{ 1, 0, -1 },
		{ 1, 0, -1 }
	},
	{
		{ 1, 1, 1 },
		{ 0, 0, 0 },
		{ -1, -1, -1 }
	}
},
gaussianMatrix = {
	{
		{ 1.0/16.0, 2.0/16.0, 1.0/16.0 },
		{ 2.0/16.0, 4.0/16.0, 2.0/16.0 },
		{ 1.0/16.0, 2.0/16.0, 1.0/16.0 }
	}
},
laplacianMatrix = { {
	{ 1,  1,	1 },
	{ 1, -8,	1 },
	{ 1,  1,	1 } } },
	sobelMatrix = {
		{
			{ 1, 0, -1 },
			{ 2, 0, -2 },
			{ 1, 0, -1 }
		},
		{
			{ 1, 2, 1 },
			{ 0, 0, 0 },
			{ -1, -2, -1 }
		}
};

struct RGBQUAD
{
	uint8_t	rgbBlue;
	uint8_t	rgbGreen;
	uint8_t	rgbRed;
	uint8_t	rgbReserved;

};
struct BITMAPFILEHEADER
{
	uint16_t	bfType;
	uint32_t	bfSize;
	uint16_t	bfReserved1;
	uint16_t	bfReserved2;
	uint32_t	bfOffBits;
};
struct BITMAPINFOHEADER
{
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
struct BMPHEADER
{
	BITMAPFILEHEADER File;
	BITMAPINFOHEADER Info;
	std::vector<RGBQUAD> Palette;
};
