#pragma once

#include "../Engine/console.h"
#include "../Engine/utility.h"

#include "../EngineExtend/GameGraphics2D.h"

using namespace std;

class CBitmapX
{
public:
	CBitmapX();
	virtual ~CBitmapX();

	BOOL LoadBitmap(const char* filename);		// ��ȡһ����ͼ��Ȼ���Զ��и�
	void Draw(int x, int y, DWORD color = 0xffffffff);		// ��ʾ������ͼ
	void Draw(int x, int y, float w, float h);	// ��ʾ������ͼ�����죩
	void Draw(int x, int y, RECT& rect);		// �ֲ���ʾ��ͼ
	void DrawBlend(int x, int y, RECT& rect);		// �ֲ���ʾ��ͼ
	void Draw(int x, int y, float w, float h, RECT& rect, DWORD color = 0xffffffff);	// �ֲ���ʾ��ͼ�����죩
	void DrawRotation(INT x, INT y, FLOAT angle, DWORD color = 0xffffffff);	// ����ת��λͼ
	void DrawRotation(float x,float y ,FLOAT angle,DWORD color = 0xffffffff);
	void DrawRotation2(INT x, INT y, int rx,int ry,FLOAT angle,float fScale, DWORD color = 0xffffffff);//�������ĵ㻭��ת��λͼ

	DWORD GetWidth()	{return m_dwWidth;}
	DWORD GetHeight()	{return m_dwHeight;}
	D3DCOLOR	GetPixel(int x, int y);
	DWORD* GetBitmap()	{return m_pBitmap;}

	int GetFirstTextureHandle();

	void SetClip(long left, long top, long right, long bottom) {m_rectClip.left=left; m_rectClip.top=top; m_rectClip.right=right; m_rectClip.bottom=bottom;}
	void SetClip(RECT *p) {m_rectClip.left=p->left; m_rectClip.top=p->top; m_rectClip.right=p->right; m_rectClip.bottom=p->bottom;}
	RECT *GetClip()	{return &m_rectClip;}
	void SetColor(DWORD color)	{m_color = color;}
	DWORD GetColor()			{return m_color;}
	string	GetFileName()	{return m_filename;}
private:
	enum
	{
		TEX_MINIMUM = 64
	};


	// ��ȡ��ͼ��Ϣ
	BOOL ReadBitmapFile(const char *filename);

	void DivideTexture(RECT); // �ָ�����
	int GetOptionalSize(int); // ��ȡ��Ѵ�С

	DWORD * m_pBitmap; // ͼ�����ݱ��ش�
	list< pair<int,RECT> > m_TexArray; // ��������
	DWORD m_dwMaxWidth, m_dwMaxHeight; // �Կ�֧��
	DWORD m_dwWidth, m_dwHeight; // ԭͼ��С
	DWORD m_dwBitmapSize; // ԭͼ��С
	RECT m_rectOrigin; // ԭͼλ��
	//render::Interface *m_pInterface;
	//render::Interface::Layer2D *m_pLayer2D;
	GameGraphics2D * m_pGameGraphics2D;
	string m_filename;
	int m_nCount;
	
	DWORD m_color;		//�û��Զ�����ɫֵ
	RECT m_rectClip;	// �������� - Ŀǰֻ��DrawԭʼͼƬ��Ч

public:
	void IncRef() { ++m_nCount; }
	void DecRef() 
	{ 
		--m_nCount; 
		if ( m_nCount == 0 ) 
		{ 
			m_map.erase(m_map.find(m_filename)); delete this;
		} 
	}
	static map<string, CBitmapX *>	m_map;
	static CBitmapX * GetBitmapX(string filename,bool bRefresh=false);
	static CBitmapX * GetBmpFromBuf(BYTE *buf,long size);	// �����ڲ�������ڴ� ��Ҫÿ�εó����ֹ��ͷ�
	BOOL LoadJPGFromBuf(BYTE *buf,long size);				// �����ڴ����ֽڶ�ȡһ����ͼ
	static void ReMoveAllBitmap();
};
