// StartTrigger.cpp : 实现文件
//

#include "stdafx.h"
#include "KSJShow3D.h"
#include "StartTrigger.h"
#include "afxdialogex.h"


// CStartTrigger 对话框

IMPLEMENT_DYNAMIC(CStartTrigger, CDialogEx)

CStartTrigger::CStartTrigger(CWnd* pParent /*=NULL*/)
	: CDialogEx(CStartTrigger::IDD, pParent)
{

}

CStartTrigger::~CStartTrigger()
{
}

void CStartTrigger::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CStartTrigger, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_STARTMODE, &CStartTrigger::OnCbnSelchangeComboStartmode)
	ON_EN_CHANGE(IDC_EDIT_STARTDELAY, &CStartTrigger::OnEnChangeEditStartdelay)
	ON_EN_CHANGE(IDC_EDIT_FILTER, &CStartTrigger::OnEnChangeEditFilter)
END_MESSAGE_MAP()


// CStartTrigger 消息处理程序


void CStartTrigger::OnCbnSelchangeComboStartmode()
{
	if (m_nDeviceCurSel == -1)   return;
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_STARTMODE);
	m_StartTriggerCondition = pComboBox->GetCurSel();
	if (m_StartTriggerCondition == CB_ERR)    return;
	KSJ3D_TRIGGER_EDGE_MODE StartTriggerCondition = TEM_FALLING_EDGE;
	if (m_StartTriggerCondition != 0) StartTriggerCondition = (KSJ3D_TRIGGER_EDGE_MODE)(m_StartTriggerCondition - 1);
	else StartTriggerCondition = (KSJ3D_TRIGGER_EDGE_MODE)m_StartTriggerCondition;

	if (m_StartTriggerCondition == 0) KSJ3D_SetStartTrigger(m_nDeviceCurSel, STS_INPUT_0, false, StartTriggerCondition);
	else KSJ3D_SetStartTrigger(m_nDeviceCurSel, STS_INPUT_0, true, StartTriggerCondition);
}


void CStartTrigger::OnEnChangeEditStartdelay()
{
	if (m_nDeviceCurSel == -1)   return;
	m_nStartTriggerDelay = GetDlgItemInt(IDC_EDIT_STARTDELAY);
	KSJ3D_SetStartTriggerParameters(m_nDeviceCurSel, m_StartTriggerFilter, m_nStartTriggerDelay);
}


void CStartTrigger::OnEnChangeEditFilter()
{
	if (m_nDeviceCurSel == -1)   return;
	m_StartTriggerFilter = GetDlgItemInt(IDC_EDIT_FILTER);
	KSJ3D_SetStartTriggerParameters(m_nDeviceCurSel, m_StartTriggerFilter, m_nStartTriggerDelay);
}


BOOL CStartTrigger::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	TCHAR szStartEndMode[3][32] =
	{
		_T("disable"),
		_T("Rising Edge"),
		_T("Falling Edge"),
	};

	int i;
	CComboBox    *pComboBox;
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_STARTMODE);

	//启动触发
	pComboBox->ResetContent();

	for (i = 0; i < 3; i++)
	{
		pComboBox->AddString(szStartEndMode[i]);
	}

	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_STARTDELAY))->SetRange32(0, 65535);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_FILTER))->SetRange32(0, 65535);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CStartTrigger::Readini(HANDLE hKSJIni)
{
	unsigned long usvalue = 0;
	int nvalue, nvalue2;
	CComboBox    *pComboBox;
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_STARTMODE);
	KSJINI_GetDWORD(hKSJIni, _T("StartTrigger"), _T("Mode"), 0, (DWORD*)&nvalue);
	pComboBox->SetCurSel(nvalue);
	OnCbnSelchangeComboStartmode();
	KSJ3D_GetStartTriggerParameters(m_nDeviceCurSel, &nvalue, &nvalue2);
	KSJINI_GetDWORD(hKSJIni, _T("StartTrigger"), _T("Delay"), nvalue, (DWORD*)&m_nStartTriggerDelay);
	KSJINI_GetDWORD(hKSJIni, _T("StartTrigger"), _T("Filter"), nvalue2, (DWORD*)&m_StartTriggerFilter);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_STARTDELAY))->SetPos32(m_nStartTriggerDelay);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_FILTER))->SetPos32(m_StartTriggerFilter);
	KSJ3D_SetStartTriggerParameters(m_nDeviceCurSel, m_StartTriggerFilter, m_nStartTriggerDelay);
}

void CStartTrigger::Writeini(HANDLE hKSJIni)
{
	KSJINI_SetInt(hKSJIni, _T("StartTrigger"), _T("Mode"), m_StartTriggerCondition);
	KSJINI_SetInt(hKSJIni, _T("StartTrigger"), _T("Delay"), m_nStartTriggerDelay);
	KSJINI_SetInt(hKSJIni, _T("StartTrigger"), _T("Filter"), m_StartTriggerFilter);
}

void CStartTrigger::SetEnable(BOOL bEnable)
{
	GetDlgItem(IDC_COMBO_STARTMODE)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_STARTDELAY)->EnableWindow(bEnable);
	GetDlgItem(IDC_SPIN_STARTDELAY)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_FILTER)->EnableWindow(bEnable);
	GetDlgItem(IDC_SPIN_FILTER)->EnableWindow(bEnable);
}