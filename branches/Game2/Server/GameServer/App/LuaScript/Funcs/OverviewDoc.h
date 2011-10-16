///
/// @file OverviewDoc.h
/// @brief �ĵ���ҳ
///

/*! @mainpage �ű�����˵��
 * @section pre_sec ����
 * ���ĵ���doxygen�����Զ����ɣ����������ű��ӿڵ�ʹ�á�
 * ���еĽű��ӿڰ���������λ�ñ���Ϊ�����ࣺ
 * - ȫ�ֽű�����������ֱ��ʹ�ã��ĵ�����ʾλ�� ScriptFunc ���ֿռ���;
 * - Script���ڽű�������λ��Script���ڣ�����ͨ�� GetScript ��ȡ�����磺
 * @code local script = GetScript()
 * script:AddInfo( "�����ַ���" ) 
 * @endcode
 * <B>����λ��ǰ׺��"Script"���ļ���ĺ�������λ��Script���£��������ȫ�ֵġ�</B>
 *
 * @section module_sec ģ��ṹ
 * - <B>��������</B>: BaseFunc.h BaseFunc.cpp ScriptBaseFunc.h
 *
 * - <B>�ͻ��˽�������</B>: ClientFunc.h ClientFunc.cpp ScriptClientFunc.h
 *
 * - <B>���ắ��</B>: OrganFunc.cpp ScriptOrganFunc.h
 *
 * - <B>GM����</B>: ScriptGlobalFunc.h
 *
 * - <B>��Ʒ��������</B>: ScriptGoodsFunc.h
 *
 * - <B>�����������</B>: ScriptQuestFunc.h
 *
 * - <B>��Ҳ�������</B>: ScriptPlayerFunc.h PlayerFunc.h PlayerFunc.cpp
 *
 * - <B>Shape��������</B>: ScriptShapeFunc.h
 *
 * - <B>��������</B>: RgnFunc.h RgnFunc.cpp ScriptRgnFunc.h
 *
 * - <B>��λ����</B>: ScriptPhaseFunc.h
 *
 * - <B>���ܺ���</B>: ScriptSkillFunc.h
 *
 * - <B>�ű���������</B>: VarListFuncs.cpp
 *
 * - <B>�����޺���</B>: ScriptTauntFunc.h
 * 
 * @section global_const_sec ȫ�ֳ���˵��
 * �ű�����������ע����һЩȫ�ֳ����������ڽű������ĵ��ò�������ǿ�ɶ���
 * - <B>�ű��������</B><BR>
 * ���� AddVar()/ SetVar()/ GetVar() �ȽӿڵĲ�����������
 *	- VT_SCRIPT: �����ڼ����˺��ű����ʾ�Լ���ʵ���ϵĽű���������
 *	- VT_SRCSHAPE: ��Դ�������ϵĽű���������
 *	- VT_DESTSHAPE: ��Ŀ��������ϵĽű���������
 *	- VT_GLOBAL: ��ȫ�ֱ�����ȫ�������������Ĳ������������� SetVar()/ GetVar()�ӿ�
 *	- INVALID_NUM: ���� GetVar() ��ȡ���ֱ���ʧ��ʱ�ķ���ֵ
 * - <B>Shape����</B><BR>
 * ���� Script::SetAttr()/ Script::GetAttr() ֮��ĺ�����������
 *	- TYPE_PLAYER: �������
 *	- TYPE_MONSTER: ��������
 * - <B>���ֺ�����ִ�н��</B><BR>
 *	- EXEC_ERROR: ����ִ�д���
 *	- EXEC_FAIL: ʧ��
 *	- EXEC_SUCCESS: ִ�гɹ�
 * - <B>�ͻ��˶Ի���</B><BR>
 *	- MB_YESNO: ���� GameMessage() ��������ʾ��������"YES/NO"ѡ��ĶԻ���
 *	- MB_OK: ���� GameMessage() ��������ʾ����ȷ���Ի���
 * - <B>��������</B><BR>
 *  - RGN_NORMAL: ��ͨ��������
 *  - RGN_PERSONAL: ���˸�������
 *  - RGN_PERSONAL_HOUSE: ���˷��ݸ�������
 *  - RGN_TEAM: ���鸱������
 * - <B>���ܽű������еĳ���</B><BR>
 * �ڼ����˺��ű��������һ��VT_SCRIPT�ű������б�����ͨ�� VarListFuncs.cpp �еĽӿڴ�ȡ��
 * �ýű������б������ȶ����˼�����������Щ���������� ScriptSkillFunc.h �еĽӿڲ�����
 *  - $ParamF: ʩ�ż��ܵ�SHAPE��ʶ
 *  - $ParamS: ����ʵ����ʶ
 *  - $ParamA: ����ӵ����SHAPE��ʶ������(DE)BUFF���ԣ�ӵ���ߺ�ʩ���߲�ͬ����ͨ������ͬ
 *  - $ParamB: Ŀ��SHPAE��ʶ
 *
 * @section code_sec �ű���дע������
 * - �����������е�lua����Ϊ�ֲ��ģ��磺local a = xxx
 * - �������������Լ������lua����Ҳ�Ǿֲ��ģ��磺local function f() ... end
 * - ����Щ�����ַ����ĺ������䷵��ֵ����Ϊnil��Ҳ����Ϊ""����Ҫ���ݾ���ӿڶ���
 * - ���ű�������ָ���ǰ��ڳ�������ҡ��������ʵ���ȶ����ϵ����ݣ���lua����ı�����ͬ��
 *    �ű��������������ͣ����֡��ַ�����GUID������ GetVar()���û���ҵ�ָ���ı�����������һ
 *    ����Чֵ���ֱ��ǣ�INVALID_NUM(��������)��""(�ַ�������)��CGUID.GUID_INVALID(GUID����)��
 *    �ж�һ��GUID�Ƿ���Ч����Ϊ�գ��������Ե��ú��� CGUID::IsInvalid��
 * @section declare_global LUA�ű���ȫ�ֱ�����ʹ��
 * LUAȫ�ֱ��������ڲ�ͬ�ű�֮������ݴ��ݣ�����Ĭ��������ǲ��ܶ���ȫ�ֱ����ģ����磺
 * @code a = 1 @endcode 
 * �޷���������ȫ�ֱ�����ȡ����֮����ʹ���ض��Ľӿ������壺
 * @code
 * declare( varName, initVal ) -- ����ȫ�ֱ����������ֱ�Ϊ����������������ʼֵ
 * undeclare( varName ) -- ɾ��ȫ�ֱ���������Ϊ��������
 * -- ���ӣ�
 * declare( "a", { [1] = 2 } )
 * undeclare( "a" )
 * @endcode
 * �����ж�һ�������Ƿ�Ϊnil��ȷ�����Ƿ񱻶��壺
 * @code
 * if a == nil then ....
 * @endcode
 *
 */

