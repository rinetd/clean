#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <stdlib.h>
#include <stdio.h>
//#include "window.h"

/*bmp file header*/
typedef struct FileHeader
{
	unsigned short	bfType;
	unsigned int	bfSize;
	unsigned short	bfReserved1;
	unsigned short	bfReserved2;
	unsigned int	bfOffBits;
}__attribute__((packed))FileHeader;

/*bmp info header*/
typedef struct InfoHeader
{
	unsigned int	biSize;
	int				biWidth;
	int				biHeight;
	unsigned short	biPlanes;
	unsigned short	biBitCount;
	unsigned int	biCompression;
	unsigned int	biSizeImage;
	int				biXPelsPerMeter;
	int 			biYPelsPerMeter;
	unsigned int	biClrUsed;
	unsigned int	biClrImportant;
}__attribute__((packed))InfoHeader;

//we won't use it while BitCount=24
typedef struct tagRGBQUAD
{
	unsigned char	rgbBlue;
	unsigned char	rgbGreen;
	unsigned char	rgbRed;
	unsigned char	rgbReserved;
}RGBQUAD;

typedef struct
{
	unsigned char b;
	unsigned char g;
	unsigned char r;
}RGB_data;

/*for test global variable*/
/*maybe can not use this in the project*/
unsigned char 	*pBmpBuf,*pBmpBufOne,*pBmpBufTwo,*pBmpBufThree;
RGBQUAD 		*pColorTable;
int		bmpWidth,bmpWidthback,bmpWidthsky;
int		bmpHeight,bmpHeightback,bmpHeightsky;
int		biBitCount,biBitCountback,biBitCountsky;

char * readBmp(char *bmpName)
{
	FILE *fp = fopen(bmpName, "rb");
	//int fp;		/*maybe fd should be more comfortably*/
	//fp = open(bmpName, O_RDONLY);
	if (NULL == fp)
	{
		printf("There is no fp!!!\n");
		return 0;
	}
    FileHeader bmpFileHeader;
	fread(&bmpFileHeader, sizeof(FileHeader), 1, fp);

	// fseek(fp, sizeof(FileHeader), SEEK_SET);
	//lseek(fp, sizeof(FileHeader), SEEK_SET);
	InfoHeader bmpInfo;
	fread(&bmpInfo, sizeof(InfoHeader), 1, fp);
	//read(fp, &head, sizeof(InfoHeader));

	bmpWidth = bmpInfo.biWidth;
	bmpHeight = bmpInfo.biHeight;
	biBitCount = bmpInfo.biBitCount;



	// 输出BMP文件的位图文件头的所有信息
	printf("位图文件头主要是对位图文件的一些描述:BMPFileHeader\n\n");
	printf("文件标识符 = 0X%X\n", bmpFileHeader.bfType);
	printf("BMP 文件大小 = %d 字节\n", bmpFileHeader.bfSize);
	printf("保留值1 = %d \n", bmpFileHeader.bfReserved1);
	printf("保留值2 = %d \n", bmpFileHeader.bfReserved2);
	printf("文件头的最后到图像数据位开始的偏移量 = %d 字节\n", bmpFileHeader.bfOffBits);

	// 输出BMP文件的位图信息头的所有信息
	printf("\n\n位图信息头主要是对位图图像方面信息的描述:BMPInfo\n\n");
	printf("信息头的大小 = %d 字节\n", bmpInfo.biSize);
	printf("位图的宽度 = %d \n", bmpInfo.biWidth);
	printf("位图的高度 = %d \n", bmpInfo.biHeight);
	printf("图像的位面数(位面数是调色板的数量,默认为1个调色板) = %d \n", bmpInfo.biPlanes);
	printf("每个像素的位数 = %d 位\n", bmpInfo.biBitCount);
	printf("压缩类型 = %d \n", bmpInfo.biCompression);
	printf("图像的大小 = %d 字节\n", bmpInfo.biSizeImage);
	printf("水平分辨率 = %d \n", bmpInfo.biXPelsPerMeter);
	printf("垂直分辨率 = %d \n", bmpInfo.biYPelsPerMeter);
	printf("使用的色彩数 = %d \n", bmpInfo.biClrUsed);
	printf("重要的色彩数 = %d \n", bmpInfo.biClrImportant);

	printf("\n\n\n压缩说明：有0（不压缩），1（RLE 8，8位RLE压缩），2（RLE 4，4位RLE压缩，3（Bitfields，位域存放）");


	// 4字节对齐
	int lineByte = (bmpWidth * biBitCount / 8 + 3) / 4 * 4;
	if (biBitCount == 8)
	{
		//pColorTable=new RGBQUAD[256];
		pColorTable = (RGBQUAD *)malloc(sizeof(RGBQUAD) * 1024);
		fread(pColorTable, sizeof(RGBQUAD), 256, fp);
	}

	pBmpBuf = (unsigned char *)malloc(sizeof(unsigned char) * lineByte * bmpHeight);
	fread(pBmpBuf, 1, lineByte * bmpHeight, fp);
	fclose(fp);

	return pBmpBuf;
}


int saveBmp(char *bmpName, unsigned char *imgBuf, int width, int height, int biBitCount, RGBQUAD *pColorTable)
{
	if(imgBuf == NULL)
	{
		printf("imgBuf is NULL!!\n");
		return -1;
	}

	int colorTablesize = 0;
	if (biBitCount == 8)
	{
		colorTablesize=1024;
	}

	int lineByte = (width * biBitCount / 8 + 3) / 4 * 4;

	FILE *fp = fopen(bmpName, "wb");
	if (fp == NULL)
	{
		printf("fopen error...\n");
		return -1;
	}

	FileHeader fileHead;
	fileHead.bfType=0x4D42;

	fileHead.bfSize = sizeof(FileHeader) + sizeof(InfoHeader) + colorTablesize + lineByte * height;

	fileHead.bfReserved1 = 0;
	fileHead.bfReserved2 = 0;

	fileHead.bfOffBits = 54 + colorTablesize;

	fwrite(&fileHead, sizeof(FileHeader), 1, fp);

	InfoHeader infoHead;
	infoHead.biBitCount = biBitCount;
	infoHead.biClrImportant = 0;
	infoHead.biClrUsed = 0;
	infoHead.biCompression = 0;
	infoHead.biHeight = height;
	infoHead.biPlanes = 1;
	infoHead.biSize = 40;
	infoHead.biSizeImage = lineByte * height;
	infoHead.biWidth = width;
	infoHead.biXPelsPerMeter = 0;
	infoHead.biYPelsPerMeter = 0;

	fwrite(&infoHead, sizeof(InfoHeader), 1, fp);

	if (biBitCount == 8)
	{
		fwrite(pColorTable,sizeof(RGBQUAD),256,fp);
	}

	fwrite(imgBuf, height * lineByte, 1, fp);

	fclose(fp);

	return 0;
}


/* main function test program*/
#if 1
int main(int argc, char *argv[])
{
	char *readPath = argv[1];
	readBmp(readPath);

	printf("width = %d, height = %d, biBitCount = %d \n", bmpWidth, bmpHeight, biBitCount);

	char writePath[] = "logo256copy.bmp";
	saveBmp(writePath, pBmpBuf, bmpWidth, bmpHeight, biBitCount, pColorTable);

	free(pBmpBuf);
	if (biBitCount == 8)
	{
		free(pColorTable);
	}

	return 0;
}
#endif
