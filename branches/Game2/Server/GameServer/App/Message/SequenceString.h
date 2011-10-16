#pragma once

// �����������ַ�����
typedef vector<unsigned long> SequenceString;
class CSequenceString
{
	/*
	 * 	��̬
	 */
public:
	static SequenceString m_sSequenceString;	// ���ļ����������д���
	static void Initialize(ULONG cnt);
	static void Uninitialize();
	void Serialize(vector<BYTE>* pByteArray);		// ��ӵ�CByteArray
	//static void Unserialize(BYTE* pByte, long& pos);	// ����

	/*
	 * 	�Ǿ�̬
	 */
public:
	CSequenceString();

	~CSequenceString();

private:
	long m_pos;
	
public:
	bool m_bIsUsable;
	void SetPos(long pos) { m_pos = pos; }
	long GetPos() { return m_pos; }
	unsigned long GetElement();
	bool Check(unsigned long lElement);
};