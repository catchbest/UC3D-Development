
// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // 从 Windows 头中排除极少使用的资料
#endif

#include "targetver.h"
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

// 关闭 MFC 对某些常见但经常可放心忽略的警告消息的隐藏
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC 核心组件和标准组件
#include <afxext.h>         // MFC 扩展


#include <afxdisp.h>        // MFC 自动化类
#include <tchar.h>   
#include "../../common/KSJ_GS.H"
#define MSG_TRACE WM_USER + 110
#define MSG_CLEAR_TRACE WM_USER + 111
// 这个是需要包含的头文件设置，路径是相对于工程的相对路径
#include "../../../KSJApi.Inc/KSJApi3D.h"
#include "../../../KSJApi.Inc/KSJApiString.h"
#include "../../../KSJApi.Inc/KSJCode.h"
#include "KSJImageZoomMode.Inc/KSJImageZoomMode.h"
#include "KSJIni.Inc/KSJIni.h"


#ifdef _WIN64
#pragma comment( lib, "../../../KSJApi.Lib/x64/KSJApi64.lib" )
#pragma comment( lib, "KSJImageZoomMode.Lib\\win32\\MT_KSJImageZoomMode_x64.Lib")
#pragma comment( lib, "KSJIni.Lib\\win32\\MT_KSJIni_x64.lib" )
#else
#pragma comment( lib, "../../../KSJApi.Lib/x86/KSJApid.lib" )
#pragma comment( lib, "KSJImageZoomMode.Lib\\win32\\MT_KSJImageZoomMode_x86.Lib")
#pragma comment( lib, "KSJIni.Lib\\win32\\MT_KSJIni_x86.lib" )
#endif

#define BUFFER_NUM  64
extern TCHAR     g_szTraceInfo[BUFFER_NUM][256];    /// 做一个buffer队列，以防止PostMessage乱序
extern int       g_nTraceNum;

#define TRACE_API(_FUNCTION_,_HANDLE_) \
{\
	CWnd*       handle = _HANDLE_; \
	int       nReturnCode; \
	TCHAR    *pszReturnCode = NULL; \
	TCHAR    *pszFunctionName = _T(#_FUNCTION_); \
	nReturnCode = _FUNCTION_; \
	_stprintf_s(g_szTraceInfo[g_nTraceNum % BUFFER_NUM], 256, _T("%s, %s"), KSJ3D_GetReturnCodeString(nReturnCode), pszFunctionName); \
	handle->PostMessage(MSG_TRACE, 0, (LPARAM)g_szTraceInfo[g_nTraceNum % BUFFER_NUM]); \
	g_nTraceNum++; \
}

// 这是需要链接的库文件设置，路径是相对于工程的相对路径



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC 对 Internet Explorer 4 公共控件的支持
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC 对 Windows 公共控件的支持
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // 功能区和控件条的 MFC 支持









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


