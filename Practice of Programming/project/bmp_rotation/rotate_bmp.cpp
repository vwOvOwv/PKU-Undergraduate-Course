#include<stdio.h>
#include<iostream>
#include<fstream>
#include<math.h>
#include<stdlib.h>

using namespace std;
typedef unsigned char BYTE;
typedef unsigned short WORD;
typedef unsigned int DWORD;
typedef long LONG;
typedef int FXPT2DOT30;

typedef struct tagCIEXYZ {
	FXPT2DOT30 ciexyzX;
	FXPT2DOT30 ciexyzY;
	FXPT2DOT30 ciexyzZ;
}CIEXYZ, * LPCIEXYZ;
typedef struct tagCIEXYZTRIPLE {
	CIEXYZ ciexyzRed;
	CIEXYZ ciexyzGreen;
	CIEXYZ ciexyzBlue;
}CIEXYZTRIPLE;
struct BITMAP_FILE_HEADER{//文件头
	WORD bfType; // 位图文件的类型，必须为“BM”
	DWORD bfSize; // 位图文件的大小，以字节为单位
	WORD bfReserved1; // 位图文件保留字，必须为0
	WORD bfReserved2; // 位图文件保留字，必须为0
	DWORD bfOffBits; // 位图数据的起始位置，以相对于位图文件头的偏移量表示，以字节为单位
};
struct BITMAP_INFO_HEADER {//信息头
	DWORD biSize; // 本结构所占用字节数
	LONG biWidth; // 位图的宽度，以像素为单位
	LONG biHeight; // 位图的高度，以像素为单位
	WORD biPlanes; // 目标设备的平面数不清，必须为1
	WORD biBitCount;// 每个像素所需的位数，必须是1(双色), 4(16色)，8(256色)或24(真彩色)之一
	DWORD biCompression; // 位图压缩类型，必须是 0(不压缩),1(BI_RLE8压缩类型)或2(BI_RLE4压缩类型)之一
	DWORD biSizeImage; // 位图的大小，以字节为单位
	LONG biXPelsPerMeter; // 位图水平分辨率，每米像素数
	LONG biYPelsPerMeter; // 位图垂直分辨率，每米像素数
	DWORD biClrUsed;// 位图实际使用的颜色表中的颜色数
	DWORD biClrImportant;// 位图显示过程中重要的颜色数
};
struct Plus{
	DWORD bV5RedMask;
	DWORD bV5GreenMask;
	DWORD bV5BlueMask;
	DWORD bV5AlphaMask;
	DWORD bV5CSType;
	CIEXYZTRIPLE bV5Endpoints;
	DWORD bV5GammaRed;
	DWORD bV5GammaGreen;
	DWORD bV5GammaBlue;
	DWORD bV5Intent;
	DWORD bV5ProfileData;
	DWORD bV5ProfileSize;
	DWORD bV5Reserved;
};
int main(int argc,char*argv[]) {
	if (argc != 3) {
		cout << "File name error!" << endl;
		return 0;
	}
	BITMAP_FILE_HEADER head;
	BITMAP_INFO_HEADER info;
	Plus plus;
	fstream inFile(argv[1], ios::in|ios::binary);
	if (!inFile){
		cout << "Source file open error!" << endl; 
		return 0;
	}
	//读取文件头与信息头
	inFile.read((char*)&head.bfType, sizeof(WORD));
	inFile.read((char*)&head.bfSize, sizeof(DWORD));
	inFile.read((char*)&head.bfReserved1, sizeof(WORD));
	inFile.read((char*)&head.bfReserved2, sizeof(WORD));
	inFile.read((char*)&head.bfOffBits, sizeof(DWORD));
	inFile.read((char*)&info.biSize, sizeof(DWORD));
	inFile.read((char*)&info.biWidth, sizeof(LONG));
	inFile.read((char*)&info.biHeight, sizeof(LONG));
	inFile.read((char*)&info.biPlanes, sizeof(WORD));
	inFile.read((char*)&info.biBitCount, sizeof(WORD));
	inFile.read((char*)&info.biCompression, sizeof(DWORD));
	inFile.read((char*)&info.biSizeImage, sizeof(DWORD));
	inFile.read((char*)&info.biXPelsPerMeter, sizeof(LONG));
	inFile.read((char*)&info.biYPelsPerMeter, sizeof(LONG));
	inFile.read((char*)&info.biClrUsed, sizeof(DWORD));
	inFile.read((char*)&info.biClrImportant, sizeof(DWORD));
	if (info.biBitCount == 32)
	{
		inFile.read((char*)&plus.bV5RedMask, sizeof(DWORD));
		inFile.read((char*)&plus.bV5GreenMask, sizeof(DWORD));
		inFile.read((char*)&plus.bV5BlueMask, sizeof(DWORD));
		inFile.read((char*)&plus.bV5AlphaMask, sizeof(DWORD));
		inFile.read((char*)&plus.bV5CSType, sizeof(DWORD));
		inFile.read((char*)&plus.bV5Endpoints, sizeof(CIEXYZTRIPLE));
		inFile.read((char*)&plus.bV5GammaRed, sizeof(DWORD));
		inFile.read((char*)&plus.bV5GammaGreen, sizeof(DWORD));
		inFile.read((char*)&plus.bV5GammaBlue, sizeof(DWORD));
		inFile.read((char*)&plus.bV5Intent, sizeof(DWORD));
		inFile.read((char*)&plus.bV5ProfileData, sizeof(DWORD));
		inFile.read((char*)&plus.bV5ProfileSize, sizeof(DWORD));
		inFile.read((char*)&plus.bV5Reserved, sizeof(DWORD));
	}
	//读取位图数据
	int DataSizePerLine = (info.biWidth * info.biBitCount / 8 + 3) / 4 * 4;// 一个扫描行所占的字节数
	int DataSize = DataSizePerLine * info.biHeight;//总字节数
 	BYTE* p1 = new BYTE[DataSize];
	memset(p1, 0, DataSize);
	for (int i = 0; i < info.biHeight; i++) {
		for (int j = 0; j < info.biWidth; j++) {
			for (int k = 0; k < info.biBitCount / 8; k++)
				inFile.read((char*)&p1[info.biBitCount / 8 * (info.biWidth * i + j) + k], sizeof(BYTE));
		}
		inFile.seekg(DataSizePerLine%(info.biBitCount/8), ios::cur);
	}
	//旋转坐标变换
	BYTE* p2 = new BYTE[DataSize];
	memset(p2,0,DataSize);
	for (int i = 0; i < info.biHeight; i++)
		for (int j = 0; j < info.biWidth; j++) {
			for (int k = 0; k < info.biBitCount / 8; k++)
				p2[info.biBitCount/8* ((info.biWidth - 1 - j) * info.biHeight + i) + k] = p1[info.biBitCount/8 * (info.biWidth * i + j) + k];
		}
	fstream outFile(argv[2], ios::out | ios::binary);
	if (!outFile) {
		cout << "New file open error!" << endl;
		inFile.close();
		return 0;
	}
	swap(info.biWidth, info.biHeight);
	//写入文件
	outFile.write((char*)&head.bfType, sizeof(WORD));
	outFile.write((char*)&head.bfSize, sizeof(DWORD));
	outFile.write((char*)&head.bfReserved1, sizeof(WORD));
	outFile.write((char*)&head.bfReserved2, sizeof(WORD));
	outFile.write((char*)&head.bfOffBits, sizeof(DWORD));
	outFile.write((char*)&info.biSize, sizeof(DWORD));
	outFile.write((char*)&info.biWidth, sizeof(LONG));
	outFile.write((char*)&info.biHeight, sizeof(LONG));
	outFile.write((char*)&info.biPlanes, sizeof(WORD));
	outFile.write((char*)&info.biBitCount, sizeof(WORD));
	outFile.write((char*)&info.biCompression, sizeof(DWORD));
	outFile.write((char*)&info.biSizeImage, sizeof(DWORD));
	outFile.write((char*)&info.biXPelsPerMeter, sizeof(LONG));
	outFile.write((char*)&info.biYPelsPerMeter, sizeof(LONG));
	outFile.write((char*)&info.biClrUsed, sizeof(DWORD));
	outFile.write((char*)&info.biClrImportant, sizeof(DWORD));
	if (info.biBitCount == 32)
	{
		outFile.write((char*)&plus.bV5RedMask, sizeof(DWORD));
		outFile.write((char*)&plus.bV5GreenMask, sizeof(DWORD));
		outFile.write((char*)&plus.bV5BlueMask, sizeof(DWORD));
		outFile.write((char*)&plus.bV5AlphaMask, sizeof(DWORD));
		outFile.write((char*)&plus.bV5CSType, sizeof(DWORD));
		outFile.write((char*)&plus.bV5Endpoints, sizeof(CIEXYZTRIPLE));
		outFile.write((char*)&plus.bV5GammaRed, sizeof(DWORD));
		outFile.write((char*)&plus.bV5GammaGreen, sizeof(DWORD));
		outFile.write((char*)&plus.bV5GammaBlue, sizeof(DWORD));
		outFile.write((char*)&plus.bV5Intent, sizeof(DWORD));
		outFile.write((char*)&plus.bV5ProfileData, sizeof(DWORD));
		outFile.write((char*)&plus.bV5ProfileSize, sizeof(DWORD));
		outFile.write((char*)&plus.bV5Reserved, sizeof(DWORD));
	}
	//写入位图数据
	DataSizePerLine = info.biWidth * info.biBitCount / 8;
	int delta = 0;
	while (DataSizePerLine % 4 != 0) {
		DataSizePerLine++;
		delta++;
	}
	char* plus1 = new char[delta];
	for (int i = 0; i < delta; i++)
		plus1[i] = 0;
	for (int i = 0; i < info.biHeight; i++) {
		for (int j = 0; j < info.biWidth; j++) {
			for (int k = 0; k < info.biBitCount / 8; k++)
				outFile.write((char*)&p2[info.biBitCount / 8 * (info.biWidth * i + j) + k], sizeof(BYTE));
		}
		outFile.write(plus1, delta);
	}
	cout << "Mission accomplished." << endl;
	//关闭文件
	delete[]p1;
	delete[]p2;
	inFile.close();
	outFile.close();
	return 0;
}
