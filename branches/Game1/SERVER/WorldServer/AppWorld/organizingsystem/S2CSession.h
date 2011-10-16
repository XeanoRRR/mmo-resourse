/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
//! �Ự��¼ģ��
//! SessionInfo��һ�����Խ��У�=��==������������
template<class SessionInfo>
class S2CSession
{
public:
	//! lMaxHoldTime���Ự�ı���ʱ�䣨��λ���룩
	//! bAllowOverlay�������»Ựʱ���Ƿ�������������ͬ�ĻỰ�����ǵ�ͬʱ�����¼�ʱ��
	S2CSession(LONG lMaxHoldTime, BOOL bAllowOverlay = TRUE)
		:
	m_lMaxHoldTime(lMaxHoldTime), 
		m_bAllowOverlay(bAllowOverlay)
	{}

	~S2CSession(void){}

public:
	//! ����һ���»Ự
	//! �������ʹ�ù���ʱ�������Ч����ʱ�䣬
	//!		�򽫲���lExceptiveHoldTime��ֵ����Ϊ����Ự��Ҫ���ֵ�ʱ��,
	//!		�������ֵֻ�Ե�ǰ�����ĻỰ��Ч������Ӱ��֮ǰ��֮��Ĳ�����
	BOOL CreateSession(const SessionInfo &Info, LONG lExceptiveHoldTime = 0)
	{
		//! ����ʱ�Ự
		LONG now_time = time(NULL);
		ClearOuttime(now_time);

		//! ���Ž��в���
		list<SessionNode>::iterator ite = m_listSession.begin();
		for (; ite != m_listSession.end(); ++ite)
		{
			if(ite->Info == Info)
			{
				if (m_bAllowOverlay)
				{
					m_listSession.erase(ite);
					break;
				}
				else
				{
					return FALSE;
				}
			}
		}

		SessionNode NewNode;
		NewNode.Info		= Info;
		if(0 != lExceptiveHoldTime)
			NewNode.lEndTime	= now_time + lExceptiveHoldTime;
		else
			NewNode.lEndTime	= now_time + m_lMaxHoldTime;

		//! �����������ʽ����Ϊ�˱�֤���а�ʱ������˳�򲻱䣬��Ҫ�ҳ�һ�����ʵ�λ�ò���
		if (0 != lExceptiveHoldTime)
		{
			list<SessionNode>::iterator ite = m_listSession.begin();
			for (; ite != m_listSession.end(); ++ite)
			{
				if (ite->lEndTime > NewNode.lEndTime)
				{
					if (m_listSession.begin() == ite)
						m_listSession.push_front(NewNode);
					else
						m_listSession.insert(--ite, NewNode);

					return TRUE;
					//! ���û���ҵ����λ�ã�˵������ֱ�ӷŵ�����ĩβ
				}
			}
		}

		//! ����ֱ�ӷ��ڶ���ĩβ
		m_listSession.push_back(NewNode);
		return TRUE;

	}
	//! ��֤һ���Ự
	//! bRemove����֤����Ƿ�ɾ���Ự
	BOOL TestSession(const SessionInfo &Info, BOOL bRemove = TRUE)
	{
		//! ����ʱ�Ự
		LONG now_time = time(NULL);
		ClearOuttime(now_time);

		list<SessionNode>::iterator ite = m_listSession.begin();
		for (; ite != m_listSession.end(); ++ite)
		{
			if(ite->Info == Info)
			{
				if (bRemove)
					m_listSession.erase(ite);
				return TRUE;
			}
		}
		return FALSE;
	}

	LONG Size(VOID)
	{
		return m_listSession.size();
	}

	//! ���������Ч�Ự��Ϊ����ĺ���������������
	//��[��һ������ָ�����] ���õĵ�һ�������ӱ����session��ȡ��
	//! [�ڶ�������pParam] ���� [��һ������ָ�����] ���õĵ�2���Զ������
	void RunFunction_ForEach(void (*pFun)(SessionInfo&, void*), void* pParam)
	{
		ClearOuttime(time(NULL));

		list<SessionNode>::iterator ite = m_listSession.begin();
		for (; ite != m_listSession.end(); ++ite)
		{
			(*pFun)(ite->Info, pParam);
		}
	}

private:
	void ClearOuttime(LONG now_time)
	{
		list<SessionNode>::iterator ite = m_listSession.begin();
		for (; ite != m_listSession.end(); ++ite)
		{
			if(ite->lEndTime < now_time)
			{
				m_listSession.erase(m_listSession.begin(), ++ite);
				return;
			}
		}
	}


private:
	//! �Ự�ڵ�
	struct  SessionNode
	{
		LONG		lEndTime;
		SessionInfo	Info;
	};

	//!					����ĻỰ
	list<SessionNode>	m_listSession;
	//!					�Ự�ʱ�䣨�룩
	LONG				m_lMaxHoldTime;
	//!					�����ĻỰ�Ѵ���ʱ���Ƿ������ǣ�����ʱ���¼�ʱ��
	BOOL				m_bAllowOverlay;
};
