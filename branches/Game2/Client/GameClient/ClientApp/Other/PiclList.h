#pragma once

class GameModel;
class CShape;

// ģ�͹�����
class CPicList
{
public:
	CPicList(void);
	virtual ~CPicList(void);

	// ģ�ͽṹ
	struct tagModel
	{
		long		lType;			// ����
		long		lGraphicsID;	// ͼƬ���
		DWORD		lUseCount;		// ʹ�õĴ���
		GameModel	*pModel;		// ģ��		
	};

protected:
	list<tagModel*>	m_listModel;					// ģ���б�

public:
	tagModel* Load(long lType, long lGraphicsID, bool bFind = true);		// ����һ����Դ( ������֮ǰ�Ƿ�Ҫ����Ҫ�������ͼ���Ƿ���� )
	BOOL Release(long lType, long lGraphicsID);		// �ͷ�һ����Դ
	BOOL ReleaseAll(void);							// ጷ������YԴ

	// ��ʾ
	BOOL Display(CShape* pShape, float x, float y, int Alpha = 16);

	// ��ȡ
	tagModel* GetModel(long lType, long lGraphicsID);	// ��ȡ

	//test Ӱ��
	DWORD m_dwShadowShaderGroupHandle;
	void  Create();
	void  Destroy();
};
