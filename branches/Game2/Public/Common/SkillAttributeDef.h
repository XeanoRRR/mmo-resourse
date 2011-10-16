/** 
 *  @brief  ���ڼ��ܸ������Եľ�����λ���� 
 *  @author expter
 */


#pragma once 


#define  SKILLPRODISPLAY(v,s)     #v#s
    
#define  SKILLPRO_DISPLAY(v,s)    v##s

#define  BIND_SKILLPRO(mp,name,value) \
    mp[name]=value;

#define  BIND_SKILLPRO_EXED(mp,num,value) \
    mp[ SKILLPRODISPLAY(value,num) ] = SKILLPRO_DISPLAY(value,num) 

#define  BIND_SKILLPRO_STATE_EXED(mp,num)\
    BIND_SKILLPRO_EXED(mp,num,BufId);\
    BIND_SKILLPRO_EXED(mp,num,BufLvl);\
    BIND_SKILLPRO_EXED(mp,num,BufNum)  


struct tagSkillAttr 
{
    short    TempId;           ///< ��ǰ�ȼ�����template id
    short    MinAtkDistance;   ///< ��С��������
    short    MaxAtkDistance;   ///< ��󹥻�����
    short    BuffPower;        ///< ����ǿ�� 
    short    IntonateTime;     ///< ����ʱ��  c
    short    CoolDown;         ///< ��ȴʱ��  c
    short    ActTime;          ///< �ͷŶ���ʱ��  c
    short    HitTime;          ///< �ͷź�����ʱ�� c
    short    FlyDelay;         ///< �����ӳ�ʱ�� c
    short    LoopCount;        ///< ѭ��ʱ�� c
    short    HurtFactor;       ///< �˺�ϵ��
    short    HealFactor;       ///< ����ϵ��
    short    BaseHurt;         ///< �̶��˺�
    short    BaseHeal;         ///< �̶�����
    short    EHit;             ///< ���ӹ���
    short    EHeal;            ///< ��������
    short    EHurtValue;       ///< �����˺�ֵ
    short    EHealValue;       ///< ��������ֵ
    short    SEnergy;          ///< ����
    short    SMp;              ///< ����
    short    SHp;              ///< ��Ѫ
    short    SStrength;        ///< ����
    short    SDexterity;       ///< ��
    short    SCon ;            ///< ����
    short    SIntellect;       ///< ��־
    short    SSpiritualism;    ///< ����
    short    TaoismLvl;        ///< ��Ч�����ȼ�
    short    OutEnergyPower;   ///< �⹦ǿ��
    short    OutEnergyHurt;    ///< �⹦�˺�
    short    EEnergyHit ;      ///< �ڹ��˺��ӳ�
    short    EEnergyHeal ;     ///< �ڹ����Ƽӳ�
    short    OutEnergyHitLvl;  ///< �⹦���еȼ�
    short    EnergyHitLvl;     ///< �ڹ����еȼ�
    short    OutEnergyCriRatio;///< �⹦�����ȼ�
    short    EnergyCriRatio ;  ///< �ڹ������ȼ�
    short    OutEnergyCri ;    ///< �⹦�����˺�
    short    EnergyCri ;       ///< �ڹ������˺�
    short    OutEnergySpeed ;  ///< �⹦���ٵȼ�
    short    EnergySpeed ;     ///< �ڹ����ٵȼ�
    short    IgnoreTarget;     ///< �����ȼ�
    short    ArmorDef;         ///< ����ֵ
    short    DefLvl;           ///< �����ȼ�
    short    ParryLvl;         ///< �мܵȼ�
    short    ToughnessLvl;     ///< ���Եȼ�
    short    BrokenArmorLvl;   ///< �Ƽ׵ȼ�
    short    MissRatio;        ///< ʧ��
    short    TaoismHurt;       ///< �����˺�
    short    TaoismHitLvl;     ///< �������еȼ�
    short    TaoismHitRatio;   ///< ���������ȼ�
    short    TaoismSpeed;      ///< �������ٵȼ�
    short    TaoismCri;        ///< ���������˺�
    short    TaosimRiftLvl;    ///< ������͸�ȼ�
    short    TaosimDefRatio;   ///< ���������ȼ�
    short    ERation;          ///< ���ʱ�������
    short    EArmorHp;         ///< ������������
    short    AttackSpeed;      ///< �����ٶ�   c maybe
    short    MoveSpeed;        ///< �ƶ��ٶ�   c maybe
    short    ReadyTime;        ///< ׼��ʱ��   c maybe
    short    GridFlyTime;      ///< ÿ�����ʱ�� c maybe
    short    RangeTimes;       ///< ���÷�ΧĿ����� c maybe
    short    RangeCount;       ///< ���÷�ΧĿ����� c maybe
    short    BufId1;           ///< ����״̬Buff1�ļ���id
    short    BufLvl1;          ///< ����״̬��buff1�ĸ��ӵȼ�
    short    BufNum1;          ///< ����״̬buff1�ĸ��Ӳ���
    short    BufId2;           ///< ����״̬Buff2�ļ���id
    short    BufLvl2;          ///< ����״̬��buff2�ĸ��ӵȼ�
    short    BufNum2;          ///< ����״̬buff2�ĸ��Ӳ���
    short    BufId3;           ///< ����״̬Buff3�ļ���id
    short    BufLvl3;          ///< ����״̬��buff3�ĸ��ӵȼ�
    short    BufNum3;          ///< ����״̬buff3�ĸ��Ӳ���
    short    BufId4;           ///< ����״̬Buff4�ļ���id
    short    BufLvl4;          ///< ����״̬��buff4�ĸ��ӵȼ�
    short    BufNum4;          ///< ����״̬buff4�ĸ��Ӳ���
    short    BufId5;           ///< ����״̬Buff5�ļ���id
    short    BufLvl5;          ///< ����״̬��buff5�ĸ��ӵȼ�
    short    BufNum5;          ///< ����״̬buff5�ĸ��Ӳ���
    short    BufId6;           ///< ����״̬Buff6�ļ���id
    short    BufLvl6;          ///< ����״̬��buff6�ĸ��ӵȼ�
    short    BufNum6;          ///< ����״̬buff6�ĸ��Ӳ���


