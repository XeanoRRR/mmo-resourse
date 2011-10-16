/*
 * CImage.h
 * ����: ͼ�������
 * ժҪ: �����ͼ��������Ǵ��ֹ�����,�����κθ������п�
 * ����: lpf
 * ��������: 2009 - 06 - 02
 */

#ifdef TIMETEST
#pragma comment(lib, "winmm.lib")
#endif

#pragma once
#include <windows.h>
#include <stdio.h>
#include <math.h>

#ifndef SAFEDELETE
#define SAFEDELETE(p)			{ if (p)  { delete(p); (p) = NULL; } }								//��ȫɾ��ָ�����
#endif

#ifndef SAFEDELETEARRAY
#define SAFEDELETEARRAY(ap)		{ if (ap)  { delete [] (ap); (ap) = NULL; } }						//��ȫɾ��ָ���������
#endif

#define A2R(fA)		((float)fA * 3.14159265f / 180.0f)			//�Ƕȵ�����



class CImage
{
#pragma pack(1)
	// λͼ�ļ�ͷ
	struct tagBMPFILEHEADER
	{
		WORD  bfType; 
		DWORD bfSize; 
		WORD  bfReserved1; 
		WORD  bfReserved2; 
		DWORD bfOffBits;
	};

	// λͼ��Ϣͷ
	struct tagBMPINFOHEADER
	{
		DWORD biSize;
		LONG  biWidth;
		LONG  biHeight;
		WORD  biPlanes;
		WORD  biBitCount;
		DWORD biCompression;
		DWORD biSizeImage;
		LONG  biXPelsPerMeter;
		LONG  biYPelsPerMeter;
		DWORD biClrUsed;
		DWORD biClrImportant;
	}; 
#pragma pack()

	char * m_pszImageFileName;	//ͼ����Դ�ļ�(������ڴ�������Ϊ��)

	DWORD * m_pBitmapOri;		//λͼ����
	DWORD * m_pBitmapBuffer;	//λͼ���󻺳�����
	DWORD   m_dwBitmapSize;		//λͼ�ߴ�

	DWORD m_dwWidth;			//���
	DWORD m_dwHeight;			//�߶�

	// �����������ʱ�ļ����
	const BYTE * m_pTmpFileData;
	unsigned int m_uiDataPoint;
	unsigned int m_uiDataSize;

public:
	CImage();			//���캯��
	~CImage();			//��������

	// �������Ա����
	DWORD GetPixel(long lX, long lY);						//�õ�����
	DWORD GetBufferPixel(long lX, long lY);					//�õ���������
	DWORD GetBufferPixel(DWORD * pBitmapBuffer, long lX, long lY);					//�õ���������
	float GetAlphaValue(DWORD dwColor);						//�õ�������ɫ��ALPHAֵ
	DWORD GetWidth()			{ return m_dwWidth; }		//�õ�ͼ����
	DWORD GetHeight()			{ return m_dwHeight; }		//�õ�ͼ��߶�

	// �趨���Ա����
	void SetPixel(long lX, long lY, DWORD dwPixelValue);		//�趨����
	void SetBufferPixel(long lX, long lY, DWORD dwPixelValue);	//�趨��������
	void SetAlphaMap(CImage * pImage, long lPosX, long lPosY);	//�趨Alphaͨ��ͼ

	// �ļ���������
	bool ReadBitmapFromFile(const char * pszFileName);									//���ļ���ȡBMP�ļ�
	bool ReadBitmapFromMemory(const BYTE * pData, unsigned int uiDataLen);				//���ڴ��ȡBMP
	bool ReadFileToMemory(FILE * pFile, BYTE *& pData, DWORD & dwSize);					//���ļ����ݶ����ڴ���
	bool ExportBitmapToFile(const char * pszFileName, WORD wBit = 32);					//����BMP�ļ�
	bool ExportJpegToFile(const char * pszFileName, int iQuality = 80);					//����Jpeg�ļ�
	bool ExportJpegToMemory(BYTE *& pData, DWORD & dwSize, int iQuality = 80);			//����Jpeg���ڴ�
	void ClearData();																	//���ͼ������

	// ͼ�����
	void AlphaBlend(CImage * pImage, long lPosX, long lPosY, float fAlphaBlendValue);	//��ǰͼ������һ��ͼ�����ALPHA���
	void Rotate(float fRotAngle);														//��תͼ��
	void Rotate(float fRotAngle, long lPosX, long lPosY);								//ָ����ת�����������תͼ��
	void RotateEx(float fRotAngle);														//��תͼ��
	void RotateEx(float fRotAngle, long lPosX, long lPosY);								//ָ����ת�����������תͼ��
	void Zoom(int iScale);																//����ͼ��
	void Blur();																		//ģ��ͼ��

private:
	bool  CreateBitmap(DWORD dwWidth, DWORD dwHeight, DWORD dwColor);					//����λͼ
	void  FlipVertical(DWORD * szpBitmap);												//��תλͼ
	DWORD AveragePixel1x1(DWORD * pBitmapBuffer, long lX, long lY);						//����ָ�����ص���Χ���ؾ�ֵ
	DWORD AveragePixel3x3(DWORD * pBitmapBuffer, long lX, long lY);						//����ָ�����ص���Χ���ؾ�ֵ
	DWORD AveragePixelEx(DWORD * pBitmapBuffer, long lX, long lY, float fCosA, float fSinA);		//����ָ�����ص���Χ���ؾ�ֵ

	// ���ݶ�ȡ����
	bool ReadDataBegin(const BYTE * pFileData, unsigned int uiDataSize);				//��ʼ��ȡ����
	bool ReadData(void * pBuf, unsigned long ulReadSize);								//��ȡ����
	void ReadDataEnd();																	//��ȡ���ݽ���
};