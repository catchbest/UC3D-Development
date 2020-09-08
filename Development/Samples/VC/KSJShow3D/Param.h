#pragma once


// CParam 对话框

class CParam : public CDialogEx
{
	DECLARE_DYNAMIC(CParam)

public:
	CParam(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CParam();
	float m_fExpTimeMS;
	int m_nGain;
	int m_nThreshold;
// 对话框数据
	enum { IDD = IDD_DIALOG_PARAM };
	int m_nDeviceCurSel;
	void Readini(HANDLE hKSJIni);
	void Writeini(HANDLE hKSJIni);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnEnChangeEditExposure();
	afx_msg void OnEnChangeEditGain();
	afx_msg void OnEnChangeEditT();
	afx_msg void OnDeltaposSpinExposure(NMHDR *pNMHDR, LRESULT *pResult);
	virtual BOOL OnInitDialog();
};
