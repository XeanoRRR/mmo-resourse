#pragma once

//#include <feffector.h>
#include  <..\..\..\client\gameclient\gameextend\gamemodel.h>
//#include <gamemodel.h>
class GameModel;
class CShape;

// ģ�͹�����
class CModelList
{
public:
	CModelList(void);
	virtual ~CModelList(void);

	// ģ�ͽṹ
	struct tagModel
	{
		DWORD		lUseCount;		// ʹ�õĴ���
		GameModel	*pModel;		// ģ��o		
	};

protected:
	struct tagModelKey
	{
		long		lType;			// ����
		long		lGraphicsID;	// ͼƬ���
	};

	class hash_model_compare:public hash_compare<tagModelKey>
	{
	public:
		//����ɢ��ֵ
		size_t operator( )( const tagModelKey& Key ) const
		{
			return Key.lType;
		}
		//less operator
		bool operator( )( const tagModelKey& _Key1,const tagModelKey& _Key2 ) const
		{
			if(_Key1.lType < _Key2.lType)	return true;
			else if(_Key1.lType == _Key2.lType && _Key1.lGraphicsID < _Key2.lGraphicsID)	return true;
			return false;
		}
	};

	typedef hash_map<tagModelKey,tagModel*,hash_model_compare>	HMModels;
	typedef hash_map<tagModelKey,tagModel*,hash_model_compare>::iterator itModel;

	HMModels	m_Models;	 				// ģ���б�

public:
	tagModel* Load(long lType, long lGraphicsID, bool bFind = true);		// ����һ����Դ( ������֮ǰ�Ƿ�Ҫ����Ҫ�������ͼ���Ƿ���� )
	BOOL Release(long lType, long lGraphicsID);		// �ͷ�һ����Դ
	BOOL ReleaseAll(void);							// ጷ������YԴ

	// ��ʾ
	void Display(CShape* pShape, float x, float y,float fHeight,render::Camera * pCamera);
	
	//Add by ��ǫ
	void DisplaySpecialEquipment(CShape* pShape,render::Camera * pCamera);
	void UpdateSpecialEquipment(CShape* pShape);
	//Add by ��ǫ
	
	void DisplayAsShadow(CShape *pShape, float x, float y,float fHeight,const D3DXMATRIX *pBillboardMatrix,const D3DXMATRIX * pBillboardMatrixY,DWORD dwColor);

	// ��ȡ
	tagModel* GetModel(long lType, long lGraphicsID);	// ��ȡ

	// ����ģ��
	BOOL SetupAnimInfo(GameModel* pGameModel, CShape* pShape, float x, float y,float fHeight,const D3DXMATRIX *pBillboardMatrix,const D3DXMATRIX * pBillboardMatrixY);

	// ��ȡ��λ������
	BOOL GetLocatorPos(CShape* pShape, DWORD dwLocatorName, const D3DXMATRIX *pBillboardMatrix,const D3DXMATRIX * pBillboardMatrixY, float &x, float &y, float &fHeight);

	// ��ʾGOODS��SKILL��ͼ��
	void DisplayIcon(long lType, long lGraphicsID, int iLevel,int iDrawX, int iDrawY,float fRotation, bool bAction,GameModel::AnimInfo *pAnimInfo,float fScale = 100.0f);
	void DisplayIcon(CShape *pShape,int iDrawX, int iDrawY, float fRotation);

	void SetupEquipment(GameModel* pGameModel, CShape * pShape);

	BOOL IsFinished(CShape* pShape);		// �Ƿ���ʾ���һ��ѭ��
private:
	void SetLevel(GameModel* pGameModel,DWORD dwGroup, DWORD dwPart, int iLevel);
};

