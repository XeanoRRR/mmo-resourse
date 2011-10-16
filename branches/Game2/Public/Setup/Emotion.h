//======================================================================================
/**
*	@file		Emotion.h
*	@data		
*	@author		
*/
//======================================================================================

#pragma once

/**
*	@class EmotionSetup
*
*	@brief ����������
*/

class EmotionSetup
{
public:
	/// ����
	EmotionSetup();

	/// ����
	~EmotionSetup();

	/// ���ر����б�
	static bool LoadSetup(const char* pFileName);

	/// ����
	static bool Serialize(vector<uchar>* pByteArray);

	/// ����
	static bool Unserialize(uchar* pByte, long& pos);

	/// ����ID��ѯ�����Ƿ��ظ�
	static BOOL IsEmotionRepeated(long lID);

	/// ȡ�ñ����б�
	static map<long, BOOL>* GetEmotionList();

private:
	/// ���鼯��
	static map<long, BOOL>	s_mEmotions;
};
