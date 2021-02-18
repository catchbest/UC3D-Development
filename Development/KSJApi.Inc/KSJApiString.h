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
	
	/// 语言类型枚举 
	enum KSJ_LANGUAGE
	{
		L_ENG,     ///< 英文
		L_CHS,     ///< 中文
	};

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ_SetLanguage
	/// @brief     设置语言
	/// @param     lang [in] 传入语言类型
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 可以在任意时刻调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_SetLanguage(IN KSJ_LANGUAGE Language);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_GetLanguage
	/// @brief     获得错误的详细信息
	/// @param     pLang [out] 输出语言类型
	/// @return    成功返回 RET_SUCCESS(0)。否则返回非0值的错误码, 请参考 KSJCode.h 中错误码的定义。
	/// @attention 可以在任意时刻调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  int __stdcall KSJ3D_GetLanguage(OUT KSJ_LANGUAGE* pLanguage);

	///-----------------------------------------------------------------------------
	///
	/// @brief     获得语言名称
	/// @param     Language [in] 传入语言类型，请参考参考KSJ_LANGUAGE
	/// @return    字符串信息地址指针
	/// @attention 调可以在任意时刻调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API PTCHAR __stdcall KSJ3D_GetLanguageString(IN KSJ_LANGUAGE Language);

	///-----------------------------------------------------------------------------
	///
	/// @brief     获得错误的详细信息
	/// @param     nErrorNo [in] 传入错误代码，参考返回状态码（KSJCode.H）
	/// @return    字符串信息地址指针
	/// @attention 调可以在任意时刻调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  PTCHAR __stdcall KSJ3D_GetReturnCodeString(IN int nErrorNo);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_GetDeviceName
	/// @brief     获得相机名称字符串
	/// @param     usDeviceType [in] 传入相机型号, 参考KSJ_DEVICETYPE的定义，可以通过KSJ_DeviceGetInformation/KSJ_DeviceGetInformationEx获得。
	/// @return    字符串信息地址指针
	/// @attention 可以在任意时刻调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API  PTCHAR __stdcall KSJ3D_GetDeviceName(int usDeviceType);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_GetTriggerModeString
	/// @brief     获得触发模式字符串
	/// @param     TriggerMode [in] 传入触发模式类型, 参考KSJ3D_DATA_TRIGGER_MODE的定义
	/// @return    字符串信息地址指针
	/// @attention 可以在任意时刻调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API PTCHAR __stdcall KSJ3D_GetTriggerModeString(KSJ3D_DATA_TRIGGER_MODE TriggerMode);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_GetTriggerSourceString
	/// @brief     获得触发源字符串
	/// @param     TriggerEdge [in] 传入触发源类型, 参考KSJ3D_START_TRIGGER_SOURCE的定义
	/// @return    字符串信息地址指针
	/// @attention 可以在任意时刻调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API PTCHAR __stdcall KSJ3D_GetTriggerSourceString(KSJ3D_START_TRIGGER_SOURCE TriggerSource);

	///-----------------------------------------------------------------------------
	///
	/// @brief     KSJ3D_GetLaserModeString
	/// @brief     获得激光模式字符串
	/// @param     TriggerEdge [in] 传入激光模式类型, 参考KSJ_LASER_MODE的定义
	/// @return    字符串信息地址指针
	/// @attention 可以在任意时刻调用
	///
	///-----------------------------------------------------------------------------
	KSJ_API PTCHAR __stdcall KSJ3D_GetLaserModeString(KSJ_LASER_MODE LaserMode);

	



#ifdef __cplusplus
}
#endif

#endif
