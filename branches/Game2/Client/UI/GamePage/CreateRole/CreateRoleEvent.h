#pragma once

#include "../../PageEvent.h"
class CPlayer;
class GameScene;
class CREvent:public PageEvent
{
public:
    CREvent();
    CREvent(EVENTTYPE type);
    ~CREvent();
    virtual void OnPageLoad(GamePage *pPage);
    virtual void OnPageOpen(GamePage *pPage);
    virtual void OnPageClose(GamePage *pPage);
    virtual void OnPageUpdate(GamePage *pPage);
    virtual void OnPageRelease(GamePage *pPage);
	//��ʾģ��
	void Display();

    bool OnCreateRoleBtn(const CEGUI::EventArgs &e);    //������ɫ��ť�߼�
    bool GoBackBtn(const CEGUI::EventArgs &e);          //�ص���ҳ
    static void SetRandCountryRange(BYTE lRange);       //�趨���������Χ
	static void SetRandCountry( BYTE nCountry ){ m_nRandCountry=nCountry; }		//�����������
	static void SetSelectSex(short chSex);                              // ���ý�ɫ�Ա�
	void SetConstellation(bool bLeft);			//�趨����
	static void SetFace(DWORD dwIndex);         //�趨����
	static void SetHairStyle(DWORD dwIndex);    //�趨����
	static void SetHairColor(DWORD dwIndex);    //�趨��ɫ 
	static void SetSelectCountry(BYTE country){nCountry = country;}	            // ���ñ�ѡ�еĹ���

	static short GetSelectSex(){return m_chSex;}					    // ��ý�ɫ�Ա�
	static short GetHairColorNum(WORD wSexIndex,WORD wHairIndex);   //��ȡ����ķ�ɫ��������0��Ů1��
	//��ȡ������沿������������0��Ů1��
	static short GetFaceNum(WORD wSexIndex);
	static short GetHairNum(WORD wSexIndex);
	static BYTE GetConstellation()	{ return m_nConstellation;}		                        //�õ���ǰ�û�ѡ���������Ϣ     
	static BYTE GetHair()			{ return (m_byHairStyle << 4) | m_byHairColor; }		//�õ���ǰ�û�ѡ���ͷ����Ϣ
	static BYTE GetFace()			{ return m_byFace; }										//�õ���ǰ�û�ѡ���������Ϣ

	bool          m_bRoleLeftRotate;    //������ת
	bool          m_bRoleRightRotate;   //������ת

private:
    static short m_wFaceNum[2];		// ������沿������������0��Ů1��
    static short m_wHairNum[2];		// ����ķ�����������0��Ů1��
    static short nSelected;         //ѡ��Player����
    static BYTE nCountry;          //����ID
    static short m_chSex;           //�Ա�
    static BYTE  m_byHairStyle;     //����
    static BYTE  m_byHairColor;     //��ɫ
    static BYTE  m_byFace;           //����
    static BYTE  m_nConstellation;  //����
    static CPlayer *m_pPlayer;      //�����������ɫ
    static BYTE  m_nRandCountry;    //�漴����ID

    static map<WORD,short> m_mapHairColorNum[2];//����ķ�ɫ��������0��Ů1��
    static vector<BYTE> m_vecRandCountryRange;//���������Χ

    static bool LoadFaceHairIni();			// ��ȡ�沿������ͷ������ɫ�������ļ�
 
    static bool CheckName(const char* strName);					                //������������Ƿ�������������
    static short GetSelectedNum(){return nSelected;}					        // ���Ҫ�������Ľ�ɫ
    static BYTE GetSelectCountry(){return nCountry;}					        // ���ѡ�еĹ���
    static BYTE GetRandCountry(){return m_nRandCountry;}				        // �������Ĺ���
    

    static void RandomChoseDetails();           //�������ϸ��
    static void RandomChoseCountry();			//���ѡ�����

	static float  s_RolePos[4];		// վ��λ��
	static int    m_CurrentAction;		// ��ǰ����
	static int    m_NextAction;           // ��һ������
	GameScene*	  m_SelectSence;   
};
