#pragma once
#include <iostream>
#include <Windows.h>
#include <conio.h>
#include <math.h>
#include <algorithm>
#include <stack>
#include <tuple>
using namespace std;
#define ROTATE 2 * (3.14 / 180) #define AROTATE -2 * (3.14 / 180) #define ROWST 4
#define COLOMNST 4
#define MOVE_XYZ 10
#define SCALE 1.2
#define ASCALE 0.8
#define COLOR1 RGB(0, 50, 200)

class T
{
    public:
// ТРЕУГОЛЬНАЯ ПИРАМИДА
    float coordinats[ROWST][COLOMNST] = { {250, 400, 10, 1}, // A 0
    {350, 400, 45, 1}, // B 1
    {400, 400, 10, 1}, // C 2
    {333, 50, 25, 1}, // D 3
    };

    int al;
    float centreWeightX = 0, centreWeightY = 0, centreWeightZ = 0;

    T()
    {
        for (int i = 0; i < 4; i++)
        {
            centreWeightX += coordinats[i][0];
            centreWeightY += coordinats[i][1];
            centreWeightZ += coordinats[i][2];
        }
        centreWeightX /= 4;
        centreWeightY /= 4;
        centreWeightZ /= 4;
    }

    void Draw(float(&coordinates)[ROWST][COLOMNST], HDC hdc); void Fill(HDC hdc, int x, int y);
    void FillShadow(HDC hdc, int x, int y);
    void Shadow(HDC hdc, float(&coordinates)[ROWST][COLOMNST]);
    void RotateX(float(&coordinates)[ROWST][COLOMNST], float x0, float y0, float z0, float rotate);
    void RotateY(float(&coordinates)[ROWST][COLOMNST], float x0, float y0, float z0, float rotate);
    void RotateZ(float(&coordinates)[ROWST][COLOMNST], float x0, float y0, float z0, float rotate);
    void Move(float(&coordinates)[ROWST][COLOMNST], int move_x, int move_y, int move_z);
    void Scale(float(&coordinates)[ROWST][COLOMNST], float x0, float y0, float z0, float scale);
};

