#ifndef __KSJ_API_STRING3D_H__
#define __KSJ_API_STRING3D_H__

#include "KSJApi3D.h"

// #pragma message("Include KSJApiBase.h") 

#ifdef KSJAPI_EXPORTS
#define KSJ_API __declspec(dllexport)
#elif defined KSJAPI_STATIC
#define KSJ_API
#else
#define KSJ_API __declspec(dllimport)
#endif

#ifndef IN
#define IN
#endif

#ifndef OUT
#define OUT
#endif

#ifndef OPTIONAL
#define OPTIONAL
#endif

#ifdef __cplusplus
extern "C"{
#endif
	
	/// ��������ö�� 
	enum KSJ_LANGUAGE
	{
		L_ENG,     ///< Ӣ��
		L_CHS,     ///< ����
	};

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ_SetLanguage
	/// @brief     ��������
	/// @param     lang [in] ������������
	/// @return    �ɹ����� RET_SUCCESS(0)�����򷵻ط�0ֵ�Ĵ�����, ��ο� KSJCode.h �д�����Ķ��塣
	/// @attention ����������ʱ�̵���
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_SetLanguage(IN KSJ_LANGUAGE Language);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_GetLanguage
	/// @brief     ��ô������ϸ��Ϣ
	/// @param     pLang [out] �����������
	/// @return    �ɹ����� RET_SUCCESS(0)�����򷵻ط�0ֵ�Ĵ�����, ��ο� KSJCode.h �д�����Ķ��塣
	/// @attention ����������ʱ�̵���
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_GetLanguage(OUT KSJ_LANGUAGE* pLanguage);

	///-----------------------------------------------------------------------------
	///
	/// @brief     �����������
	/// @param     Language [in] �����������ͣ���ο��ο�KSJ_LANGUAGE
	/// @return    �ַ�����Ϣ��ַָ��
	/// @attention ������������ʱ�̵���
	///
	///-----------------------------------------------------------------------------
	KSJ_API PTCHAR __stdcall KSJ3D_GetLanguageString(IN KSJ_LANGUAGE Language);

	///-----------------------------------------------------------------------------
	///
	/// @brief     ��ô������ϸ��Ϣ
	/// @param     nErrorNo [in] ���������룬�ο�����״̬�루KSJCode.H��
	/// @return    �ַ�����Ϣ��ַָ��
	/// @attention ������������ʱ�̵���
	///
	///-----------------------------------------------------------------------------
	KSJ_API  PTCHAR __stdcall KSJ3D_GetReturnCodeString(IN int nErrorNo);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_GetDeviceName
	/// @brief     �����������ַ���
	/// @param     usDeviceType [in] ��������ͺ�, �ο�KSJ_DEVICETYPE�Ķ��壬����ͨ��KSJ_DeviceGetInformation/KSJ_DeviceGetInformationEx��á�
	/// @return    �ַ�����Ϣ��ַָ��
	/// @attention ����������ʱ�̵���
	///
	///-----------------------------------------------------------------------------
	KSJ_API  PTCHAR __stdcall KSJ3D_GetDeviceName(int usDeviceType);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_GetTriggerModeString
	/// @brief     ��ô���ģʽ�ַ���
	/// @param     TriggerMode [in] ���봥��ģʽ����, �ο�KSJ3D_DATA_TRIGGER_MODE�Ķ���
	/// @return    �ַ�����Ϣ��ַָ��
	/// @attention ����������ʱ�̵���
	///
	///-----------------------------------------------------------------------------
	KSJ_API PTCHAR __stdcall KSJ3D_GetTriggerModeString(KSJ3D_DATA_TRIGGER_MODE TriggerMode);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_GetTriggerSourceString
	/// @brief     ��ô���Դ�ַ���
	/// @param     TriggerEdge [in] ���봥��Դ����, �ο�KSJ3D_START_TRIGGER_SOURCE�Ķ���
	/// @return    �ַ�����Ϣ��ַָ��
	/// @attention ����������ʱ�̵���
	///
	///-----------------------------------------------------------------------------
	KSJ_API PTCHAR __stdcall KSJ3D_GetTriggerSourceString(KSJ3D_START_TRIGGER_SOURCE TriggerSource);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_GetLaserModeString
	/// @brief     ��ü���ģʽ�ַ���
	/// @param     TriggerEdge [in] ���뼤��ģʽ����, �ο�KSJ_LASER_MODE�Ķ���
	/// @return    �ַ�����Ϣ��ַָ��
	/// @attention ����������ʱ�̵���
	///
	///-----------------------------------------------------------------------------
	KSJ_API PTCHAR __stdcall KSJ3D_GetLaserModeString(KSJ_LASER_MODE LaserMode);

	



#ifdef __cplusplus
}
#endif

#endif
