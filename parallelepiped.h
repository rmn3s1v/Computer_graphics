#pragma once
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <math.h>
#include <algorithm>
#include <stack>
#include <tuple>
#include "T.h"
using namespace std;
#define ROTATE 2 * (3.14 / 180)
#define AROTATE -2 * (3.14 / 180)
#define ROWS 8 // ПАРАЛ
#define COLOMNS 4 // ПАРАЛ
#define MOVE_XYZ 10
#define SCALE 1.2
#define ASCALE 0.8
#define COLOR RGB(200, 50, 150)

class P
{
public:
	float centreWeightX = 0;
	float centreWeightY = 0;
	float centreWeightZ = 0;

	float coordinats[ROWS][COLOMNS] =
	{
		{200, 100, 0, 1}, // A 0
		{400, 100, 0, 1}, // B 1
		{400, 100, 50, 1}, // C 2
		{200, 100, 50, 1}, // D 3
		{200, 300, 0, 1}, // E 4
		{400, 300, 0, 1}, // F 5
		{400, 300, 50, 1}, // G 6
		{200, 300, 50, 1}, // H 7
	};

	P()
	{
		centreWeightX = 0;
		centreWeightY = 0;
		centreWeightZ = 0;

		for (int i = 0; i < 8; i++)
		{
			centreWeightX += coordinats[i][0];
			centreWeightY += coordinats[i][1];
			centreWeightZ += coordinats[i][2];
		}
		centreWeightX /= 8;
		centreWeightY /= 8;
		centreWeightZ /= 8;
	}
	void Draw(float(&coordinates)[ROWS][COLOMNS], HDC hdc);
	void Fill(HDC hdc, int x, int y);
	void FillShadow(HDC hdc, int x, int y);
	void Shadow(HDC hdc, float(&coordinates)[ROWS][COLOMNS]);
	void RotateX(float(&coordinates)[ROWS][COLOMNS], float x0, float y0, float z0, float rotate);
	void RotateY(float(&coordinates)[ROWS][COLOMNS], float x0, float y0, float z0, float rotate);
	void RotateZ(float(&coordinates)[ROWS][COLOMNS], float x0, float y0, float z0, float rotate);
	void Move(float(&coordinates)[ROWS][COLOMNS], int move_x, int move_y, int move_z);
	void Scale(float(&coordinates)[ROWS][COLOMNS], float x0, float y0, float z0, float scale);

};
void P::Draw(float(&coordinates)[ROWS][COLOMNS], HDC hdc)
{
	Shadow(hdc, coordinates);
	float matrix[4][4] = {
		{1, 0, 0, 0},
		{0,1, 0, 0},
		{0.5, -0.5, 0, 0},
		{0, 0, 0, 1}
	};

	float result[ROWS][COLOMNS] = { 0 };
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLOMNS; col++)
		{
			for (int inner = 0; inner < COLOMNS; inner++)
			{
				result[row][col] += coordinates[row][inner] * matrix[inner][col];
			}
		}
	}

	float facets[6][3] = {
	{result[2][0] * result[1][1] - result[1][0] * result[2][1], -(result[3][0] * result[1][1] -
	result[1][0] * result[3][1]), result[3][0] * result[2][1] - result[2][0] * result[3][1]}, // верхняя грань ABCD
	{result[5][0] * result[6][1] - result[6][0] * result[5][1], -(result[4][0] * result[6][1] -
	result[6][0] * result[4][1]), result[4][0] * result[5][1] - result[5][0] * result[4][1]}, // нижняя грань EFGH
	{result[3][0] * result[0][1] - result[0][0] * result[3][1], -(result[7][0] * result[0][1] -
	result[0][0] * result[7][1]), result[7][0] * result[3][1] - result[3][0] * result[7][1]}, // левая грань HEAD
	{result[1][0] * result[5][1] - result[5][0] * result[1][1], -(result[0][0] * result[5][1] -
	result[5][0] * result[0][1]), result[0][0] * result[1][1] - result[1][0] * result[0][1]}, // задняя грань ABFE
	{result[6][0] * result[5][1] - result[5][0] * result[6][1], -(result[2][0] * result[5][1] -
	result[5][0] * result[2][1]), result[2][0] * result[6][1] - result[6][0] * result[2][1]}, // правая грань CBFG
	{result[6][0] * result[2][1] - result[2][0] * result[6][1], -(result[7][0] * result[2][1] -
	result[2][0] * result[7][1]), result[7][0] * result[6][1] - result[6][0] * result[7][1]} // передняя грань DCGH
	};

	float orientedfacets[6] = { 0 };

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			orientedfacets[i] += facets[i][j];
		}

		if (orientedfacets[i] <= 0) orientedfacets[i] = -1;
		else orientedfacets[i] = 1;
	}

	float VisAndInvisCoord[12][4] =
	{
		{result[0][0], result[0][1], result[1][0], result[1][1]}, // AB
		{result[1][0], result[1][1], result[2][0], result[2][1]}, // BC
		{result[2][0], result[2][1], result[3][0], result[3][1]}, // CD
		{result[3][0], result[3][1], result[0][0], result[0][1]}, // DA
		{result[4][0], result[4][1], result[5][0], result[5][1]}, // EF
		{result[5][0], result[5][1], result[6][0], result[6][1]}, // FG
		{result[6][0], result[6][1], result[7][0], result[7][1]}, // GH
		{result[7][0], result[7][1], result[4][0], result[4][1]}, // HE
		{result[2][0], result[2][1], result[6][0], result[6][1]}, // CG
		{result[3][0], result[3][1], result[7][0], result[7][1]}, // HD
		{result[0][0], result[0][1], result[4][0], result[4][1]}, // EA
		{result[1][0], result[1][1], result[5][0], result[5][1]}, // FB
	};

	float ribs[12] =
	{
		orientedfacets[0] + orientedfacets[3] + 1, // AB
		orientedfacets[0] + orientedfacets[4] + 1, // BC
		orientedfacets[0] + orientedfacets[5] + 1, // CD
		orientedfacets[0] + orientedfacets[2] + 1, // DA
		orientedfacets[1] + orientedfacets[3] + 1, // EF
		orientedfacets[1] + orientedfacets[4] + 1, // FG
		orientedfacets[1] + orientedfacets[5] + 1, // GH
		orientedfacets[1] + orientedfacets[2] + 1, // HE
		orientedfacets[5] + orientedfacets[4] + 1, // CG
		orientedfacets[5] + orientedfacets[2] + 1, // HD
		orientedfacets[3] + orientedfacets[2] + 1, // EA
		orientedfacets[3] + orientedfacets[4] + 1, // FB
	};

	for (int i = 0; i < 12; i++)
	{
		if (ribs[i] < 0) ribs[i] = -1;
		else ribs[i] = 1;
	}

	for (int i = 0; i < 12; i++)
	{
		if (ribs[i] > 0)
		{
			HPEN pen = CreatePen(PS_SOLID, 4, RGB(200, 50, 150));
			SelectObject(hdc, pen);
			MoveToEx(hdc, VisAndInvisCoord[i][0], VisAndInvisCoord[i][1], nullptr);
			LineTo(hdc, VisAndInvisCoord[i][2], VisAndInvisCoord[i][3]);
		}
	}

	for (int i = 0; i < 6; i++)
	{
		if (orientedfacets[i] == 1)
		{
			// СЕРЕДИНЫ ГРАНЕЙ
			if (i == 0)
			{
				// верхняя грань ABCD
				Fill(hdc, (result[0][0] + result[1][0] + result[2][0] + result[3][0]) / 4, (result[0][1] + result[1][1] + result[2][1] + result[3][1]) / 4);
			}
			else if (i == 1)
			{
				// нижняя грань EFGH
				Fill(hdc, (result[4][0] + result[5][0] + result[6][0] + result[7][0]) / 4, (result[4][1] + result[5][1] + result[6][1] + result[7][1]) / 4);
			}
			else if (i == 2)
			{
				// левая грань HEAD
				Fill(hdc, (result[7][0] + result[4][0] + result[3][0] + result[0][0]) / 4, (result[7][1] + result[4][1] + result[3][1] + result[0][1]) / 4);
			}
			else if (i == 3)
			{
				// задняя грань ABFE
				Fill(hdc, (result[0][0] + result[1][0] + result[5][0] + result[4][0]) / 4, (result[0][1] + result[1][1] + result[5][1] + result[4][1]) / 4);
			}
			else if (i == 4)
			{
				// правая грань CBFG
				Fill(hdc, (result[1][0] + result[2][0] + result[5][0] + result[6][0]) / 4, (result[1][1] + result[2][1] + result[5][1] + result[6][1]) / 4);
			}
			else
			{
				// передняя грань DCGH
				Fill(hdc, (result[3][0] + result[2][0] + result[7][0] + result[6][0]) / 4, (result[3][1] + result[2][1] + result[7][1] + result[6][1]) / 4);
			}
		}
	}

}
void P::Fill(HDC hdc, int x, int y)
{
	// ОПРЕДЕЛЯЕМ РАЗМЕР ОКНА
	HWND consoleWindow = GetConsoleWindow();
	RECT consoleRect;
	GetClientRect(consoleWindow, &consoleRect);
	int consoleWidth = consoleRect.right - consoleRect.left;
	int consoleHeight = consoleRect.bottom - consoleRect.top;

	stack<tuple<int, int>> pixels;
	pixels.push(make_tuple(x, y));

	int** visitedPixels = new int* [consoleWidth];
	for (int i = 0; i < consoleWidth; i++)
	{
		visitedPixels[i] = new int[consoleHeight];
	}
	for (int i = 0; i < consoleWidth; i++)
	{
		for (int j = 0; j < consoleHeight; j++)
		{
			visitedPixels[i][j] = 0;
		}
	}

	while (!pixels.empty())
	{
		auto currentPixel = pixels.top();
		pixels.pop();

		int currentX, currentY;
		tie(currentX, currentY) = currentPixel;

		if (!(currentX >= 0 && currentX < consoleWidth && currentY >= 0 && currentY < consoleHeight) || visitedPixels[currentX][currentY] == 1) continue;

		visitedPixels[currentX][currentY] = 1;

		if (GetPixel(hdc, currentX, currentY) != COLOR)
		{
			SetPixel(hdc, currentX, currentY, RGB(200, 100, 100));
			pixels.push(make_tuple(currentX + 1, currentY));
			pixels.push(make_tuple(currentX - 1, currentY));
			pixels.push(make_tuple(currentX, currentY + 1));
			pixels.push(make_tuple(currentX, currentY - 1));
		}
	}

}
void P::FillShadow(HDC hdc, int x, int y)
{
	// ОПРЕДЕЛЯЕМ РАЗМЕР ОКНА
	HWND consoleWindow = GetConsoleWindow();
	RECT consoleRect;
	GetClientRect(consoleWindow, &consoleRect);
	int consoleWidth = consoleRect.right - consoleRect.left;
	int consoleHeight = consoleRect.bottom - consoleRect.top;

	stack<tuple<int, int>> pixels;
	pixels.push(make_tuple(x, y));

	int** visitedPixels = new int* [consoleWidth];
	for (int i = 0; i < consoleWidth; i++)
	{
		visitedPixels[i] = new int[consoleHeight];
	}
	for (int i = 0; i < consoleWidth; i++)
	{
		for (int j = 0; j < consoleHeight; j++)
		{
			visitedPixels[i][j] = 0;
		}
	}

	while (!pixels.empty())
	{
		auto currentPixel = pixels.top();
		pixels.pop();

		int currentX, currentY;
		tie(currentX, currentY) = currentPixel;

		if (!(currentX >= 0 && currentX < consoleWidth && currentY >= 0 && currentY < consoleHeight) || visitedPixels[currentX][currentY] == 1) continue;

		visitedPixels[currentX][currentY] = 1;

		if (GetPixel(hdc, currentX, currentY) != RGB(50, 150, 100))
		{
			SetPixel(hdc, currentX, currentY, RGB(50, 150, 100));
			pixels.push(make_tuple(currentX + 1, currentY));
			pixels.push(make_tuple(currentX - 1, currentY));
			pixels.push(make_tuple(currentX, currentY + 1));
			pixels.push(make_tuple(currentX, currentY - 1));
		}
	}

}
void P::Shadow(HDC hdc, float(&coordinates)[ROWS][COLOMNS])
{

	int matrixShadow[4][4] =
	{
		{100, 0, 1, 0},
		{0, 100, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 1, 100}
	};

	float cor[ROWS][COLOMNS] = { 0 };

	for (int i = 0; i < ROWS; i++)
	{
		cor[i][0] = coordinates[i][0];
		cor[i][1] = coordinates[i][2];
		cor[i][2] = coordinates[i][1];
		cor[i][3] = coordinates[i][3];
	}

	float result1[ROWS][COLOMNS] = { 0 };
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLOMNS; col++)
		{
			for (int inner = 0; inner < COLOMNS; inner++)
			{
				result1[row][col] += cor[row][inner] * matrixShadow[inner][col];
			}
		}
	}
	if (result1[0][3] != 1)
	{
		for (int i = 0; i < ROWS; i++)
		{
			result1[i][0] /= result1[i][3];
			result1[i][1] /= result1[i][3];
			result1[i][2] /= result1[i][3];
			result1[i][3] /= result1[i][3];
		}
	}
	//for (int i = 0; i < ROWS; i++) cout << result[i][0] << "|" << result[i][1] << "|" << result[i][2] << "|" << result[i][3] << endl;
	for (int i = 0; i < ROWS; i++)
	{
		cor[i][0] = result1[i][0];
		cor[i][1] = result1[i][2] + 500;
		cor[i][2] = result1[i][1];
		cor[i][3] = result1[i][3];
	}
	//for (int i = 0; i < ROWS; i++) cout << cor[i][0] << "|" << cor[i][1] << "|" << cor[i][2] << "|" << cor[i][3] << endl;

	float matrix[4][4] = {
		{1, 0, 0, 0},
		{0,1, 0, 0},
		{0.5, -0.5, 0, 0},
		{0, 0, 0, 1}
	};

	float result[ROWS][COLOMNS] = { 0 };
	for (int row = 0; row < ROWS; row++)
	{
		for (int col = 0; col < COLOMNS; col++)
		{
			for (int inner = 0; inner < COLOMNS; inner++)
			{
				result[row][col] += cor[row][inner] * matrix[inner][col];
			}
		}
	}

	float facets[6][3] = {
	{result[2][0] * result[1][1] - result[1][0] * result[2][1], -(result[3][0] * result[1][1] -
	result[1][0] * result[3][1]), result[3][0] * result[2][1] - result[2][0] * result[3][1]}, // верхняя грань ABCD
	{result[5][0] * result[6][1] - result[6][0] * result[5][1], -(result[4][0] * result[6][1] -
	result[6][0] * result[4][1]), result[4][0] * result[5][1] - result[5][0] * result[4][1]}, // нижняя грань EFGH
	{result[3][0] * result[0][1] - result[0][0] * result[3][1], -(result[7][0] * result[0][1] -
	result[0][0] * result[7][1]), result[7][0] * result[3][1] - result[3][0] * result[7][1]}, // левая грань HEAD
	{result[1][0] * result[5][1] - result[5][0] * result[1][1], -(result[0][0] * result[5][1] -
	result[5][0] * result[0][1]), result[0][0] * result[1][1] - result[1][0] * result[0][1]}, // задняя грань ABFE
	{result[6][0] * result[5][1] - result[5][0] * result[6][1], -(result[2][0] * result[5][1] -
	result[5][0] * result[2][1]), result[2][0] * result[6][1] - result[6][0] * result[2][1]}, // правая грань CBFG
	{result[6][0] * result[2][1] - result[2][0] * result[6][1], -(result[7][0] * result[2][1] -
	result[2][0] * result[7][1]), result[7][0] * result[6][1] - result[6][0] * result[7][1]} // передняя грань DCGH
	};

	float orientedfacets[6] = { 0 };

	for (int i = 0; i < 6; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			orientedfacets[i] += facets[i][j];
		}

		if (orientedfacets[i] <= 0) orientedfacets[i] = -1;
		else orientedfacets[i] = 1;
	}

	float VisAndInvisCoord[12][4] =
	{
		{result[0][0], result[0][1], result[1][0], result[1][1]}, // AB
		{result[1][0], result[1][1], result[2][0], result[2][1]}, // BC
		{result[2][0], result[2][1], result[3][0], result[3][1]}, // CD
		{result[3][0], result[3][1], result[0][0], result[0][1]}, // DA
		{result[4][0], result[4][1], result[5][0], result[5][1]}, // EF
		{result[5][0], result[5][1], result[6][0], result[6][1]}, // FG
		{result[6][0], result[6][1], result[7][0], result[7][1]}, // GH
		{result[7][0], result[7][1], result[4][0], result[4][1]}, // HE
		{result[2][0], result[2][1], result[6][0], result[6][1]}, // CG
		{result[3][0], result[3][1], result[7][0], result[7][1]}, // HD
		{result[0][0], result[0][1], result[4][0], result[4][1]}, // EA
		{result[1][0], result[1][1], result[5][0], result[5][1]}, // FB
	};

	float ribs[12] =
	{
		orientedfacets[0] + orientedfacets[3] + 1, // AB
		orientedfacets[0] + orientedfacets[4] + 1, // BC
		orientedfacets[0] + orientedfacets[5] + 1, // CD
		orientedfacets[0] + orientedfacets[2] + 1, // DA
		orientedfacets[1] + orientedfacets[3] + 1, // EF
		orientedfacets[1] + orientedfacets[4] + 1, // FG
		orientedfacets[1] + orientedfacets[5] + 1, // GH
		orientedfacets[1] + orientedfacets[2] + 1, // HE
		orientedfacets[5] + orientedfacets[4] + 1, // CG
		orientedfacets[5] + orientedfacets[2] + 1, // HD
		orientedfacets[3] + orientedfacets[2] + 1, // EA
		orientedfacets[3] + orientedfacets[4] + 1, // FB
	};

	for (int i = 0; i < 12; i++)
	{
		if (ribs[i] < 0) ribs[i] = -1;
		else ribs[i] = 1;
	}

	for (int i = 0; i < 12; i++)
	{
		if (ribs[i] > 0)
		{
			HPEN pen = CreatePen(PS_SOLID, 4, RGB(50, 150, 100));
			SelectObject(hdc, pen);
			MoveToEx(hdc, VisAndInvisCoord[i][0], VisAndInvisCoord[i][1], nullptr);
			LineTo(hdc, VisAndInvisCoord[i][2], VisAndInvisCoord[i][3]);
		}
	}
	for (int i = 0; i < 6; i++)
	{
		if (orientedfacets[i] == 1)
		{
			// СЕРЕДИНЫ ГРАНЕЙ
			if (i == 0)
			{
				// верхняя грань ABCD
				FillShadow(hdc, (result[0][0] + result[1][0] + result[2][0] + result[3][0]) / 4, (result[0][1] + result[1][1] + result[2][1] + result[3][1]) / 4);
			}
			else if (i == 1)
			{
				// нижняя грань EFGH
				FillShadow(hdc, (result[4][0] + result[5][0] + result[6][0] + result[7][0]) / 4, (result[4][1] + result[5][1] + result[6][1] + result[7][1]) / 4);
			}
			else if (i == 2)
			{
				// левая грань HEAD
				FillShadow(hdc, (result[7][0] + result[4][0] + result[3][0] + result[0][0]) / 4, (result[7][1] + result[4][1] + result[3][1] + result[0][1]) / 4);
			}
			else if (i == 3)
			{
				// задняя грань ABFE
				FillShadow(hdc, (result[0][0] + result[1][0] + result[5][0] + result[4][0]) / 4, (result[0][1] + result[1][1] + result[5][1] + result[4][1]) / 4);
			}
			else if (i == 4)
			{
				// правая грань CBFG
				FillShadow(hdc, (result[1][0] + result[2][0] + result[5][0] + result[6][0]) / 4, (result[1][1] + result[2][1] + result[5][1] + result[6][1]) / 4);
			}
			else
			{
				// передняя грань DCGH
				FillShadow(hdc, (result[3][0] + result[2][0] + result[7][0] + result[6][0]) / 4, (result[3][1] + result[2][1] + result[7][1] + result[6][1]) / 4);
			}
		}
	}
}
void P::RotateX(float(&coordinates)[ROWS][COLOMNS], float x0, float y0, float z0, float rotate)
{
	float RotateX[4][4] =
	{
		{1, 0, 0, 0},
		{0, cos(rotate), sin(rotate), 0},
		{0, -sin(rotate), cos(rotate), 0},
		{0, -y0 * cos(rotate) + z0 * sin(rotate) + y0,
		-y0 * sin(rotate) - z0 * cos(rotate) + z0, 1}
	};

	float result[ROWS][COLOMNS] = { 0 };

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result[i][j] += coordinates[i][k] * RotateX[k][j];
			}
		}
	}

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			coordinates[i][j] = result[i][j];
		}
	}

	if (result[0][3] != 1)
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				coordinates[i][j] = result[i][j] / result[0][3];
			}
		}
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				coordinates[i][j] = result[i][j];
			}
		}
	}
}
void P::RotateY(float(&coordinates)[ROWS][COLOMNS], float x0, float y0, float z0, float rotate)
{
	float RotateX[4][4] =
	{
		{cos(rotate), 0, sin(rotate), 0},
		{0, 1, 0, 0},
		{-sin(rotate), 0, cos(rotate), 0},
		{-x0 * cos(rotate) + z0 * sin(rotate) + x0, 0, -x0 * sin(rotate) - z0 * cos(rotate) + z0, 1}
	};

	float result[ROWS][COLOMNS] = { 0 };

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result[i][j] += coordinates[i][k] * RotateX[k][j];
			}
		}
	}

	if (result[0][3] != 1)
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				coordinates[i][j] = result[i][j] / result[0][3];
			}
		}
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				coordinates[i][j] = result[i][j];
			}
		}
	}
}
void P::RotateZ(float(&coordinates)[ROWS][COLOMNS], float x0, float y0, float z0, float rotate)
{
	float RotateZ[4][4] =
	{
		{cos(rotate), sin(rotate), 0, 0},
		{-sin(rotate), cos(rotate), 0, 0},
		{0, 0, 1, 0},
		{-x0 * cos(rotate) + y0 * sin(rotate) + x0, -x0 * sin(rotate) - y0 * cos(rotate) + y0, 0, 1}
	};

	float result[ROWS][COLOMNS] = { 0 };

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result[i][j] += coordinates[i][k] * RotateZ[k][j];
			}
		}
	}

	if (result[0][3] != 1)
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				coordinates[i][j] = result[i][j] / result[0][3];
			}
		}
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				coordinates[i][j] = result[i][j];
			}
		}
	}
}
void P::Move(float(&coordinates)[ROWS][COLOMNS], int move_x, int move_y, int move_z)
{
	float offset[4][4] = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{move_x, move_y, move_z, 1}
	};

	float result[ROWS][COLOMNS] = { 0 };

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result[i][j] += coordinates[i][k] * offset[k][j];
			}
		}
	}

	if (result[0][3] != 1)
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				coordinates[i][j] = result[i][j] / result[0][3];
			}
		}
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				coordinates[i][j] = result[i][j];
			}
		}
	}
}
void P::Scale(float(&coordinates)[ROWS][COLOMNS], float x0, float y0, float z0, float scale)
{
	float size[4][4] =
	{
		{scale, 0, 0, 0},
		{0, scale, 0, 0},
		{0, 0, scale, 0},
		{-x0 * scale + x0, -y0 * scale + y0, -z0 * scale + z0, 1}
	};

	float result[ROWS][COLOMNS] = { 0 };

	for (int i = 0; i < 8; i++)
	{
		for (int j = 0; j < 4; j++)
		{
			for (int k = 0; k < 4; k++)
			{
				result[i][j] += coordinates[i][k] * size[k][j];
			}
		}
	}

	if (result[0][3] != 1)
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				coordinates[i][j] = result[i][j] / result[0][3];
			}
		}
	}
	else
	{
		for (int i = 0; i < 8; i++)
		{
			for (int j = 0; j < 4; j++)
			{
				coordinates[i][j] = result[i][j];
			}
		}
	}
}
