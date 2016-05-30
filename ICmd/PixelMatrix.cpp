
#include "PixelMatrix.h"

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
				tmp = mat[i][j];
				if (header.Info.biClrUsed) {
					#pragma omp parallel for private(tmp)
					for (int k = 0; k < (int)header.Info.biClrUsed; k++) {
						(mat[i][j] == header.Palette[k].rgbBlue) ? tmp = (uint8_t)k : tmp;
					}
				}
				uint8_t bits = 0;
				while (bits != header.Info.bcBitCount) {
					bmpfile.write(reinterpret_cast<char*>(&tmp), 1);
					bits += 8;
				}
			}
			bmpfile.write(0, padding_size);
		}
	}
	else {
		for (size_t i = 0; i < height; i++) {
			for (size_t j = 0; j < width; j++) {
				tmp = mat[i][j];
				if (header.Info.biClrUsed) {
					#pragma omp parallel for private(tmp)
					for (int k = 0; k < (int)header.Info.biClrUsed; k++) {
						(mat[i][j] == header.Palette[k].rgbBlue) ? tmp = (uint8_t)k : tmp;
					}
				}
				uint8_t bits = 0;
				while (bits != header.Info.bcBitCount) {
					bmpfile.write(reinterpret_cast<char*>(&tmp), 1);
					bits += 8;
				}
			}
			bmpfile.write(0, padding_size);
		}
	}
	bmpfile.close();
	return true;
}

PixelMatrix PixelMatrix::apllyFilter(const Operator & filter, const bool d) {
	PixelMatrix tmp = *this;
#pragma omp parallel for
	for (int y= 0; y < height; y++) {
		#pragma omp parallel for
		for (int x = 0; x < width; x++) {
			if (d) {
				auto dx = applyOperatorOnPixel(x, y, filter, Dim::X);
				auto dy = applyOperatorOnPixel(x, y, filter, Dim::Y);
				auto temp = static_cast<int>(sqrt(dx*dx + dy*dy));
				temp = temp > 255 ? 255 : temp;
				tmp.mat[y][x] = static_cast<Pixel>(temp);
			}
			else {
				auto temp = abs(applyOperatorOnPixel(x, y, filter, Dim::N));
				temp = temp > 255 ? 255 : temp;
				tmp.mat[y][x] = static_cast<Pixel>(temp);
			}
		}
	}
	return tmp;
}


PixelMatrix PixelMatrix::averageFilter(void) {
	return this->apllyFilter(averageMatrix,false);
}

PixelMatrix PixelMatrix::prewittFilter(void) {
	return this->apllyFilter(prewittMatrix,true);
}

PixelMatrix PixelMatrix::sobelFilter(void) {
	return this->apllyFilter(sobelMatrix, true);
	
}

PixelMatrix PixelMatrix::laplacianFilter(void) {
	return this->apllyFilter(laplacianMatrix, false);
}
PixelMatrix PixelMatrix::gaussianFilter(void) {
	return this->apllyFilter(gaussianMatrix, false);
}


PixelMatrix PixelMatrix::medianFilter(void) {
	PixelMatrix tmp=*this;
#pragma omp parallel for
	for (int y = 0; y < height; y++) {
		#pragma omp parallel for
		for (int x = 0; x < width; x++) {
			tmp.mat[y][x] = calcMedianOfNeighbor(x, y);
		}
	}
	return tmp;
}

Operator PixelMatrix::transpose(const Operator& filter) {
	Operator f = filter;
#pragma omp parallel for
	for (int i = 0; i < 3;i++) {
#pragma omp parallel for
		for (int j = 0; j < 3;j++) {
			f[j][i] = filter[i][j];
		}
	}
	return f;
}

int PixelMatrix::applyOperatorOnPixel(const int x, const int y, const Operator& filter, Dim xy) {
	float ans=0;
	auto f = filter;
	if (xy == Dim::Y) {
		f = transpose(filter);
	}
	if (y == 0) {
		if (x == 0) {
			ans =	mat[y][x]		* f[1][1]
				+	mat[y][x+1]		* f[1][2] 
				+	mat[y+1][x]		* f[2][1] 
				+	mat[y+1][x+1]	* f[2][2];
				
		}
		else if (x == width - 1) {
			ans = mat[y][x]		* f[1][1]
				+ mat[y][x-1]	* f[1][0]
				+ mat[y+1][x]	* f[2][1]
				+ mat[y+1][x-1]	* f[2][0];
		}
		else {
			ans = mat[y][x]		* f[1][1]
				+ mat[y][x+1]	* f[1][2]
				+ mat[y][x-1]	* f[1][0]
				+ mat[y+1][x]	* f[2][1]
				+ mat[y+1][x+1]	* f[2][2]
				+ mat[y+1][x-1]	* f[2][0];
		}
	}
	else if (y == height - 1) {
		if (x == 0) {
			ans = mat[y][x]		* f[1][1]
				+ mat[y][x+1]	* f[1][2]
				+ mat[y-1][x]	* f[0][1]
				+ mat[y-1][x+1]	* f[0][2];
		}
		else if (x == width - 1) {
			ans = mat[y][x]		* f[1][1]
				+ mat[y][x-1]	* f[1][0]
				+ mat[y-1][x]	* f[0][1]
				+ mat[y-1][x-1]	* f[0][0];

		}
		else {
			ans = mat[y][x]		* f[1][1]
				+ mat[y][x+1]	* f[1][2]
				+ mat[y][x-1]	* f[1][0]
				+ mat[y-1][x]	* f[0][1]
				+ mat[y-1][x+1]	* f[0][2]
				+ mat[y-1][x-1]	* f[0][0];

		}
	}
	else {
		if (x == 0) {
			ans = mat[y][x]		* f[1][1]
				+ mat[y][x+1]	* f[1][2]
				+ mat[y+1][x]	* f[2][1]
				+ mat[y+1][x+1]	* f[2][2]
				+ mat[y-1][x]	* f[0][1]
				+ mat[y-1][x+1]	* f[0][2];
		}
		else if (x == width - 1) {
			ans = mat[y][x]		* f[1][1]
				+ mat[y][x-1]	* f[1][0]
				+ mat[y+1][x]	* f[2][1]
				+ mat[y+1][x-1]	* f[2][0]
				+ mat[y-1][x]	* f[0][1]
				+ mat[y-1][x-1]	* f[0][0];
		}
		else {
			ans = mat[y][x]		* f[1][1]
				+ mat[y][x+1]	* f[1][2]
				+ mat[y][x-1]	* f[1][0]
				+ mat[y+1][x]	* f[2][1]
				+ mat[y+1][x+1]	* f[2][2]
				+ mat[y+1][x-1]	* f[2][0]
				+ mat[y-1][x]	* f[0][1]
				+ mat[y-1][x+1]	* f[0][2]
				+ mat[y-1][x-1]	* f[0][0];
		}
	}
	return ( static_cast<int>(ans) );
}

