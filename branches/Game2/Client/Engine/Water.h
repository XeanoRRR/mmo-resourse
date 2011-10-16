/*
* Water.h
* ����: ˮ����Ⱦģ��
* ժҪ: ��Ҫ���ڴ�����÷����Ŷ�ģʽ�µ��µ�ˮ��Ч��
* ����: ������lpf
* ��������: 2009-09-02
*/
#pragma once
#include "singleton.h"

#define WATERVERTEX_MAX 500

namespace Water
{
    class CWater;

    enum eType
    {
        WaterType_Rect,			//����
        WaterType_Round,		//Բ��
    };

    // ��ȾԪ�ؽṹ��
    struct _tagRenderElement
    {
        D3DXVECTOR3 vPos;					//����
        DWORD		dwElapseTime;			//����ʱ��

        CWater * pWater;		//ָ��ԭʼ����,ֻ����ָ��
    };

    // ��������
    class CManager
    {
        DECLARE_SINGLETON(CManager);

        std::list<CWater *>			  m_listAllWaters;					//����ˮ���б�
        std::list<CWater *>::iterator m_itWater;						//������
        std::list<_tagRenderElement>			m_listRenderWaters;		//��Ⱦ�б�
        std::list<_tagRenderElement>::iterator  m_itRenderWaters;		//������

        void BeginRender(BOOL bFogEnable);		//��ʼˮ����Ⱦ
        void EndRender();						//����ˮ����Ⱦ

    public:
        static bool s_bIsRenderWater;			//ˮ����Ⱦ����

        void Destroy();							//����

        long GetNumAllWater()			{ return (long)m_listAllWaters.size(); }			//�õ�����ˮ������
        long GetNumRenderWater()		{ return (long)m_listRenderWaters.size(); }			//�õ���Ⱦˮ������

        void AddWater(CWater * pWater);							//���ˮ��
        void AddRenderWater(_tagRenderElement & Element);		//�����Ⱦˮ��
        void RemoveWater(CWater * pWater);						//�Ƴ�ˮ��

        void Render(BOOL bFogEnable);							//��Ⱦ
    };

    // ˮ����(�൱��GameModel,ͬ���Ķ���ֻ����һ��)
    class CWater : public LoadState
    {
        friend class CManager;

        long m_lID;								//ID
        char m_szWaterFileName[MAX_PATH];		//ˮ�������ļ�����
        char m_szShaderFileName[MAX_PATH];		//��ɫ���ļ�����

    public:
        eType	 m_eType;		//����
        long	 m_lNumVertex;	//��������
        float	 m_fRadius;		//�뾶
        float	 m_fWidth;		//���
        float	 m_fLength;		//����
        D3DCOLOR * m_dwpColor;	//��ɫ

        // ������������
        float m_fNorSpeedX;			//�������������ٶ�X�᷽��
        float m_fNorSpeedY;			//�������������ٶ�Y�᷽��
        float m_fNormat00;			//��������������
        float m_fNormat01;			//��������������
        float m_fNormat10;			//��������������
        float m_fNormat11;			//��������������

        // ������������
        float m_fEnvSpeedX;			//�������������ٶ�X�᷽��
        float m_fEnvSpeedY;			//�������������ٶ�Y�᷽��

    private:
        DWORD m_dwShaderGroupHandle;			//��ɫ��������

        void Render(_tagRenderElement & Element);		//��Ⱦˮ��

    public:
        CWater();			//���캯��
        ~CWater();			//��������

        const char * GetWaterFileName()						{ return m_szWaterFileName; }				//�õ�ˮ����������ļ���
        const char * GetShaderFileName()					{ return m_szShaderFileName; }				//�õ���ɫ���������ļ���
        DWORD		 GetShaderGroupHandle()					{ return m_dwShaderGroupHandle; }			//�õ���ɫ��������

        void SetWaterFileName(const char * pszFileName)		{ strcpy_s(m_szWaterFileName, MAX_PATH, pszFileName); }			//�趨ˮ����������ļ���
        void SetShaderFileName(const char * pszFileName)	{ strcpy_s(m_szShaderFileName, MAX_PATH, pszFileName); }		//�趨��ɫ���������ļ���

        bool Create(CRFile * pFile);												//����ˮ��
        bool CreateShaderGroup(CRFile * pFile, DWORD dwTextureProcision = 0);		//������ɫ������
    };
}