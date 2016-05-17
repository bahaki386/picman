#include "PixelMatrix.h"

PixelMatrix::PixelMatrix(const char* filename) :PixelMatrix(){
	readBmpFile(filename);
}

bool PixelMatrix::readBmpFile(const char * fileName)
{
	std::ifstream bmpfile{};
	bmpfile.exceptions(std::ios_base::failbit | std::ios_base::badbit);
	bmpfile.open(fileName, std::ios_base::binary | std::ios_base::in);
	if (!bmpfile.is_open()) {
		return false;
	}
	bmpfile.seekg(0,std::ios::end);
	auto eofPos = bmpfile.tellg();
	bmpfile.clear();
	bmpfile.seekg(0, std::ios::beg);
	
	bmpfile.read(reinterpret_cast<char*>(&(header.File.bfType)),		sizeof(header.File.bfType));
	bmpfile.read(reinterpret_cast<char*>(&(header.File.bfSize)),		sizeof(header.File.bfSize));
	bmpfile.read(reinterpret_cast<char*>(&(header.File.bfReserved1)),	sizeof(header.File.bfReserved1));
	bmpfile.read(reinterpret_cast<char*>(&(header.File.bfReserved2)),	sizeof(header.File.bfReserved2));
	bmpfile.read(reinterpret_cast<char*>(&(header.File.bfOffBits)),		sizeof(header.File.bfOffBits));
	
	bmpfile.read(reinterpret_cast<char*>(&(header.Info.bcSize)),			sizeof(header.Info.bcSize));
	bmpfile.read(reinterpret_cast<char*>(&(header.Info.bcWidth)),			sizeof(header.Info.bcWidth));
	bmpfile.read(reinterpret_cast<char*>(&(header.Info.bcHeight)),			sizeof(header.Info.bcHeight));
	bmpfile.read(reinterpret_cast<char*>(&(header.Info.bcPlanes)),			sizeof(header.Info.bcPlanes));
	bmpfile.read(reinterpret_cast<char*>(&(header.Info.bcBitCount)),		sizeof(header.Info.bcBitCount));
	bmpfile.read(reinterpret_cast<char*>(&(header.Info.biCompression)),		sizeof(header.Info.biCompression));
	bmpfile.read(reinterpret_cast<char*>(&(header.Info.biSizeImage)),		sizeof(header.Info.biSizeImage));
	bmpfile.read(reinterpret_cast<char*>(&(header.Info.biXPixPerMeter)),	sizeof(header.Info.biXPixPerMeter));
	bmpfile.read(reinterpret_cast<char*>(&(header.Info.biYPixPerMeter)),	sizeof(header.Info.biYPixPerMeter));
	bmpfile.read(reinterpret_cast<char*>(&(header.Info.biClrUsed)),			sizeof(header.Info.biClrUsed));
	bmpfile.read(reinterpret_cast<char*>(&(header.Info.biCirImportamt)),	sizeof(header.Info.biCirImportamt));
	
	header.Palette.resize(header.Info.biClrUsed);
	if (header.Info.biClrUsed) {
		for (size_t i = 0; i < header.Info.biClrUsed; i++) {
			bmpfile.read(reinterpret_cast<char*>(&(header.Palette[i].rgbBlue)), sizeof(header.Palette[i].rgbBlue));
			bmpfile.read(reinterpret_cast<char*>(&(header.Palette[i].rgbGreen)), sizeof(header.Palette[i].rgbGreen));
			bmpfile.read(reinterpret_cast<char*>(&(header.Palette[i].rgbRed)), sizeof(header.Palette[i].rgbRed));
			bmpfile.read(reinterpret_cast<char*>(&(header.Palette[i].rgbReserved)), sizeof(header.Palette[i].rgbReserved));
		}
	}

	header_size = header.File.bfOffBits;
	header_raw.resize(header_size);
	bmpfile.seekg(0, std::ios_base::beg);
	bmpfile.read(&(header_raw[0]), header_size);
	
	height = header.Info.bcHeight;
	width= header.Info.bcWidth;
	if (height < 0) {
		height = -height;
		flagh = true;
	}
	mat.assign(height, std::vector<Pixel>(width));
	if ((width*header.Info.bcBitCount) % 4) {
		padding_size = 4 - ((width*header.Info.bcBitCount/8) % 4);
	}
	else{
		padding_size = 0;
	}
	Pixel tmp{};
	if (!flagh) {
		for (size_t i = (height - 1); i >= 0 && i < height; i--) {
			for (size_t j = 0; j < width; j++) {
				bmpfile.read(reinterpret_cast<char*>(&tmp), 1);
				mat[i][j] = header.Info.biClrUsed?header.Palette[tmp].rgbBlue:tmp;
				bmpfile.ignore((header.Info.bcBitCount/8) - 1);
			}
			bmpfile.ignore(padding_size);
		}
	}
	else {
		for (size_t i = 0; i < height; i++) {
			for (size_t j = 0; j < width; j++) {
				bmpfile.read(reinterpret_cast<char*>(&tmp), 1);
				mat[i][j] = header.Info.biClrUsed?header.Palette[tmp].rgbBlue:tmp;
				bmpfile.ignore(2);
			}
			bmpfile.ignore(padding_size);
		}
	}
	bmpfile.close();
	return true;
}

