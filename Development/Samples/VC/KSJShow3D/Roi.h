#pragma once


// CRoi �Ի���

class CRoi : public CDialogEx
{
	DECLARE_DYNAMIC(CRoi)

public:
	CRoi(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CRoi();

// �Ի�������
	enum { IDD = IDD_DIALOG_ROI };
	int m_nColStart;
	int m_nRowStart;
	int m_nColSize;
	int m_nRowSize;
	int m_nMaxRowSize,m_nMaxColSize;
	float m_fZMin;
	float m_fZMax;
	float m_fFovNear, m_fFovFar;
	int m_nDeviceCurSel;
	void Readini(HANDLE hKSJIni);
	void Writeini(HANDLE hKSJIni);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonSetroi();
	afx_msg void OnEnChangeEditLow();
	afx_msg void OnEnChangeEditHigh2();
	afx_msg void OnDeltaposSpinLow(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinHigh2(NMHDR *pNMHDR, LRESULT *pResult);
	void SetEnable(BOOL bEnable);
};
