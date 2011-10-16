/********************************************************************
created:	2010/09/13
created:	13:9:2010   16:15
filename: 	e:\SVN_P6\trunk\Client\GameClient\ClientApp\Skills\SkillListIni.h
file path:	e:\SVN_P6\trunk\Client\GameClient\ClientApp\Skills
file base:	SkillListIni
file ext:	h
author:		zhaohang(kaba) (kuailekaba@gmail.com)

purpose:	�µļ������ʵ��
*********************************************************************/
#include "../../../../Public/Common/SkillRelated.h"
class tagSkillBaseAttr;
class tagEffectAttr;
class tagStpAttrSound;
class tagStpAttr;
class tagIntonateStp;
class tagRelEffectAttr;
class tagFlyEffectAttr;
class tagReleaseStp;
class tagEndStp;
class tagSkillLevelAttr;
class tagSkillAttr;
class SkillAttrIniSetup;

/**���ܳ�ʼ����׶λ�������*/
class tagSkillBaseAttr{
	///����ID
	uint mID;
	///��������
	char mName[256];
	///�������� 0.���� 1.˲�� 2.����
	uint mSkillType;
	///�Ƿ�����Ŀ�� 0.�� 1.��
	bool mbLockTarget;
	///�Ƿ��������� 0.�� 1.��
	bool mbLockSkill;
public:
	///���캯��
	tagSkillBaseAttr();
	///��������
	~tagSkillBaseAttr();

	///��ȡ����ID
	uint GetID();
	///���ü���ID
	void SetID(uint id);
	///��ȡ��������
	const char* GetName();
	///���ü�������
	void SetName(const char* name);
	///��ü������� 0.���� 1.˲�� 2.����
	uint GetSkillType();
	///���ü������� 0.���� 1.˲�� 2.����
	void SetSkillType(uint);
	///�Ƿ�����Ŀ��
	bool IsLockTarget();
	///�����Ƿ�����Ŀ��
	void SetLockTarget(bool bLockTarget);
	///�Ƿ���������
	bool IsLockSkill();
	///�����Ƿ���������
	void SetLockSkill(bool bLockSkill);
};

/**һ����Ч�Ļ�������*/
class tagEffectAttr{
private:
	///��ЧID
	uint mID;
	///��Ч����
	uint mIndex;
	///��Ч��λ��
	uint mBindPos;
	///��Ч�Ƿ�ѭ������
	bool mCycle;
	///��ʼ����
	bool mPlay;
public:
	tagEffectAttr();
	~tagEffectAttr();
	///������ЧID
	void SetID(uint id);
	///��ȡ��ЧID
	uint GetID();
	///��������
	void SetIndex(uint index);
	///��ȡ����
	uint GetIndex();
	void SetBindPos(uint pos);
	uint GetBindPos();
	void SetCycle(bool b);
	bool IsCycle();
	void SetPlay(bool b);
	bool IsPlay();
};
/**һ���׶λ������������Ч����*/
class tagStpAttrSound{
private:
	///��ЧID -1��ʶû����Ч
	uint mSoundID;
	///��Ч�Ƿ�ѭ������
	bool mSoundCycle;
	///��ʼ������Ч
	bool mSoundPlay;
public:
	tagStpAttrSound();
	~tagStpAttrSound();
	void SetSoundID(uint id);
	uint GetSoundID();
	void SetSoundCycle(bool b);
	bool IsSoundCycle();
	void SetSoundPlay(bool b);
	bool IsSoundPlay();
};

/**һ���׶εĻ�������*/
class tagStpAttr{
private:
	///����Id -1��ʶû�ж���
	uint mActID;
public:
	tagStpAttr();
	~tagStpAttr();
	void SetActID(uint id);
	uint GetActID();
	//��Ч����
	tagStpAttrSound mSoundAttr;
};

