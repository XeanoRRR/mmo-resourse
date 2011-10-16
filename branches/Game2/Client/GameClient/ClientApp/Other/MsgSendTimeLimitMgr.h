/**
* @filename:MsgSendTimeLimitMgr.h
* @date: 2010/4/1
* @author:  zhaohang(kaba) (kuailekaba@gmail.com) 
      
* @purpose: �ͻ�����Ϣ����ʱ�����ƹ�����,����Chat.hʵ��
*/
enum eMsgSendType{
	MST_TYPE_CHAT_SCRIPT,//�����﷢�ͽű�
	MST_TYPE_RANK_TIME,//���а��ѯ
	MSG_TYPE_SKILL_SET_HOTKEY,//mainbar�����ü����ȼ�
	MSG_TYPE_QUEST,    //������Ϣ
	MST_NUM,			//��ѯ������Ŀ
};
class MsgSendTimeLimitMgr
{
public:
	MsgSendTimeLimitMgr();
	~MsgSendTimeLimitMgr();

	/// @brief
	/// �ж��Ƿ񳬳�ʱ������ 
	/// @param e
	/// ��Ϣ��������
	/// @return 
	/// ��� �����ͷ���true��û�У��͵�����ʾ�򣬲�����false
	static bool IsTimeOut(eMsgSendType e);

	/// @brief
	/// �����ϴη���ʱ��
	/// @param e
	/// ��Ϣ��������
	/// @return
	/// 
	static void UpdateLastSendTime(eMsgSendType e);
private:
	static ulong m_Interleave[MST_NUM]; //���ͼ��
	static ulong m_LastSendTime[MST_NUM];//�ϴη���ʱ��
};

