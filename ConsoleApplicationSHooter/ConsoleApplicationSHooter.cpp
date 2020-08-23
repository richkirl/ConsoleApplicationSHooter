#include <iostream>
#include <Windows.h>
#include <chrono>
int nScreenWidth = 120;
int nScreenHeight = 40;
float fPlayerX = 8.0f;
float fPlayerY = 8.0f;
float fPlayerA = 0.0f;
int nMapWidth = 16;
int nMapHeight = 16;
float fFov = 3.14159f / 4.0f;
float fDepth = 32.0f;
//source  javidx9
//https://youtu.be/xW8skO7MFYw
int main()
{
    wchar_t* screen = new wchar_t[nScreenWidth * nScreenHeight];
    HANDLE hConsole = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
    SetConsoleActiveScreenBuffer(hConsole);
    DWORD dwBytesWriten = 0;
    std::wstring map;
    map += L"################";
    map += L"#..............#";
    map += L"#..####........#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#.......#.#....#";
    map += L"#..............#";
    map += L"#.......#.#....#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#..............#";
    map += L"#........#######";
    map += L"#..............#";
    map += L"#..............#";
    map += L"################";

    auto tp1 = std::chrono::system_clock::now();
    auto tp2 = std::chrono::system_clock::now();
    while (1)
    {
        tp2 = std::chrono::system_clock::now();
        std::chrono::duration<float> elapsedTime = tp2 - tp1;
        tp1 = tp2;
        float fElapsedTime = elapsedTime.count();

        if (GetAsyncKeyState((unsigned short)'A') & 0x8000)
            fPlayerA -= (2.0f)*fElapsedTime;
        if (GetAsyncKeyState(0x1B))
            ExitProcess(0);
        if (GetAsyncKeyState((unsigned short)'D') & 0x8000)
            fPlayerA += (2.0f)*fElapsedTime;
        if (GetAsyncKeyState((unsigned short)'W') & 0x8000)
        {
            fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;
            fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;
            if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#')
            {
                fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
                fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;
            }
        }
        if (GetAsyncKeyState((unsigned short)'S') & 0x8000)
        {
            fPlayerX -= sinf(fPlayerA) * 5.0f * fElapsedTime;
            fPlayerY -= cosf(fPlayerA) * 5.0f * fElapsedTime;
            if (map[(int)fPlayerY * nMapWidth + (int)fPlayerX] == '#')
            {
                fPlayerX += sinf(fPlayerA) * 5.0f * fElapsedTime;
                fPlayerY += cosf(fPlayerA) * 5.0f * fElapsedTime;
            }
        }
        for (int x = 0; x < nScreenWidth; x++)
        {
            float fRayAngle = (fPlayerA - fFov / 2.0f) + ((float)x / (float)nScreenWidth) * fFov;

            float FDistanceToWall = 0;
            bool bHitWall = false;
            float fEyeX = sinf(fRayAngle)*0.5f;
            float fEyeY = cosf(fRayAngle)*0.5f;
            while (!bHitWall && FDistanceToWall < fDepth)
            {
                FDistanceToWall += 0.01f;
                int nTestX = (int)(fPlayerX + fEyeX*FDistanceToWall);
                int nTestY = (int)(fPlayerY + fEyeY*FDistanceToWall);
                if (nTestX < 0 || nTestX >= nMapWidth || nTestY < 0 || nTestY >= nMapHeight)
                {
                    bHitWall = true;
                    FDistanceToWall = fDepth;
                }
                else
                {
                    if (map[nTestY * nMapWidth + nTestX] == '#')
                    {
                        bHitWall = true;
                    }
                }
            }
            int nCeiling =int((float)(nScreenHeight/2.0f)- nScreenHeight/((float)FDistanceToWall));
            int nFloor = nScreenHeight - nCeiling;

            short nShade = ' ';

                                                      
            if (FDistanceToWall <= fDepth / 4.0f)            nShade = 0x2588;
            else if (FDistanceToWall < fDepth / 3.0f)        nShade = 0x2593;
            else if (FDistanceToWall < fDepth / 2.0f)        nShade = 0x2592;
            else if (FDistanceToWall < fDepth)               nShade = 0x2591;
            else                                             nShade = ' ';
            for (int y = 0; y < nScreenHeight; y++)
            {
                if (y < nCeiling)
                    screen[y * nScreenWidth + x] = ' ';
                else if (y > nCeiling && y <= nFloor)
                {

                    screen[y * nScreenWidth + x] = nShade;
                }
                else
                {   
                    float b = 0.1f - (((float)y - nScreenHeight / 2.0f) / ((float)nScreenHeight / 2.0f));
                    if (b < 0.25)               nShade =0x2578;
                    else if (b<0.5)             nShade ='x';
                    else if (b < 0.75)          nShade ='.';
                    else if (b < 0.9)           nShade = 0x2592;
                    else                        nShade = 0x2591;
                    screen[y * nScreenWidth + x] = nShade;
                }
                    
            }
        }
        screen[nScreenWidth-1* nScreenHeight-1] = '\0';
        WriteConsoleOutputCharacter(hConsole, screen, nScreenWidth * nScreenHeight, { 0,0 }, &dwBytesWriten);
    }
    delete[] screen;
    ExitProcess(0);
}
