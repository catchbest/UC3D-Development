// Param.cpp : 实现文件
//

#include "stdafx.h"
#include "KSJShow3D.h"
#include "Param.h"
#include "afxdialogex.h"


// CParam 对话框

IMPLEMENT_DYNAMIC(CParam, CDialogEx)

CParam::CParam(CWnd* pParent /*=NULL*/)
	: CDialogEx(CParam::IDD, pParent)
{
	m_nDeviceCurSel = -1;
}

CParam::~CParam()
{
}

void CParam::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CParam, CDialogEx)
	ON_EN_CHANGE(IDC_EDIT_EXPOSURE, &CParam::OnEnChangeEditExposure)
	ON_EN_CHANGE(IDC_EDIT_GAIN, &CParam::OnEnChangeEditGain)
	ON_EN_CHANGE(IDC_EDIT_T, &CParam::OnEnChangeEditT)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_EXPOSURE, &CParam::OnDeltaposSpinExposure)
END_MESSAGE_MAP()


// CParam 消息处理程序


void CParam::OnEnChangeEditExposure()
{
	if (m_nDeviceCurSel == -1)   return;
	TCHAR str[32];
	GetDlgItem(IDC_EDIT_EXPOSURE)->GetWindowText(str, 32);
	m_fExpTimeMS = _tstof(str);
	KSJ3D_SetExposureTime(m_nDeviceCurSel, m_fExpTimeMS);
}


void CParam::OnEnChangeEditGain()
{
	if (m_nDeviceCurSel == -1)   return;
	m_nGain = GetDlgItemInt(IDC_EDIT_GAIN);
	KSJ3D_SetGain(m_nDeviceCurSel, m_nGain);
}


void CParam::OnEnChangeEditT()
{
	if (m_nDeviceCurSel == -1)   return;
	m_nThreshold = GetDlgItemInt(IDC_EDIT_T);
	KSJ3D_Set3DLaserLineBrightnessThreshold(m_nDeviceCurSel, m_nThreshold);
}


void CParam::OnDeltaposSpinExposure(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	if (m_nDeviceCurSel == -1)   return;
	*pResult = 0;
	float fMin, fMax;
	KSJ3D_GetExposureTimeRange(m_nDeviceCurSel, &fMin, &fMax);
	m_fExpTimeMS = m_fExpTimeMS + 0.001*pNMUpDown->iDelta;
	if (m_fExpTimeMS < fMin) m_fExpTimeMS = fMin;
	if (m_fExpTimeMS > fMax) m_fExpTimeMS = fMax;
	TCHAR szText[32];
	_stprintf_s(szText, _T("%.3f"), m_fExpTimeMS);
	GetDlgItem(IDC_EDIT_EXPOSURE)->SetWindowText(szText);
}


void CParam::Readini(HANDLE hKSJIni)
{
	TCHAR szText[64] = { 0 };
	TCHAR szTemp[64];
	float fMin, fMax, fCur;
	int    nMin, nMax, nCur;//相机曝光增益、视场
	KSJ3D_GetExposureTimeRange(m_nDeviceCurSel, &fMin, &fMax);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_EXPOSURE))->SetRange32(fMin, fMax);
	KSJ3D_GetExposureTime(m_nDeviceCurSel, &fCur);
	_stprintf_s(szText, _T("%.3f"), fCur);
	KSJINI_GetString(hKSJIni, _T("Param"), _T("Exposure"), szText, szTemp);
	m_fExpTimeMS = _tstof(szTemp);
	GetDlgItem(IDC_EDIT_EXPOSURE)->SetWindowText(szTemp);
	KSJ3D_SetExposureTime(m_nDeviceCurSel, m_fExpTimeMS);
	KSJ3D_GetGainRange(m_nDeviceCurSel, &nMin, &nMax);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_GAIN))->SetRange32(nMin, nMax);
	KSJ3D_GetGain(m_nDeviceCurSel, &nCur);
	KSJINI_GetDWORD(hKSJIni, _T("Param"), _T("Gain"), nCur, (DWORD*)&m_nGain);
	SetDlgItemInt(IDC_EDIT_GAIN, m_nGain);
	KSJ3D_SetGain(m_nDeviceCurSel, m_nGain);

	KSJINI_GetDWORD(hKSJIni, _T("3D"), _T("T"), 10, (DWORD*)&m_nThreshold);//3d参数
	SetDlgItemInt(IDC_EDIT_T, m_nThreshold);
	KSJ3D_Set3DLaserLineBrightnessThreshold(m_nDeviceCurSel, m_nThreshold);
}

void CParam::Writeini(HANDLE hKSJIni)
{
	TCHAR szText[64] = { 0 };
	TCHAR szTemp[64];
	sprintf(szText,"%f", m_fExpTimeMS);
	KSJINI_SetString(hKSJIni, _T("Param"), _T("Exposure"), szText);
	KSJINI_SetInt(hKSJIni, _T("Param"), _T("Gain"), m_nGain);
	KSJINI_SetInt(hKSJIni, _T("3D"), _T("T"), m_nThreshold);
}

BOOL CParam::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_T))->SetRange32(0, 255);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常:  OCX 属性页应返回 FALSE
}
