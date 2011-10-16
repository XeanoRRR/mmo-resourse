/********************************************************************
created:	2010/09/13
created:	13:9:2010   16:27
filename: 	e:\SVN_P6\trunk\Client\GameClient\ClientApp\Skills\SkillListIni.cpp
file path:	e:\SVN_P6\trunk\Client\GameClient\ClientApp\Skills
file base:	SkillListIni
file ext:	cpp
author:		zhaohang(kaba) (kuailekaba@gmail.com)

purpose:	SkillListIni.h��ʵ��
*********************************************************************/

#include "stdafx.h"
#include "SkillListIni.h"
#include "../../Engine/Utility.h"
#include "../../../../Public/Common/SkillRelated.h"
/***********************************************************************/
/*  2010-10-11 ���ܳ�ʼ����׶λ�������*/
/***********************************************************************/
std::map<uint,tagSkillAttr>SkillAttrIniSetup::m_SkillAttrMap ;
tagSkillBaseAttr::tagSkillBaseAttr():
mID(0),
mSkillType(0),
mbLockTarget(0),
mbLockSkill(0)
{
	strcpy(mName,"");
}
tagSkillBaseAttr::~tagSkillBaseAttr()
{

}

uint tagSkillBaseAttr::GetID()
{
	return mID;
}
void tagSkillBaseAttr::SetID(uint id)
{
	mID = id;
}

const char* tagSkillBaseAttr::GetName()
{
	return mName;
}

void tagSkillBaseAttr::SetName(const char* name)
{
	strcpy(mName,name);
}

uint tagSkillBaseAttr::GetSkillType()
{
	return mSkillType;
}

void tagSkillBaseAttr::SetSkillType(uint type)
{
	mSkillType = type;
}

void tagSkillBaseAttr::SetLockSkill(bool bLockSkill)
{
	mbLockSkill = bLockSkill;
}

bool tagSkillBaseAttr::IsLockSkill()
{
	return mbLockSkill;
}

bool tagSkillBaseAttr::IsLockTarget()
{
	return mbLockTarget;
}
void tagSkillBaseAttr::SetLockTarget(bool bLockTarget)
{
	mbLockTarget = bLockTarget;
}

/***********************************************************************/
/*  2010-10-11 һ����Ч�Ļ�������*/
/***********************************************************************/
tagEffectAttr::tagEffectAttr():
mID(0),
mIndex(-1),
mBindPos(-1),
mCycle(0),
mPlay(0)
{

}

tagEffectAttr::~tagEffectAttr()
{

}

void tagEffectAttr::SetID(uint id)
{
	mID = id;
}
uint tagEffectAttr::GetID()
{
	return mID;
}
void tagEffectAttr::SetIndex(uint index)
{
	mIndex = index;
}
uint tagEffectAttr::GetIndex()
{
	return mIndex;
}
void tagEffectAttr::SetBindPos(uint pos)
{
	mBindPos = pos;
}
uint tagEffectAttr::GetBindPos()
{
	return mBindPos;
}

void tagEffectAttr::SetCycle(bool b)
{
	mCycle = b;
}

bool tagEffectAttr::IsCycle()
{
	return mCycle;
}
void tagEffectAttr::SetPlay(bool b)
{
	mPlay = b;
}
bool tagEffectAttr::IsPlay()
{
	return mPlay;
}
/***********************************************************************/
/*  2010-10-11 һ���׶εĻ�������*/
/***********************************************************************/
tagStpAttr::tagStpAttr():
mActID(-1)
{

}
tagStpAttr::~tagStpAttr()
{

}
void tagStpAttr::SetActID(uint id)
{
	mActID = id;
}
uint tagStpAttr::GetActID()
{
	return mActID;
}

