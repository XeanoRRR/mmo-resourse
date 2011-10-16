//======================================================================================
/**
*	@file		ScriptAILoader.h
*	@data		Kevin Lynx
*	@author		
*	@brief		�������AI�ű�������
*/
//======================================================================================
#pragma once

/**
*	@class CScriptAILoader
*
*	@brief �������AI�ű�������
*/
class CScriptAILoader
{
public:
	/// function table [function name] - [script file]
	typedef std::map<std::string, std::string> FuncTableType;
	/// funtion table reference table [ai type] - [function table]
	typedef std::map<long, FuncTableType*> FuncRefTableType;

public:
	static CScriptAILoader &Instance()
	{
		static CScriptAILoader instance;
		return instance;
	}

	/// �������ñ�
	bool Load( const std::string &file );

	/// ����
	void AddToByteArray( std::vector<unsigned char> &OutBuf );

	/// ����
	void DecodeFromByteArray( const unsigned char *pBuf, long &lPos );

	/// ����ID��ѯ������
	FuncTableType *QueryFuncTable( long id ) const;

private:
	/// ����
	CScriptAILoader() {}

	/// ����
	~CScriptAILoader();

	/// ���ؿ�ṹ
	bool LoadOneChunk( TiXmlElement *pElem );

private:
	/// ������
	FuncRefTableType mFuncRefTable;
};
