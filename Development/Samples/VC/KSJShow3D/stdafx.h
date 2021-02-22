
// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

#pragma once

#ifndef VC_EXTRALEAN
#define VC_EXTRALEAN            // �� Windows ͷ���ų�����ʹ�õ�����
#endif

#include "targetver.h"
#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // ĳЩ CString ���캯��������ʽ��

// �ر� MFC ��ĳЩ�����������ɷ��ĺ��Եľ�����Ϣ������
#define _AFX_ALL_WARNINGS

#include <afxwin.h>         // MFC ��������ͱ�׼���
#include <afxext.h>         // MFC ��չ


#include <afxdisp.h>        // MFC �Զ�����
#include <tchar.h>   
#include "../../common/KSJ_GS.H"
#define MSG_TRACE WM_USER + 110
#define MSG_CLEAR_TRACE WM_USER + 111
// �������Ҫ������ͷ�ļ����ã�·��������ڹ��̵����·��
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
extern TCHAR     g_szTraceInfo[BUFFER_NUM][256];    /// ��һ��buffer���У��Է�ֹPostMessage����
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

// ������Ҫ���ӵĿ��ļ����ã�·��������ڹ��̵����·��



#ifndef _AFX_NO_OLE_SUPPORT
#include <afxdtctl.h>           // MFC �� Internet Explorer 4 �����ؼ���֧��
#endif
#ifndef _AFX_NO_AFXCMN_SUPPORT
#include <afxcmn.h>             // MFC �� Windows �����ؼ���֧��
#endif // _AFX_NO_AFXCMN_SUPPORT

#include <afxcontrolbars.h>     // �������Ϳؼ����� MFC ֧��









#ifdef _UNICODE
#if defined _M_IX86
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='x86' publicKeyToken='6595b64144ccf1df' language='*'\"")
#elif defined _M_X64
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='amd64' publicKeyToken='6595b64144ccf1df' language='*'\"")
#else
#pragma comment(linker,"/manifestdependency:\"type='win32' name='Microsoft.Windows.Common-Controls' version='6.0.0.0' processorArchitecture='*' publicKeyToken='6595b64144ccf1df' language='*'\"")
#endif
#endif


