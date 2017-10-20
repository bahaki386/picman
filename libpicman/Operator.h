#pragma once
#include "stdafx.h"

using Operator = std::vector<std::vector<std::vector<double>>>;
static const Operator
averageMatrix = {
	{
	{1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0},
	{1.0 / 9.0, 1.0 / 9.0, 1.0/ 9.0},
	{1.0 / 9.0, 1.0 / 9.0, 1.0 / 9.0}
	}
},
prewittMatrix = {
	{
	{1, 0, -1},
	{1, 0, -1},
	{1, 0, -1}
	},
	{
	{1, 1, 1},
	{0, 0, 0},
	{-1, -1, -1}
	}
},
gaussianMatrix = {
	{
	{1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0},
	{2.0 / 16.0, 4.0 / 16.0, 2.0 / 16.0},
	{1.0 / 16.0, 2.0 / 16.0, 1.0 / 16.0}
	}
},
laplacianMatrix = { {
	{1,  1,	1},
	{1, -8,	1},
	{1,  1,	1} } },
	sobelMatrix = {
		{
		{1, 0, -1},
		{2, 0, -2},
		{1, 0, -1}
		},
		{
	{1, 2, 1},
		{0, 0, 0},
		{-1, -2, -1}
}
};
