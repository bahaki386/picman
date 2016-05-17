#include<vector>
#include<cstdint>
#include<fstream>
#include<sstream>
#include<algorithm>

typedef std::vector<std::vector<float>> Operator;
typedef uint8_t Pixel;

class PixelMatrix
{
	public:

	PixelMatrix(const char*);

	PixelMatrix() = default;
	PixelMatrix(const PixelMatrix&) = default;
	PixelMatrix(PixelMatrix&&) = default;
	PixelMatrix& operator=(const PixelMatrix&) = default;
	PixelMatrix& operator=(PixelMatrix&&) = default;

	bool readBmpFile(const char*);
	bool readFile(const char*);

	bool makeBmpFile(const char*);
	bool writeFile(const char*);

	PixelMatrix apllyFilter(const Operator&, const bool);

	PixelMatrix averageFilter(void);
	PixelMatrix prewittFilter(void);
	PixelMatrix sobelFilter(void);
	PixelMatrix laplacianFilter(void);
	PixelMatrix medianFilter(void);

	inline const int getWidth() {
		return width;
	}

	inline const int getHeight() {
		return height;
	}

	std::string showHistgram(void);

	private:
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
	static constexpr int
		posOffsetSize = 0xa,
		posWidth = 0x12,
		posHeight = 0x16;
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
	} header;
	const Operator
		averageMatrix = {
			{1.0f / 10.0f, 1.0f / 10.0f, 1.0f / 10.0f},
			{1.0f / 10.0f, 2.0f / 10.0f, 1.0f / 10.0f},
			{1.0f / 10.0f, 1.0f / 10.0f, 1.0f / 10.0f} },
		prewittMatrix = {
			{1, 0, -1},
			{1, 0, -1},
			{1, 0, -1} },
		laplacianMatrix = {
			{0,  1,	0},
			{1, -4,	1},
			{0,  1,	0} },
		sobelMatrix = {
			{1, 0, -1},
			{2, 0, -2},
			{1, 0, -1} };

	Operator transpose(const Operator&);
	int applyOperatorOnPixel(const int, const int, const Operator&, Dim);
	Pixel calcMedianOfNeighbor(const int, const int);
	void makeHistgram(void);

	void parseHeader(const char*);
	void parseBody(const char*);
	char* makeRawHeader(void);
	char* makeRawBody(void);

	inline int8_t mkint8_t(const char* data, int& i) {
		return((int8_t)data[i]);
	}
	inline int16_t mkint16_t(const char* data, int& i) {
		return((((int16_t)data[i += 1]) << 4) + ((int16_t)data[i]));
	}
	inline int32_t mkint32_t(const char* data, int& i) {
		return((((int32_t)data[i += 1]) << 8) + (((int32_t)data[i += 1]) << 4) + ((int32_t)data[i]));
	}

};