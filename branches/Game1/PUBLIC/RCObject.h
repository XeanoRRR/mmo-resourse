//=============================================================================
/**
 *  file: RCObject.h
 *
 *  Brief:���ü����Ļ���
 *
 *  Authtor:wangqiao
 *	
 *	Datae:2007-10-26
 */
//=============================================================================
#ifndef __RCOBJECT__
#define __RCOBJECT__

class RCObject
{
protected:
	RCObject(void)
	:m_nRefCount(0)
	{
		m_dwNotRefTime = timeGetTime();
	}
	RCObject(const RCObject& rhs);
	RCObject& operator=(const RCObject& rhs);
	virtual ~RCObject()
	{
	}
private:
	int m_nRefCount;
	//���ü���Ϊ0ʱ,��ʼ��ʱʱ��
	DWORD m_dwNotRefTime;
public:
	int AddRef(){return ++m_nRefCount;}

	int RemoveRef()
	{
		--m_nRefCount;
		if(m_nRefCount == 0)
			m_dwNotRefTime = timeGetTime();
		return m_nRefCount;
	}

	bool IsShared() const {return m_nRefCount>0;}

	void SetNotRefTime(DWORD dwTime)	{m_dwNotRefTime=dwTime;}
	DWORD GetNotRefTime()	{return m_dwNotRefTime;}
	int   GetRefCount(){return m_nRefCount;}
	void ResetCount()	{m_nRefCount=0;}
};

#endif
