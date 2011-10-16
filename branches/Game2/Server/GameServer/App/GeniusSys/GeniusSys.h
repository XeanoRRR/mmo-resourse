/** 
 *  @brief  �츳Ӱ���ÿһ������
 *  @author expter
 *  @date   2010/08/31
 */           

#pragma  once 

#include <string>
#include <map>
#include <vector>

#include "../../../Public/Common/SkillAttributeDef.h"
#include "../../../Public/Common/GeniusDef.h"
#include "GeniusComDef.h"

#define  SKILLTBL       GetInst(SkillAttribute)
#define  SKILLGNOPER    GetInst(GeniusSysOper)

class  CPlayer;
class  GeniusSysOper;

class  GeniusSysOper
{
    typedef std::map<std::string,long>  AttriMap;
    typedef std::vector<tagSkillEffPro> SkillPro;
    typedef void  (*GeniusFun)(AttriMap& mp,string name,long temp,long value);
    typedef std::map<GeniusOper,GeniusFun>   OperFun;

public:
    GeniusSysOper();
    ~GeniusSysOper();

    long     GetBaseNum(){ return m_BaseNum;}

    void     GeniusPolicy(SkillPro&  Attri,
                          AttriMap&  mp,
                          long id,
                          long lvl);
private:

    OperFun   m_OperTable;
    long      m_BaseNum;
};

/// �츳Ӱ��ĳ���ض����ܵ�����Ч��
class  GeniusSys 
{

public:
    GeniusSys(long skillid,long Lvl,CPlayer * player);
    ~GeniusSys();

    /// ���µ�ǰ���ܵ����е�����
    void   Update(UpdateEvent Event, long id , long Lvl );

    /// ����
    void   AddSkillPro(std::vector<tagGeniusSkill>& GeniusVec,long Lvl);

    /// ȡ�ü��ܶ�Ӧ����ֵ
    long   GetSkillAttri(long lvl , std::string  name); 

    /// ���±����
    void   CodeGeniusToDataBlock(DBWriteSet& setWriteDB);

    /// �������½ӿ�
    void   CodeDateToClient();

private:

    void   CalcUpdateSkillPro();

    /// ���ܷ��ͱ仯�������Ա�
    void   UpdateSkillAttri(long  skillid);          

    /// ����������������
    void   Assign(std::vector<tagGeniusSkill>& GeniusVec,
                  std::vector<tagSkillEffPro>& MultiS,
                  std::vector<tagSkillEffPro>& AddSub,
                  std::vector<tagSkillEffPro>& MultiE);

private:

    typedef std::map<std::string,long>   AttriMap;
 
    
    long       m_Skillid;
    long       m_SkillLvl;
    CPlayer*   m_pOwner;

    AttriMap   m_SkillMap;
    AttriMap   m_UpdateAttr;
    
};

void   AddMpPolicy(std::map<string,long>& mp,
                   string name,
                   long temp,
                   long value);

void   SubMpPolicy(std::map<string,long>& mp,
                   string name,
                   long temp,
                   long value);

void   MultiMpPolicy(std::map<string,
                     long>& mp,
                     string name,
                     long temp,
                     long value);
