// Trigger.cpp : 实现文件
//

#include "stdafx.h"
#include "KSJShow3D.h"
#include "Trigger.h"
#include "afxdialogex.h"


// CTrigger 对话框

IMPLEMENT_DYNAMIC(CTrigger, CDialogEx)

CTrigger::CTrigger(CWnd* pParent /*=NULL*/)
	: CDialogEx(CTrigger::IDD, pParent)
{

}

CTrigger::~CTrigger()
{
}

void CTrigger::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CTrigger, CDialogEx)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGERMODE, &CTrigger::OnCbnSelchangeComboTriggermode)
	ON_EN_CHANGE(IDC_EDIT_FIXED_FRAME_RATE, &CTrigger::OnEnChangeEditFixedFrameRate)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGERMETHOD, &CTrigger::OnCbnSelchangeComboTriggermethod)
	ON_EN_CHANGE(IDC_EDIT_TRIGGERDELAY, &CTrigger::OnEnChangeEditTriggerdelay)
	ON_EN_CHANGE(IDC_EDIT_PERIOD, &CTrigger::OnEnChangeEditPeriod)
	ON_EN_CHANGE(IDC_EDIT_FILTER2, &CTrigger::OnEnChangeEditFilter2)
	ON_EN_CHANGE(IDC_EDIT_Y, &CTrigger::OnEnChangeEditY)
	ON_CBN_SELCHANGE(IDC_COMBO_LASER, &CTrigger::OnCbnSelchangeComboLaser)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Y, &CTrigger::OnDeltaposSpinY)
END_MESSAGE_MAP()


// CTrigger 消息处理程序


void CTrigger::OnCbnSelchangeComboTriggermode()
{
	if (m_nDeviceCurSel == -1)   return;
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGERMODE);
	int nIndex = pComboBox->GetCurSel();
	if (nIndex == CB_ERR)    return;
	m_DataTriggerMode = (KSJ3D_DATA_TRIGGER_MODE)nIndex;
	if (nIndex == 0)
	{
		GetDlgItem(IDC_EDIT_FIXED_FRAME_RATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_FIXED_FRAME_RATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_TRIGGERMETHOD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TRIGGERDELAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_TRIGGERDELAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PERIOD)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_PERIOD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FILTER2)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_FILTER2)->EnableWindow(FALSE);
	}
	else if (nIndex == 1)
	{
		GetDlgItem(IDC_EDIT_FIXED_FRAME_RATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_FIXED_FRAME_RATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_TRIGGERMETHOD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_TRIGGERDELAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_TRIGGERDELAY)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_PERIOD)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_PERIOD)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_FILTER2)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_FILTER2)->EnableWindow(FALSE);
	}
	else if (nIndex == 2)
	{
		GetDlgItem(IDC_EDIT_FIXED_FRAME_RATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_FIXED_FRAME_RATE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_TRIGGERMETHOD)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TRIGGERDELAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_TRIGGERDELAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PERIOD)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_PERIOD)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FILTER2)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_FILTER2)->EnableWindow(TRUE);
	}

	KSJ3D_SetDataTriggerMode(m_nDeviceCurSel, m_DataTriggerMode);
}


void CTrigger::OnEnChangeEditFixedFrameRate()
{
	if (m_nDeviceCurSel == -1)   return;
	m_nInternalFrequencyHz = GetDlgItemInt(IDC_EDIT_FIXED_FRAME_RATE);
	KSJ3D_SetDataTriggerInternalFrequency(m_nDeviceCurSel, m_nInternalFrequencyHz);
}


void CTrigger::OnCbnSelchangeComboTriggermethod()
{
	if (m_nDeviceCurSel == -1)   return;
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGERMETHOD);
	int nIndex = pComboBox->GetCurSel();
	if (nIndex == CB_ERR)    return;
	m_DataTriggerCondition = (KSJ3D_TRIGGER_EDGE_MODE)nIndex;
	KSJ3D_SetDataTriggerExternalTriggerParameters(m_nDeviceCurSel, m_nDataTriggerDivider, m_nDataTriggerDelay, m_nDataTriggerFilter, m_DataTriggerCondition);
}


