/** 
 *  @file   PhaseDef.h
 *  @author
 *
 *  @brief  �ṩ�ͻ��ͷ���������Ŀ�����Ĳ�������
 *  @date   2010/03/18
 */

#pragma  once 

/// ����״̬
enum  Follow_State
{
    Not_Follow_State,                   /// ��������״̬
    In_Follow_State                     /// ����״̬
};

/// ��������
enum  TEAM_FOLLOW_TYPE
{
    TF_Team_Leader_Invite=1,            /// �ӳ�������Ѹ���ӳ�
    TF_Team_Return_TeamFollow,          /// ��Ա�ظ��Ƿ������Ӹ���״̬
    TF_Team_Member_Follow_Leader,       /// ����������������ӳ�
    TF_Team_Stop_TeamFollow,            /// ����ֹͣ����
    TF_Team_Member_ChangeRgn,           /// ��Ա���������л�����
};

/// �������
enum TEAM_FOLLOW_RESULT
{
    TF_Result_InValid = 0,              /// ��ֹ����
    TF_Result_Not_Team,                 /// û�ж��鲻�ܽ��и���
    TF_Result_Not_SameGS,               /// ���ܿ�GSѰ·
    TF_Result_Not_Follow_Self,          /// �ӳ����ܸ�������
    TF_Result_Refuse_Follow,            /// ��Ա�ܾ�����ӳ�
    TF_Result_Enter_Follow,             /// ��Ա������Ӹ���״̬
    TF_Result_Stop_Follow,              /// ��Ա�˳���Ӹ���״̬
    TF_Result_Follow_Flag,              /// ֪ͨ�ͻ��˽�����Ӹ���״̬�ı��
    TF_Result_Too_Far,                  /// ��ӳ�̫Զ�ˣ������ܸ��棩
    TF_Error_In_Team_Follow,            /// �Ѿ�������Ӹ���״̬����������ĳЩ������
    TF_Errow_Not_In_Follow_Mode,        /// ����Ŀǰ�����ڸ���״̬
    TF_Result_Follow_Success,           /// ��Ӹ���ɹ�
};
