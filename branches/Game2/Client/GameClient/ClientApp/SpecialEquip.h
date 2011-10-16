#pragma once

class GameMap;
class AnimInfo;
class CDisplayModel;
class CShape;
class CSpecialEquip
{
public:
	CSpecialEquip();
	virtual ~CSpecialEquip();

	void SetModelID(DWORD wID) { m_dwModelID = wID;}
	void SetType(DWORD dwType) { m_dwType = dwType;}
	void SetLevel(DWORD level);
	DWORD GetLevel(){return m_dwLevel;}
	AnimInfo* GetAnimInfo(){return m_pAnimInfo;}
	virtual void SetSimpleMesh(bool bsimplemesh){m_bsimplemesh = bsimplemesh;}
	//���ð󶨵�λ��
	void SetLocator(DWORD dwLocator){m_dwLocator=dwLocator;}
	DWORD GetLocator(){return m_dwLocator;}

	void SetModelColorCompel(DWORD dwColor);		//ǿ���趨װ����ʾģ����ɫ

	BOOL IsVisible() const { return m_bVisiable;}
	DWORD&  GetState()  {return m_eStatus;}
	CDisplayModel* GetDisplayModel(){return m_pDisplayModel;}
	VOID SetVisible(BOOL bVisible)
	{
		m_bVisiable=bVisible;
	}
	virtual void Copy(CSpecialEquip* psequip);

	virtual void Destroy();
	//������ʾģ��
	virtual void SetDisplayModel();

	//����
	void Hide();

	//��ʾ
	virtual void Display(CShape * pParentObj);																				//��ʾ����
	virtual void DisplayForFlashColor(CDisplayModel * pParentModel, AnimInfo * pParentAnimInfo, DWORD dwFlashColor);		//��ʾװ����������ɫ������Ч��(ǿ�ƽ��ܵƹ⴦��)
	virtual void DisplayForExStateColor(CDisplayModel * pParentModel, AnimInfo * pParentAnimInfo, DWORD dwExColor);			//��ʾװ�����쳣״̬��ɫ������Ч��(ǿ�ƽ��ܵƹ⴦��)

protected:
	DWORD m_dwModelID;	
	DWORD m_dwLevel;
	DWORD m_dwType;
	BOOL m_bVisiable;
	DWORD m_eStatus;
	bool m_bsimplemesh;

	//�󶨵�λ��
	DWORD m_dwLocator;
	AnimInfo *m_pAnimInfo;
	//��ʾģ��
	CDisplayModel *m_pDisplayModel;
};