bool PixelMatrix::readFile(const char *) {
	return false;
}

bool PixelMatrix::makeBmpFile(const char * fileName)
{
	std::ofstream bmpfile{};
	bmpfile.exceptions(std::ofstream::failbit | std::ofstream::badbit);
	bmpfile.open(fileName, std::ios_base::binary | std::ios_base::out | std::ios_base::trunc);
	if (!bmpfile.is_open()) {
		return false;
	}
	Pixel tmp{};
	bmpfile.write(&(header_raw[0]), header_size);
	
	if (!flagh) {
		for (size_t i = (height - 1); i >= 0 && i < height; i--) {
			for (size_t j = 0; j < width; j++) {
				if (header.Info.biClrUsed) {
					for (int k = 0; k < header.Info.biClrUsed; k++) {
						(mat[i][j] == header.Palette[k].rgbBlue) ? tmp = k : tmp;
					}
				}
				tmp = mat[i][j];
				bmpfile.write(reinterpret_cast<char*>(&tmp), 1);				
			}
			bmpfile.write(0, padding_size);
		}
	}
	else {
		for (size_t i = 0; i < height; i++) {
			for (size_t j = 0; j < width; j++) {
				if (header.Info.biClrUsed) {
					for (int k = 0; k < header.Info.biClrUsed; k++) {
						(mat[i][j] == header.Palette[k].rgbBlue) ? tmp=k : tmp;
					}
				}
				tmp = mat[i][j];bmpfile.write(reinterpret_cast<char*>(&tmp), 1);
			}
			bmpfile.write(0, padding_size);
		}
	}
	bmpfile.close();
	return true;
}

bool PixelMatrix::writeFile(const char *) {
	return false;
}

PixelMatrix PixelMatrix::apllyFilter(const Operator & filter, const bool d) {
	PixelMatrix after = *this;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			if (d) {
				auto dx = applyOperatorOnPixel(i, j, filter, Dim::X);
				auto dy = applyOperatorOnPixel(i, j, filter, Dim::Y);
				auto tmp = static_cast<int>(sqrt(dx*dx + dy*dy));
				tmp = tmp > 255 ? 255 : tmp;
				after.mat[i][j] = static_cast<Pixel>(tmp);
			}
			else {
				auto tmp = abs(after.applyOperatorOnPixel(i, j, filter, Dim::N));
				tmp = tmp > 255 ? 255 : tmp;
				after.mat[i][j] = static_cast<Pixel>(tmp);
			}
		}
	}
	return after;
}


PixelMatrix PixelMatrix::averageFilter(void) {
	auto after = apllyFilter(averageMatrix,false);
	return after;
}

PixelMatrix PixelMatrix::prewittFilter(void) {
	auto after = apllyFilter(prewittMatrix,true);
	return after;
}

PixelMatrix PixelMatrix::sobelFilter(void) {
	auto after = apllyFilter(sobelMatrix, true);
	return after;
}

