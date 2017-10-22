// stdafx.h : 標準のシステム インクルード ファイルのインクルード ファイル、または
// 参照回数が多く、かつあまり変更されない、プロジェクト専用のインクルード ファイル
// を記述します。
//

#pragma once

#include "targetver.h"

#include <stdio.h>
#include <tchar.h>



// TODO: プログラムに必要な追加ヘッダーをここで参照してください
#include <vector>
#include <cstdint>
#include <fstream>
#include <sstream>
#include <algorithm>
#include <valarray>
#include <stdexcept>
#include <numeric>
#ifdef PIXELMATRIXDLL

#define PIXELMATRIXDLL_API __declspec(dllexport)

#else

#define PIXELMATRIXDLL_API __declspec(dllimport)

#endif
enum class PIXELMATRIXDLL_API Dim { X, Y, N };
using Pixel = uint8_t;

