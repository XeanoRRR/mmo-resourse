/// 
/// @file  ScriptPhaseFunc.h
/// @brief ��λ���ܵ�һЩ�ű�����
/// 

/// 
/// �����λ���ܵĴ���.. �˽ű�ֻ�����Ұս����
/// @param   size1      ��Χ1�Ĵ�С
/// @param   size2      ��Χ2�Ĵ�С
/// @param   trigger    ���������������� (0Ϊ������1Ϊ����)
/// @param   scriptfile ���нű��ļ�
/// @return  �ɹ����
/// 
bool   CreatePhase(float size1,float size2,long trigger,const char* scriptfile);

/// 
/// ������λ����,����״̬
/// @param etype      ����������(add,sub)
/// @param lTimer     ��ʱ������ʱ��
/// @param lExipires  ��ʱ������ʱ��(0,1)
/// 
void   ActivePhase(long etype,long lTimer,long lExipires);

/// 
/// �õ���λ������ֵ
/// @param  pName  ���������������
/// @return ���ز���ֵ
/// 
long  GetPhasePoint(const char* pName);

/// 
/// �õ���λ������ֵ
/// @param  GID    ��λID
/// @param  pName  ���������������
/// @return ���ز���ֵ
///  
long  GetPhasePoint(const CGUID& id , const char* pName);

///
/// ����λ�����д���һ��ָ���Ĺ���
/// @param  OrgName   �����ԭʼ��
/// @param  x_point   �����λ�ľ�������
/// 
void CreatePhaseMonster(const char* OrgName,int Amount,int x_point,int y_point,
                        int Width,int Height,int rgnId,const char* scriptFileName,
                        const char* talkFileName,int dir);

/// 
/// ��λ����
/// @param  Oper   eRemit=1, //����  
///                eLeave=2, //�뿪(���,�ӳ�����,�˳���Ϸ) 
///                eEscape=3,//���� 
///                eReJoin=4,//��Ӳ���
/// 
long  OnPhaseOper(long Oper);

/// 
/// ��λ�¼�
/// @param  Event  eExit=1,   //���������¼�����  
///                eDie=2,    ���� 
///                eSecKill=3,��ɱ 
///                eAgonal=4, ��ұ���״̬
/// 
long  OnPhaseEvent(long eExit);
   
/// 
/// ������λ����
/// 
bool   SetPhaseEnd();

/// 
/// ������λ�����Ĵ���
/// @param  Filename   ��λ�������������Ľű�
/// 
void   SetPhaseEndScriptFile(const char* Filename);

///
/// �ر�ָ����λ��λ����
/// @param  PhaseId   �ر�ָ������λ
/// 
void   ClosePhase(const CGUID&  PhaseId);

///
/// �ر����������λ��λ����
/// 
void   ClosePhase();

/// 
/// ������λ��ID
/// 
const CGUID& GetPhaseID();

///
/// ���������λID
/// 
const CGUID& GetShapePhaseID();

/// 
/// �õ���λ����ʱ��
/// 
long  GetCostTime(const CGUID&  PhaseId);

/// 
/// �رն�ʱ��
/// 
void  CancelPhaseTimer(const CGUID&  PhaseId);

///
/// ����λһ������һ����Ϣ
/// 
void  PostPhaseMsg(const CGUID& PhaseId,const char* pMsg,ulong dwColor =0, ulong dwBkColor=0,long ePos=1);
