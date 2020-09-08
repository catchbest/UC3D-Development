#pragma once


// CStartTrigger 对话框

class CStartTrigger : public CDialogEx
{
	DECLARE_DYNAMIC(CStartTrigger)

public:
	CStartTrigger(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CStartTrigger();

// 对话框数据
	enum { IDD = IDD_DIALOG_START_TRIGGER };
	int m_nDeviceCurSel;
	int m_StartTriggerCondition;
	int m_nStartTriggerDelay;
	int m_StartTriggerFilter;
	void Readini(HANDLE hKSJIni);
	void Writeini(HANDLE hKSJIni);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboStartmode();
	afx_msg void OnEnChangeEditStartdelay();
	afx_msg void OnEnChangeEditFilter();
	virtual BOOL OnInitDialog();
	void SetEnable(BOOL bEnable);
};
