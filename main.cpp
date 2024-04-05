#include "parallelepiped.h"
#include "pyramid.h"
#include <list>

void CalculateCentre(T& t, P&p);

int main() {
    HWND hWnd = GetConsoleWindow();
    HDC hdc = GetDC(hWnd);
    P p;
    T t;
    t.Draw(t.coordinats, hdc);
    p.Draw(p.coordinats, hdc);
    while (1)
    {
        switch (char buttom = _getch())
        {
            case 'd':
                system("cls");
                CalculateCentre(t, p);
                p.Move(p.coordinats, MOVE_XYZ, 0, 0);
                t.Move(t.coordinats, MOVE_XYZ, 0, 0);
                t.Draw(t.coordinats, hdc);
                p.Draw(p.coordinats, hdc);
                break;

            case 'a':
                system("cls");
                CalculateCentre(t, p);
                p.Move(p.coordinats, -MOVE_XYZ, 0, 0);
                t.Move(t.coordinats, -MOVE_XYZ, 0, 0);
                t.Draw(t.coordinats, hdc);
                p.Draw(p.coordinats, hdc);
                break;

            case 'w':
                system("cls");
                CalculateCentre(t, p);
                p.Move(p.coordinats, 0, -MOVE_XYZ, 0);
                t.Move(t.coordinats, 0, -MOVE_XYZ, 0);
                t.Draw(t.coordinats, hdc);
                p.Draw(p.coordinats, hdc);
                p.Draw(p.coordinats, hdc); t.Draw(t.coordinats, hdc);
                p.Draw(p.coordinats, hdc); t.Draw(t.coordinats, hdc);
                p.Draw(p.coordinats, hdc); t.Draw(t.coordinats, hdc);
                break;

            case 's':
                system("cls");
                CalculateCentre(t, p);
                p.Move(p.coordinats, 0, MOVE_XYZ, 0);
                t.Move(t.coordinats, 0, MOVE_XYZ, 0);
                t.Draw(t.coordinats, hdc);
                p.Draw(p.coordinats, hdc);
                break;

            case '+':
                system("cls");
                CalculateCentre(t, p);
                p.Scale(p.coordinats, p.centreWeightZ, p.centreWeightY, p.centreWeightZ, SCALE);
                t.Scale(t.coordinats, t.centreWeightX, t.centreWeightY, t.centreWeightZ, SCALE);
                t.Draw(t.coordinats, hdc);
                p.Draw(p.coordinats, hdc);

            break; case '-':
                system("cls");
                CalculateCentre(t, p);
                p.Scale(p.coordinats, p.centreWeightX, p.centreWeightY, p.centreWeightZ, ASCALE);
                t.Scale(t.coordinats, t.centreWeightX, t.centreWeightY, t.centreWeightZ, ASCALE);
                t.Draw(t.coordinats, hdc);
                p.Draw(p.coordinats, hdc);
                break;

            case 'x':
                system("cls");
                CalculateCentre(t, p);
                p.RotateX(p.coordinats, p.centreWeightX, p.centreWeightY, p.centreWeightZ, ROTATE);
                t.RotateX(t.coordinats, t.centreWeightX, t.centreWeightY, t.centreWeightZ, ROTATE);
                t.Draw(t.coordinats, hdc);
                p.Draw(p.coordinats, hdc);
                break;

            case 'X':
                system("cls");
                CalculateCentre(t, p);
                p.RotateX(p.coordinats, p.centreWeightX, p.centreWeightY, p.centreWeightZ, AROTATE);
                t.RotateX(t.coordinats, t.centreWeightX, t.centreWeightY, t.centreWeightZ, AROTATE);
                t.Draw(t.coordinats, hdc);
                p.Draw(p.coordinats, hdc);
                break;

            case 'y':
                system("cls");
                CalculateCentre(t, p);
                p.RotateX(p.coordinats, p.centreWeightX, p.centreWeightY, p.centreWeightZ, ROTATE);
                t.RotateX(t.coordinats, t.centreWeightX, t.centreWeightY, t.centreWeightZ, ROTATE);
                t.Draw(t.coordinats, hdc);
                p.Draw(p.coordinats, hdc);
                break;

            case 'Y':
                system("cls");
                CalculateCentre(t, p);
                p.RotateX(p.coordinats, p.centreWeightX, p.centreWeightY, p.centreWeightZ, AROTATE);
                t.RotateX(t.coordinats, t.centreWeightX, t.centreWeightY, t.centreWeightZ, AROTATE);
                t.Draw(t.coordinats, hdc);
                p.Draw(p.coordinats, hdc);
                break;

            case 'z':
                system("cls");
                CalculateCentre(t, p);
                p.RotateX(p.coordinats, p.centreWeightX, p.centreWeightY, p.centreWeightZ, ROTATE);
                t.RotateX(t.coordinats, t.centreWeightX, t.centreWeightY, t.centreWeightZ, ROTATE);
                t.Draw(t.coordinats, hdc);
                p.Draw(p.coordinats, hdc);
                break;

            case 'Z':
                system("cls");
                CalculateCentre(t, p);
                p.RotateX(p.coordinats, p.centreWeightX, p.centreWeightY, p.centreWeightZ, AROTATE);
                t.RotateX(t.coordinats, t.centreWeightX, t.centreWeightY, t.centreWeightZ, AROTATE);
                t.Draw(t.coordinats, hdc);
                p.Draw(p.coordinats, hdc);
                break;

            case 'D':
                system("cls");
                CalculateCentre(t, p);
                p.Move(p.coordinats, 0, 0, MOVE_XYZ);
                t.Move(t.coordinats, 0, 0, MOVE_XYZ);
                t.Draw(t.coordinats, hdc);
                p.Draw(p.coordinats, hdc);
                break;

            case 'A':
                system("cls");
                CalculateCentre(t, p);
                p.Move(p.coordinats, 0, 0, -MOVE_XYZ);
                t.Move(t.coordinats, 0, 0, -MOVE_XYZ);
                t.Draw(t.coordinats, hdc);
                p.Draw(p.coordinats, hdc);
                break;

            default:
                break;
        }
    }
    return 0;
}

void CalculateCentre(T& t, P& p)
{
    p.centreWeightX = 0;
    p.centreWeightY = 0;
    p.centreWeightZ = 0;
    t.centreWeightX = 0;
    t.centreWeightY = 0;
    t.centreWeightZ = 0;
    float centreX = 0;
    float centreY = 0;
    float centreZ = 0;

    for (int i = 0; i < 8; i++)
    {
        centreX += p.coordinats[i][0]; centreY += p.coordinats[i][1]; centreZ += p.coordinats[i][2];
    }

    for (int i = 0; i < 4; i++)
    {
        centreX += t.coordinats[i][0]; centreY += t.coordinats[i][1]; centreZ += t.coordinats[i][2];
    }

    p.centreWeightX = centreX/12;
    p.centreWeightY = centreY/12;
    p.centreWeightZ = centreZ/12;
    t.centreWeightX = centreX/12;
    t.centreWeightY = centreY/12;
    t.centreWeightZ = centreZ/12;
}
