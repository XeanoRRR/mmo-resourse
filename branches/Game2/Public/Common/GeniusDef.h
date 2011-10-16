/** 
 *  @brief  �����츳��������Ϣ
 *  @author expter
 */

#pragma  once 

struct  tagGeniusPro
{
    tagGeniusPro()
    {
        memset( this , 0 , sizeof( tagGeniusPro) );
    }
    long  id;               /// �츳Id
    long  skillNum;         /// �츳Ӱ�켼������
    long  maxLvl;           /// ��ߵȼ�
    char  desid[16];        /// �츳����ID
};

/// ���Ӷ���
enum  EffObj
{
    Obj_ESelf=1,
    Obj_EDest,
};

/// ��������
enum ObjType
{
    Obj_Invalid=-1,
    Obj_Pet             =0x0002,
    Obj_Monster         =0x0004,
    Obj_Player          =0x0008,    
    Obj_Monster_Player  =0x000C,
    Obj_Monster_Pet     =0x0006,
    Obj_Player_Pet      =0x000A,
    Obj_AllObj          =0x000F,
};

/// ���㹫ʽ
enum GeniusOper
{
    No_Type=0,
    Add_Type=1,
    Sub_Type,
    Multi_Type_S,          /// �ȳ˷� �ڼӼ�
    Multi_Type_E,          /// �ȼӼ��ڳ˷�
};

/// ��������ֵ
struct tagSkillEffPro 
{
     tagSkillEffPro()
     {
         memset( this , 0 , sizeof(tagSkillEffPro) );
     }
     GeniusOper eff;        
     long       value; 
     char       type[16];
};

/// �츳���Ӽ���buff
struct tagGeniusBuff 
{
    tagGeniusBuff()
    {
        memset( this , 0 ,sizeof(tagGeniusBuff) );
    }
    long        AnnexNum;
    long        Level;
    long        AnnexId;
    ObjType     objType;
    EffObj      effobj;
};

/// һ���츳�ȼ������ĳ����������
struct  tagGeniusSkill 
{
    long        id;
    long        AnnexSum;
    std::vector<tagGeniusBuff>   GeniusBuff;
    std::vector<tagSkillEffPro>  SkillPro;

    tagGeniusSkill():id(0),AnnexSum(0)
    {
        GeniusBuff.clear();
        SkillPro.clear();
    }
};

/// һ���츳�ȼ���Ӧ�ļ���map
struct  tagGeniusLvl 
{
    tagGeniusLvl()
    {
        GeniusSkill.clear();
    }
    std::map<long,tagGeniusSkill>   GeniusSkill;
};

/// һ���츳ȫ������
struct  tagGenius 
{
    tagGenius()
    {
       GeniusConfig.clear();
    }
    tagGeniusPro                    GeniusPro;  
    /// ȫ���ȼ���Ӧmap
    std::map<long,tagGeniusLvl>     GeniusConfig;
};