    tagSkillAttr()
    {
        memset( this , 0 , sizeof(tagSkillAttr) );
    }

    void   AddMapTable( std::map<string,short>&  mp)
    {
        BIND_SKILLPRO(mp,"TemplateId",TempId);
        BIND_SKILLPRO(mp,"MinAtkDistance",MinAtkDistance);
        BIND_SKILLPRO(mp,"MaxAtkDistance",MaxAtkDistance);
        BIND_SKILLPRO(mp,"BuffPower",BuffPower);
        BIND_SKILLPRO(mp,"IntonateTime",IntonateTime);
        BIND_SKILLPRO(mp,"CoolDown",CoolDown);
        BIND_SKILLPRO(mp,"ActTime",ActTime);
        BIND_SKILLPRO(mp,"HitTime",HitTime);
        BIND_SKILLPRO(mp,"FlyDelay",FlyDelay);
        BIND_SKILLPRO(mp,"LoopCount",LoopCount);     
        BIND_SKILLPRO(mp,"HurtFactor",HurtFactor);      
        BIND_SKILLPRO(mp,"HealFactor",HealFactor);     
        BIND_SKILLPRO(mp,"BaseHurt",BaseHurt);        
        BIND_SKILLPRO(mp,"BaseHeal",BaseHeal);       
        BIND_SKILLPRO(mp,"EHit",EHit);            
        BIND_SKILLPRO(mp,"EHeal",EHeal);           
        BIND_SKILLPRO(mp,"EHurtValue",EHurtValue);      
        BIND_SKILLPRO(mp,"EHealValue",EHealValue);      
        BIND_SKILLPRO(mp,"SEnergy",SEnergy);          
        BIND_SKILLPRO(mp,"SMp",SMp);            
        BIND_SKILLPRO(mp,"SHp",SHp);             
        BIND_SKILLPRO(mp,"SStrength",SStrength);        
        BIND_SKILLPRO(mp,"SDexterity",SDexterity);      
        BIND_SKILLPRO(mp,"SCon",SCon);            
        BIND_SKILLPRO(mp,"SIntellect",SIntellect);      
        BIND_SKILLPRO(mp,"SSpiritualism",SSpiritualism);    
        BIND_SKILLPRO(mp,"TaoismLvl",TaoismLvl);      
        BIND_SKILLPRO(mp,"OutEnergyPower",OutEnergyPower); 
        BIND_SKILLPRO(mp,"OutEnergyHurt",OutEnergyHurt);  
        BIND_SKILLPRO(mp,"EEnergyHit",EEnergyHit);    
        BIND_SKILLPRO(mp,"EEnergyHeal",EEnergyHeal);   
        BIND_SKILLPRO(mp,"OutEnergyHitLvl",OutEnergyHitLvl);    
        BIND_SKILLPRO(mp,"EnergyHitLvl",EnergyHitLvl);       
        BIND_SKILLPRO(mp,"OutEnergyCriRatio",OutEnergyCriRatio);
        BIND_SKILLPRO(mp,"EnergyCriRatio",EnergyCriRatio); 
        BIND_SKILLPRO(mp,"OutEnergyCri",OutEnergyCri);   
        BIND_SKILLPRO(mp,"EnergyCri",EnergyCri);       
        BIND_SKILLPRO(mp,"OutEnergySpeed",OutEnergySpeed);  
        BIND_SKILLPRO(mp,"EnergySpeed",EnergySpeed);    
        BIND_SKILLPRO(mp,"IgnoreTarget",IgnoreTarget);    
        BIND_SKILLPRO(mp,"ArmorDef",ArmorDef);        
        BIND_SKILLPRO(mp,"DefLvl",DefLvl);         
        BIND_SKILLPRO(mp,"ParryLvl",ParryLvl);       
        BIND_SKILLPRO(mp,"ToughnessLvl",ToughnessLvl);    
        BIND_SKILLPRO(mp,"BrokenArmorLvl",BrokenArmorLvl);  
        BIND_SKILLPRO(mp,"MissRatio",MissRatio);      
        BIND_SKILLPRO(mp,"TaoismHurt",TaoismHurt);      
        BIND_SKILLPRO(mp,"TaoismHitLvl",TaoismHitLvl);    
        BIND_SKILLPRO(mp,"TaoismHitRatio",TaoismHitRatio);  
        BIND_SKILLPRO(mp,"TaoismSpeed",TaoismSpeed);    
        BIND_SKILLPRO(mp,"TaoismCri",TaoismCri);       
        BIND_SKILLPRO(mp,"TaosimRiftLvl",TaosimRiftLvl);  
        BIND_SKILLPRO(mp,"TaosimDefRatio",TaosimDefRatio);  
        BIND_SKILLPRO(mp,"ERation",ERation);         
        BIND_SKILLPRO(mp,"EArmorHp",EArmorHp);      
        BIND_SKILLPRO(mp,"AttackSpeed",AttackSpeed);
        BIND_SKILLPRO(mp,"MoveSpeed",MoveSpeed);
        BIND_SKILLPRO(mp,"ReadyTime",ReadyTime);
        BIND_SKILLPRO(mp,"GridFlyTime",GridFlyTime);
        BIND_SKILLPRO(mp,"RangeTimes",RangeTimes);
        BIND_SKILLPRO(mp,"RangeCount",RangeCount);


        BIND_SKILLPRO_STATE_EXED(mp,1);
        BIND_SKILLPRO_STATE_EXED(mp,2);
        BIND_SKILLPRO_STATE_EXED(mp,3);
        BIND_SKILLPRO_STATE_EXED(mp,4);
        BIND_SKILLPRO_STATE_EXED(mp,5);
        BIND_SKILLPRO_STATE_EXED(mp,6);
 
    }
};
