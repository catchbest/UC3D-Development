// Roi.cpp : 实现文件
//

#include "stdafx.h"
#include "KSJShow3D.h"
#include "Roi.h"
#include "afxdialogex.h"


// CRoi 对话框

IMPLEMENT_DYNAMIC(CRoi, CDialogEx)

CRoi::CRoi(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRoi::IDD, pParent)
{

}

CRoi::~CRoi()
{
}

void CRoi::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CRoi, CDialogEx)
	ON_BN_CLICKED(IDC_BUTTON_SETROI, &CRoi::OnBnClickedButtonSetroi)
	ON_EN_CHANGE(IDC_EDIT_LOW, &CRoi::OnEnChangeEditLow)
	ON_EN_CHANGE(IDC_EDIT_HIGH2, &CRoi::OnEnChangeEditHigh2)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_LOW, &CRoi::OnDeltaposSpinLow)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_HIGH2, &CRoi::OnDeltaposSpinHigh2)
END_MESSAGE_MAP()


// CRoi 消息处理程序


void CRoi::OnBnClickedButtonSetroi()
{
	if (m_nDeviceCurSel == -1)   return;
	m_nColSize = GetDlgItemInt(IDC_EDIT_COL_SIZE);
	m_nRowSize = GetDlgItemInt(IDC_EDIT_ROW_SIZE);
	m_nColStart = GetDlgItemInt(IDC_EDIT_COL_START);
	m_nRowStart = GetDlgItemInt(IDC_EDIT_ROW_START);
	KSJ3D_SetRoi(m_nDeviceCurSel, m_nColStart, m_nRowStart, m_nColSize, m_nRowSize);
	KSJ3D_GetRoi(m_nDeviceCurSel, &m_nColStart, &m_nRowStart, &m_nColSize, &m_nRowSize);
	SetDlgItemInt(IDC_EDIT_COL_START, m_nColStart);
	SetDlgItemInt(IDC_EDIT_ROW_START, m_nRowStart);
	SetDlgItemInt(IDC_EDIT_COL_SIZE, m_nColSize);
	SetDlgItemInt(IDC_EDIT_ROW_SIZE, m_nRowSize);
	float fFovNear, fFovFar;
	KSJ3D_GetMeasurementRange(m_nDeviceCurSel, &m_fZMin, &m_fZMax, &fFovNear, &fFovFar);
	char szTemp[32];
	if (m_fZMin < 0) m_fZMin = 0;
	_stprintf_s(szTemp, _T("%.1f"), m_fZMin);
	GetDlgItem(IDC_EDIT_LOW)->SetWindowText(szTemp);
	_stprintf_s(szTemp, _T("%.1f"), m_fZMax);
	GetDlgItem(IDC_EDIT_HIGH2)->SetWindowText(szTemp);
}


void CRoi::OnEnChangeEditLow()
{
	if (m_nDeviceCurSel == -1)   return;
	TCHAR str[32];
	GetDlgItem(IDC_EDIT_LOW)->GetWindowText(str, 32);
	m_fZMin = _tstof(str);
	KSJ3D_SetZMap(m_nDeviceCurSel, m_fZMin, m_fZMax);
}


void CRoi::OnEnChangeEditHigh2()
{
	if (m_nDeviceCurSel == -1)   return;
	TCHAR str[32];
	GetDlgItem(IDC_EDIT_HIGH2)->GetWindowText(str, 32);
	m_fZMax = _tstof(str);
	KSJ3D_SetZMap(m_nDeviceCurSel, m_fZMin, m_fZMax);
}