/**�����׶�����StepBegin*/
class tagIntonateStp{
private:
	///��Ҫѭ������ID -1��ʶû����Ҫѭ������ID
	uint mMainActID;
	///�Ƿ�����ж�
	bool mBreak;
public:
	///�����׶λ�������
	tagStpAttr mBaseAttr;
	///������Ч
	std::vector<tagEffectAttr>mVecEffect;
	tagIntonateStp();
	~tagIntonateStp();
	void SetMainActID(uint id);
	uint GetMainActID();
	void SetBreak(bool b);
	bool IsBreak();
};

/***�ͷ���Ч����*/
class tagRelEffectAttr{
private:
	///�ͷ�Ŀ�� -1��ʶû��Ŀ��
	uint mDest;
public:
	///��Ч��������
	tagEffectAttr mEffect;

	tagRelEffectAttr();
	~tagRelEffectAttr();

	void SetDest(uint dest);
	uint GetDest();

};

/**������Ч����*/
class tagFlyEffectAttr{
private:
	///X��ת��Ƕ�
	int mTurnX;
	///Y��ת��Ƕ�
	int mTurnY;
	///��������ʱ��
	uint mActTime;
	///������Ч�ٶ�
	uint mSpeed;
	///������Ч�ӳ�ʱ��
	uint mDelayTime;

	///��Ч��������һ�����ԣ���Ч�Ӻδ�����,-1��ʶ��
	uint mSoundPos;

public:
	///������Ч���ֻ�������
	tagEffectAttr mBaseAttr;
	///������Ч��Ч����
	tagStpAttrSound mSoundAttr;
	tagFlyEffectAttr();
	~tagFlyEffectAttr();

	void SetTurnX(int x);
	int GetTurnX();
	void SetTurnY(int y);
	int GetTurnY();

	void SetActTime(uint time);
	uint GetActTime();

	void SetSpeed(uint speed);
	uint GetSpeed();

	void SetDelayTime(uint delay);
	uint GetDelayTime();

	void SetSoundPos(uint pos);
	uint GetSoundPos();

};
/**�ͷŽ׶�����StepRun*/
class tagReleaseStp{
private:
	///�ͷ������ͷ���ЧĿ��
	uint mSoundDest;
	///�ͷ������ͷŶ���ʱ��,-1��ʶû�д���
	uint mActTime;
	///�Ƿ��з��н׶�
	bool mbFly;
public:
	///�ͷŽ׶λ�������
	tagStpAttr mBaseAttr;
	///�ͷ���Ч
	std::vector<tagRelEffectAttr> mVecEffect;
	///������Ч
	tagFlyEffectAttr mFlyEffect;
	void SetHasFly(bool);
	bool IsHasFly();

	tagReleaseStp();
	~tagReleaseStp();

	void SetSoundDest(uint dest);
	uint GetSoundDest();

	void SetActTime(uint t);
	uint GetActTime();
};

/**�����׶�����StepEnd*/
class tagEndStp{
private:
	///������ЧID -1��ʶû��
	uint mHitEffectID;
	///������Ч -1��ʶû��
	uint mHitSoundID;
	///�Ƿ�������Ч��
	bool mFlashEffect;
	///��������
	uint mFlashEffectLum;
	///����Ч������ʱ��
	uint mFlashEffectTime;
	///��Ч��
	bool mShakeEffect;
	///�𶯷���ֵ
	uint mShakeEffectExtent;
	///��Ч������ʱ��
	uint mShakeEffectTime;
public:
	tagEndStp();
	~tagEndStp();
	
	void SetHitEffID(uint id);
	uint GetHitEffID();
	
	uint GetHitSoundID();
	void SetHitSoundID(uint id);
	
	void SetFlashEffect(bool b);
	bool IsFlashEffect();

	void SetFlashEffLum(uint num);
	uint GetFlashEffLum();

	void SetFlashEffTime(uint t);
	uint GetFlashEffTime();

	void SetShakeEffect(bool b);
	bool IsShakeEffect();

