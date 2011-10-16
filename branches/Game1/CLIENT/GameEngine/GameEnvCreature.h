#pragma once
#include "..\frostengine\utility.h"
#include "gamemodel.h"
#include "DisplayModel.h"

class GameMap;

//�����ڵ�ͼ��
class GameBaseEnvCreature
{
protected:
	enum eEnvCreatureAlphaFade
	{
		EC_Fade_Out	 = -1,			//����״̬
		EC_Fade_None = 0,			//û�н���Ч��
		EC_Fade_In	 = 1,			//����״̬
	};

	CDisplayModel * m_pModel;
	render::BoundingBox m_RegionBox;

	float m_fScale;
	float m_fRadius;
	DWORD m_dwAlphaFadeTimeLength;

	BOOL m_bIsDisplay;
	BOOL m_bFollowPlayer;		//�Ƿ������ҳ���
	D3DXVECTOR3 m_vBirthPos;	//�̶�����������

public:
	GameBaseEnvCreature();
	virtual ~GameBaseEnvCreature() {}

	virtual void Update(const D3DXVECTOR3 * pPosition, render::Frustum * pFrustum) {};
	virtual void Render(const GameMap * pMap, render::Camera * pCamera) {};
	virtual BOOL Create(utility::Text::Node * pNode, char * szResourcePath) { return TRUE; };
	virtual void Destroy() {};

	static GameBaseEnvCreature * CreateEnvCreature(const char * szFileName);
	static FLOAT FieldRandom(FLOAT fStart, FLOAT fEnd);			//ȡ�ø����������
	static DWORD FieldRandom(DWORD dwStart, DWORD dwEnd);		//ȡ�����������
};



class GameEnvCreature_Butterfly : public GameBaseEnvCreature
{
	DWORD m_dwMinLifeTime;
	DWORD m_dwMaxLifeTime;
	DWORD m_dwLastTime;

	float m_fMinSpeed;
	float m_fMaxSpeed;

	float m_fMinAngle;
	float m_fMaxAngle;

	struct tagButterfly
	{
		int	  iAlphaMode;				//����Ϊ����������Ϊ������0Ϊ����ģʽ
		float fSpeed;
		float fRotationY;
		float fCurrentAlphaValue;
		DWORD dwLifeTime;
		DWORD dwAlphaFadeStartTime;

		D3DXVECTOR3	  vPosition;
		AnimInfo	* pAnimInfo;
	};

	tagButterfly * m_pButterflyArray;
	DWORD		   m_dwButterflyCount;

public:
	GameEnvCreature_Butterfly();
	~GameEnvCreature_Butterfly();

	BOOL Create(utility::Text::Node * pNode, char * szResourcePath);		//����������������
	void Destroy();															//���ٺ�����Դ
	void Update(const D3DXVECTOR3 * pPosition, render::Frustum * pFrustum);
	void AI(tagButterfly & tButterfly, DWORD dwTimeElapse);					//����AI
	void Render(const GameMap * pMap, render::Camera * pCamera);
};



class GameEnvCreature_Crow : public GameBaseEnvCreature
{
	DWORD m_dwMinLifeTime;
	DWORD m_dwMaxLifeTime;

	DWORD m_dwLastTime;

	float m_fMinRadomSpeed;
	float m_fMaxRadomSpeed;

	DWORD m_dwMinTurnForceTime;			//���ת�����ȱ仯����
	DWORD m_dwMaxTurnForceTime;			//���ת�����ȱ仯����
	float m_fMinTurnForce;				//��Сת������
	float m_fMaxTurnForce;				//���ת������

	DWORD m_dwMinTurnDirectionTime;		//����仯ʱ��
	DWORD m_dwMaxTurnDirectionTime;

	BOOL m_bLighted;					//��

	struct tagCrow
	{
		int	  iAlphaMode;				//����Ϊ����������Ϊ������0Ϊ����ģʽ
		float fSpeed;
		float fRotationY;
		float fTurnForce;
		float fTurnDirection;
		float fCurrentAlphaValue;
		DWORD dwLifeTime;
		DWORD dwTurnForceLastTime;
		DWORD dwLastTurnDirectionTime;
		DWORD dwAlphaFadeStartTime;
		//DWORD dwModelLevel;

		D3DXVECTOR3   vPosition;
		AnimInfo	* pAnimInfo;
	};	

	tagCrow * m_pCrowArray;
	DWORD	  m_dwCrowCount;

public:
	GameEnvCreature_Crow();
	~GameEnvCreature_Crow();

	BOOL Create(utility::Text::Node * pNode, char * szResourcePath);
	void Destroy();
	void Update(const D3DXVECTOR3 * pPosition, render::Frustum * pFrustum);
	void AI(const D3DXVECTOR3 * pPosition, tagCrow * pCrow, DWORD dwCurrentTime, DWORD dwTimeElapse);
	void Render(const GameMap * pMap, render::Camera * pCamera);

	float CalculateDirection(const D3DXVECTOR3 * pDst, const D3DXVECTOR3 * pSrc);
};



class GameEnvCreature_Beetle : public GameBaseEnvCreature
{
	D3DXVECTOR3 m_vCenter;

	DWORD m_dwMinLifeTime;
	DWORD m_dwMaxLifeTime;

	DWORD m_dwLastTime;

	float m_fMinRadomSpeed;
	float m_fMaxRadomSpeed;

	DWORD m_dwMinTurnForceTime;			//���ת�����ȱ仯����
	DWORD m_dwMaxTurnForceTime;			//���ת�����ȱ仯����
	float m_fMinTurnForce;				//ת������
	float m_fMaxTurnForce;