void CTrigger::OnEnChangeEditTriggerdelay()
{
	if (m_nDeviceCurSel == -1)   return;
	m_nDataTriggerDelay = GetDlgItemInt(IDC_EDIT_TRIGGERDELAY);
	KSJ3D_SetDataTriggerExternalTriggerParameters(m_nDeviceCurSel, m_nDataTriggerDivider, m_nDataTriggerDelay, m_nDataTriggerFilter, m_DataTriggerCondition);
}


void CTrigger::OnEnChangeEditPeriod()
{
	if (m_nDeviceCurSel == -1)   return;
	m_nDataTriggerDivider = GetDlgItemInt(IDC_EDIT_PERIOD);
	KSJ3D_SetDataTriggerExternalTriggerParameters(m_nDeviceCurSel, m_nDataTriggerDivider, m_nDataTriggerDelay, m_nDataTriggerFilter, m_DataTriggerCondition);
}


void CTrigger::OnEnChangeEditFilter2()
{
	if (m_nDeviceCurSel == -1)   return;
	m_nDataTriggerFilter = GetDlgItemInt(IDC_EDIT_FILTER2);
	KSJ3D_SetDataTriggerExternalTriggerParameters(m_nDeviceCurSel, m_nDataTriggerDivider, m_nDataTriggerDelay, m_nDataTriggerFilter, m_DataTriggerCondition);
}


void CTrigger::OnEnChangeEditY()
{
	if (m_nDeviceCurSel == -1)   return;
	TCHAR str[32];
	GetDlgItem(IDC_EDIT_Y)->GetWindowText(str, 32);
	m_fProfiley = _tstof(str);
	KSJ3D_SetYResolution(m_nDeviceCurSel, m_fProfiley);
}


void CTrigger::OnCbnSelchangeComboLaser()
{
	if (m_nDeviceCurSel == -1)   return;
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_LASER);
	int nIndex = pComboBox->GetCurSel();
	if (nIndex == CB_ERR)    return;
	KSJ3D_LaserModeSet(m_nDeviceCurSel, KSJ_LASER_MODE(nIndex));
}


BOOL CTrigger::OnInitDialog()
{
	CDialogEx::OnInitDialog();
	TCHAR szTriggerMode[3][32] =
	{
		_T("Free Run"),
		_T("Internal"),
		_T("External"),
	};

	TCHAR szTriggerMethod[2][32] =
	{
		_T("Rising Edge"),
		_T("Falling Edge"),
	};

	TCHAR szLaser[3][32] =
	{
		_T("Open"),
		_T("Close"),
		_T("Flash"),
	};

	int i;
	CComboBox    *pComboBox;
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGERMODE);//数据触发
	pComboBox->ResetContent();
	for (i = 0; i < 3; i++)
	{
		pComboBox->AddString(szTriggerMode[i]);
	}

	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGERMETHOD);

	pComboBox->ResetContent();
	for (i = 0; i < 2; i++)
	{
		pComboBox->AddString(szTriggerMethod[i]);
	}
	
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_LASER);//数据触发
	pComboBox->ResetContent();
	for (i = 0; i < 3; i++)
	{
		pComboBox->AddString(szLaser[i]);
	}

	pComboBox->SetCurSel(2);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_TRIGGERDELAY))->SetRange32(0, 65535);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_FILTER2))->SetRange32(0, 65535);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_PERIOD))->SetRange32(1, 65535);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_Y))->SetRange32(0, 100);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_FIXED_FRAME_RATE))->SetRange32(1, 20000);
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}

