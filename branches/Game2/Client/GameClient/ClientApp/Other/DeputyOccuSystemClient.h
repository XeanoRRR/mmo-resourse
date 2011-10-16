//		�ļ���:		CDeputyOccuSystemClient.h
//		����:		Ф��
//		ʱ��:		2008/2/3
//		���:		��ְҵϵͳ�ͻ�������
#pragma once

class CCollectionLayout;
class CFacture;
class CFactureStudy;
#include "../../../Public/Common/DeputyOccu.h"
class CDeputyOccuSystemClient : public CDeputyOccuSystem
{
public:
		//��ʼ��
		static		 bool				InitOccuSystemClient(const char* filename);
		static		 BOOL				InitCollectionListClient();
		static		 BOOL				InitFactureStudyListClient();

		//�ɼ��ӿ�
		//���ݴ�������ֻ�ȡĳ���ɼ���������Ϣ
		static const CCollectionLayout* GetCollectionLayout(const char* Name);
		
		//���ݴ���Ĳɼ���ID��ȡĳ���ɼ���������Ϣ
		static const CCollectionLayout* GetCollectionLayout(DWORD dwCollectionGoodsID);
		
		//���ݴ����������ɼ���Ĳɼ��ȼ����ö�Ӧ�Ĳɼ�����
		static		 LONG				GetCollectionExp(LONG level);
		
		//���ݴ����������ɼ���Ĳɼ��ȼ�������ʾ�ɼ������ֵ���ɫֵ
		static		 DWORD				GetCollectionColor(DWORD level);

		//���ݴ��������Ĳɼ��ȼ���ȡ��ǰ�ȼ������ľ���ֵ
		static		 DWORD				GetCollectionNeededExp(DWORD level);
		
		//�����ӿ�
		//���ݴ����ID��ȡ������Ŀ��������Ϣ
		static const CFacture*			GetFacture(DWORD ID);

		//���ݴ��������������ȼ���������ĿҪ��ĵȼ����ȡ��������Ʒ�ܻ�ȡ�ľ���ֵ
		static		 LONG				GetFactureExp(LONG level);

		//���ݴ��������������ȼ���������ĿҪ��ĵȼ����ȡ��������Ʒ�ܻ�ȡ������ֵ
		static		 LONG				GetFactureCredit(LONG level);

		//���ݴ��������������ȼ���������ĿҪ��ĵȼ����ȡ��ʾ��������Ŀ����ɫֵ
		static		 DWORD				GetFactureColor(DWORD level);

		//���ݴ��������������ȼ���ȡ��ǰ������Ҫ���پ���ֵ�����ÿ����������ֵ��Ҫ���㣩
		static		 DWORD				GetFactureNeededExp(DWORD level);

		//���ݴ����NPC���ֻ�ȡ��NPC������ѧ����������Ŀ�б�
		static const CFactureStudy*		GetFactureStudy(const char* NpcName);

		//���ݴ������Ʒ����ID��ȡ���͵�����
		static const string*			GetFactureTypeName(DWORD FactureType);

		//�ɼ����ڳ����е���Ч��ʾˢ��
		static		 void              UpdateCollectionEffectByQuest(DWORD dwQuestID,bool bAddOrDel); 
	
		//�ɼ���������������Ӱ����Ч����ʾ
		//static		 void				UpdateCollectionEffectByCondition();
private:
		//�ɼ��б�(�ͻ���ʹ��)
		static map<DWORD,CCollectionLayout*> m_mapCollectionListClient;
};
