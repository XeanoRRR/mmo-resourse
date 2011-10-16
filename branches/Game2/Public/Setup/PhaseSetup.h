/** 
 *  @file   PhaseSetup.h
 *  @author Expter
 *
 *  @brief  ������λ���ܵķ������������ļ�����
 *  @date   2010/03/18
 */

#pragma once

class PhaseSetup
{

public:
    /// ������Ϣ�ṹ������
    struct  Config 
    {
        long  lAgonal;
        long  lHpNum;
        long  lDropGoodNum;
        long  lIsOpen;
    };

    typedef std::map<long,std::string>     PhaseTable;
    typedef PhaseTable::iterator           PIter;
    typedef std::map<long,PhaseTable>      PhaseHash;
    typedef PhaseHash::iterator            PHITER;
    typedef PhaseHash::const_iterator      PHCITER;
    typedef PhaseTable::const_iterator     PCIter;

public:

    /// ���캯��
    PhaseSetup(void);

    ~PhaseSetup(void);

    ///  �����ͷ�
    void Release();

    ///  �����ļ���ȡ
    long Load();

    ///  ������������
    void PhaseConfigDecode(DBReadSet& dbRead) ;

    ///  ������������
    void PhaseConfigEnCode(DBWriteSet&  dbWrite);

    ///  ���ܿ���
    int  PhaseEnable()  const  { return m_config.lIsOpen ; }

    ///  ������������
    Config   GetConfig() const { return m_config;  }

    ///  ��õ�ǰӦ�ô����ű��ļ���
    const char*  GetScript(long Scriptid , long type = 0 )const ;

private:
    ///  ����ͳһ������Ϣ
    Config              m_config;
    ///  �ű������б�
    PhaseHash           m_PhaseTable;
};
