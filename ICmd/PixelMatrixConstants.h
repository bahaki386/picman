#pragma once
#include<vector>
#include<cstdint>
using Pixel = uint8_t;

enum class Dim { X, Y, N };
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