/***********************************************************************/
/*  2010-10-11 һ���׶λ������������Ч����*/
/***********************************************************************/
tagStpAttrSound::tagStpAttrSound():
mSoundID(-1),
mSoundCycle(false),
mSoundPlay(false)
{

}
tagStpAttrSound::~tagStpAttrSound()
{

}
void tagStpAttrSound::SetSoundID(uint id)
{
	mSoundID = id;
}
uint tagStpAttrSound::GetSoundID()
{
	return mSoundID;
}
void tagStpAttrSound::SetSoundCycle(bool b)
{
	mSoundCycle = b;
}
bool tagStpAttrSound::IsSoundCycle()
{
	return mSoundCycle;
}
void tagStpAttrSound::SetSoundPlay(bool b)
{
	mSoundPlay = b;
}
bool tagStpAttrSound::IsSoundPlay()
{
	return mSoundPlay;
}

/***********************************************************************/
/*  2010-10-11 �����׶�����StepBegin*/
/***********************************************************************/
tagIntonateStp::tagIntonateStp():
mMainActID(-1),
mBreak(false)
{
	mVecEffect.clear();
}

tagIntonateStp::~tagIntonateStp()
{
	mVecEffect.clear();
}



void tagIntonateStp::SetMainActID(uint id)
{
	mMainActID = id;
}

uint tagIntonateStp::GetMainActID()
{
	return mMainActID;
}

void tagIntonateStp::SetBreak(bool b)
{
	mBreak = b;
}
bool tagIntonateStp::IsBreak()
{
	return mBreak;
}


/***********************************************************************/
/*  2010-10-11 �ͷ���Ч����*/
/***********************************************************************/
tagRelEffectAttr::tagRelEffectAttr():
mDest(-1)
{

}
tagRelEffectAttr::~tagRelEffectAttr()
{

}

void tagRelEffectAttr::SetDest(uint dest)
{
	mDest = dest;
}

uint tagRelEffectAttr::GetDest()
{
	return mDest;
}

/***********************************************************************/
/*  2010-10-11 ������Ч���� */
/***********************************************************************/
tagFlyEffectAttr::tagFlyEffectAttr():
mTurnX(0),
mTurnY(0),
mActTime(0),
mSpeed(0),
mDelayTime(0),
mSoundPos(-1)
{

}

tagFlyEffectAttr::~tagFlyEffectAttr()
{

}

void tagFlyEffectAttr::SetTurnX(int x)
{
	mTurnX = x;
}

int tagFlyEffectAttr::GetTurnX()
{
	return mTurnX;
}
void tagFlyEffectAttr::SetTurnY(int y)
{
	mTurnY = 0;
}
int tagFlyEffectAttr::GetTurnY()
{
	return mTurnY;
}

void tagFlyEffectAttr::SetActTime(uint time)
{
	mActTime = time;
}
uint tagFlyEffectAttr::GetActTime()
{
	return mActTime;
}
void tagFlyEffectAttr::SetSpeed(uint speed)
{
	mSpeed = speed;
}
uint tagFlyEffectAttr::GetSpeed()
{
	return mSpeed;
}

void tagFlyEffectAttr::SetDelayTime(uint delay)
{
	mDelayTime = delay;
}
uint tagFlyEffectAttr::GetDelayTime()
{
	return mDelayTime;
}
void tagFlyEffectAttr::SetSoundPos(uint pos)
{
	mSoundPos = pos;
}
uint tagFlyEffectAttr::GetSoundPos()
{
	return mSoundPos;
}


/***********************************************************************/
/*  2010-10-11 */
/***********************************************************************/
tagReleaseStp::tagReleaseStp():
mSoundDest(0),
mActTime(-1)
{
	mVecEffect.clear();
}

tagReleaseStp::~tagReleaseStp()
{
	mVecEffect.clear();
}

void tagReleaseStp::SetSoundDest(uint dest)
{
	mSoundDest = dest;
}
uint tagReleaseStp::GetSoundDest()
{
	return mSoundDest;
}

void tagReleaseStp::SetActTime(uint t)
{
	mActTime = t;
}

