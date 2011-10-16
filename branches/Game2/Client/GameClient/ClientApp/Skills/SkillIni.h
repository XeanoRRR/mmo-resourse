/********************************************************************
created:	2010/09/06
created:	6:9:2010   10:32
filename: 	e:\SVN_P6\trunk\Client\GameClient\ClientApp\Skills\SkillIni.h
file path:	e:\SVN_P6\trunk\Client\GameClient\ClientApp\Skills
file base:	SkillIni
file ext:	h
author:		zhaohang(kaba) (kuailekaba@gmail.com)

purpose:	�°漼�ܵ��İ����������ʾ�����ļ�����̬���������ļ�������
*********************************************************************/
#include "../../../../Public/AppFrame/TextRes.h"
/** \addtogroup Skill
* @{
*/
#ifndef SAFEDELETE
#define SAFEDELETE(e) if(e) { delete e ; e = NULL ;}
#endif

#ifndef MSGBOX
#define MSGBOX(e) MessageBox(NULL,e,"Error",MB_ICONERROR); Log4c::Error(ROOT_MODULE,e);
#endif

/**��鼼��������õĹ��߻���*/
class CheckSkillIni{
public:
	///��������ģʽ��Ϣ������
	enum eMsgBoxType{
		///�ļ�������
		MBT_FILENOTFOUND,
		///��ʽ����ȷ
		MBT_FORMATERROR,
	};
	///���캯��
	CheckSkillIni();
	///��������
	virtual ~CheckSkillIni();

	/**�����������ü��ģʽ��Ϣ��
	@param pathName ���������ļ�·����
	@param msgtype ������ģʽ��Ϣ������
	*/
	void MsgBox(const char* pathName,eMsgBoxType msgtype);
	/**����ļ��Ƿ�򿪳ɹ�
	@param file ��CRFile�򿪵��ļ�ָ��
	@param pathName �ļ�����·����
	*/
	bool CheckFile(CRFile* file,const char* pathName);

private:
	/**����ļ���ʽ�Ƿ���ȷ
	@param vecStr �����ַ�������
	@param pathName �����ļ�����·����
	@return ƥ�䷵��true����ƥ�䷵��false
	*/
	virtual bool CheckFileHeader(std::vector<std::string>& vecStr,const char* pathName) = 0;

};
/**����һ������һ���ȼ������������ݽṹ*/
struct tagSkillDocLevel{
private:
	///�ȼ�
	uint level; 
	///��Ӧ������
	std::string strDesc;
public:
	/**���캯��*/
	tagSkillDocLevel();
	/**��������*/
	~tagSkillDocLevel();

	/**���õȼ�
	@param Level ���õȼ�ֵ
	*/
	void SetLevel(uint Level);

	/**��õȼ�*/
	uint GetLevel();

	/**���õȼ�����
	@remarks
	����ı�ΪNULL�����Զ���������Ϊ""
	@param desc �����ı�
	*/
	void SetDesc(const char* desc);
	/**���õȼ�����
	@remarks
	����ı�ΪNULL�����Զ���������Ϊ""
	@param desc �����ı�
	*/
	void SetDesc(std::string desc);
	/**���صȼ�����*/
	const char* GetDesc();
};

/**�ض���ı���һ�����ܵȼ������������ݽṹ*/
typedef tagSkillDocLevel tagSkillLD;

/**����һ����İ������ݽṹ*/
struct tagSkillDoc{
private:
	///����ID
	uint skillID;
	///������
	std::string strSkillName;
	///���ܵȼ�-���� ��map
	std::map<uint,tagSkillLD*> mapSkillLD; 
	///
	typedef std::map<uint,tagSkillLD*> MapSKILLLD;
public:
	///���캯��
	tagSkillDoc();
	//��������
	~tagSkillDoc();

	/**���ؼ���ID*/
	uint GetSkillID();
	/**���ü���ID
	@param id 
	*/
	void SetSkillID(uint id);
	/**���ؼ�������*/
	const char* GetSkillName();
	/**���ü�������
	@param name ���ܵ�����
	*/
	void SetSkillName(const char* name);

	/**���ü�������
	@param name ���ܵ�����
	*/
	void SetSkillName(std::string name);

