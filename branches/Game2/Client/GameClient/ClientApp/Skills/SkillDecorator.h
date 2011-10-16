/********************************************************************
	created:	2010/09/26
	created:	26:9:2010   14:23
	filename: 	e:\SVN_P6\trunk\Client\GameClient\ClientApp\Skills\SkillDecorator.h
	file path:	e:\SVN_P6\trunk\Client\GameClient\ClientApp\Skills
	file base:	SkillDecorator
	file ext:	h
	author:		zhaohang(kaba) (kuailekaba@gmail.com)
	
	purpose:	����ʹ��װ��ģʽ����չCSkillXml�Ĺ���
*********************************************************************/
class CMoveShape;

///���ܵĹ�������
class SkillComponent{
public:
	///����������,�����̳е������Լ�����������ʵ��
	virtual ~SkillComponent();

	///ִ�м��ܿ�ʼ����
	bool virtual StepBegin(long lID, long lLevel, 
		long sType,
		const CGUID& sID, long lDestX,
		long lDestY, float fDir, 
		vector<long> vType, vector<CGUID> vID) = 0;
	/// ִ�з��ͼ�����Ϣ����
	bool virtual StepSend(DWORD dwID,DWORD dwLevel,
		CMoveShape *sShape, CMoveShape *dShape,
		long dX, long dY) = 0;
	///ִ�м���ʵʩ����
	bool virtual StepRun(long lID,long lLevel,
		long sType,const CGUID& sID,
		long dX,long dY,float fDir,
		vector<long>vType,vector<CGUID>vID) = 0;
	///ִ�м��ܽ�������
	bool virtual StepEnd(long lID,long lLevel,
		long sType,const CGUID& sID,float fDir)=0;
	///ִ�м��ܻ��ղ���
	bool virtual StepSummon()=0;
};

///���ܵı�װ����
class ConcreteSkillComp:public SkillComponent
{
public:
	///���캯��
	ConcreteSkillComp();
	///��������
	~ConcreteSkillComp();

	///ִ�м��ܿ�ʼ����
	bool virtual StepBegin(long lID, long lLevel, 
		long sType,
		const CGUID& sID, long lDestX,
		long lDestY, float fDir, 
		vector<long> vType, vector<CGUID> vID);
	/// ִ�з��ͼ�����Ϣ����
	bool virtual StepSend(DWORD dwID,DWORD dwLevel,
		CMoveShape *sShape, CMoveShape *dShape,
		long dX, long dY);
	///ִ�м���ʵʩ����
	bool virtual StepRun(long lID,long lLevel,
		long sType,const CGUID& sID,
		long dX,long dY,float fDir,
		vector<long>vType,vector<CGUID>vID);
	///ִ�м��ܽ�������
	bool virtual StepEnd(long lID,long lLevel,
		long sType,const CGUID& sID,float fDir);
	///ִ�м��ܻ��ղ���
	bool virtual StepSummon();
protected:
	SkillComponent* m_Comp;
};
