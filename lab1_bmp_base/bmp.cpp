#include <windows.h>
#include <iostream>
#include <string>

using namespace std;

void main()
{
	string sFileName;
	BITMAPFILEHEADER bmpFileHeader;
    BITMAPINFOHEADER bmpInfoHeader;
    int Width, Height;
    RGBQUAD Palette[256];
    RGBTRIPLE *inBuf;
    BYTE *outBuf;
    HANDLE hInputFile, hOutFile;
    DWORD RW;

	cout << "Enter the full name, please: ";
	cin >> sFileName;

	hInputFile = CreateFile(sFileName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hInputFile == INVALID_HANDLE_VALUE)
		return;

	hOutFile = CreateFile("Result.bmp", GENERIC_WRITE, 0, NULL, CREATE_NEW, 0, NULL);
    if (hOutFile == INVALID_HANDLE_VALUE)
    {
		CloseHandle (hInputFile);
		return;
	}
	
	// ��������� ����
    ReadFile (hInputFile, &bmpFileHeader, sizeof(bmpFileHeader), &RW, NULL);
    ReadFile (hInputFile, &bmpInfoHeader, sizeof(bmpInfoHeader), &RW, NULL);

	// ��������� ��������� �� ������ ������
    SetFilePointer (hInputFile, bmpFileHeader.bfOffBits, NULL, FILE_BEGIN);
    Width = bmpInfoHeader.biWidth;
    Height = bmpInfoHeader.biHeight;

    // ������� ������
    inBuf = new RGBTRIPLE [Width];
    outBuf = new BYTE [Width];

    // �������� ���������
    bmpFileHeader.bfOffBits = sizeof (bmpFileHeader) + sizeof (bmpInfoHeader) + 1024;                              
    bmpInfoHeader.biBitCount = 8;
    bmpFileHeader.bfSize = bmpFileHeader.bfOffBits + Width * Height + Height * (3*Width % 4);   

    // ������� ���������
    WriteFile (hOutFile, &bmpFileHeader, sizeof(bmpFileHeader), &RW, NULL);
    WriteFile (hOutFile, &bmpInfoHeader, sizeof(bmpInfoHeader), &RW, NULL);
	// ������� �����-�����
	for (int i = 0; i < 256; i++)
	{
		Palette[i].rgbBlue = i;
		Palette[i].rgbGreen = i;
		Palette[i].rgbRed = i;
	}
	WriteFile (hOutFile, Palette, 256 * sizeof (RGBQUAD), &RW, NULL);

    // ������ ���������������
    for (int i = 0; i < Height; i++)
    {
        ReadFile (hInputFile, inBuf, sizeof(RGBTRIPLE) * Width, &RW, NULL);
        for (int j = 0; j < Width; j++)
            outBuf[j] = 0.3*inBuf[j].rgbtRed + 0.59*inBuf[j].rgbtGreen + 0.11*inBuf[j].rgbtBlue;

        WriteFile (hOutFile, outBuf, sizeof(BYTE) * Width, &RW, NULL);

        // ����� ����� ��� ������������
        WriteFile (hOutFile, Palette, (3*Width) % 4, &RW, NULL);
        SetFilePointer (hInputFile, Width % 4, NULL, FILE_CURRENT);
    }

    delete[] inBuf;
    delete[] outBuf;
    CloseHandle (hInputFile);
    CloseHandle (hOutFile);

	cout << "Updating has come to the end successfully!";
	system("pause");
}