void T::Draw(float(&coordinates)[ROWST][COLOMNST], HDC hdc)
{
	Shadow(hdc, coordinates);
	float matrix[4][4] = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0.5, -0.5, 0, 0},
		{0, 0, 0, 1}
	};

	// ПРОЕЦИРОВАНИЕ НА ПЛОСКОСТЬ
	float result[ROWST][COLOMNST] = { 0 };
	for (int row = 0; row < ROWST; row++)
	{
		for (int col = 0; col < COLOMNST; col++)
		{
			for (int inner = 0; inner < COLOMNST; inner++)
			{
				result[row][col] += coordinates[row][inner] * matrix[inner][col];
			}
		}
	}

	/*for (int row = 0; row < ROWST; row++)
	{
		for (int col = 0; col < COLOMNST; col++)
		{
			cout << result[row][col] << " ";
		}
		cout << endl;
	}*/

	float facets[4][3] =
	{
		{result[2][0] * result[0][1] - result[0][0] * result[2][1], -(result[1][0] * result[0][1] - result[0][0] * result[1][1]), result[1][0] * result[2][1] - result[2][0] * result[1][1]}, //  грань ABC
		{result[2][0] * result[3][1] - result[3][0] * result[2][1], -(result[0][0] * result[3][1] - result[3][0] * result[0][1]), result[0][0] * result[2][1] - result[2][0] * result[0][1]}, //  грань CAD
		{result[2][0] * result[1][1] - result[1][0] * result[2][1], -(result[3][0] * result[1][1] - result[1][0] * result[3][1]), result[3][0] * result[2][1] - result[2][0] * result[3][1]}, //  грань CBD
		{result[3][0] * result[1][1] - result[1][0] * result[3][1], -(result[0][0] * result[1][1] - result[1][0] * result[0][1]), result[0][0] * result[3][1] - result[3][0] * result[0][1]} //  грань ABD
	};


	float orientedfacets[4] = { 0 };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			orientedfacets[i] += facets[i][j];
		}
		if (orientedfacets[i] <= 0) orientedfacets[i] = -1;
		else orientedfacets[i] = 1;
	}

	//for (int i = 0; i < 4; i++) cout << orientedfacets[i] << endl;

	float VisAndInvisCoord[6][4] = {
		{result[0][0], result[0][1], result[1][0], result[1][1]}, // AB
		{result[1][0], result[1][1], result[2][0], result[2][1]}, // BC
		{result[2][0], result[2][1], result[0][0], result[0][1]}, // CA
		{result[0][0], result[0][1], result[3][0], result[3][1]}, // AD
		{result[3][0], result[3][1], result[2][0], result[2][1]}, // DC
		{result[1][0], result[1][1], result[3][0], result[3][1]}, // BD
	};

	float ribs[6] =
	{
		orientedfacets[0] + orientedfacets[3] + 1, // AB
		orientedfacets[0] + orientedfacets[2] + 1, // BC
		orientedfacets[0] + orientedfacets[1] + 1, // AC
		orientedfacets[1] + orientedfacets[3] + 1, // AD
		orientedfacets[1] + orientedfacets[2] + 1, // DC
		orientedfacets[2] + orientedfacets[3] + 1 // BD
	};

	//for (int i = 0; i < 6; i++) cout << ribs[i] << endl;

	for (int i = 0; i < 6; i++)
	{
		if (ribs[i] < 0) ribs[i] = -1;
		else ribs[i] = 1;
	}

	//for (int i = 0; i < 6; i++) cout << ribs[i] << endl;

	for (int i = 0; i < 6; i++)
	{
		if (ribs[i] > 0)
		{
			HPEN pen = CreatePen(PS_SOLID, 4, COLOR1);
			SelectObject(hdc, pen);
			MoveToEx(hdc, VisAndInvisCoord[i][0], VisAndInvisCoord[i][1], nullptr);
			LineTo(hdc, VisAndInvisCoord[i][2], VisAndInvisCoord[i][3]);
		}
	}

	for (int i = 0; i < 4; i++)
	{
		if (orientedfacets[i] == 1)
		{
			// СЕРЕДИНЫ ГРАНЕЙ
			if (i == 0)
			{
				//  грань ABC
				Fill(hdc, (result[0][0] + result[1][0] + result[2][0]) / 3, (result[0][1] + result[1][1] + result[2][1]) / 3);
			}
			else if (i == 1)
			{
				//  грань CAD
				Fill(hdc, (result[0][0] + result[2][0] + result[3][0]) / 3, (result[0][1] + result[2][1] + result[3][1]) / 3);
			}
			else if (i == 2)
			{
				//  грань CBD
				Fill(hdc, (result[2][0] + result[1][0] + result[3][0]) / 3, (result[2][1] + result[1][1] + result[3][1]) / 3);
			}
			else
			{
				//  грань ABD
				Fill(hdc, (result[0][0] + result[1][0] + result[3][0]) / 3, (result[0][1] + result[1][1] + result[3][1]) / 3);
			}
		}
	}

}
void T::Fill(HDC hdc, int x, int y)
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

		if (GetPixel(hdc, currentX, currentY) != COLOR1)
		{
			SetPixel(hdc, currentX, currentY, RGB(200, 100, 100));
			pixels.push(make_tuple(currentX + 1, currentY));
			pixels.push(make_tuple(currentX - 1, currentY));
			pixels.push(make_tuple(currentX, currentY + 1));
			pixels.push(make_tuple(currentX, currentY - 1));
		}
	}
}
void T::FillShadow(HDC hdc, int x, int y)
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

		if (GetPixel(hdc, currentX, currentY) != RGB(50, 151, 100))
		{
			SetPixel(hdc, currentX, currentY, RGB(50, 151, 100));
			pixels.push(make_tuple(currentX + 1, currentY));
			pixels.push(make_tuple(currentX - 1, currentY));
			pixels.push(make_tuple(currentX, currentY + 1));
			pixels.push(make_tuple(currentX, currentY - 1));
		}
	}
}
void T::Shadow(HDC hdc, float(&coordinates)[ROWST][COLOMNST])
{

	int matrixShadow[4][4] =
	{
		{100, 0, 1, 0},
		{0, 100, 0, 0},
		{0, 0, 1, 0},
		{0, 0, 1, 100}
	};

	float cor[ROWST][COLOMNST] = { 0 };

	for (int i = 0; i < ROWST; i++)
	{
		cor[i][0] = coordinates[i][0];
		cor[i][1] = coordinates[i][2];
		cor[i][2] = coordinates[i][1];
		cor[i][3] = coordinates[i][3];
	}

	float result1[ROWST][COLOMNST] = { 0 };
	for (int row = 0; row < ROWST; row++)
	{
		for (int col = 0; col < COLOMNST; col++)
		{
			for (int inner = 0; inner < COLOMNST; inner++)
			{
				result1[row][col] += cor[row][inner] * matrixShadow[inner][col];
			}
		}
	}
	if (result1[0][3] != 1)
	{
		for (int i = 0; i < ROWST; i++)
		{
			result1[i][0] /= result1[i][3];
			result1[i][1] /= result1[i][3];
			result1[i][2] /= result1[i][3];
			result1[i][3] /= result1[i][3];
		}
	}
	//for (int i = 0; i < ROWS; i++) cout << result[i][0] << "|" << result[i][1] << "|" << result[i][2] << "|" << result[i][3] << endl;
	for (int i = 0; i < ROWST; i++)
	{
		cor[i][0] = result1[i][0];
		cor[i][1] = result1[i][2] + 500;
		cor[i][2] = result1[i][1];
		cor[i][3] = result1[i][3];
	}
	//for (int i = 0; i < ROWST; i++) cout << cor[i][0] << "|" << cor[i][1] << "|" << cor[i][2] << "|" << cor[i][3] << endl;

	float matrix[4][4] = {
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0.5, -0.5, 0, 0},
		{0, 0, 0, 1}
	};

	// ПРОЕЦИРОВАНИЕ НА ПЛОСКОСТЬ
	float result[ROWST][COLOMNST] = { 0 };
	for (int row = 0; row < ROWST; row++)
	{
		for (int col = 0; col < COLOMNST; col++)
		{
			for (int inner = 0; inner < COLOMNST; inner++)
			{
				result[row][col] += cor[row][inner] * matrix[inner][col];
			}
		}
	}

	float facets[4][3] =
	{
		{result[2][0] * result[0][1] - result[0][0] * result[2][1], -(result[1][0] * result[0][1] - result[0][0] * result[1][1]), result[1][0] * result[2][1] - result[2][0] * result[1][1]}, //  грань ABC
		{result[2][0] * result[3][1] - result[3][0] * result[2][1], -(result[0][0] * result[3][1] - result[3][0] * result[0][1]), result[0][0] * result[2][1] - result[2][0] * result[0][1]}, //  грань CAD
		{result[2][0] * result[1][1] - result[1][0] * result[2][1], -(result[3][0] * result[1][1] - result[1][0] * result[3][1]), result[3][0] * result[2][1] - result[2][0] * result[3][1]}, //  грань CBD
		{result[3][0] * result[1][1] - result[1][0] * result[3][1], -(result[0][0] * result[1][1] - result[1][0] * result[0][1]), result[0][0] * result[3][1] - result[3][0] * result[0][1]} //  грань ABD
	};


	float orientedfacets[4] = { 0 };

	for (int i = 0; i < 4; i++)
	{
		for (int j = 0; j < 3; j++)
		{
			orientedfacets[i] += facets[i][j];
		}
		if (orientedfacets[i] <= 0) orientedfacets[i] = -1;
		else orientedfacets[i] = 1;
	}


	float VisAndInvisCoord[6][4] = {
		{result[0][0], result[0][1], result[1][0], result[1][1]}, // AB
		{result[1][0], result[1][1], result[2][0], result[2][1]}, // BC
		{result[2][0], result[2][1], result[0][0], result[0][1]}, // CA
		{result[0][0], result[0][1], result[3][0], result[3][1]}, // AD
		{result[3][0], result[3][1], result[2][0], result[2][1]}, // DC
		{result[1][0], result[1][1], result[3][0], result[3][1]}, // BD
	};

	float ribs[6] =
	{
		orientedfacets[0] + orientedfacets[3] + 1, // AB
		orientedfacets[0] + orientedfacets[2] + 1, // BC
		orientedfacets[0] + orientedfacets[1] + 1, // AC
		orientedfacets[1] + orientedfacets[3] + 1, // AD
		orientedfacets[1] + orientedfacets[2] + 1, // DC
		orientedfacets[2] + orientedfacets[3] + 1 // BD
	};

	//for (int i = 0; i < 6; i++) cout << ribs[i] << endl;

	for (int i = 0; i < 6; i++)
	{
		if (ribs[i] < 0) ribs[i] = -1;
		else ribs[i] = 1;
	}

	//for (int i = 0; i < 6; i++) cout << ribs[i] << endl;

	for (int i = 0; i < 6; i++)
	{
		if (ribs[i] > 0)
		{
			HPEN pen = CreatePen(PS_SOLID, 4, RGB(50, 151, 100));
			SelectObject(hdc, pen);
			MoveToEx(hdc, VisAndInvisCoord[i][0], VisAndInvisCoord[i][1], nullptr);
			LineTo(hdc, VisAndInvisCoord[i][2], VisAndInvisCoord[i][3]);
		}
	}
	for (int i = 0; i < 4; i++)
	{
		if (orientedfacets[i] == 1)
		{
			// СЕРЕДИНЫ ГРАНЕЙ
			if (i == 0)
			{
				//  грань ABC
				FillShadow(hdc, (result[0][0] + result[1][0] + result[2][0]) / 3, (result[0][1] + result[1][1] + result[2][1]) / 3);
			}
			else if (i == 1)
			{
				//  грань CAD
				FillShadow(hdc, (result[0][0] + result[2][0] + result[3][0]) / 3, (result[0][1] + result[2][1] + result[3][1]) / 3);
			}
			else if (i == 2)
			{
				//  грань CBD
				FillShadow(hdc, (result[2][0] + result[1][0] + result[3][0]) / 3, (result[2][1] + result[1][1] + result[3][1]) / 3);
			}
			else
			{
				//  грань ABD
				FillShadow(hdc, (result[0][0] + result[1][0] + result[3][0]) / 3, (result[0][1] + result[1][1] + result[3][1]) / 3);
			}
		}
	}
}
void T::RotateX(float(&coordinates)[ROWST][COLOMNST], float x0, float y0, float z0, float rotate)
{
	float RotateX[4][4] =
	{
		{1, 0, 0, 0},
		{0, cos(rotate), sin(rotate), 0},
		{0, -sin(rotate), cos(rotate), 0},
		{0, -y0 * cos(rotate) + z0 * sin(rotate) + y0,
		-y0 * sin(rotate) - z0 * cos(rotate) + z0, 1}
	};

	float result[ROWST][COLOMNST] = { 0 };

	for (int i = 0; i < ROWST; i++)
	{
		for (int j = 0; j < COLOMNST; j++)
		{
			for (int k = 0; k < COLOMNST; k++)
			{
				result[i][j] += coordinates[i][k] * RotateX[k][j];
			}
		}

	}
	for (int i = 0; i < ROWST; i++)
	{
		for (int j = 0; j < COLOMNST; j++)
		{
			coordinates[i][j] = result[i][j];
		}
	}

	if (result[0][3] != 1)
	{
		for (int i = 0; i < ROWST; i++)
		{
			for (int j = 0; j < COLOMNST; j++)
			{
				coordinates[i][j] = result[i][j] / result[0][3];
			}
		}
	}
	else
	{
		for (int i = 0; i < ROWST; i++)
		{
			for (int j = 0; j < COLOMNST; j++)
			{
				coordinates[i][j] = result[i][j];
			}
		}
	}
}
void T::RotateY(float(&coordinates)[ROWST][COLOMNST], float x0, float y0, float z0, float rotate)
{
	float RotateX[4][4] =
	{
		{cos(rotate), 0, sin(rotate), 0},
		{0, 1, 0, 0},
		{-sin(rotate), 0, cos(rotate), 0},
		{-x0 * cos(rotate) + z0 * sin(rotate) + x0, 0, -x0 * sin(rotate) - z0 * cos(rotate) + z0, 1}
	};

	float result[ROWST][COLOMNST] = { 0 };

	for (int i = 0; i < ROWST; i++)
	{
		for (int j = 0; j < COLOMNST; j++)
		{
			for (int k = 0; k < COLOMNST; k++)
			{
				result[i][j] += coordinates[i][k] * RotateX[k][j];
			}
		}
	}

	if (result[0][3] != 1)
	{
		for (int i = 0; i < ROWST; i++)
		{
			for (int j = 0; j < COLOMNST; j++)
			{
				coordinates[i][j] = result[i][j] / result[0][3];
			}
		}
	}
	else
	{
		for (int i = 0; i < ROWST; i++)
		{
			for (int j = 0; j < COLOMNST; j++)
			{
				coordinates[i][j] = result[i][j];
			}
		}
	}
}
void T::RotateZ(float(&coordinates)[ROWST][COLOMNST], float x0, float y0, float z0, float rotate)
{
	float RotateZ[4][4] =
	{
		{cos(rotate), sin(rotate), 0, 0},
		{-sin(rotate), cos(rotate), 0, 0},
		{0, 0, 1, 0},
		{-x0 * cos(rotate) + y0 * sin(rotate) + x0, -x0 * sin(rotate) - y0 * cos(rotate) + y0, 0, 1}
	};

	float result[ROWST][COLOMNST] = { 0 };

	for (int i = 0; i < ROWST; i++)
	{
		for (int j = 0; j < COLOMNST; j++)
		{
			for (int k = 0; k < COLOMNST; k++)
			{
				result[i][j] += coordinates[i][k] * RotateZ[k][j];
			}
		}
	}

	if (result[0][3] != 1)
	{
		for (int i = 0; i < ROWST; i++)
		{
			for (int j = 0; j < COLOMNST; j++)
			{
				coordinates[i][j] = result[i][j] / result[0][3];
			}
		}
	}
	else
	{
		for (int i = 0; i < ROWST; i++)
		{
			for (int j = 0; j < COLOMNST; j++)
			{
				coordinates[i][j] = result[i][j];
			}
		}
	}
}
void T::Move(float(&coordinates)[ROWST][COLOMNST], int move_x, int move_y, int move_z)
{
	float offset[4][4] =
	{
		{1, 0, 0, 0},
		{0, 1, 0, 0},
		{0, 0, 1, 0},
		{move_x, move_y, move_z, 1}
	};

	float result[ROWST][COLOMNST] = { 0 };

	for (int i = 0; i < ROWST; i++)
	{
		for (int j = 0; j < COLOMNST; j++)
		{
			for (int k = 0; k < COLOMNST; k++)
			{
				result[i][j] += coordinates[i][k] * offset[k][j];
			}
		}
	}

	if (result[0][3] != 1)
	{
		for (int i = 0; i < ROWST; i++)
		{
			for (int j = 0; j < COLOMNST; j++)
			{
				coordinates[i][j] = result[i][j] / result[0][3];
			}
		}
	}
	else
	{
		for (int i = 0; i < ROWST; i++)
		{
			for (int j = 0; j < COLOMNST; j++)
			{
				coordinates[i][j] = result[i][j];
			}
		}
	}
}
void T::Scale(float(&coordinates)[ROWST][COLOMNST], float x0, float y0, float z0, float scale)
{
	float size[4][4] =
	{
		{scale, 0, 0, 0},
		{0, scale, 0, 0},
		{0, 0, scale, 0},
		{-x0 * scale + x0, -y0 * scale + y0, -z0 * scale + z0, 1}
	};

	float result[ROWST][COLOMNST] = { 0 };

	for (int i = 0; i < ROWST; i++)
	{
		for (int j = 0; j < COLOMNST; j++)
		{
			for (int k = 0; k < COLOMNST; k++)
			{
				result[i][j] += coordinates[i][k] * size[k][j];
			}
		}
	}

	if (result[0][3] != 1)
	{
		for (int i = 0; i < ROWST; i++)
		{
			for (int j = 0; j < COLOMNST; j++)
			{
				coordinates[i][j] = result[i][j] / result[0][3];
			}
		}
	}
	else
	{
		for (int i = 0; i < ROWST; i++)
		{
			for (int j = 0; j < COLOMNST; j++)
			{
				coordinates[i][j] = result[i][j];
			}
		}
	}
}
