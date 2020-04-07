#ifndef    __KSJ_INI_H__
#define    __KSJ_INI_H__

#ifdef _WIN64
#pragma message("_WIN64 defined")
#else
#pragma message("_WIN64 not defined")
#endif

#ifdef _UNICODE
#pragma message("_UNICODE defined")
#else
#pragma message("_UNICODE not defined")
#endif

#ifdef KSJINI_EXPORTS						    // 编译动态链接库时定义
    #pragma message("export dynamic ksjini lib")
	#define KSJINI_API __declspec(dllexport)  int WINAPI
#elif defined KSJINI_STATIC			            // 编译静态链接库时定义或者使用静态链接库是定义
    #pragma message("export or import static ksjini lib")  
    #define KSJINI_API int WINAPI
#else
    #pragma message("import dynamic ksjini lib")
	#define KSJINI_API __declspec(dllimport)  int WINAPI
#endif


#include <windows.h>

#define KSJINI_SUCCESS 0
#define KSJINI_FAIL    1

#ifdef __cplusplus
extern "C"{
#else
#endif

// strFullPathName必须是ini文件的完整路径。
// 初始化函数不会创建ini文件。只有对ini文件进行写入的时候才会自动创建。
KSJINI_API KSJINI_Init(OUT HANDLE *hIni, IN LPCTSTR strFullPathName);

// 根据调用者的hModule，在与调用者相同的文件夹下创建一个名字为strIniFileName的文件。
// 初始化函数不会创建ini文件。只有对ini文件进行写入的时候才会自动创建。
// hModule=NULL,表示当前的调用者。动态链接库入口函数DllMain的第一个参数HANDLE hModule可以作为动态库的hModule
KSJINI_API KSJINI_InitEx(OUT HANDLE *hIni, IN HMODULE hModule, IN LPCTSTR strIniFileName);

KSJINI_API KSJINI_UnInit( IN  HANDLE *hIni );

KSJINI_API KSJINI_GetString( IN HANDLE hIni, LPCTSTR  szSection, LPCTSTR szKey, LPCTSTR szDefault, LPTSTR szValue );
KSJINI_API KSJINI_GetBOOL  ( IN HANDLE hIni, LPCTSTR  szSection, LPCTSTR szKey, BOOL bDefault,     BOOL *bValue );
KSJINI_API KSJINI_GetDWORD ( IN HANDLE hIni, LPCTSTR  szSection, LPCTSTR szKey, DWORD dwDefault, DWORD *dwValue);


KSJINI_API KSJINI_SetString( IN HANDLE hIni, LPCTSTR  szSection, LPCTSTR  szKey, LPCTSTR strVal );
KSJINI_API KSJINI_SetInt   ( IN HANDLE hIni, LPCTSTR  szSection, LPCTSTR  szKey, int nVal       );
KSJINI_API KSJINI_SetBOOL  ( IN HANDLE hIni, LPCTSTR  szSection, LPCTSTR  szKey, BOOL bVal      );

#ifdef __cplusplus
}
#endif

#endif