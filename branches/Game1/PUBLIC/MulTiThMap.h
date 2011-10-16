#pragma once
//�ṩһ���Զ��̲߳�����map
//��ʾ:����ǰִ��Lock(),������ִ��UnLock();
//Author:Joe
//Create Time:2004.6.17
template<class TKey,class TData>
class MulTiThMap:
	public map<TKey,TData>
{
private:
	CRITICAL_SECTION	csMap;
public:
	MulTiThMap()	{InitializeCriticalSection(&csMap);}
	~MulTiThMap()	{DeleteCriticalSection(&csMap);	}
	void Lock()		{EnterCriticalSection(&csMap);}
	void UnLock()	{LeaveCriticalSection(&csMap);}

};