	///**���ӵȼ�*/
	//void AddLevel(tagSkillLD* Level);

	///**���ӵȼ�
	//@return �������ָ���ĵȼ����򷵻�false�����򷵻�true
	//*/
	bool AddLevel(tagSkillLD* Level);
	/**�Ƴ�ָ���ĵȼ�*/
	void RemovedLevel(uint Level);

	/**�Ƴ����еȼ�*/
	void RemovedAllLevel();

	/**��ȡָ���ȼ���Level*/
	tagSkillLD* GetLevel(uint Level);

};
/**��鼼���İ������Ƿ���ȷ����*/
class CheckSkillDocIni: public CheckSkillIni
{
private:
	friend class SkillDocIni;
	/**
	@copydoc CheckSkillIni::CheckFileHeader
	*/	bool CheckFileHeader(std::vector<std::string>&vecStr,const char* pathName);
	
	///��鼼��ID�Ƿ���Ϲ���
	bool CheckID(uint id);
	///��鼼�������Ƿ���Ϲ���
	bool CheckSkillName(std::string str);
	///���ȼ��Ƿ���Ϲ���
	bool CheckLevel(uint level);
	///���ȼ������Ƿ���Ϲ���
	bool CheckLevelDesc(std::string str);

};

/**��ָ�����ļ���ȡ�����İ����õ���*/
class SkillDocIni{
private:
	///������IDΪ�������İ����ñ�
	typedef std::map<uint,tagSkillDoc*> MapSKILLDOC;
	///�İ����ñ�
	MapSKILLDOC mSkillDoc;
	///�ļ����õļ����
	CheckSkillDocIni check;
public:
	/**���캯��*/
	SkillDocIni();
	/**��������*/
	~SkillDocIni();

	/**��ȡ�����ļ�
	@param pathName ����·������
	*/
	bool LoadFile(const char* pathName = "data/skills/SkillDocCon.ini");

	/**�洢�����ļ�*/
	bool SaveFile(const char* pathName = "data/skills/SkillDocCon.ini");

	/**������ID��Ϊ��������һ����İ�����
	@param skilldoc ָ���������ָ��
	@return 
		ID�Ѵ��ڣ�����false�����򷵻�true
	*/
	bool AddSkillDoc(tagSkillDoc* skilldoc);
	/**�Ƴ�һ����İ�����
	@param skillid ָ��Ҫ�Ƴ���ļ��ܵ�id
	*/
	void RemovedSkillDoc(uint skillid); 

	/**����ָ������ID�ļ����İ�������
	@param skillid
	*/
	tagSkillDoc* GetSkillDoc(uint skillid);

protected:
	/**�Ƴ�������*/
	void RemovedAllSkillDoc();
};

/**�ͻ��˼�����ʾ���ݽṹ*/
struct tagSkillShowState 
{
	///��Ӧ�ļ��ܵȼ�
	int mlevel;
	///����ʱ��
	int mIntonateTime;
	///��������ֵ
	int mEnergy;
	///��������ֵ
	int mMP;
	///��Ѫ����ֵ
	int mHP;
	///������Ʒԭʼ��
	char mGoodsName[256];
	///������Ʒ����
	int mGoodsNum;
	///��ȴʱ��
	int mCoolDown;
	///��С��������
	int mMinAttackDistance;
	///��󹥻�����
	int mMaxAttackDistance;
	///���캯����������ֵΪ-1
	tagSkillShowState();
	///��������
	~tagSkillShowState();
	///���ظ�ֵ�����
	tagSkillShowState& operator = (const tagSkillShowState& ref);
};

///�����ܵȼ���Ϊ��������ʾ����map��
typedef std::map<uint,tagSkillShowState*> MapSkillLevelData;

/**�ͻ��˼�����ʾ�������ݴ洢��*/
class SkillShow{
private:
	///����ID
	int mSkillID;
	///���ܵȼ�����map��
	MapSkillLevelData mLevelDataMap;
public:
	///���캯��
	SkillShow();
	///��������
	~SkillShow();
	/**���ü���ID
	@param id
	*/
	void SetID(int id);
	/**��ȡ����ID*/
	int GetID();
	/**��ȡָ���ȼ��ļ�������
	@param level ���ܵĵȼ�
	*/
	tagSkillShowState* GetSkillShowState(int level);
	/**��ӵȼ�����
	@param add  �ȼ�����
	*/
	bool AddSkillShowState(tagSkillShowState* add);

	/**�Ƴ�ָ���ȼ�����ʾ����
	@param level ָ���ȼ�
	*/
	bool RemovedSkillShowState(int level);

	/**�Ƴ����еȼ�����ʾ����*/
	void RemovedAllSkillShowState();
};

///������ID�ļ�����ʾ����map��
typedef std::map<int,SkillShow*>MapSkillShow;

/**SkillShowIni���õ��ļ��ĸ�ʽ�����*/
class CheckSkillShowIni: public CheckSkillIni
{
private:
	friend class SkillShowIni;
	/**
	@copydoc CheckSkillIni::CheckFileHeader
	*/
	bool CheckFileHeader(std::vector<std::string>&vecStr,const char* pathName);
protected:
private:
};

/**�ͻ��˼�����ʾ�����ļ�������*/
class SkillShowIni{
private:
	///������ID�ļ�����ʾ����map��
	MapSkillShow  mSkillShowMap;
	///�����ļ��ĸ�ʽ���
	CheckSkillShowIni mCheck;
public:
	///���캯��
	SkillShowIni();
	///��������
	~SkillShowIni();
	/**���SkillShow��
	@param item ���ӵ�SkillShow��
	@return 
		����ָ���ļ���ID�ͷ���false�����򷵻�true
	*/
	bool AddSkillShow(SkillShow* item);

	/**�Ƴ�ָ��ID��SkillShow��
	@param id ����id
	@return
		�ɹ�����true��ʧ�ܷ���false
	*/
	bool RemovedSkillShow(int id);

	/**�Ƴ�����SkillShow��*/
	void RemovedAllSkillShow();

	/**����ָ��id��skillShow��
	@param id ����ID
	@return
		���ض�Ӧ����id����ʾ�û�оͷ���false
	*/
	SkillShow* GetSkillSow(int id);

	/**��ȡָ���������ļ�
	@param pathName �ļ�������·����
	@return
		�ɹ�����true��ʧ�ܷ���false
	*/
	bool LoadFile(const char* pathName);

	/**�洢�����ļ�
	@param pathName �ļ�������·����
	*/
	void SaveFile(const char* pathName);

};


/**������̬Pose�ļ������*/
class CheckSkillPoseIni: public CheckSkillIni
{
private:
	friend class SkillPoseIni;
	/**
	@copydoc CheckSkillIni::CheckFileHeader
	*/	bool CheckFileHeader(std::vector<std::string>&vecStr,const char* pathName);
protected:
private:
};

///�洢����ID-��̬���ݽṹ
struct tagSkillPose 
{
	///����ID
	int id;
	///���ܶ�Ӧ����̬����
	std::string strPose;
};
///������ID�洢����̬���ݱ�
typedef std::map<int,tagSkillPose*>MapSkillPose;
///������̬�����ļ�������
class SkillPoseIni{
private:
	///�ļ����
	CheckSkillPoseIni mCheck;
	///��ȡ����̬���ݱ�
	MapSkillPose   mSkillPoseList;
public:
	///���캯��
	SkillPoseIni();
	///��������
	~SkillPoseIni();

	/**��ȡ�ļ�
	@param pathName �ļ�����·����
	*/
	bool LoadFile(const char* pathName);

	/**�����ļ�
	@param pathName �ļ�����·����
	*/
	bool SaveFile(const char* pathName);

	/**�����̬
	@param add ����ӵ�һ����̬����
	*/
	bool AddPose(tagSkillPose* add);

	/**��key�Ƴ���̬
	@param key  �ؼ���Ϊ����ID
	*/
	bool RemovedPose(int key);

	/**�Ƴ�������̬����*/
	void RemovedAllPose();

	/**��key��ȡ��̬
	@param id ����IDΪ�ؼ���
	*/
	tagSkillPose* GetPose(int id);

};
/** @} */