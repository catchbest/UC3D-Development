
// KSJShow3DDlg.h : 头文件
//

#include "Resource.h"
#pragma once
#include "KSJImageWnd.h"
#include "KSJProfileWnd.h"
#include "StartTrigger.h"
#include "Roi.h"
#include "Param.h"
#include "Trigger.h"
#define MAX_DEVICE    64
typedef unsigned (WINAPI *PBEGINTHREADEX_FUNC)(LPVOID lpParam);
typedef unsigned *PBEGINTHREADEX_ID;
#define HCPP_LEGACY_API
#ifdef halcon12
#include "HalconCpp.h"
#include "afxwin.h"
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
	void UpdateDeviceList();
	void UpdateInterface();
	int m_nDeviceNum;
	int m_nDeviceCurSel;
	int m_3dDataType;

	int m_nNumberOfProfiles;
	CKSJImageWnd *m_pWnd3DImage;
	CKSJProfileWnd *m_pWndZmapImage;
	CKSJImageWnd *m_pWndSrcImage;
	CRect         m_rcZmapImageWnd;
	CRect         m_rcSrcImageWnd;
	int m_save;
	int se;
	int type;
	int m_one;
	HANDLE m_hKSJIni;
	TCHAR m_szMsg[32];
	
	bool m_bIs3DCamera;
	CString m_path;
	CStartTrigger m_StartTriggerDlg;
	CRoi m_RoiDlg;
	CParam m_ParamDlg;
	CTrigger m_TriggerDlg;
	int m_nTotalLost;
protected:
	int m_nSystemTimer;
	KSJ3D_DATA_TRIGGER_MODE m_triggermode;
	int m_startmode;
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
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	void SelectSet(int type);

	void hidecontrol();
	afx_msg void OnBnClickedCheckPreview();
	afx_msg void OnBnClickedCheckOne();
	void Readini(HANDLE hKSJIni);
	void Writeini(HANDLE hKSJIni);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMsgMsg(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgLost(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgEndone(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnMsgProfile(WPARAM wParam, LPARAM lParam);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnBnClickedCheckSave();
	afx_msg void OnBnClickedButtonLoadini();
	afx_msg void OnBnClickedButtonSaveini();
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnCbnSelchangeComboDeviceList();
	bool CheckCameraName(char * name);
	afx_msg void OnBnClickedButtonStarttrigger();
	afx_msg void OnBnClickedButtonRoi();
	afx_msg void OnBnClickedButtonEg();
	afx_msg void OnBnClickedButtonDatatrigger();
	afx_msg void OnBnClickedButtonLog();
	afx_msg void OnEnChangeEditLine();
	afx_msg void OnBnClickedCheckLog();
	LRESULT OnTraceMsg(WPARAM wParam, LPARAM lParam);
	LRESULT OnClearMsg(WPARAM wParam, LPARAM lParam);
	CListBox m_ListTrace;
	afx_msg void OnBnClickedButtonSerials();
};
