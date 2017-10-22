#include "stdafx.h"
#include "PixelMatrix.h"

bool PixelMatrix::readBmpFile(const char * fileName) {
	using std::ifstream;
	using std::ios_base;
	using std::ios;
	ifstream bmpfile{};
	bmpfile.exceptions(ios_base::failbit | ios_base::badbit);
	bmpfile.open(fileName, ios_base::binary | ios_base::in);
	if (!bmpfile.is_open()) {
		return false;
	}
	bmpfile.seekg(0, ios::end);
	auto eofPos = bmpfile.tellg();
	bmpfile.clear();
	bmpfile.seekg(0, ios::beg);

	bmpfile.read(reinterpret_cast<char*>(&(header.File.bfType)), sizeof(header.File.bfType));
	bmpfile.read(reinterpret_cast<char*>(&(header.File.bfSize)), sizeof(header.File.bfSize));
	bmpfile.read(reinterpret_cast<char*>(&(header.File.bfReserved1)), sizeof(header.File.bfReserved1));
	bmpfile.read(reinterpret_cast<char*>(&(header.File.bfReserved2)), sizeof(header.File.bfReserved2));
	bmpfile.read(reinterpret_cast<char*>(&(header.File.bfOffBits)), sizeof(header.File.bfOffBits));

	bmpfile.read(reinterpret_cast<char*>(&(header.Info.bcSize)), sizeof(header.Info.bcSize));
	bmpfile.read(reinterpret_cast<char*>(&(header.Info.bcWidth)), sizeof(header.Info.bcWidth));
	bmpfile.read(reinterpret_cast<char*>(&(header.Info.bcHeight)), sizeof(header.Info.bcHeight));
	bmpfile.read(reinterpret_cast<char*>(&(header.Info.bcPlanes)), sizeof(header.Info.bcPlanes));
	bmpfile.read(reinterpret_cast<char*>(&(header.Info.bcBitCount)), sizeof(header.Info.bcBitCount));
	bmpfile.read(reinterpret_cast<char*>(&(header.Info.biCompression)), sizeof(header.Info.biCompression));
	bmpfile.read(reinterpret_cast<char*>(&(header.Info.biSizeImage)), sizeof(header.Info.biSizeImage));
	bmpfile.read(reinterpret_cast<char*>(&(header.Info.biXPixPerMeter)), sizeof(header.Info.biXPixPerMeter));
	bmpfile.read(reinterpret_cast<char*>(&(header.Info.biYPixPerMeter)), sizeof(header.Info.biYPixPerMeter));
	bmpfile.read(reinterpret_cast<char*>(&(header.Info.biClrUsed)), sizeof(header.Info.biClrUsed));
	bmpfile.read(reinterpret_cast<char*>(&(header.Info.biCirImportamt)), sizeof(header.Info.biCirImportamt));

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
	bmpfile.seekg(0, ios_base::beg);
	bmpfile.read(&(header_raw[0]), header_size);

	height = header.Info.bcHeight;
	width = header.Info.bcWidth;
	if (height < 0) {
		height = -height;
		flagh = true;
	}
	mat.assign(width,height);
	if ((width*header.Info.bcBitCount) % 4) {
		padding_size = 4 - ((width*header.Info.bcBitCount / 8) % 4);
	}
	else {
		padding_size = 0;
	}
	Pixel tmp{};
	if (!flagh) {
		for (size_t i = (height - 1); i >= 0 && i < height; i--) {
			for (size_t j = 0; j < width; j++) {
				bmpfile.read(reinterpret_cast<char*>(&tmp), 1);
				mat.at(j,i) = header.Info.biClrUsed ? header.Palette[tmp].rgbBlue : tmp;
				bmpfile.ignore((header.Info.bcBitCount / 8) - 1);
			}
			bmpfile.ignore(padding_size);
		}
	}
	else {
		for (size_t i = 0; i < height; i++) {
			for (size_t j = 0; j < width; j++) {
				bmpfile.read(reinterpret_cast<char*>(&tmp), 1);
				mat.at(j,i) = header.Info.biClrUsed ? header.Palette[tmp].rgbBlue : tmp;
				bmpfile.ignore(2);
			}
			bmpfile.ignore(padding_size);
		}
	}
	bmpfile.close();
	return true;
}