PixelMatrix PixelMatrix::laplacianFilter(void) {
	auto after = apllyFilter(laplacianMatrix, false);
	return after;
}


PixelMatrix PixelMatrix::medianFilter(void) {
	PixelMatrix after = *this;
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			after.mat[i][j] = calcMedianOfNeighbor(i, j);
		}
	}
	return after;
}

Operator PixelMatrix::transpose(const Operator& filter) {
	Operator f = filter;
	f[0][0] = filter[0][0];
	f[0][1] = filter[1][0];
	f[0][2] = filter[2][0];
	f[1][0] = filter[0][1];
	f[1][1] = filter[1][1];
	f[1][2] = filter[2][1];
	f[2][0] = filter[0][2];
	f[2][1] = filter[1][2];
	f[2][2] = filter[2][2];
	return f;
}

int PixelMatrix::applyOperatorOnPixel(const int i, const int j, const Operator& filter, Dim xy) {
	float ans=0;
	auto f = filter;
	if (xy == Dim::Y) {
		f = transpose(filter);
	}
	if (i == 0) {
		if (j == 0) {
			ans =	mat[i][j]		* f[1][1]
				+	mat[i][j+1]		* f[1][2] 
				+	mat[i+1][j]		* f[2][1] 
				+	mat[i+1][j+1]	* f[2][2];
				
		}
		else if (j == width - 1) {
			ans = mat[i][j]		* f[1][1]
				+ mat[i][j-1]	* f[1][0]
				+ mat[i+1][j]	* f[2][1]
				+ mat[i+1][j-1]	* f[2][0];
		}
		else {
			ans = mat[i][j]		* f[1][1]
				+ mat[i][j+1]	* f[1][2]
				+ mat[i][j-1]	* f[1][0]
				+ mat[i+1][j]	* f[2][1]
				+ mat[i+1][j+1]	* f[2][2]
				+ mat[i+1][j-1]	* f[2][0];
		}
	}
	else if (i == height - 1) {
		if (j == 0) {
			ans = mat[i][j]		* f[1][1]
				+ mat[i][j+1]	* f[1][2]
				+ mat[i-1][j]	* f[0][1]
				+ mat[i-1][j+1]	* f[0][2];
		}
		else if (j == width - 1) {
			ans = mat[i][j]		* f[1][1]
				+ mat[i][j-1]	* f[1][0]
				+ mat[i-1][j]	* f[0][1]
				+ mat[i-1][j-1]	* f[0][0];

		}
		else {
			ans = mat[i][j]		* f[1][1]
				+ mat[i][j+1]	* f[1][2]
				+ mat[i][j-1]	* f[1][0]
				+ mat[i-1][j]	* f[0][1]
				+ mat[i-1][j+1]	* f[0][2]
				+ mat[i-1][j-1]	* f[0][0];

		}
	}
	else {
		if (j == 0) {
			ans = mat[i][j]		* f[1][1]
				+ mat[i][j+1]	* f[1][2]
				+ mat[i+1][j]	* f[2][1]
				+ mat[i+1][j+1]	* f[2][2]
				+ mat[i-1][j]	* f[0][1]
				+ mat[i-1][j+1]	* f[0][2];
		}
		else if (j == width - 1) {
			ans = mat[i][j]		* f[1][1]
				+ mat[i][j-1]	* f[1][0]
				+ mat[i+1][j]	* f[2][1]
				+ mat[i+1][j-1]	* f[2][0]
				+ mat[i-1][j]	* f[0][1]
				+ mat[i-1][j-1]	* f[0][0];
		}
		else {
			ans = mat[i][j]		* f[1][1]
				+ mat[i][j+1]	* f[1][2]
				+ mat[i][j-1]	* f[1][0]
				+ mat[i+1][j]	* f[2][1]
				+ mat[i+1][j+1]	* f[2][2]
				+ mat[i+1][j-1]	* f[2][0]
				+ mat[i-1][j]	* f[0][1]
				+ mat[i-1][j+1]	* f[0][2]
				+ mat[i-1][j-1]	* f[0][0];
		}
	}
	return ( static_cast<int>(ans) );
}