uint tagReleaseStp::GetActTime()
{
	return mActTime;
}
void tagReleaseStp::SetHasFly(bool b)
{
	mbFly = b;
}
bool tagReleaseStp::IsHasFly()
{
	return mbFly;
}
/***********************************************************************/
/*  2010-10-11 �����׶�����StepEnd*/
/***********************************************************************/
tagEndStp::tagEndStp():
mHitEffectID(-1),
mHitSoundID(-1),
mFlashEffect(false),
mFlashEffectLum(0),
mFlashEffectTime(0),
mShakeEffect(false),
mShakeEffectExtent(0),
mShakeEffectTime(0)
{

}

tagEndStp::~tagEndStp()
{

}

void tagEndStp::SetHitEffID(uint id)
{
	mHitEffectID = id;
}
uint tagEndStp::GetHitEffID()
{
	return mHitEffectID;
}
void tagEndStp::SetHitSoundID(uint id)
{
	mHitSoundID = id;
}
uint tagEndStp::GetHitSoundID()
{
	return mHitSoundID;
}

void tagEndStp::SetFlashEffect(bool b)
{
	mFlashEffect = b;
}
bool tagEndStp::IsFlashEffect()
{
	return mFlashEffect;
}
void tagEndStp::SetFlashEffLum(uint num)
{
	mFlashEffectLum = num;
}

uint tagEndStp::GetFlashEffLum()
{
	return mFlashEffectLum;
}

void tagEndStp::SetFlashEffTime(uint t)
{
	mFlashEffectTime = t;
}
uint tagEndStp::GetFlashEffTime()
{
	return mFlashEffectTime;
}
void tagEndStp::SetShakeEffect(bool b)
{
	mShakeEffect = b;
}
bool tagEndStp::IsShakeEffect()
{
	return mShakeEffect;
}

void tagEndStp::SetShakeEffExtent(uint num)
{
	mShakeEffectExtent = num;
}

uint tagEndStp::GetShakeEffExtent()
{
	return mShakeEffectExtent;
}
void tagEndStp::SetShakeEffTime(uint t)
{
	mShakeEffectTime = t;
}
uint tagEndStp::GetShakeEffTime()
{
	return mShakeEffectTime;
}

/***********************************************************************/
/*  2010-10-11 �ٻ���Ч�׶�StepSummon*/
/***********************************************************************/
tagSummonStp::tagSummonStp():
mID(-1),
mSoundID(-1),
mLifeTime(0),
mRangeX(0),
mRangeY(0),
mRandomNum(0),
mRandomDelayTime(0)
{

}
tagSummonStp::~tagSummonStp()
{

}

void tagSummonStp::SetID(uint id)
{
	mID = id;
}

uint tagSummonStp::GetID()
{
	return mID;
}
void tagSummonStp::SetSoundID(uint id)
{
	mSoundID = id;
}
uint tagSummonStp::GetSoundID()
{
	return mSoundID;
}

void tagSummonStp::SetLifeTime(uint t)
{
	mLifeTime = t;
}
uint tagSummonStp::GetLifeTime()
{
	return mLifeTime;
}
void tagSummonStp::SetRangeX(uint x)
{
	mRangeX = x;
}
uint tagSummonStp::GetRangeX()
{
	return mRangeX;
}
void tagSummonStp::SetRangeY(uint y)
{
	mRangeY = y;
}
uint tagSummonStp::GetRangeY()
{
	return mRangeY;
}

void tagSummonStp::SetRandomNum(uint num)
{
	mRandomNum = num;
}
uint tagSummonStp::GetRandomNum()
{
	return mRandomNum;
}
void tagSummonStp::SetRandomDelayTime(uint t)
{
	mRandomDelayTime = t;
}
uint tagSummonStp::GetRandomDelayTime()
{
	return mRandomDelayTime;
}

/***********************************************************************/
/*  2010-10-12 ���ܵȼ�����*/
/***********************************************************************/
tagSkillLevelAttr::tagSkillLevelAttr():
mLevel(0),
mAIType(0),
mStartState(0)
{
	mObjVec.clear();
	mForceObjVec.clear();
}
tagSkillLevelAttr::~tagSkillLevelAttr()
{
	mObjVec.clear();
	mForceObjVec.clear();
}

