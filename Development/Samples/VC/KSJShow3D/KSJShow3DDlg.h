
// KSJShow3DDlg.h : 头文件
//
//#define halcon12 //开放宏定义可以调用halcon保存om3文件
#include "Resource.h"
#pragma once
#include "KSJImageWnd.h"
#define MAX_IMAGEBUF 64
#define MAX_DEVICE    64
typedef unsigned (WINAPI *PBEGINTHREADEX_FUNC)(LPVOID lpParam);
typedef unsigned *PBEGINTHREADEX_ID;
#define HCPP_LEGACY_API
#ifdef halcon12
#include "HalconCpp.h"
#pragma comment( lib, "halconcpp.lib" )
using namespace HalconCpp;
#endif

#define WM_MSG    WM_USER + 100
#define WM_LOST   WM_USER + 101
#define WM_ENDONE WM_USER + 102


struct DEVICEINFO
{
	int                 nIndex;
	int                 nDeviceType;
	int					nSerials;
	WORD				wFirmwareVersion;
	WORD                wFpgaVersion;
};

// CKSJShow3DDlg 对话框
class CKSJShow3DDlg : public CDialogEx
{
// 构造
public:
	CKSJShow3DDlg(CWnd* pParent = NULL);	// 标准构造函数
// 对话框数据
	enum { IDD = IDD_KSJShow3D_DIALOG };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
	
private:
	DEVICEINFO    m_DeviceInfo[MAX_DEVICE];
	void UpdateDeviceList(void);
	void UpdateInterface();
	int m_nDeviceNum;
	int m_nDeviceCurSel;
	int m_3dDataType;
	int m_nColStart;
	int m_nRowStart;
	int m_nColSize;
	int m_nRowSize;
	float m_fExpTimeMS;
	int m_nGain;
	KSJ3D_TRIGGER_EDGE_MODE m_StartTriggerCondition;
	int m_nStartTriggerDelay;
	KSJ3D_DATA_TRIGGER_MODE m_DataTriggerMode;
	int m_StartTriggerFilter;
	int m_nInternalFrequencyHz;
	int m_nDataTriggerDivider;
	int m_nDataTriggerDelay;
	int m_nDataTriggerFilter;
	KSJ3D_TRIGGER_EDGE_MODE m_DataTriggerCondition;

	int m_nNumberOfProfiles;
	int m_nThreshold;

	KSJ_LASER_MODE m_LaserMode;
	float m_fZMin;
	float m_fZMax;

	float m_fProfiley;

	KSJ3D_DATA_TRIGGER_MODE m_triggermode;
	KSJ3D_TRIGGER_EDGE_MODE m_startmode;
	
	//CKSJImageWnd *m_pWndProfileImage;
	CKSJImageWnd *m_pWndZmapImage;
	CKSJImageWnd *m_pWndSrcImage;
	//CRect         m_rcProfileImageWnd;
	CRect         m_rcZmapImageWnd;
	CRect         m_rcSrcImageWnd;
	int m_save;
	int se;
	int type;
	int m_one;
	int m_init;
	HANDLE m_hKSJIni;
	TCHAR m_szMsg[32];
	bool m_bIs3DCamera;
protected:
	int m_nSystemTimer;

public:
	static void __stdcall LiveImageCallback(int nChannel, unsigned char *pData, int nWidth, int nHeight, int nBitCount, void *lpContext);
	static void __stdcall ProfileDataCallback(int nChannel, int nPointNum, float fYMillimeters, int nProfileIndex, float *x, float *z, int nLostProfileNum, void *lpContext);
	static void __stdcall ProfileImageCallback(int nChannel, unsigned char *pData, int nWidth, int nHeight, int nBitCount, void *lpContext);
	static void __stdcall PointCloudDataCallback(int nChannel, int nTotalPointNum, int nProfileNum, float *x, float *y, float *z, int nTotalLostProfileNum, void *lpContext);
	static void __stdcall CloudImageCallback(int nChannel, unsigned char *pData, int nWidth, int nHeight, int nBitCount, void *lpContext);

	void ShowLiveImage(unsigned char *pData, int nWidth, int nHeight, int nBitCount);
	void ShowProfileImage(unsigned char *pData, int nWidth, int nHeight, int nBitCount);
	void ShowZmapImage(unsigned char *pData, int nWidth, int nHeight, int nBitCount);
	void ProfileData(int nChannel, int nPointNum, float fYMillimeters, int nProfileIndex, float *x, float *z, int nLostProfileNum);
	void PointCloudData(int nChannel, int nTotalPointNum, int nProfileNum, float *x, float *y, float *z, int nTotalLostProfileNum);
// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnCbnSelchangeComboType();
	afx_msg void OnDestroy();
	afx_msg void OnBnClickedCheckCapture();
	afx_msg void OnEnChangeEditExposure();
	afx_msg void OnEnChangeEditGain();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SelectSet(int type);
	afx_msg void OnBnClickedButtonStarttrigger();
	afx_msg void OnBnClickedButtonRoi();
	afx_msg void OnBnClickedButtonEg();
	afx_msg void OnBnClickedButtonDatatrigger();
	afx_msg void OnBnClickedButton3d();
	afx_msg void OnBnClickedButtonZmap();
	void hidecontrol();
	afx_msg void OnCbnSelchangeComboStartmode();
	afx_msg void OnEnChangeEditStartdelay();
	afx_msg void OnEnChangeEditFilter();
	afx_msg void OnCbnSelchangeComboTriggermode();
	afx_msg void OnEnChangeEditFixedFrameRate();
	afx_msg void OnCbnSelchangeComboTriggermethod();
	afx_msg void OnEnChangeEditTriggerdelay();
	afx_msg void OnBnClickedCheckPreview();
	afx_msg void OnBnClickedCheckOne();
	afx_msg void OnEnChangeEditPeriod();
	void Readini(HANDLE hKSJIni);
	void Writeini(HANDLE hKSJIni);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMsgMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgLost(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgEndone(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnEnChangeEditT();
	afx_msg void OnEnChangeEditY();
	afx_msg void OnEnChangeEditLow();
	afx_msg void OnEnChangeEditHigh2();
	afx_msg void OnEnChangeEditFilter2();
	afx_msg void OnEnChangeEditLine();
	afx_msg void OnBnClickedCheckSave();
	afx_msg void OnBnClickedButtonSetroi();
	afx_msg void OnDeltaposSpinExposure(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinY(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinLow(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnDeltaposSpinHigh2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonLoadini();
	afx_msg void OnBnClickedButtonSaveini();
};
