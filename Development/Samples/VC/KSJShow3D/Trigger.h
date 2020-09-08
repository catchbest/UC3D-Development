#pragma once


// CTrigger 对话框

class CTrigger : public CDialogEx
{
	DECLARE_DYNAMIC(CTrigger)

public:
	CTrigger(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTrigger();

// 对话框数据
	enum { IDD = IDD_DIALOG_TRIGGER };
	int m_nDeviceCurSel;
	KSJ3D_DATA_TRIGGER_MODE m_DataTriggerMode;
	int m_nInternalFrequencyHz;
	int m_nDataTriggerDivider;
	int m_nDataTriggerDelay;
	int m_nDataTriggerFilter;
	KSJ3D_TRIGGER_EDGE_MODE m_DataTriggerCondition;
	float m_fProfiley;

	void Readini(HANDLE hKSJIni);
	void Writeini(HANDLE hKSJIni);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboTriggermode();
	afx_msg void OnEnChangeEditFixedFrameRate();
	afx_msg void OnCbnSelchangeComboTriggermethod();
	afx_msg void OnEnChangeEditTriggerdelay();
	afx_msg void OnEnChangeEditPeriod();
	afx_msg void OnEnChangeEditFilter2();
	afx_msg void OnEnChangeEditY();
	afx_msg void OnCbnSelchangeComboLaser();
	virtual BOOL OnInitDialog();
	afx_msg void OnDeltaposSpinY(NMHDR *pNMHDR, LRESULT *pResult);
	void SetEnable(BOOL bEnable);
};