Pixel PixelMatrix::calcMedianOfNeighbor(const int i, const int j) {
	std::vector<Pixel> v{};
	if (i == 0) {
		if (j == 0) {
			v.push_back(mat[i][j]);
			v.push_back(mat[i][j + 1]);
			v.push_back(mat[i + 1][j]);
			v.push_back(mat[i + 1][j + 1]);

		}
		else if (j == width - 1) {
			v.push_back(mat[i][j - 1]);
			v.push_back(mat[i][j]);
			v.push_back(mat[i + 1][j - 1]);
			v.push_back(mat[i + 1][j]);
		}
		else {
			v.push_back(mat[i][j - 1]);
			v.push_back(mat[i][j]);
			v.push_back(mat[i][j + 1]);
			v.push_back(mat[i + 1][j - 1]);
			v.push_back(mat[i + 1][j]);
			v.push_back(mat[i + 1][j + 1]);
		}
	}
	else if (i == height - 1) {
		if (j == 0) {
			v.push_back(mat[i - 1][j]);
			v.push_back(mat[i - 1][j + 1]);
			v.push_back(mat[i][j]);
			v.push_back(mat[i][j + 1]);
			}
		else if (j == width - 1) {
			v.push_back(mat[i - 1][j - 1]);
			v.push_back(mat[i - 1][j]);
			v.push_back(mat[i][j - 1]);
			v.push_back(mat[i][j]);
		}
		else {
			v.push_back(mat[i - 1][j - 1]);
			v.push_back(mat[i - 1][j]);
			v.push_back(mat[i - 1][j + 1]);
			v.push_back(mat[i][j - 1]);
			v.push_back(mat[i][j]);
			v.push_back(mat[i][j + 1]);
		}
	}
	else {
		if (j == 0) {
			v.push_back(mat[i - 1][j]);
			v.push_back(mat[i - 1][j + 1]);
			v.push_back(mat[i][j]);
			v.push_back(mat[i][j + 1]);
			v.push_back(mat[i + 1][j]);
			v.push_back(mat[i + 1][j + 1]);

		}
		else if (j == width - 1) {
			v.push_back(mat[i - 1][j - 1]);
			v.push_back(mat[i - 1][j]);
			v.push_back(mat[i][j - 1]);
			v.push_back(mat[i][j]);
			v.push_back(mat[i + 1][j - 1]);
			v.push_back(mat[i + 1][j]);
			

		}
		else {
			v.push_back(mat[i - 1][j - 1]);
			v.push_back(mat[i - 1][j]);
			v.push_back(mat[i - 1][j + 1]);
			v.push_back(mat[i][j - 1]);
			v.push_back(mat[i][j]);
			v.push_back(mat[i][j + 1]);
			v.push_back(mat[i + 1][j - 1]);
			v.push_back(mat[i + 1][j]);
			v.push_back(mat[i + 1][j + 1]);
		}
	}
	size_t tmp = round((double)v.size()/2);
	std::nth_element(v.begin(),v.begin()+tmp,v.end());
	Pixel ans = v[tmp];

	return ans;
}



void PixelMatrix::makeHistgram(void) {
	size_t grade{};
	hist.assign((header.Info.biClrUsed?header.Info.biClrUsed:256), 0);
	for (int i = 0; i < height; i++) {
		for (int j = 0; j < width; j++) {
			grade = mat[i][j];
			hist[grade] = hist[grade]+1;
		}
	}
}

void PixelMatrix::parseHeader(const char* raw_header) {
	
}

void PixelMatrix::parseBody(const char* filename) {

}

char * PixelMatrix::makeRawHeader() {
	return nullptr;
}

char * PixelMatrix::makeRawBody(void) {
	return nullptr;
}

std::string PixelMatrix::showHistgram(void) {
	makeHistgram();
	std::stringstream ss{};
	std::string s{};
	int i = 0;
	for(auto& n : hist) {
		ss << i << " : " << n << "  :"<< s.assign((n*100*256)/(width*height),'*') << '\n';
		i++;
	}
	return ss.str();
}

