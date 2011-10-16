

#include <list>
#include <vector>
#include <set>
#include <map>

using namespace std;

//!Ĭ�ϵ�һ���ڴ�Ķ�������
static	long	g_ObjCount = 100;
//!Ĭ�ϵĳ�ʼ�ڴ������
static	long	g_blockCount = 1;

///////////////////////////////////////////////////////////////////////////////////////////////////////////////
class ObjectPool
{
public:
	ObjectPool(void);

	~ObjectPool(void);

public:
	//!			����ĳ�ֳߴ����ķ������
	bool		SetPolicy(long size, long objCount, long blockCount);

	//!			���һ��ռ�
	void*		AllotObj(long size);

	//!			�黹һ��ռ�
	bool		FreeObj(void* p, long size = 0);

	//!			�ͷ����ж���ռ䣬�����ж���δͨ��FreeObj�ӿڹ黹�����׳��쳣
	void		Release(void);

public:
	//!			ռ�õ��ڴ�ռ��С(byte)
	long		GetBufSize(void);

	//!			����ʹ�õĿռ��С(byte)
	long		GetUesedSize(void);

	//!			δʹ�õĿռ��С(byte)
	long		GetFreeSize(void);

private:

	class Blocks
	{
	public:
		Blocks(long objSize, long blockObjNum = g_ObjCount, long g_blockCount = 1);

		~Blocks(void);
		

	public:
		void*				alloc_obj(void);

		bool				free_obj(void *p);

		bool				free_buf(void);

	public:
		long				get_buf_size(void);

		long				get_uesed_size(void);

		long				get_free_size(void);

	private:
		set<void*>			m_useSet;
		list<void*>			m_unUseList;
		vector<char*>		m_bufs;
		long				m_objSize;
		long				m_num;
	};

	//!�ڴ���б�<�ߴ�, �����>
	map<long, Blocks*>				m_SizeTable;

};