Pixel PixelMatrix::calcMedianOfNeighbor(const int x, const int y) {
	std::vector<Pixel> v{};
	if (y == 0) {
		if (x == 0) {
			v.push_back(mat[y][x]);
			v.push_back(mat[y][x + 1]);
			v.push_back(mat[y + 1][x]);
			v.push_back(mat[y + 1][x + 1]);

		}
		else if (x == width - 1) {
			v.push_back(mat[y][x - 1]);
			v.push_back(mat[y][x]);
			v.push_back(mat[y + 1][x - 1]);
			v.push_back(mat[y + 1][x]);
		}
		else {
			v.push_back(mat[y][x - 1]);
			v.push_back(mat[y][x]);
			v.push_back(mat[y][x + 1]);
			v.push_back(mat[y + 1][x - 1]);
			v.push_back(mat[y + 1][x]);
			v.push_back(mat[y + 1][x + 1]);
		}
	}
	else if (y == height - 1) {
		if (x == 0) {
			v.push_back(mat[y - 1][x]);
			v.push_back(mat[y - 1][x + 1]);
			v.push_back(mat[y][x]);
			v.push_back(mat[y][x + 1]);
			}
		else if (x == width - 1) {
			v.push_back(mat[y - 1][x - 1]);
			v.push_back(mat[y - 1][x]);
			v.push_back(mat[y][x - 1]);
			v.push_back(mat[y][x]);
		}
		else {
			v.push_back(mat[y - 1][x - 1]);
			v.push_back(mat[y - 1][x]);
			v.push_back(mat[y - 1][x + 1]);
			v.push_back(mat[y][x - 1]);
			v.push_back(mat[y][x]);
			v.push_back(mat[y][x + 1]);
		}
	}
	else {
		if (x == 0) {
			v.push_back(mat[y - 1][x]);
			v.push_back(mat[y - 1][x + 1]);
			v.push_back(mat[y][x]);
			v.push_back(mat[y][x + 1]);
			v.push_back(mat[y + 1][x]);
			v.push_back(mat[y + 1][x + 1]);

		}
		else if (x == width - 1) {
			v.push_back(mat[y - 1][x - 1]);
			v.push_back(mat[y - 1][x]);
			v.push_back(mat[y][x - 1]);
			v.push_back(mat[y][x]);
			v.push_back(mat[y + 1][x - 1]);
			v.push_back(mat[y + 1][x]);
			

		}
		else {
			v.push_back(mat[y - 1][x - 1]);
			v.push_back(mat[y - 1][x]);
			v.push_back(mat[y - 1][x + 1]);
			v.push_back(mat[y][x - 1]);
			v.push_back(mat[y][x]);
			v.push_back(mat[y][x + 1]);
			v.push_back(mat[y + 1][x - 1]);
			v.push_back(mat[y + 1][x]);
			v.push_back(mat[y + 1][x + 1]);
		}
	}
	size_t tmp = (size_t)round((double)v.size()/2);
	std::nth_element(v.begin(),v.begin()+tmp,v.end());
	Pixel ans = v[tmp];

	return ans;
}



void PixelMatrix::makeHistgram(void) {
	hist.assign((header.Info.biClrUsed ? header.Info.biClrUsed : 256), 0);
	#pragma omp parallel for
	for (int i = 0; i < height;i++) {
		#pragma omp parallel for
		for (int x = 0; x < width;x++) {
			hist[mat[i][x]]++;
		}
	}
}



std::string PixelMatrix::showHistgram(void) {
	makeHistgram();
	std::stringstream ss{};
	std::string s{};
	size_t sum=0;
	int i = 0;
	for(auto& n : hist) {
		ss << i <<':'<<  n <<':'<< s.assign(n*100*128/(width*height),'*') << '\n';
		i++;
		sum += n;
	}
	ss << sum <<'\n'<< width*height<<'\n';
	return ss.str();
}

