/** 
 *  @file   GeniusSetup.h
 *
 *  @brief  �����츳������
 *  @author Expter
 */

#pragma  once 

#include "../Common/GeniusDef.h"

class  GeniusSetup
{

public:
    /// ���캯��
    GeniusSetup();
    ~GeniusSetup();

    /// ���������ļ�
    bool  Load(const char* filename="Data/GeniusSys/GeniusList.xml");

    /// �����츳�б�
    bool  LoadList(const char* filename);

    /// ����
    void  GeniusConfigEnCode(DBWriteSet&  dbWrite);

    /// ����
    void  GeniusConfigDecode(DBReadSet& dbRead);

    /// �õ���ǰ�����������츳�б�
    std::map<long,vector<long> >  GetGeniusList(long skillid);

    /// �õ����ܱ�
    void  GetSkillList(long geniusid,long lvl,std::map<long,long>&  mp);

    /// 
    /// �õ��츳id��ĳ���ȼ���ȫ���������ñ�
    /// id  �츳id
    /// skill ����id
    /// lvl �츳�ȼ�
    ///
    tagGeniusSkill*   GetGeniusVec(long id,long skill,long lvl);

    /// �Ƿ���ڴ��츳
    bool  FindGenius(long Geniusid);

    /// �õ��츳�˷�����
    long  GetGeniusBase()const {    return m_BaseNum;   }

    /// �Ƿ��Ǹ�������
    bool  IsSkillUpdatePro( std::string pro) ;

private:

    /// ��������б�
    bool    LoadSkillPro();

    /// ��ʽ���ַ���
    ObjType ForMatObjType(string str);

    EffObj  ForMatEffObj( string str);

    GeniusOper ForMatOper( string str);

private:
    /// �������
    long                          m_BaseNum;
    /// ��������
    std::string                   m_UpdateSkillPro;
    /// ��������
    std::set<string>              m_UpdateSkillProTable;
    /// ��Ա����
    std::map<long,tagGenius>      m_GeniusConfig;    
    /// ���ܶ�Ӧ Ӱ����츳��
    std::map<long,std::map<long,vector<long> > >  m_SkillTable;
    /// ���е��ļ��б�
    std::vector<std::string>      m_filelist;
};