void tagSkillLevelAttr::SetLevel(uint level)
{
	mLevel = level;
}
uint tagSkillLevelAttr::GetLevel()
{
	return mLevel;
}
void tagSkillLevelAttr::SetAiType(uint aitype)
{
	mAIType = aitype;
}
uint tagSkillLevelAttr::GetAiType()
{
	return mAIType;
}
void tagSkillLevelAttr::SetStartState(uint state)
{
	mStartState = state;
}
uint tagSkillLevelAttr::GetStartState()
{
	return mStartState;
}

void tagSkillLevelAttr::SetHasIntoStp(bool b)
{
	mbInto = b;
}
bool tagSkillLevelAttr::IsHasIntoStp()
{
	return mbInto;
}
void tagSkillLevelAttr::SetHasRelStp(bool b)
{
	mbRel = b;
}
bool tagSkillLevelAttr::IsHasRelStp()
{
	return mbRel;
}
void tagSkillLevelAttr::SetHasEndStp(bool b)
{
	mbEnd = b;
}
bool tagSkillLevelAttr::IsHasEndStp()
{
	return mbEnd;
}
void tagSkillLevelAttr::SetHasSummonStp(bool b)
{
	mbSummon = b;
}
bool tagSkillLevelAttr::IsHasSummonStp()
{
	return mbSummon;
}
/***********************************************************************/
/*  2010-10-12 һ���������Խṹ*/
/***********************************************************************/
tagSkillAttr::tagSkillAttr()
{
	mLevelAttrVec.clear();
}

tagSkillAttr::~tagSkillAttr()
{
	mLevelAttrVec.clear();
}

bool tagSkillAttr::IsHasAct()
{
	return mbHasAct;
}
void tagSkillAttr::SetHasAct(bool b)
{
	mbHasAct = b;
}
tagSkillBaseAttr* tagSkillAttr::GetBaseAttr()
{
	return &mBaseAttr;
}

tagSkillLevelAttr* tagSkillAttr::GetLevelAttr(uint level)
{
	for (size_t i = 0 ; i < mLevelAttrVec.size() ; ++i)
	{
		if(mLevelAttrVec[i].GetLevel() == level)
		{
			return &mLevelAttrVec[i];
		}
	}
	return NULL;
}

