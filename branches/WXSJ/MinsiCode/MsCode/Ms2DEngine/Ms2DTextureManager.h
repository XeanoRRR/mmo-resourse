#pragma once                    //��    //��
#ifndef __MS2DTEXTUREMANAGER_H__//��    //��
#define __MS2DTEXTUREMANAGER_H__//��    //��
//����������������������������������    //��
//�ļ����ƣ�Ms2DTextureManager.h        //��
//����������2D�����������������        //��
//ƽ̨��أ�Windows                     //��
//������������������������������������������
//���������

// �Զ��嶥������
#define D3DFVF_CUSTOMVERTEX (D3DFVF_XYZRHW | D3DFVF_DIFFUSE | D3DFVF_TEX1)

// �������ζ������
#define RECTANGLE_VERTEX_COUNT  4

// �Զ��嶨������
struct CUSTOMVERTEX
{
    FLOAT x, y, z;  // ����
    FLOAT rhw;
    DWORD color;    // ��ɫ
    FLOAT tu, tv;   // ��ͼUV
};

class CMs2DTexture
{
public:
    CMs2DTexture(TCHAR* szFileName);
    CMs2DTexture(HMODULE hSrcModule, INT nResId);
    ~CMs2DTexture(VOID);

    // ����������
    VOID CreateBuff(VOID);

    // ��Ⱦ������ͼ
    VOID RenderTexture(LPDIRECT3DDEVICE9 lpDirectDevice9);

    // ����ͼƬλ��
    VOID SetRect(RECT& DesRc, RECT& SrcRc);

    // ������Ⱦ״̬
    VOID UpdataRenderState(LPDIRECT3DDEVICE9 lpDirectDevice9);

    // ���¶��㻺����
    VOID UpdataVB(VOID);
protected:
    LPDIRECT3DBASETEXTURE9  m_lpTexture;
    D3DXIMAGE_INFO          m_Imageinfo;
    DWORD                   m_dwCubeMapFlags;

    LPDIRECT3DVERTEXBUFFER9 m_pVB;
    LPVOID                  m_lpVBuff;
    CUSTOMVERTEX            m_Vertex[RECTANGLE_VERTEX_COUNT];

    LPDIRECT3DINDEXBUFFER9  m_pIB;
    LPVOID                  m_lpIBuff;
    DWORD                   m_dwIndexed[RECTANGLE_VERTEX_COUNT];
};
typedef CMs2DTexture* LPCMs2DTexture;

//����������������������
#endif//#pragma once  ��
//����������������������
