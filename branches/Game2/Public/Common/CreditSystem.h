///
/// ����ϵͳ: ��д��ģ��ʱ��������ƹ淶һ��
/// credit ��Ҫָ����, ����; reputation ����ָ����, ����
/// --FOX SEAL 07212010--
/// Modify By:Expter [1/25/ 2010 ] 
/// 
#pragma once

const char MAXAREANAMESIZE  =32;
const char MAXCREDITNAMESIZE=16;

typedef struct  
{
	int   _iIndex;                       //���������ڷ���������ͨ����������������	
	ulong _dwAreaID;                     //����ID
	char  _cName[MAXAREANAMESIZE];       //��������
}AREA;


//������������
typedef struct  
{
	ulong _dwValue;                      //����ֵ
	ulong _dwLevelID;                    //�ȼ�
	char  _cName[MAXCREDITNAMESIZE];     //����
}CREDITLEVEL;
	

//������֯����
typedef struct  
{
	ulong _dwOrganizeID;                //��֯ID
	char _cDirtyName[MAXAREANAMESIZE];  //����������е���BatakCredit�ֶ�����
	char _cName[MAXAREANAMESIZE];       //��������
	vector<CREDITLEVEL> _vecLevel;      //�ȼ��б�
}ORGANCREDIT;

class CCreditSystem
{
    typedef map<ulong,ORGANCREDIT>::iterator ORGANITR;
    typedef vector<CREDITLEVEL>::iterator	CREDITITR;

public:
	CCreditSystem(void);
	~CCreditSystem(void);
	bool LoadAreaCredit();
    void Release(){m_mapTradeAreas.clear();m_vecCreditLevel.clear(); }
	long CreditLevel(ulong dwValue);                                //��������ֵ��ȡ���������ȼ�
	const char* CreditName(ulong dwValue);                          //��������ֵ��ȡ������������
	long MaxAreaCreditLevel();                                      //��ߵȼ�����
	long MaxAreaCredit();                                           //��ߵȼ�����


	//��������
	bool CodeToByteArray(DBWriteSet& dbWriteSet);
	bool DecodeToByteArray(DBReadSet& dbReadSet);
	const char* GetDirtyName(ulong dwOrganID);		                //��ȡ�������ֶ���
	long MaxOrganValByDirtyName(const char* strDirtyName);          //ָ����֯����ߵȼ�����
	long CreditOrganLeveyByDirtyName(const char* strDirtyName,
                                     ulong dwValue);                //ͨ�����������ֵ���֯�ȼ�
	
	//�ͻ�����
	long MaxAreaValue(ulong dwLevel);                               //ָ���ȼ��������������
	long MaxOrganValue(const char* strName);                        //ָ����֯����ߵȼ�����
	long MaxOrganValue(ulong dwOrganID,ulong dwLevel);              //ָ����ָ֯���ȼ����������
    long  GetMaxCreditByOrgan(ulong dwOrganID);                     //���һ����֯���������ֵ
    long  GetMaxCreditLevelByOrgan(ulong dwOrganID);                //���һ����֯����������ȼ�
    long  CreditOrganLevelByName(const char* strName,ulong dwValue);//ͨ���������ֵ���֯�ȼ�
    long  CreditOrganLevel(ulong dwOrganID,ulong dwValue);          //��ȡָ����֯��ָ������ֵ ����Ӧ�ĵȼ�


    const char* CreditAreaNameByIndex(int index);                   //ͨ����������������
	const char* CreditAreaNameByID(ulong dwID);                     //ͨ����ͼid����������
	const char* GetOrganName(ulong dwOrganID);			            //��ȡ��������
    const char* CreditOrganName(ulong dwOrganID,ulong dwValue);     //��ȡָ����֯��ָ������ֵ ����Ӧ������

	size_t   GetCreditAreaSize();                                      //����������������
	size_t   GetCreditOrganSize();                                     //���������֯������
	

private:
     //������֯����
	bool LoadOrganizeCredit(TiXmlElement* p);                  
	bool LoadCreditLevel(TiXmlElement* p,
                         vector<CREDITLEVEL>& vecCreditLevel);

    CREDITLEVEL* GetLevel(ulong dwValue ,
                          vector<CREDITLEVEL>& vecCreditLevel);
    CREDITLEVEL* GetOrganLevel(ulong dwOrganID,ulong dwLevel);

    //��ȡ��֯ID
	long GetOrganID(const char* Name);                          
	long GetOrganIDByDirtyName(const char* DirtyName);

private:

	map<ulong, AREA >               m_mapTradeAreas;
	vector<CREDITLEVEL>             m_vecCreditLevel;
	map<ulong,ORGANCREDIT>          m_mapOrganCredit;
};