void CRoi::Readini(HANDLE hKSJIni)
{
	int nColStart;
	int nRowStart;
	int nColSize;
	int nRowSize;
	KSJ3D_GetRoi(m_nDeviceCurSel, &nColStart, &nRowStart, &nColSize, &nRowSize);
	KSJINI_GetDWORD(hKSJIni, _T("Param"), _T("ColStart"), nColStart, (DWORD*)&m_nColStart);
	SetDlgItemInt(IDC_EDIT_COL_START, m_nColStart);
	KSJINI_GetDWORD(hKSJIni, _T("Param"), _T("RowStart"), nRowStart, (DWORD*)&m_nRowStart);
	SetDlgItemInt(IDC_EDIT_ROW_START, m_nRowStart);
	KSJINI_GetDWORD(hKSJIni, _T("Param"), _T("ColSize"), nColSize, (DWORD*)&m_nColSize);
	SetDlgItemInt(IDC_EDIT_COL_SIZE, m_nColSize);
	KSJINI_GetDWORD(hKSJIni, _T("Param"), _T("RowSize"), nRowSize, (DWORD*)&m_nRowSize);
	SetDlgItemInt(IDC_EDIT_ROW_SIZE, m_nRowSize);
	KSJ3D_SetRoi(m_nDeviceCurSel, m_nColStart, m_nRowStart, m_nColSize, m_nRowSize);
	KSJ3D_GetRoiMax(m_nDeviceCurSel, &m_nMaxColSize, &m_nMaxRowSize);
	float fFovNear, fFovFar;
	float fMax;
	TCHAR szText[64] = { 0 };
	TCHAR szTemp[64];
	KSJ3D_GetMeasurementRange(m_nDeviceCurSel, &m_fZMin, &m_fZMax, &fFovNear, &fFovFar);
	KSJ3D_GetMeasurementRangeMax(m_nDeviceCurSel, &fMax, &m_fFovNear, &m_fFovFar);
	if (m_fZMin < 0) m_fZMin = 0;
	_stprintf_s(szText, _T("%.1f"), m_fZMin);
	KSJINI_GetString(hKSJIni, _T("3D"), _T("LOW"), szText, szTemp);
	m_fZMin = _tstof(szTemp);
	GetDlgItem(IDC_EDIT_LOW)->SetWindowText(szTemp);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_LOW))->SetRange32(0, 255);
	_stprintf_s(szText, _T("%.1f"), m_fZMax);
	KSJINI_GetString(hKSJIni, _T("3D"), _T("HIGH"), szText, szTemp);
	m_fZMax = _tstof(szTemp);
	GetDlgItem(IDC_EDIT_HIGH2)->SetWindowText(szTemp);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_HIGH2))->SetRange32(0, 255);
}

void CRoi::Writeini(HANDLE hKSJIni)
{
	TCHAR szText[32];
	GetDlgItemText(IDC_EDIT_EXPOSURE, szText, 32);
	KSJINI_SetInt(hKSJIni, _T("Param"), _T("ColStart"), m_nColStart);
	KSJINI_SetInt(hKSJIni, _T("Param"), _T("ColSize"), m_nColSize);
	KSJINI_SetInt(hKSJIni, _T("Param"), _T("RowStart"), m_nRowStart);
	KSJINI_SetInt(hKSJIni, _T("Param"), _T("RowSize"), m_nRowSize);
	GetDlgItemText(IDC_EDIT_LOW, szText, 32);
	KSJINI_SetString(hKSJIni, _T("3D"), _T("LOW"), szText);
	GetDlgItemText(IDC_EDIT_HIGH2, szText, 32);
	KSJINI_SetString(hKSJIni, _T("3D"), _T("HIGH"), szText);
}

void CRoi::OnDeltaposSpinLow(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	m_fZMin = m_fZMin + 0.1*pNMUpDown->iDelta;
	if (m_fZMin < 0.1) m_fZMin = 0.1;
	if (m_fZMin > 255) m_fZMin = 255;
	TCHAR szText[32];
	_stprintf_s(szText, _T("%.1f"), m_fZMin);
	GetDlgItem(IDC_EDIT_LOW)->SetWindowText(szText);
}


void CRoi::OnDeltaposSpinHigh2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	m_fZMax = m_fZMax + 0.1*pNMUpDown->iDelta;
	if (m_fZMax < 0.1) m_fZMax = 0.1;
	if (m_fZMax > 255) m_fZMax = 255;
	TCHAR szText[32];
	_stprintf_s(szText, _T("%.1f"), m_fZMax);
	GetDlgItem(IDC_EDIT_HIGH2)->SetWindowText(szText);
}


void CRoi::SetEnable(BOOL bEnable)
{
	GetDlgItem(IDC_EDIT_COL_SIZE)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_ROW_SIZE)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_ROW_START)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_COL_START)->EnableWindow(bEnable);
	GetDlgItem(IDC_BUTTON_SETROI)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_LOW)->EnableWindow(bEnable);
	GetDlgItem(IDC_SPIN_LOW)->EnableWindow(bEnable);
	GetDlgItem(IDC_EDIT_HIGH2)->EnableWindow(bEnable);
	GetDlgItem(IDC_SPIN_HIGH2)->EnableWindow(bEnable);
}