	void SetShakeEffExtent(uint num);
	uint GetShakeEffExtent();
	
	void SetShakeEffTime(uint t);
	uint GetShakeEffTime();
};

/**�ٻ���Ч�׶�StepSummon*/
class tagSummonStp{
private:
	///��Чid -1��ʶû��
	uint mID;
	///��ЧID -1��ʶû��
	uint mSoundID; 
	///�ٻ���������
	uint mLifeTime;
	///X�᷶Χ
	uint mRangeX;
	///Y�᷶Χ
	uint mRangeY;
	///����ٻ������ٻ�������
	uint mRandomNum;
	///����ٻ������ٻ����ӳ�ʱ��
	uint mRandomDelayTime;
public:
	tagSummonStp();
	~tagSummonStp();

	void SetID(uint id);
	uint GetID();

	void SetSoundID(uint id);
	uint GetSoundID();

	void SetLifeTime(uint t);
	uint GetLifeTime();

	void SetRangeX(uint x);
	uint GetRangeX();

	void SetRangeY(uint y);
	uint GetRangeY();

	void SetRandomNum(uint num);
	uint GetRandomNum();

	void SetRandomDelayTime(uint t);
	uint GetRandomDelayTime();
};

/**���ܵȼ�����*/
class tagSkillLevelAttr{
private:
	///���ܵȼ�
	uint mLevel;
	///Ai����
	uint mAIType;
	///����ʹ��ʱ��״̬ 
	uint mStartState;

	///�Ƿ��������׶�
	bool mbInto;
	///�Ƿ����ͷŽ׶�
	bool mbRel;
	///�Ƿ��н����׶�
	bool mbEnd;
	///�Ƿ����ٻ��׶�
	bool mbSummon;
public:
	typedef std::vector<uint>VecObject;
	typedef VecObject VecForceObject;
	///�ͷŶ���
	VecObject mObjVec;
	///ǿ���ͷŶ���
	VecForceObject mForceObjVec;
	
	///�����׶�
	tagIntonateStp		mIntoStp;
	void SetHasIntoStp(bool b);
	bool IsHasIntoStp();

	///�ͷŽ׶�
	tagReleaseStp		mRelStp;
	void SetHasRelStp(bool);
	bool IsHasRelStp();

	///�����׶�
	tagEndStp			mEndStp;
	void SetHasEndStp(bool);
	bool IsHasEndStp();
	///�ٻ��׶�
	tagSummonStp		mSummonStp;
	void SetHasSummonStp(bool b);
	bool IsHasSummonStp();
	tagSkillLevelAttr();
	~tagSkillLevelAttr();

	void SetLevel(uint level);
	uint GetLevel();
	void SetAiType(uint aitype);
	uint GetAiType();
	void SetStartState(uint state);
	uint GetStartState();
};

/**һ���������Խṹ*/
class tagSkillAttr{
private:
	///���ܻ�������
	tagSkillBaseAttr mBaseAttr;
	///�����Ƿ��ж���,ֻҪ��INTONATE_ACT RELEASE_ACT INTONATE_MAINACT RELEASE_ACTEX ���ж���
	bool mbHasAct;
public:
	///���ܵȼ�����
	std::vector<tagSkillLevelAttr> mLevelAttrVec;

	tagSkillAttr();
	~tagSkillAttr();

	tagSkillBaseAttr* GetBaseAttr();
	tagSkillLevelAttr* GetLevelAttr(uint level);
	tagSkillLevelAttr* GetLevelAttrByIndex(uint index);
	bool IsHasAct();
	void SetHasAct(bool b);

};
/*�����������ñ�Ķ�ȡ��*/
class SkillAttrIniSetup{
private:
	typedef std::map<uint,tagSkillAttr>SkillAttrMap;
	///���м�������
	static SkillAttrMap m_SkillAttrMap;
	
	friend class CSkillListXml;
public:
	static bool LoadFile(const char* fileName);

};