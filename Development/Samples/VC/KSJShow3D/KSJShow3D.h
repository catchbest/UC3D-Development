
// KSJShow3D.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// CKSJShow3DApp: 
// �йش����ʵ�֣������ KSJShow3D.cpp
//

class CKSJShow3DApp : public CWinApp
{
public:
	CKSJShow3DApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CKSJShow3DApp theApp;