void CTrigger::Readini(HANDLE hKSJIni)
{
	int nvalue, nvalue2, nvalue3, nvalue4;
	CComboBox    *pComboBox;
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGERMODE);//数据触发
	KSJINI_GetDWORD(hKSJIni, _T("DataTrigger"), _T("Mode"), 0, (DWORD*)&m_DataTriggerMode);
	pComboBox->SetCurSel(m_DataTriggerMode);
	KSJ3D_SetDataTriggerMode(m_nDeviceCurSel, m_DataTriggerMode);
	OnCbnSelchangeComboTriggermode();
	KSJ3D_GetDataTriggerExternalTriggerParameters(m_nDeviceCurSel, &nvalue, &nvalue2, &nvalue3, (KSJ3D_TRIGGER_EDGE_MODE*)&nvalue4);
	
	KSJINI_GetDWORD(hKSJIni, _T("DataTrigger"), _T("IntervalOfEveryPulse"), nvalue, (DWORD*)&m_nDataTriggerDivider);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_PERIOD))->SetPos32(m_nDataTriggerDivider);

	KSJINI_GetDWORD(hKSJIni, _T("DataTrigger"), _T("Delay"), nvalue2, (DWORD*)&m_nDataTriggerDelay);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_TRIGGERDELAY))->SetPos32(m_nDataTriggerDelay);

	KSJINI_GetDWORD(hKSJIni, _T("DataTrigger"), _T("Filter"), nvalue3, (DWORD*)&m_nDataTriggerFilter);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_FILTER2))->SetPos32(m_nDataTriggerFilter);

	KSJINI_GetDWORD(hKSJIni, _T("DataTrigger"), _T("Method"), nvalue4, (DWORD*)&m_DataTriggerCondition);
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGERMETHOD);
	pComboBox->SetCurSel(m_DataTriggerCondition);
	OnCbnSelchangeComboTriggermethod();
	TCHAR szTemp[64];
	KSJINI_GetString(hKSJIni, _T("3D"), _T("Y"), _T("0.1"), szTemp);
	m_fProfiley = _tstof(szTemp);
	KSJ3D_SetYResolution(m_nDeviceCurSel, m_fProfiley);
	GetDlgItem(IDC_EDIT_Y)->SetWindowText(szTemp);

	KSJ3D_GetDataTriggerInternalFrequency(m_nDeviceCurSel, &nvalue);
	KSJINI_GetDWORD(hKSJIni, _T("DataTrigger"), _T("FrameRate"), nvalue, (DWORD*)&m_nInternalFrequencyHz);
	SetDlgItemInt(IDC_EDIT_FIXED_FRAME_RATE, m_nInternalFrequencyHz);
	KSJ3D_SetDataTriggerInternalFrequency(m_nDeviceCurSel, m_nInternalFrequencyHz);
}

void CTrigger::Writeini(HANDLE hKSJIni)
{
	KSJINI_SetInt(hKSJIni, _T("DataTrigger"), _T("Mode"), m_DataTriggerMode);
	KSJINI_SetInt(hKSJIni, _T("DataTrigger"), _T("Method"), m_DataTriggerCondition);
	KSJINI_SetInt(hKSJIni, _T("DataTrigger"), _T("FrameRate"), m_nInternalFrequencyHz);
	KSJINI_SetInt(hKSJIni, _T("DataTrigger"), _T("Delay"), m_nDataTriggerDelay);
	KSJINI_SetInt(hKSJIni, _T("DataTrigger"), _T("IntervalOfEveryPulse"), m_nDataTriggerDivider);
	KSJINI_SetInt(hKSJIni, _T("DataTrigger"), _T("Filter"), m_nDataTriggerFilter);
	TCHAR szText[32];
	GetDlgItemText(IDC_EDIT_Y, szText, 32);
	KSJINI_SetString(hKSJIni, _T("3D"), _T("Y"), szText);
}

void CTrigger::OnDeltaposSpinY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	m_fProfiley = m_fProfiley + 0.01*pNMUpDown->iDelta;
	if (m_fProfiley < 0.01) m_fProfiley = 0.01;
	if (m_fProfiley > 100) m_fProfiley = 100;
	TCHAR szText[32];
	_stprintf_s(szText, _T("%.2f"), m_fProfiley);
	GetDlgItem(IDC_EDIT_Y)->SetWindowText(szText);
}

void CTrigger::SetEnable(BOOL bEnable)
{
	GetDlgItem(IDC_COMBO_TRIGGERMODE)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_FIXED_FRAME_RATE)->EnableWindow(bEnable);
	GetDlgItem(IDC_SPIN_FIXED_FRAME_RATE)->EnableWindow(bEnable);
	GetDlgItem(IDC_COMBO_TRIGGERMETHOD)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_TRIGGERDELAY)->EnableWindow(bEnable);
	GetDlgItem(IDC_SPIN_TRIGGERDELAY)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_PERIOD)->EnableWindow(bEnable);
	GetDlgItem(IDC_SPIN_PERIOD)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_FILTER2)->EnableWindow(bEnable);
	GetDlgItem(IDC_SPIN_FILTER2)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_Y)->EnableWindow(bEnable);
	GetDlgItem(IDC_SPIN_Y)->EnableWindow(bEnable);
	GetDlgItem(IDC_COMBO_LASER)->EnableWindow(bEnable);
	if (bEnable) OnCbnSelchangeComboTriggermode();
}