	DWORD m_dwMinTurnDirectionTime;
	DWORD m_dwMaxTurnDirectionTime;

	// ���ĸı�ʱ��
	DWORD m_dwMinCenterChangeTime;
	DWORD m_dwMaxCenterChangeTime;
	DWORD m_dwLastCenterChangeTime;
	float m_fRandomCenterRadius;

	BOOL m_bLighted;			//��

	struct tagBeetle
	{
		int	  iAlphaMode;				//����Ϊ����������Ϊ������0Ϊ����ģʽ
		float fSpeed;
		float fRotationY;
		float fTurnForce;
		float fTurnDirection;
		float fCurrentAlphaValue;
		DWORD dwLifeTime;
		DWORD dwTurnForceLastTime;
		DWORD dwTurnDirectionLastTime;
		DWORD dwModelLevel;
		DWORD dwAlphaFadeStartTime;

		D3DXVECTOR3   vPosition;
		AnimInfo	* pAnimInfo;
	};	

	tagBeetle * m_pBeetleArray;
	DWORD		m_dwBeetleCount;

public:
	GameEnvCreature_Beetle();
	~GameEnvCreature_Beetle();

	BOOL Create(utility::Text::Node * pNode, char * szResourcePath);
	void Destroy();
	void Update(const D3DXVECTOR3 * pPosition, render::Frustum * pFrustum);
	void AI(tagBeetle * pBeetle, DWORD dwCurrentTime, DWORD dwTimeElapse);
	void Render(const GameMap * pMap, render::Camera * pCamera);

	float CalculationDirection(const D3DXVECTOR3 * pDest,const D3DXVECTOR3 * pSrc);
};



class GameEnvCreature_Rabbit : public GameBaseEnvCreature
{
	enum RABBIT_STATUS
	{
		RS_STAND = 0,
		RS_IDLE,
		RS_WALK,
	};

	DWORD m_dwMinLifeTime;
	DWORD m_dwMaxLifeTime;

	DWORD m_dwLastTime;

	float m_fMinRadomSpeed;
	float m_fMaxRadomSpeed;

	DWORD m_dwMinTurnForceTime;			//���ת�����ȱ仯����
	DWORD m_dwMaxTurnForceTime;			//���ת�����ȱ仯����
	float m_fMinTurnForce;				//ת������
	float m_fMaxTurnForce;

	DWORD m_dwMinTurnDirectionTime;
	DWORD m_dwMaxTurnDirectionTime;

	DWORD m_dwMinStandInterval;
	DWORD m_dwMaxStandInterval;

	DWORD m_dwMinIdleInterval;
	DWORD m_dwMaxIdleInterval;

	DWORD m_dwMinWalkInterval; 
	DWORD m_dwMaxWalkInterval;

	BOOL  m_bLighted;				//��

	struct tagRabbit
	{
		int	  iAlphaMode;				//����Ϊ����������Ϊ������0Ϊ����ģʽ
		float fSpeed;
		float fRotationY;
		float fTurnForce;
		float fTurnDirection;
		float fCurrentAlphaValue;
		DWORD dwLifeTime;
		DWORD dwTurnForceLastTime;
		DWORD dwTurnDirectionLastTime;
		DWORD dwLastAI_Time;
		DWORD dwAlphaFadeStartTime;

		D3DXVECTOR3		vPosition;
		RABBIT_STATUS	eStatus;
		AnimInfo	  * pAnimInfo;
	};	

	tagRabbit * m_pRabbitArray;
	DWORD		m_dwRabbitCount;

public:
	GameEnvCreature_Rabbit();
	~GameEnvCreature_Rabbit();

	BOOL Create(utility::Text::Node * pNode, char * szResourcePath);
	void Destroy();
	void Update(const D3DXVECTOR3 * pPosition, render::Frustum * pFrustum);
	void AI(tagRabbit * pRabbit, DWORD dwCurrentTime, DWORD dwTimeElapse);
	void Render(const GameMap * pMap, render::Camera * pCamera);
	void ChangeStatus(tagRabbit * pRabbit, RABBIT_STATUS eStatus, DWORD dwCurTime);
};



class GameEnvCreature_Meteor : public GameBaseEnvCreature
{
	float m_fHeight;

	BOOL  m_bLighted;				//�Ƿ��ܹ���Ӱ��
	DWORD m_dwLiveTimeMax;			//���ǵ������ʱ��

	D3DXVECTOR3 m_vSpeedMin;		//�����ٶ�ʸ������
	D3DXVECTOR3 m_vSpeedMax;		//�����ٶ�ʸ������

	struct tagMeteor
	{
		int	  iAlphaMode;			//����Ϊ����������Ϊ������0Ϊ����ģʽ
		float fCurrentAlphaValue;
		DWORD dwStartTime;			//���ǵĳ���ʱ��
		DWORD dwLiveTimeMax;		//���ǵ������ʱ��
		DWORD dwAlphaFadeStartTime;

		D3DXVECTOR3 vPosition;
		D3DXVECTOR3 vSpeed;			//���ǵ��ٶ�ʸ��

		AnimInfo * pAnimInfo;
	};	

	tagMeteor * m_pMeteorArray;
	DWORD		m_dwMeteorCount;

public:
	GameEnvCreature_Meteor();
	~GameEnvCreature_Meteor();

	BOOL Create(utility::Text::Node * pNode, char * szResourcePath);		//��������
	void Destroy();															//��������
	void Update(const D3DXVECTOR3 * pPosition, render::Frustum * pFrustum);	//��������
	void AI(tagMeteor * pMeteor);											//����AI
	void Render(const GameMap * pMap, render::Camera * pCamera);			//��Ⱦ����
};