bool PixelMatrix::makeBmpFile(const char * fileName) {
	using std::ofstream;
	using std::ios_base;
	using std::ios;
	ofstream bmpfile{};
	bmpfile.exceptions(ofstream::failbit | ofstream::badbit);
	bmpfile.open(fileName, ios_base::binary | ios_base::out | ios_base::trunc);
	if (!bmpfile.is_open()) {
		return false;
	}
	Pixel tmp{};
	bmpfile.write(&(header_raw[0]), header_size);

	if (!flagh) {
		for (size_t i = (height - 1); i >= 0 && i < height; i--) {
			for (size_t j = 0; j < width; j++) {
				tmp = mat.at(j,i);
				if (header.Info.biClrUsed) {
					for (int k = 0; k < (int)header.Info.biClrUsed; k++) {
						(mat.at(j,i) == header.Palette[k].rgbBlue) ? tmp = (uint8_t)k : tmp;
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
				tmp = mat.at(j,i);
				if (header.Info.biClrUsed) {
					for (int k = 0; k < (int)header.Info.biClrUsed; k++) {
						(mat.at(j,i) == header.Palette[k].rgbBlue) ? tmp = (uint8_t)k : tmp;
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

/**
*	TODO:以降に定義する関数はOpenMPによる並列化の対象となりうる
*/

PixelMatrix PixelMatrix::apllyFilter(const Operator & filter) {
	PixelMatrix tmp = *this;
#pragma omp parallel for
	for (int y = 0; y < height; y++) {
#pragma omp parallel for
		for (int x = 0; x < width; x++) {
			if (filter.size() == 2) {
				auto dx = applyOperatorOnPixel(x, y, filter, Dim::X);
				auto dy = applyOperatorOnPixel(x, y, filter, Dim::Y);
				auto temp = abs(dx) + abs(dy);
				temp = temp > 255 ? 255 : temp;
				tmp.mat.at(x,y) = static_cast<Pixel>(temp);
			}
			else {
				auto temp = abs(applyOperatorOnPixel(x, y, filter, Dim::N));
				temp = temp > 255 ? 255 : temp;
				tmp.mat.at(x,y) = static_cast<Pixel>(temp);
			}
		}
	}
	return tmp;
}


PixelMatrix PixelMatrix::averageFilter(void) {
	return this->apllyFilter(averageMatrix);
}

PixelMatrix PixelMatrix::prewittFilter(void) {
	return this->apllyFilter(prewittMatrix);
}

PixelMatrix PixelMatrix::sobelFilter(void) {
	return this->apllyFilter(sobelMatrix);

}

PixelMatrix PixelMatrix::laplacianFilter(void) {
	return this->apllyFilter(laplacianMatrix);
}
PixelMatrix PixelMatrix::gaussianFilter(void) {
	return this->apllyFilter(gaussianMatrix);
}

PixelMatrix PixelMatrix::binalize(Mat2d<Pixel> threshold) {
	auto tmp = *this;
#pragma omp parallel for
	for (int i = 0; i < height; i++) {
#pragma omp parallel for
		for (int j = 0; j < width; j++) {
			tmp.mat.at(j,i) = ((this->mat.at(j,i) > threshold.at(j,i)) ? 255 : 0);
		}
	}
	return tmp;
}

PixelMatrix PixelMatrix::binalize(Pixel threshold) {
	auto tmp = *this;
#pragma omp parallel for
	for (int i = 0; i < height; i++) {
#pragma omp parallel for
		for (int j = 0; j < width; j++) {
			tmp.mat.at(j,i) = ((this->mat.at(j,i) > threshold) ? 255 : 0);
		}
	}
	return tmp;
}

Mat2d<Pixel> PixelMatrix::detThreshold(const int size) {
	auto out{ mat };
	auto avg = averageFilter().mat;
	Pixel th_p = 0;
#pragma omp parallel for
	for (int y = 0; y < height; y++) {
#pragma omp parallel for
		for (int x = 0; x < width; x++) {
			int ans{};
			Pixel f = avg.at(x,y);
#pragma omp parallel for
			for (int i = -size / 2; i <= size / 2; i++) {
#pragma omp parallel for
				for (int j = -size / 2; j <= size / 2; j++) {
					auto cx = x + j, cy = y + i;
					if (cx >= 0 && cx < width && cy >= 0 && cy < height) {
						auto temp= ((mat.at(cx, cy) - f)*(mat.at(cx, cy) - f));
#pragma omp atomic
						ans += temp;
					}
				}
			}
			if (ans < (400 * size * size)) {
				out.at(x,y) = th_p;
			}
			else {
				out.at(x,y) = f;
			}
			th_p = out.at(x,y);
		}
	}
	return out;
}
Pixel PixelMatrix::detThreshold(Pixel min, Pixel max) {
	makeHistgram();
	Pixel ans{};
	ans = Pixel(std::min_element(hist.begin() + min, hist.begin() + max) - hist.begin());
	return ans;
}



PixelMatrix PixelMatrix::medianFilter(void) {
	PixelMatrix tmp = *this;
#pragma omp parallel for
	for (int y = 0; y < height; y++) {
#pragma omp parallel for
		for (int x = 0; x < width; x++) {
			tmp.mat.at(x,y) = calcMedianOfNeighbor(x, y, 3);
		}
	}
	return tmp;
}



int PixelMatrix::applyOperatorOnPixel(const int x, const int y, const Operator& filter, Dim xy) {
	double ans = 0.0;
	auto f = filter[0];
	if (xy == Dim::Y) {
		f = filter[1];
	}
	auto s = static_cast<int>(f.size() / 2);
#pragma omp parallel for
	for (int i = -s; i <= s; i++) {
#pragma omp parallel for
		for (int j = -s; j <= s; j++) {
			auto cx = x + j, cy = y + i;
			if (cx >= 0 && cx < width && cy >= 0 && cy < height) {
				auto tmp = (mat.at(cx,cy) * f.at(j + s,i + s));
#pragma omp atomic
				ans += tmp;
			}
		}
	}
	return (static_cast<int>(ans));
}

Pixel PixelMatrix::calcMedianOfNeighbor(const int x, const int y, const int size) {
	std::vector<Pixel> v{};
#pragma omp parallel for
	for (int i = -size / 2; i <= size / 2; i++) {
#pragma omp parallel for
		for (int j = -size / 2; j <= size / 2; j++) {
			auto cx = x + j, cy = y + i;
			if (cx >= 0 && cx < width && cy >= 0 && cy < height) {
				v.push_back(mat.at(cx,cy));
			}
		}
	}
	size_t tmp = (v.size() / 2) + 1;
	std::nth_element(v.begin(), v.begin() + tmp, v.end());

	return v[tmp];
}



void PixelMatrix::makeHistgram(void) {
	hist.assign((header.Info.biClrUsed ? header.Info.biClrUsed : 256), 0);
#pragma omp parallel for
	for (int y = 0; y < height; y++) {
#pragma omp parallel for
		for (int x = 0; x < width; x++) {
#pragma omp critical (hUpdate)
			hist[mat.at(x, y)]++;
			
		}
	}
}



std::string PixelMatrix::showHistgram(void) {
	makeHistgram();
	std::stringstream ss{};
	int i{};
	for (auto& n : hist) {
		i++;
		ss << i << "\t" << n << '\n';
	}
	ss << std::accumulate(hist.begin(), hist.end(), (size_t)0) << '\n' << (size_t)width*(size_t)height << '\n';
	return ss.str();
}