tagSkillLevelAttr* tagSkillAttr::GetLevelAttrByIndex(uint index)
{
	if(index < mLevelAttrVec.size())
	{
		return &mLevelAttrVec[index];
	}
	else
		return NULL;
}
/***********************************************************************/
/*  2010-10-12 �����������ñ�Ķ�ȡ��*/
/***********************************************************************/
bool SkillAttrIniSetup::LoadFile(const char* fileName)
{
	CRFile* prfile = rfOpen(fileName);
	if(!prfile)
	{
		//���������Ϣ
		char strError[128];
		wsprintf(strError,"LoadFileError:%s",fileName);
		MessageBox(NULL,strError,"",MB_OK); 
		return false;
	}
	stringstream stream;
	prfile->ReadToStream(stream);
	rfClose(prfile);
	//��ʼ������
	while(ReadTo(stream,"#"))
	{
		//id
		uint id;
		stream >> id;
		//�������޶�Ӧ����
		tagSkillAttr* pSkillAttr = NULL;
		bool isNewSkillAttr = false;
		if( m_SkillAttrMap.find(id) == m_SkillAttrMap.end())
		{
			//�½�һ�����ܽṹ
			pSkillAttr = new tagSkillAttr();
			isNewSkillAttr = true;
		}
		else
			pSkillAttr = &m_SkillAttrMap[id];
		//����id
		pSkillAttr->GetBaseAttr()->SetID(id);
		//��������
		char skillname[256];
		stream >> skillname;
		pSkillAttr->GetBaseAttr()->SetName(skillname);
		//��������
		uint skilltype;
		stream >> skilltype;
		pSkillAttr->GetBaseAttr()->SetSkillType(skilltype);
		//�Ƿ�����Ŀ��
		bool bLocktarget;
		stream >> bLocktarget;
		pSkillAttr->GetBaseAttr()->SetLockTarget(bLocktarget);
		//�Ƿ���������
		bool bLockSkill;
		stream >> bLockSkill;
		pSkillAttr->GetBaseAttr()->SetLockSkill(bLockSkill);
		//�ȼ��׶�
		uint Level;
		stream >> Level;
		tagSkillLevelAttr* pLevelAttr = pSkillAttr->GetLevelAttr(Level);
		bool isNewLevel = false;
		if(!pLevelAttr)
		{
			pLevelAttr = new tagSkillLevelAttr();
			isNewLevel = true;
		}
		pLevelAttr->SetLevel(Level);
		//�ȼ�Ai����
		uint levelAiType;
		stream >> levelAiType;
		pLevelAttr->SetAiType(levelAiType);
		//�ͷŶ������
		uint ObjNum;
		stream >> ObjNum;
		for (uint i = 0 ; i < ObjNum ; ++i)
		{
			//����ֵ
			uint object;
			//�ָ���
			char temp;
			stream >> object >> temp;
			pLevelAttr->mObjVec.push_back(object);
		}
		//ǿ���ͷŶ���
		uint objforcenum;
		stream >> objforcenum;
		for (uint i = 0  ; i < objforcenum ; ++i)
		{
			//ǿ���ͷŶ���ֵ
			uint objforce;
			//�ָ���
			char temp;
			stream >> objforce >> temp;
			pLevelAttr->mForceObjVec.push_back(objforce);
		}
		//����ʹ��ʱ��״̬
		uint startstate;
		stream >> startstate;
		pLevelAttr->SetStartState(startstate);
		//�����׶�
		bool bInto;
		stream >> bInto;
		pLevelAttr->SetHasIntoStp(bInto);
		if(bInto)//�вŶ�
		{
			tagIntonateStp* ptagIntoStp = &(pLevelAttr->mIntoStp);
			//��������
			uint IntoActID;
			stream >> IntoActID;
			ptagIntoStp->mBaseAttr.SetActID(IntoActID);
			if(IntoActID != -1)
			{
				pSkillAttr->SetHasAct(true);
			}
			//������Ҫѭ������
			uint IntoMainActID;
			stream >> IntoMainActID;
			ptagIntoStp->SetMainActID(IntoMainActID);
			if(IntoMainActID != -1)
			{
				pSkillAttr->SetHasAct(true);
			}
			//������ЧID
			uint IntoSoundID;
			stream >> IntoSoundID;
			ptagIntoStp->mBaseAttr.mSoundAttr.SetSoundID(IntoSoundID);
			//������Ч�Ƿ�ѭ��
			bool bIntoSoundCycle;
			stream >> bIntoSoundCycle;
			ptagIntoStp->mBaseAttr.mSoundAttr.SetSoundCycle(bIntoSoundCycle);
			//��ʼ����������Ч
			bool bIntoSoundPlay;
			stream >> bIntoSoundPlay;
			ptagIntoStp->mBaseAttr.mSoundAttr.SetSoundPlay(bIntoSoundPlay);
			//���������Ƿ�����ж�
			bool bIntoBreak;
			stream >> bIntoBreak;
			ptagIntoStp->SetBreak(bIntoBreak);
			//�����׶���Ч
			uint IntoEffNum;
			stream >> IntoEffNum;
			//�����Ч
			ptagIntoStp->mVecEffect.clear();
			for (uint i = 0 ; i < IntoEffNum ; ++i)
			{
				//Ч��
				tagEffectAttr effect;
				//id
				uint id;
				stream >>id; 
				effect.SetID(id);
				//����
				uint index;
				stream >> index;
				effect.SetIndex(index);
				//��λ��
				uint pos;
				stream >> pos;
				effect.SetBindPos(pos);
				//ѭ������
				bool bCycle;
				stream >> bCycle;
				effect.SetCycle(bCycle);
				//��ʼ����
				bool bPlay;
				stream >> bPlay;
				effect.SetPlay(bPlay);
				ptagIntoStp->mVecEffect.push_back(effect);
			}
		}
		//�ͷŽ׶�����
		bool bRel;
		stream >> bRel;
		pLevelAttr->SetHasRelStp(bRel);
		tagReleaseStp* pRelStp = &(pLevelAttr->mRelStp);
		if(bRel)//�вŶ�
		{
			//����ID
			uint RelActID;
			stream >> RelActID;
			pRelStp->mBaseAttr.SetActID(RelActID);
			if(RelActID != -1)
			{
				pSkillAttr->SetHasAct(true);
			}
			//��ЧID
			uint RelSoundID;
			stream >> RelSoundID;
			pRelStp->mBaseAttr.mSoundAttr.SetSoundID(RelSoundID);
			//��Ч�Ƿ�ѭ��
			bool bRelSoundCycle;
			stream >> bRelSoundCycle;
			pRelStp->mBaseAttr.mSoundAttr.SetSoundCycle(bRelSoundCycle);
			//�ͷ���ЧĿ��
			uint RelSoundDest;
			stream >> RelSoundDest;
			pRelStp->SetSoundDest(RelSoundDest);
			//�����ͷ���Ч
			bool bRelSoundPlay;
			stream >> bRelSoundPlay;
			pRelStp->mBaseAttr.mSoundAttr.SetSoundPlay(bRelSoundPlay);
			//����ʱ��
			uint RelActTime;
			stream >> RelActTime;
			pRelStp->SetActTime(RelActTime);
			//�ͷŽ׶���Ч
			uint RelEffNum;
			stream >> RelEffNum;
			pRelStp->mVecEffect.clear();
			for (uint i = 0 ; i < RelEffNum ; ++i)
			{
				tagRelEffectAttr effect;
				//id
				uint id;
				stream >> id;
				effect.mEffect.SetID(id);
				//����
				uint index;
				stream >> index;
				effect.mEffect.SetIndex(index);
				//��ЧĿ��
				uint dest;
				stream >> dest;
				effect.SetDest(dest);
				//��Ч��λ��
				uint pos;
				stream >> pos;
				effect.mEffect.SetBindPos(pos);
				//�Ƿ�ѭ��
				bool bCycle;
				stream >> bCycle;
				effect.mEffect.SetCycle(bCycle);
				//��ʼ����
				bool bPlay;
				stream >> bPlay;
				effect.mEffect.SetPlay(bPlay);
				pRelStp->mVecEffect.push_back(effect);
			}
		}
		//������Ч
		bool bFly;
		stream >> bFly;
		pRelStp->SetHasFly(bFly);
		if(bFly)// �вŶ�
		{
			//id
			uint FlyID;
			stream >> FlyID;
			pRelStp->mFlyEffect.mBaseAttr.SetID(FlyID);
			//����
			uint FlyIndex;
			stream >> FlyIndex;
			pRelStp->mFlyEffect.mBaseAttr.SetIndex(FlyIndex);
			//�Ӻδ�����
			uint FlyPos;
			stream >> FlyPos;
			pRelStp->mFlyEffect.mBaseAttr.SetBindPos(FlyPos);
			//ѭ��
			bool bFlyCycle;
			stream >> bFlyCycle;
			pRelStp->mFlyEffect.mBaseAttr.SetCycle(bFlyCycle);
			//Xƫ�ƽǶ�
			int FlyTurnX;
			stream >> FlyTurnX;
			pRelStp->mFlyEffect.SetTurnX(FlyTurnX);
			//Yƫ�ƽǶ�
			int FlyTurnY;
			stream >> FlyTurnY;
			pRelStp->mFlyEffect.SetTurnY(FlyTurnY);
			//�ٶ�
			uint FlySpeed;
			stream >> FlySpeed;
			pRelStp->mFlyEffect.SetSpeed(FlySpeed);
			//�ӳ�ʱ��
			uint FlyDelay;
			stream >> FlyDelay;
			pRelStp->mFlyEffect.SetDelayTime(FlyDelay);
			//��ʼ����
			bool bFlayPlay;
			stream >> bFlayPlay;
			pRelStp->mFlyEffect.mBaseAttr.SetPlay(bFlayPlay);
			//������Ч
			uint FlySoundID;
			stream >> FlySoundID;
			pRelStp->mFlyEffect.mSoundAttr.SetSoundID(FlySoundID);
			//��Ч�Ӻδ��ͷ�
			uint FlySoundPos;
			stream >> FlySoundPos;
			pRelStp->mFlyEffect.SetSoundPos(FlySoundPos);
			//��Ч�Ƿ�ѭ��
			bool bFlySoundCycle;
			stream >> bFlySoundCycle;
			pRelStp->mFlyEffect.mSoundAttr.SetSoundCycle(bFlySoundCycle);
			//��ʼ���ŷ�����Ч
			bool bFlySoundPlay;
			stream >> bFlySoundPlay;
			pRelStp->mFlyEffect.mSoundAttr.SetSoundPlay(bFlySoundPlay);
		}
		//�����׶�����
		bool bEnd;
		stream >> bEnd;
		pLevelAttr->SetHasEndStp(bEnd);
		if(bEnd)//�вŶ�
		{
			//������Ч
			uint HitID;
			stream >> HitID;
			tagEndStp* pEndStp = &(pLevelAttr->mEndStp);
			pEndStp->SetHitEffID(HitID);
			//������Ч
			uint HidSoundID;
			stream >> HidSoundID;
			pEndStp->SetHitSoundID(HidSoundID);
			//����Ч��
			bool bFlashEff;
			stream >> bFlashEff;
			pEndStp->SetFlashEffect(bFlashEff);
			//��������
			uint FlashLum;
			stream >> FlashLum;
			pEndStp->SetFlashEffLum(FlashLum);
			//�������ʱ��
			uint FlashTime;
			stream >> FlashTime;
			pEndStp->SetFlashEffTime(FlashTime);
			//��Ч��
			bool bShake;
			stream >> bShake;
			pEndStp->SetShakeEffect(bShake);
			//�𶯷���ֵ
			uint ShakeExtent;
			stream >> ShakeExtent;
			pEndStp->SetShakeEffExtent(ShakeExtent);
			//��Ч��ʱ��
			uint ShakeTime;
			stream >> ShakeTime;
			pEndStp->SetShakeEffTime(ShakeTime);
		}
		//���ܻ��ս׶�
		bool bSummon;
		stream >> bSummon;
		pLevelAttr->SetHasSummonStp(bSummon);
		if(bSummon)//�вŶ�
		{
			tagSummonStp* pSumStp = &(pLevelAttr->mSummonStp);
			//��Чid
			uint SummonID;
			stream >> SummonID;
			pSumStp->SetID(SummonID);
			//��Ч
			uint SummonSoundID;
			stream >> SummonSoundID;
			pSumStp->SetSoundID(SummonSoundID);
			//��������
			uint SummonLifeTime;
			stream >> SummonLifeTime;
			pSumStp->SetLifeTime(SummonLifeTime);
			//X �᷶Χ
			int RangeX;
			stream >> RangeX;
			pSumStp->SetRangeX(RangeX);
			//y�᷶Χ
			int RangeY;
			stream >> RangeY;
			pSumStp->SetRangeY(RangeY);
			//����ٻ�������
			uint RandomNum;
			stream >> RandomNum;
			pSumStp->SetRandomNum(RandomNum);
			//�ٻ��ӳ�ʱ��
			uint SummonDelayTime;
			stream >> SummonDelayTime;
			pSumStp->SetRandomDelayTime(SummonDelayTime);
		}
		if( isNewLevel == true)
		{
			pSkillAttr->mLevelAttrVec.push_back(*pLevelAttr);
			SAFE_DELETE(pLevelAttr);
		}
		if(isNewSkillAttr)
		{
			m_SkillAttrMap[pSkillAttr->GetBaseAttr()->GetID()] = *pSkillAttr;
			SAFE_DELETE(pSkillAttr);
		}
	}
	return true;
}