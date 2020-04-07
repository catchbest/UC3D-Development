
// KSJShow3DDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "KSJShow3D.h"
#include "KSJShow3DDlg.h"
#include "afxdialogex.h"
#include <fstream>
using namespace std;
#ifdef _DEBUG
#define new DEBUG_NEW
#endif
#define FIXED_FRAME_RATE_PRESION       1.0f
//int c = 0;

#define  SYSTEM_TIMER_INDEX   1

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CKSJShow3DDlg 对话框

CKSJShow3DDlg::CKSJShow3DDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CKSJShow3DDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	m_3dDataType = 0;
	m_nDeviceCurSel = -1;
	m_nDeviceNum = 0;

	HMODULE module = GetModuleHandle(0);

	//KSJ_Init();//动态库初始化
	m_nSystemTimer = 0;
	m_one = 0;
	m_save = 0;
	m_bIs3DCamera = false;
	m_init = 0;
}

void CKSJShow3DDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CKSJShow3DDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CKSJShow3DDlg::OnCbnSelchangeComboType)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_CAPTURE, &CKSJShow3DDlg::OnBnClickedCheckCapture)
	ON_EN_CHANGE(IDC_EDIT_EXPOSURE, &CKSJShow3DDlg::OnEnChangeEditExposure)
	ON_EN_CHANGE(IDC_EDIT_GAIN, &CKSJShow3DDlg::OnEnChangeEditGain)
	ON_BN_CLICKED(IDC_BUTTON_STARTTRIGGER, &CKSJShow3DDlg::OnBnClickedButtonStarttrigger)
	ON_BN_CLICKED(IDC_BUTTON_ROI, &CKSJShow3DDlg::OnBnClickedButtonRoi)
	ON_BN_CLICKED(IDC_BUTTON_EG, &CKSJShow3DDlg::OnBnClickedButtonEg)
	ON_BN_CLICKED(IDC_BUTTON_DATATRIGGER, &CKSJShow3DDlg::OnBnClickedButtonDatatrigger)
	ON_BN_CLICKED(IDC_BUTTON_3D, &CKSJShow3DDlg::OnBnClickedButton3d)
	ON_BN_CLICKED(IDC_BUTTON_ZMAP, &CKSJShow3DDlg::OnBnClickedButtonZmap)
	ON_CBN_SELCHANGE(IDC_COMBO_STARTMODE, &CKSJShow3DDlg::OnCbnSelchangeComboStartmode)
	ON_EN_CHANGE(IDC_EDIT_STARTDELAY, &CKSJShow3DDlg::OnEnChangeEditStartdelay)
	ON_EN_CHANGE(IDC_EDIT_FILTER, &CKSJShow3DDlg::OnEnChangeEditFilter)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGERMODE, &CKSJShow3DDlg::OnCbnSelchangeComboTriggermode)
	ON_EN_CHANGE(IDC_EDIT_FIXED_FRAME_RATE, &CKSJShow3DDlg::OnEnChangeEditFixedFrameRate)
	ON_CBN_SELCHANGE(IDC_COMBO_TRIGGERMETHOD, &CKSJShow3DDlg::OnCbnSelchangeComboTriggermethod)
	ON_EN_CHANGE(IDC_EDIT_TRIGGERDELAY, &CKSJShow3DDlg::OnEnChangeEditTriggerdelay)
	ON_BN_CLICKED(IDC_CHECK_PREVIEW, &CKSJShow3DDlg::OnBnClickedCheckPreview)
	ON_BN_CLICKED(IDC_CHECK_ONE, &CKSJShow3DDlg::OnBnClickedCheckOne)
	ON_EN_CHANGE(IDC_EDIT_PERIOD, &CKSJShow3DDlg::OnEnChangeEditPeriod)
	ON_MESSAGE(WM_MSG, &CKSJShow3DDlg::OnMsgMsg)
	ON_MESSAGE(WM_LOST, &CKSJShow3DDlg::OnMsgLost)
	ON_MESSAGE(WM_ENDONE, &CKSJShow3DDlg::OnMsgEndone)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_EN_CHANGE(IDC_EDIT_T, &CKSJShow3DDlg::OnEnChangeEditT)
	ON_EN_CHANGE(IDC_EDIT_Y, &CKSJShow3DDlg::OnEnChangeEditY)
	ON_EN_CHANGE(IDC_EDIT_LOW, &CKSJShow3DDlg::OnEnChangeEditLow)
	ON_EN_CHANGE(IDC_EDIT_HIGH2, &CKSJShow3DDlg::OnEnChangeEditHigh2)
	ON_EN_CHANGE(IDC_EDIT_FILTER2, &CKSJShow3DDlg::OnEnChangeEditFilter2)
	ON_EN_CHANGE(IDC_EDIT_LINE, &CKSJShow3DDlg::OnEnChangeEditLine)
	ON_BN_CLICKED(IDC_CHECK_SAVE, &CKSJShow3DDlg::OnBnClickedCheckSave)
	ON_BN_CLICKED(IDC_BUTTON_SETROI, &CKSJShow3DDlg::OnBnClickedButtonSetroi)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_EXPOSURE, &CKSJShow3DDlg::OnDeltaposSpinExposure)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_Y, &CKSJShow3DDlg::OnDeltaposSpinY)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_LOW, &CKSJShow3DDlg::OnDeltaposSpinLow)
	ON_NOTIFY(UDN_DELTAPOS, IDC_SPIN_HIGH2, &CKSJShow3DDlg::OnDeltaposSpinHigh2)
	ON_BN_CLICKED(IDC_BUTTON_LOADINI, &CKSJShow3DDlg::OnBnClickedButtonLoadini)
	ON_BN_CLICKED(IDC_BUTTON_SAVEINI, &CKSJShow3DDlg::OnBnClickedButtonSaveini)
END_MESSAGE_MAP()


// CKSJShow3DDlg 消息处理程序

BOOL CKSJShow3DDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	KSJ3D_Inital();
	UpdateDeviceList();//获取相机的列表插入到界面上放下拉框
	UpdateInterface();

	GetDlgItem(IDC_BUTTON_STARTTRIGGER)->MoveWindow(40, 110, 170, 25);
	GetDlgItem(IDC_BUTTON_ROI)->MoveWindow(40, 150, 170, 25);
	GetDlgItem(IDC_BUTTON_EG)->MoveWindow(40, 190, 170, 25);
	GetDlgItem(IDC_BUTTON_DATATRIGGER)->MoveWindow(40, 230, 170, 25);
	GetDlgItem(IDC_BUTTON_3D)->MoveWindow(40, 270, 170, 25);
	GetDlgItem(IDC_BUTTON_ZMAP)->MoveWindow(40, 310, 170, 25);

	m_rcSrcImageWnd.left = 280;
	m_rcSrcImageWnd.top = 110;
	m_rcSrcImageWnd.right = 680;
	m_rcSrcImageWnd.bottom = 680;
	m_pWndSrcImage = new CKSJImageWnd(this, -1);
	m_pWndSrcImage->Create(NULL, TEXT(""), WS_CHILD | WS_BORDER | WS_HSCROLL | WS_VSCROLL | WS_VISIBLE, m_rcSrcImageWnd, this, 0, NULL);   // 
	m_pWndSrcImage->ShowWindow(SW_SHOW);
	m_pWndSrcImage->SetFocus();

	m_rcZmapImageWnd = m_rcSrcImageWnd;
	m_rcZmapImageWnd.left += 450;
	m_rcZmapImageWnd.right += 450;
	m_pWndZmapImage = new CKSJImageWnd(this, -1);
	m_pWndZmapImage->Create(NULL, TEXT(""), WS_CHILD | WS_BORDER | WS_HSCROLL | WS_VSCROLL | WS_VISIBLE, m_rcZmapImageWnd, this, 0, NULL);   // 
	m_pWndZmapImage->ShowWindow(SW_SHOW);


	float fZMin = 0.0f;
	float fZMax = 0.0f;
	KSJ3D_GetMeasurementRange(m_nDeviceCurSel, &fZMin, &fZMax, NULL, NULL);

	m_nSystemTimer = SetTimer(SYSTEM_TIMER_INDEX, 1000, NULL);
	CRect rect;
	rect.left = 730;
	rect.top = 690;
	rect.right = 1130;
	rect.bottom = 720;
	GetDlgItem(IDC_STATIC_MSG)->MoveWindow(rect);
	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CKSJShow3DDlg::OnDestroy()
{
	if (m_nSystemTimer != 0)
	{
		KillTimer(m_nSystemTimer);
		m_nSystemTimer = 0;
	}

	CDialogEx::OnDestroy();

	if (m_bIs3DCamera)
	{
		Writeini(m_hKSJIni);
		KSJINI_UnInit(&m_hKSJIni);
	}

	KSJ3D_UnInitial();

	m_pWndZmapImage->DestroyWindow();
	delete m_pWndZmapImage;
	m_pWndZmapImage = NULL;

	m_pWndSrcImage->DestroyWindow();
	delete m_pWndSrcImage;
	m_pWndSrcImage = NULL;

	//KSJ_UnInit();//释放动态库
}

void CKSJShow3DDlg::UpdateDeviceList(void)
{
	KSJ3D_UnInitial();
	KSJ3D_Inital();

	KSJ3D_GetDeviceCount(&m_nDeviceNum);//获取电脑连接的相机数量

	if (m_nDeviceNum == 0)//如果未连接相机则跳出
	{
		GetDlgItem(IDC_STATIC_DEVICENUM)->SetWindowText(_T("未查询到相机"));
		m_nDeviceCurSel = -1;
		GetDlgItem(IDC_BUTTON_STARTTRIGGER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ROI)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_EG)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DATATRIGGER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_3D)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ZMAP)->EnableWindow(FALSE);

		GetDlgItem(IDC_CHECK_PREVIEW)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_ONE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CAPTURE)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_LINE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_LINE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_TYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SAVE)->EnableWindow(FALSE);
		return;
	}

	m_nDeviceCurSel = 0;
	int i = 0;//获取每个相机的参数插入到顶部下拉框中
	
	m_DeviceInfo[i].nIndex = i;
	KSJ3D_GetDeviceInformation(i, &(m_DeviceInfo[i].nDeviceType), &(m_DeviceInfo[i].nSerials), &(m_DeviceInfo[i].wFirmwareVersion), &(m_DeviceInfo[i].wFpgaVersion));
	CString path;
	GetModuleFileName(NULL, path.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	path.ReleaseBuffer();
	int pos = path.ReverseFind(_T('\\'));
	path = path.Left(pos);

	path += _T("\\param");
	CString str;
	str.Format(_T("%d"), m_DeviceInfo[i].nDeviceType);
	path += str;
	path += _T(".ini");
	KSJINI_Init(&m_hKSJIni, path);

	BYTE btMajVersion = (m_DeviceInfo[i].wFirmwareVersion & 0xFF00) >> 8;		// 得到主版本号
	BYTE btMinVersion = m_DeviceInfo[i].wFirmwareVersion & 0x00FF;				// 得到副版本号

	BYTE btFpgaMajVersion = (m_DeviceInfo[i].wFpgaVersion & 0xFF00) >> 8;		// 得到主版本号
	BYTE btFpgaMinVersion = m_DeviceInfo[i].wFpgaVersion & 0x00FF;				// 得到副版本号

	GetDlgItem(IDC_STATIC_DEVICENUM)->SetWindowText(_T("相机连接成功"));

	KSJ3D_RegisterLiveImageCB(m_nDeviceCurSel, NULL, this);
	KSJ3D_RegisterProfileDataCB(m_nDeviceCurSel, NULL, this);
	KSJ3D_RegisterProfileImageCB(m_nDeviceCurSel, NULL, this);
	KSJ3D_RegisterPointCloudDataCB(m_nDeviceCurSel, NULL, this);
	KSJ3D_RegisterPointCloudImageCB(m_nDeviceCurSel, NULL, this);

}


void CKSJShow3DDlg::UpdateInterface()//获取选中相机的数据显示在界面上
{
	if (m_nDeviceCurSel == -1)   return;

	
	KSJ3D_Is3DCamera(m_nDeviceCurSel, &m_bIs3DCamera);

	if (m_bIs3DCamera)
	{
		KSJ3D_SetObliterateInvalidData(m_nDeviceCurSel, false);
		Readini(m_hKSJIni);
	}
	else
	{
		GetDlgItem(IDC_STATIC_DEVICENUM)->SetWindowText(_T("相机非3d相机"));
	}
}

void CKSJShow3DDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CKSJShow3DDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect m_rc3dPictureWnd;
		GetClientRect(&m_rc3dPictureWnd);
		int x = (m_rc3dPictureWnd.Width() - cxIcon + 1) / 2;
		int y = (m_rc3dPictureWnd.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CKSJShow3DDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CKSJShow3DDlg::OnCbnSelchangeComboType()
{
	CComboBox    *pComboBox = NULL;
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TYPE);
	m_3dDataType = pComboBox->GetCurSel();
	if (m_3dDataType == 0) GetDlgItem(IDC_CHECK_SAVE)->EnableWindow(FALSE);
	else GetDlgItem(IDC_CHECK_SAVE)->EnableWindow(TRUE);
}


void CKSJShow3DDlg::OnBnClickedCheckCapture()
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_CHECK_CAPTURE))->GetCheck();
	if (bCheck)
	{
		KSJ3D_RegisterLiveImageCB(m_nDeviceCurSel, LiveImageCallback, this);
		if (m_3dDataType)
		{
			KSJ3D_RegisterPointCloudDataCB(m_nDeviceCurSel, PointCloudDataCallback, this);
			KSJ3D_RegisterPointCloudImageCB(m_nDeviceCurSel, CloudImageCallback, this);
			m_pWndZmapImage->SetImageInfo(m_nColSize, m_nNumberOfProfiles, 8);
		}
		else
		{
			KSJ3D_RegisterProfileDataCB(m_nDeviceCurSel, ProfileDataCallback, this);
			KSJ3D_RegisterProfileImageCB(m_nDeviceCurSel, ProfileImageCallback, this);
			m_pWndZmapImage->SetImageInfo(m_nColSize, 256, 8);
		}

		if (KSJ3D_StartAcquisition(m_nDeviceCurSel) == RET_SUCCESS)
		{
			GetDlgItem(IDC_STATIC_MSG)->SetWindowText(_T("开始采集"));
			GetDlgItem(IDC_COMBO_STARTMODE)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_LINE)->EnableWindow(FALSE);
			GetDlgItem(IDC_SPIN_LINE)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_STARTDELAY)->EnableWindow(FALSE);
			GetDlgItem(IDC_SPIN_STARTDELAY)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_FILTER)->EnableWindow(FALSE);
			GetDlgItem(IDC_SPIN_FILTER)->EnableWindow(FALSE);

			GetDlgItem(IDC_EDIT_COL_SIZE)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_ROW_SIZE)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_COL_START)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_ROW_START)->EnableWindow(FALSE);

			GetDlgItem(IDC_COMBO_TRIGGERMODE)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_FIXED_FRAME_RATE)->EnableWindow(FALSE);
			GetDlgItem(IDC_SPIN_FIXED_FRAME_RATE)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_TRIGGERMETHOD)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_TRIGGERDELAY)->EnableWindow(FALSE);
			GetDlgItem(IDC_SPIN_TRIGGERDELAY)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_ONE)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_PREVIEW)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_ONE)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_TYPE)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_PERIOD)->EnableWindow(FALSE);
			GetDlgItem(IDC_SPIN_PERIOD)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_FILTER2)->EnableWindow(FALSE);
			GetDlgItem(IDC_SPIN_FILTER2)->EnableWindow(FALSE);
		}
	}
	else
	{
		KSJ3D_StopAcquisition(m_nDeviceCurSel);
		KSJ3D_RegisterLiveImageCB(m_nDeviceCurSel, NULL, this);
		if (m_3dDataType)
		{
			KSJ3D_RegisterPointCloudDataCB(m_nDeviceCurSel, NULL, this);
			KSJ3D_RegisterPointCloudImageCB(m_nDeviceCurSel, NULL, this);
		}
		else
		{
			KSJ3D_RegisterProfileDataCB(m_nDeviceCurSel, NULL, this);
			KSJ3D_RegisterProfileImageCB(m_nDeviceCurSel, NULL, this);
		}

		GetDlgItem(IDC_COMBO_STARTMODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LINE)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_LINE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_STARTDELAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_STARTDELAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FILTER)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_FILTER)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_COL_SIZE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ROW_SIZE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COL_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ROW_START)->EnableWindow(TRUE);

		GetDlgItem(IDC_COMBO_TRIGGERMODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FIXED_FRAME_RATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_FIXED_FRAME_RATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_TRIGGERMETHOD)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TRIGGERDELAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_TRIGGERDELAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_ONE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_PREVIEW)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_ONE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_TYPE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PERIOD)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_PERIOD)->EnableWindow(TRUE);
		OnCbnSelchangeComboTriggermode();
		OnCbnSelchangeComboStartmode();
		GetDlgItem(IDC_STATIC_MSG)->SetWindowText(_T("结束采集"));
	}
}

void CKSJShow3DDlg::OnEnChangeEditExposure()
{
	TCHAR str[32];
	GetDlgItem(IDC_EDIT_EXPOSURE)->GetWindowText(str, 32);
	m_fExpTimeMS = _tstof (str);
	KSJ3D_SetExposureTime(m_nDeviceCurSel, m_fExpTimeMS);
}


void CKSJShow3DDlg::OnEnChangeEditGain()
{
	if (m_nDeviceCurSel == -1)   return;
	m_nGain = GetDlgItemInt(IDC_EDIT_GAIN);
	KSJ3D_SetGain(m_nDeviceCurSel, m_nGain);
}


BOOL CKSJShow3DDlg::PreTranslateMessage(MSG* pMsg)
{
	// TODO:  在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_ESCAPE: //Esc按键事件
			return true;
		case VK_RETURN: //Enter按键事件
			return true;
		default:
			;
		}
	}

	return CDialogEx::PreTranslateMessage(pMsg);
}


void CKSJShow3DDlg::SelectSet(int type)
{
	hidecontrol();
	if (se > 0)
	{
		int high = 260;
		if (type == 1)
		{
			GetDlgItem(IDC_BUTTON_STARTTRIGGER)->MoveWindow(40, 110, 170, 25);
			GetDlgItem(IDC_BUTTON_ROI)->MoveWindow(40, 150 + high, 170, 25);
			GetDlgItem(IDC_BUTTON_EG)->MoveWindow(40, 190 + high, 170, 25);
			GetDlgItem(IDC_BUTTON_DATATRIGGER)->MoveWindow(40, 230 + high, 170, 25);
			GetDlgItem(IDC_BUTTON_3D)->MoveWindow(40, 270 + high, 170, 25);
			GetDlgItem(IDC_BUTTON_ZMAP)->MoveWindow(40, 310 + high, 170, 25);

			GetDlgItem(IDC_STATIC1)->MoveWindow(40, 150, 60, 20);
			GetDlgItem(IDC_COMBO_STARTMODE)->MoveWindow(110, 150, 150, 20);
			GetDlgItem(IDC_STATIC3)->MoveWindow(40, 180, 60, 20);
			GetDlgItem(IDC_EDIT_STARTDELAY)->MoveWindow(110, 180, 60, 20);
			GetDlgItem(IDC_SPIN_STARTDELAY)->MoveWindow(170, 180, 15, 20);
			GetDlgItem(IDC_STATIC4)->MoveWindow(200, 180, 60, 20);
			GetDlgItem(IDC_STATIC5)->MoveWindow(40, 210, 60, 20);
			GetDlgItem(IDC_EDIT_FILTER)->MoveWindow(110, 210, 60, 20);
			GetDlgItem(IDC_SPIN_FILTER)->MoveWindow(170, 210, 15, 20);
			GetDlgItem(IDC_STATIC6)->MoveWindow(200, 210, 60, 20);

			GetDlgItem(IDC_COMBO_STARTMODE)->ShowWindow(TRUE);
			GetDlgItem(IDC_EDIT_STARTDELAY)->ShowWindow(TRUE);
			GetDlgItem(IDC_SPIN_STARTDELAY)->ShowWindow(TRUE);
			GetDlgItem(IDC_EDIT_FILTER)->ShowWindow(TRUE);
			GetDlgItem(IDC_SPIN_FILTER)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC1)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC3)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC4)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC5)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC6)->ShowWindow(TRUE);
		}
		else if (type == 2)
		{
			GetDlgItem(IDC_BUTTON_STARTTRIGGER)->MoveWindow(40, 110, 170, 25);
			GetDlgItem(IDC_BUTTON_ROI)->MoveWindow(40, 150, 170, 25);
			GetDlgItem(IDC_BUTTON_EG)->MoveWindow(40, 190 + high, 170, 25);
			GetDlgItem(IDC_BUTTON_DATATRIGGER)->MoveWindow(40, 230 + high, 170, 25);
			GetDlgItem(IDC_BUTTON_3D)->MoveWindow(40, 270 + high, 170, 25);
			GetDlgItem(IDC_BUTTON_ZMAP)->MoveWindow(40, 310 + high, 170, 25);

			GetDlgItem(IDC_STATIC11)->MoveWindow(40, 190, 60, 20);
			GetDlgItem(IDC_EDIT_COL_SIZE)->MoveWindow(110, 190, 60, 20);
			GetDlgItem(IDC_STATIC12)->MoveWindow(40, 230, 60, 20);
			GetDlgItem(IDC_EDIT_ROW_SIZE)->MoveWindow(110, 230, 60, 20);
			GetDlgItem(IDC_STATIC13)->MoveWindow(40, 270, 60, 20);
			GetDlgItem(IDC_EDIT_COL_START)->MoveWindow(110, 270, 60, 20);
			GetDlgItem(IDC_STATIC14)->MoveWindow(40, 310, 60, 20);
			GetDlgItem(IDC_EDIT_ROW_START)->MoveWindow(110, 310, 60, 20);
			GetDlgItem(IDC_BUTTON_SETROI)->MoveWindow(40, 350, 60, 20);

			GetDlgItem(IDC_STATIC11)->ShowWindow(TRUE);
			GetDlgItem(IDC_EDIT_COL_SIZE)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC12)->ShowWindow(TRUE);
			GetDlgItem(IDC_EDIT_ROW_SIZE)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC13)->ShowWindow(TRUE);
			GetDlgItem(IDC_EDIT_COL_START)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC14)->ShowWindow(TRUE);
			GetDlgItem(IDC_EDIT_ROW_START)->ShowWindow(TRUE);
			GetDlgItem(IDC_BUTTON_SETROI)->ShowWindow(TRUE);
		}
		else if (type == 3)
		{
			GetDlgItem(IDC_BUTTON_STARTTRIGGER)->MoveWindow(40, 110, 170, 25);
			GetDlgItem(IDC_BUTTON_ROI)->MoveWindow(40, 150, 170, 25);
			GetDlgItem(IDC_BUTTON_EG)->MoveWindow(40, 190, 170, 25);
			GetDlgItem(IDC_BUTTON_DATATRIGGER)->MoveWindow(40, 230 + high, 170, 25);
			GetDlgItem(IDC_BUTTON_3D)->MoveWindow(40, 270 + high, 170, 25);
			GetDlgItem(IDC_BUTTON_ZMAP)->MoveWindow(40, 310 + high, 170, 25);

			GetDlgItem(IDC_STATIC21)->MoveWindow(40, 230, 60, 20);
			GetDlgItem(IDC_EDIT_EXPOSURE)->MoveWindow(110, 230, 60, 20);
			GetDlgItem(IDC_SPIN_EXPOSURE)->MoveWindow(170, 230, 15, 20);
			GetDlgItem(IDC_STATIC22)->MoveWindow(40, 260, 60, 20);
			GetDlgItem(IDC_EDIT_GAIN)->MoveWindow(110, 260, 60, 20);
			GetDlgItem(IDC_SPIN_GAIN)->MoveWindow(170, 260, 15, 20);

			GetDlgItem(IDC_STATIC21)->ShowWindow(TRUE);
			GetDlgItem(IDC_EDIT_EXPOSURE)->ShowWindow(TRUE);
			GetDlgItem(IDC_SPIN_EXPOSURE)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC22)->ShowWindow(TRUE);
			GetDlgItem(IDC_EDIT_GAIN)->ShowWindow(TRUE);
			GetDlgItem(IDC_SPIN_GAIN)->ShowWindow(TRUE);
		}
		else if (type == 4)
		{
			GetDlgItem(IDC_BUTTON_STARTTRIGGER)->MoveWindow(40, 110, 170, 25);
			GetDlgItem(IDC_BUTTON_ROI)->MoveWindow(40, 150, 170, 25);
			GetDlgItem(IDC_BUTTON_EG)->MoveWindow(40, 190, 170, 25);
			GetDlgItem(IDC_BUTTON_DATATRIGGER)->MoveWindow(40, 230, 170, 25);
			GetDlgItem(IDC_BUTTON_3D)->MoveWindow(40, 270 + high, 170, 25);
			GetDlgItem(IDC_BUTTON_ZMAP)->MoveWindow(40, 310 + high, 170, 25);

			GetDlgItem(IDC_STATIC31)->MoveWindow(40, 270, 60, 20);
			GetDlgItem(IDC_COMBO_TRIGGERMODE)->MoveWindow(110, 270, 150, 20);
			GetDlgItem(IDC_STATIC32)->MoveWindow(30, 300, 70, 20);
			GetDlgItem(IDC_EDIT_FIXED_FRAME_RATE)->MoveWindow(110, 300, 60, 20);
			GetDlgItem(IDC_SPIN_FIXED_FRAME_RATE)->MoveWindow(170, 300, 15, 20);
			GetDlgItem(IDC_STATIC33)->MoveWindow(40, 330, 60, 20);
			GetDlgItem(IDC_COMBO_TRIGGERMETHOD)->MoveWindow(110, 330, 150, 20);
			GetDlgItem(IDC_STATIC34)->MoveWindow(30, 360, 70, 20);
			GetDlgItem(IDC_EDIT_TRIGGERDELAY)->MoveWindow(110, 360, 60, 20);
			GetDlgItem(IDC_SPIN_TRIGGERDELAY)->MoveWindow(170, 360, 15, 20);
			GetDlgItem(IDC_STATIC35)->MoveWindow(190, 360, 60, 20);
			GetDlgItem(IDC_STATIC36)->MoveWindow(30, 390, 70, 20);
			GetDlgItem(IDC_EDIT_PERIOD)->MoveWindow(110, 390, 60, 20);
			GetDlgItem(IDC_SPIN_PERIOD)->MoveWindow(170, 390, 15, 20);
			GetDlgItem(IDC_STATIC37)->MoveWindow(190, 390, 60, 20);
			GetDlgItem(IDC_STATIC38)->MoveWindow(30, 420, 70, 20);
			GetDlgItem(IDC_EDIT_FILTER2)->MoveWindow(110, 420, 60, 20);
			GetDlgItem(IDC_SPIN_FILTER2)->MoveWindow(170, 420, 15, 20);
			GetDlgItem(IDC_STATIC39)->MoveWindow(190, 420, 60, 20);

			GetDlgItem(IDC_COMBO_TRIGGERMODE)->ShowWindow(TRUE);
			GetDlgItem(IDC_EDIT_FIXED_FRAME_RATE)->ShowWindow(TRUE);
			GetDlgItem(IDC_SPIN_FIXED_FRAME_RATE)->ShowWindow(TRUE);
			GetDlgItem(IDC_COMBO_TRIGGERMETHOD)->ShowWindow(TRUE);
			GetDlgItem(IDC_EDIT_TRIGGERDELAY)->ShowWindow(TRUE);
			GetDlgItem(IDC_SPIN_TRIGGERDELAY)->ShowWindow(TRUE);
			GetDlgItem(IDC_EDIT_PERIOD)->ShowWindow(TRUE);
			GetDlgItem(IDC_SPIN_PERIOD)->ShowWindow(TRUE);
			GetDlgItem(IDC_EDIT_FILTER2)->ShowWindow(TRUE);
			GetDlgItem(IDC_SPIN_FILTER2)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC31)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC32)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC33)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC34)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC35)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC36)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC37)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC38)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC39)->ShowWindow(TRUE);
		}
		else if (type == 5)
		{
			GetDlgItem(IDC_BUTTON_STARTTRIGGER)->MoveWindow(40, 110, 170, 25);
			GetDlgItem(IDC_BUTTON_ROI)->MoveWindow(40, 150, 170, 25);
			GetDlgItem(IDC_BUTTON_EG)->MoveWindow(40, 190, 170, 25);
			GetDlgItem(IDC_BUTTON_DATATRIGGER)->MoveWindow(40, 230, 170, 25);
			GetDlgItem(IDC_BUTTON_3D)->MoveWindow(40, 270, 170, 25);
			GetDlgItem(IDC_BUTTON_ZMAP)->MoveWindow(40, 310 + high, 170, 25);

			GetDlgItem(IDC_STATIC41)->MoveWindow(40, 310, 60, 20);
			GetDlgItem(IDC_EDIT_T)->MoveWindow(110, 310, 60, 20);
			GetDlgItem(IDC_SPIN_T)->MoveWindow(170, 310, 15, 20);
			GetDlgItem(IDC_STATIC42)->MoveWindow(20, 340, 80, 20);
			GetDlgItem(IDC_EDIT_Y)->MoveWindow(110, 340, 60, 20);
			GetDlgItem(IDC_SPIN_Y)->MoveWindow(170, 340, 15, 20);
			GetDlgItem(IDC_STATIC43)->MoveWindow(190, 340, 80, 20);

			GetDlgItem(IDC_STATIC41)->ShowWindow(TRUE);
			GetDlgItem(IDC_EDIT_T)->ShowWindow(TRUE);
			GetDlgItem(IDC_SPIN_T)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC42)->ShowWindow(TRUE);
			GetDlgItem(IDC_EDIT_Y)->ShowWindow(TRUE);
			GetDlgItem(IDC_SPIN_Y)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC43)->ShowWindow(TRUE);
		}
		else if (type == 6)
		{
			GetDlgItem(IDC_BUTTON_STARTTRIGGER)->MoveWindow(40, 110, 170, 25);
			GetDlgItem(IDC_BUTTON_ROI)->MoveWindow(40, 150, 170, 25);
			GetDlgItem(IDC_BUTTON_EG)->MoveWindow(40, 190, 170, 25);
			GetDlgItem(IDC_BUTTON_DATATRIGGER)->MoveWindow(40, 230, 170, 25);
			GetDlgItem(IDC_BUTTON_3D)->MoveWindow(40, 270, 170, 25);
			GetDlgItem(IDC_BUTTON_ZMAP)->MoveWindow(40, 310, 170, 25);

			GetDlgItem(IDC_STATIC51)->MoveWindow(40, 350, 60, 20);
			GetDlgItem(IDC_EDIT_LOW)->MoveWindow(110, 350, 60, 20);
			GetDlgItem(IDC_SPIN_LOW)->MoveWindow(170, 350, 15, 20);
			GetDlgItem(IDC_STATIC52)->MoveWindow(200, 350, 60, 20);
			GetDlgItem(IDC_STATIC53)->MoveWindow(40, 380, 60, 20);
			GetDlgItem(IDC_EDIT_HIGH2)->MoveWindow(110, 380, 60, 20);
			GetDlgItem(IDC_SPIN_HIGH2)->MoveWindow(170, 380, 15, 20);
			GetDlgItem(IDC_STATIC54)->MoveWindow(200, 380, 60, 20);

			GetDlgItem(IDC_STATIC51)->ShowWindow(TRUE);
			GetDlgItem(IDC_EDIT_LOW)->ShowWindow(TRUE);
			GetDlgItem(IDC_SPIN_LOW)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC52)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC53)->ShowWindow(TRUE);
			GetDlgItem(IDC_EDIT_HIGH2)->ShowWindow(TRUE);
			GetDlgItem(IDC_SPIN_HIGH2)->ShowWindow(TRUE);
			GetDlgItem(IDC_STATIC54)->ShowWindow(TRUE);
		}
	}
	else
	{
		GetDlgItem(IDC_BUTTON_STARTTRIGGER)->MoveWindow(40, 110, 170, 25);
		GetDlgItem(IDC_BUTTON_ROI)->MoveWindow(40, 150, 170, 25);
		GetDlgItem(IDC_BUTTON_EG)->MoveWindow(40, 190, 170, 25);
		GetDlgItem(IDC_BUTTON_DATATRIGGER)->MoveWindow(40, 230, 170, 25);
		GetDlgItem(IDC_BUTTON_3D)->MoveWindow(40, 270, 170, 25);
		GetDlgItem(IDC_BUTTON_ZMAP)->MoveWindow(40, 310, 170, 25);
	}
}


void CKSJShow3DDlg::OnBnClickedButtonStarttrigger()
{
	type = 1;
	if (se != type) se = type;
	else se = 0;
	SelectSet(type);
}


void CKSJShow3DDlg::OnBnClickedButtonRoi()
{
	type = 2;
	if (se != type) se = type;
	else se = 0;
	SelectSet(type);
}


void CKSJShow3DDlg::OnBnClickedButtonEg()
{
	type = 3;
	if (se != type) se = type;
	else se = 0;
	SelectSet(type);
}


void CKSJShow3DDlg::OnBnClickedButtonDatatrigger()
{
	type = 4;
	if (se != type) se = type;
	else se = 0;
	SelectSet(type);
}


void CKSJShow3DDlg::OnBnClickedButton3d()
{
	type = 5;
	if (se != type) se = type;
	else se = 0;
	SelectSet(type);
}


void CKSJShow3DDlg::OnBnClickedButtonZmap()
{
	type = 6;
	if (se != type) se = type;
	else se = 0;
	SelectSet(type);
}


void CKSJShow3DDlg::hidecontrol()
{
	GetDlgItem(IDC_COMBO_STARTMODE)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_STARTDELAY)->ShowWindow(FALSE);
	GetDlgItem(IDC_SPIN_STARTDELAY)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_FILTER)->ShowWindow(FALSE);
	GetDlgItem(IDC_SPIN_FILTER)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC1)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC3)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC4)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC5)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC6)->ShowWindow(FALSE);

	GetDlgItem(IDC_STATIC11)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_COL_SIZE)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC12)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_ROW_SIZE)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC13)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_COL_START)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC14)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_ROW_START)->ShowWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SETROI)->ShowWindow(FALSE);

	GetDlgItem(IDC_STATIC21)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_EXPOSURE)->ShowWindow(FALSE);
	GetDlgItem(IDC_SPIN_EXPOSURE)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC22)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_GAIN)->ShowWindow(FALSE);
	GetDlgItem(IDC_SPIN_GAIN)->ShowWindow(FALSE);

	GetDlgItem(IDC_COMBO_TRIGGERMODE)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_FIXED_FRAME_RATE)->ShowWindow(FALSE);
	GetDlgItem(IDC_SPIN_FIXED_FRAME_RATE)->ShowWindow(FALSE);
	GetDlgItem(IDC_COMBO_TRIGGERMETHOD)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_TRIGGERDELAY)->ShowWindow(FALSE);
	GetDlgItem(IDC_SPIN_TRIGGERDELAY)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_PERIOD)->ShowWindow(FALSE);
	GetDlgItem(IDC_SPIN_PERIOD)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_FILTER2)->ShowWindow(FALSE);
	GetDlgItem(IDC_SPIN_FILTER2)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC31)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC32)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC33)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC34)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC35)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC36)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC37)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC38)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC39)->ShowWindow(FALSE);

	GetDlgItem(IDC_STATIC41)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_T)->ShowWindow(FALSE);
	GetDlgItem(IDC_SPIN_T)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC42)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_Y)->ShowWindow(FALSE);
	GetDlgItem(IDC_SPIN_Y)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC43)->ShowWindow(FALSE);

	GetDlgItem(IDC_STATIC51)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_LOW)->ShowWindow(FALSE);
	GetDlgItem(IDC_SPIN_LOW)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC52)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC53)->ShowWindow(FALSE);
	GetDlgItem(IDC_EDIT_HIGH2)->ShowWindow(FALSE);
	GetDlgItem(IDC_SPIN_HIGH2)->ShowWindow(FALSE);
	GetDlgItem(IDC_STATIC54)->ShowWindow(FALSE);
}


void CKSJShow3DDlg::OnCbnSelchangeComboStartmode()
{
	if (m_nDeviceCurSel == -1)   return;
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_STARTMODE);
	int nIndex = pComboBox->GetCurSel();
	if (nIndex == CB_ERR)    return;
	if (nIndex != 0) m_StartTriggerCondition = (KSJ3D_TRIGGER_EDGE_MODE)(nIndex - 1);
	else m_StartTriggerCondition = (KSJ3D_TRIGGER_EDGE_MODE)nIndex;
	
	if (nIndex == 0) KSJ3D_SetStartTrigger(m_nDeviceCurSel, STS_INPUT_0, false, m_StartTriggerCondition);
	else KSJ3D_SetStartTrigger(m_nDeviceCurSel, STS_INPUT_0, true, m_StartTriggerCondition);
}


void CKSJShow3DDlg::OnEnChangeEditStartdelay()
{
	if (m_nDeviceCurSel == -1)   return;
	m_nStartTriggerDelay = GetDlgItemInt(IDC_EDIT_STARTDELAY);
	KSJ3D_SetStartTriggerParameters(m_nDeviceCurSel, m_StartTriggerFilter, m_nStartTriggerDelay);
}


void CKSJShow3DDlg::OnEnChangeEditFilter()
{
	if (m_nDeviceCurSel == -1)   return;
	m_StartTriggerFilter = GetDlgItemInt(IDC_EDIT_FILTER);
	KSJ3D_SetStartTriggerParameters(m_nDeviceCurSel, m_StartTriggerFilter, m_nStartTriggerDelay);
}


void CKSJShow3DDlg::OnCbnSelchangeComboTriggermode()
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

	//PostMessage(KSJ_INFO_MSG, KSJ_INfO_TYPE_NEWTRRIGEMODE, nIndex);
	KSJ3D_SetDataTriggerMode(m_nDeviceCurSel, m_DataTriggerMode);
}


void CKSJShow3DDlg::OnEnChangeEditFixedFrameRate()
{
	if (m_nDeviceCurSel == -1)   return;
	m_nInternalFrequencyHz = GetDlgItemInt(IDC_EDIT_FIXED_FRAME_RATE);
	KSJ3D_SetDataTriggerInternalFrequency(m_nDeviceCurSel, m_nInternalFrequencyHz);
}


void CKSJShow3DDlg::OnCbnSelchangeComboTriggermethod()
{
	if (m_nDeviceCurSel == -1)   return;
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGERMETHOD);
	int nIndex = pComboBox->GetCurSel();
	if (nIndex == CB_ERR)    return;
	m_DataTriggerCondition = (KSJ3D_TRIGGER_EDGE_MODE)nIndex;
	KSJ3D_SetDataTriggerExternalTriggerParameters(m_nDeviceCurSel, m_nDataTriggerDivider, m_nDataTriggerDelay, m_nDataTriggerFilter, m_DataTriggerCondition);
}


void CKSJShow3DDlg::OnEnChangeEditTriggerdelay()
{
	if (m_nDeviceCurSel == -1)   return;
	m_nDataTriggerDelay = GetDlgItemInt(IDC_EDIT_TRIGGERDELAY);
	KSJ3D_SetDataTriggerExternalTriggerParameters(m_nDeviceCurSel, m_nDataTriggerDivider, m_nDataTriggerDelay, m_nDataTriggerFilter, m_DataTriggerCondition);
}

void CKSJShow3DDlg::OnBnClickedCheckPreview()
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_CHECK_PREVIEW))->GetCheck();
	if (bCheck)
	{
		CComboBox    *pComboBox;
		pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGERMODE);
		m_triggermode = (KSJ3D_DATA_TRIGGER_MODE)pComboBox->GetCurSel();
		pComboBox->SetCurSel(0);
		OnCbnSelchangeComboTriggermode();
		KSJ3D_SetDataTriggerMode(m_nDeviceCurSel, DTM_FREE_RUN);

		pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_STARTMODE);
		m_startmode = (KSJ3D_TRIGGER_EDGE_MODE)pComboBox->GetCurSel();
		pComboBox->SetCurSel(0);
		OnCbnSelchangeComboStartmode();
		KSJ3D_SetStartTrigger(m_nDeviceCurSel, STS_INPUT_0, false, m_StartTriggerCondition);

		KSJ3D_RegisterLiveImageCB(m_nDeviceCurSel, LiveImageCallback, this);
		if (KSJ3D_StartAcquisition(m_nDeviceCurSel) == RET_SUCCESS)
		{
			GetDlgItem(IDC_STATIC_MSG)->SetWindowText(_T("开始预览"));
			GetDlgItem(IDC_COMBO_STARTMODE)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_LINE)->EnableWindow(FALSE);
			GetDlgItem(IDC_SPIN_LINE)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_STARTDELAY)->EnableWindow(FALSE);
			GetDlgItem(IDC_SPIN_STARTDELAY)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_FILTER)->EnableWindow(FALSE);
			GetDlgItem(IDC_SPIN_FILTER)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_COL_SIZE)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_ROW_SIZE)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_COL_START)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_ROW_START)->EnableWindow(FALSE);

			GetDlgItem(IDC_COMBO_TRIGGERMODE)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_FIXED_FRAME_RATE)->EnableWindow(FALSE);
			GetDlgItem(IDC_SPIN_FIXED_FRAME_RATE)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_TRIGGERMETHOD)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_TRIGGERDELAY)->EnableWindow(FALSE);
			GetDlgItem(IDC_SPIN_TRIGGERDELAY)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_PERIOD)->EnableWindow(FALSE);
			GetDlgItem(IDC_SPIN_PERIOD)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_ONE)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_CAPTURE)->EnableWindow(FALSE);
		}
	}
	else
	{
		KSJ3D_StopAcquisition(m_nDeviceCurSel);
		KSJ3D_RegisterLiveImageCB(m_nDeviceCurSel, NULL, this);
		GetDlgItem(IDC_COMBO_STARTMODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LINE)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_LINE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_STARTDELAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_STARTDELAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FILTER)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_FILTER)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_COL_SIZE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ROW_SIZE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COL_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ROW_START)->EnableWindow(TRUE);

		GetDlgItem(IDC_COMBO_TRIGGERMODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FIXED_FRAME_RATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_FIXED_FRAME_RATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_TRIGGERMETHOD)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TRIGGERDELAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_TRIGGERDELAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_ONE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_CAPTURE)->EnableWindow(TRUE);
		CComboBox    *pComboBox;
		pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGERMODE);
		pComboBox->SetCurSel(m_triggermode);
		pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_STARTMODE);
		pComboBox->SetCurSel(m_startmode);
		KSJ3D_SetStartTrigger(m_nDeviceCurSel, STS_INPUT_0, true, m_StartTriggerCondition);
		OnCbnSelchangeComboTriggermode();
		OnCbnSelchangeComboStartmode();
		GetDlgItem(IDC_STATIC_MSG)->SetWindowText(_T("停止预览"));
	}
}

void CKSJShow3DDlg::OnBnClickedCheckOne()
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_CHECK_ONE))->GetCheck();
	if (bCheck)
	{
		GetDlgItem(IDC_STATIC_MSG)->SetWindowText(_T("开始一次采集"));
		m_one = 1;
		KSJ3D_RegisterLiveImageCB(m_nDeviceCurSel, LiveImageCallback, this);
		if (m_3dDataType)
		{
			KSJ3D_RegisterPointCloudDataCB(m_nDeviceCurSel, PointCloudDataCallback, this);
			KSJ3D_RegisterPointCloudImageCB(m_nDeviceCurSel, CloudImageCallback, this);
		}
		else
		{
			KSJ3D_RegisterProfileDataCB(m_nDeviceCurSel, ProfileDataCallback, this);
			KSJ3D_RegisterProfileImageCB(m_nDeviceCurSel, CloudImageCallback, this);
		}

		if (KSJ3D_StartAcquisition(m_nDeviceCurSel) == RET_SUCCESS)
		{
			GetDlgItem(IDC_COMBO_STARTMODE)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_LINE)->EnableWindow(FALSE);
			GetDlgItem(IDC_SPIN_LINE)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_STARTDELAY)->EnableWindow(FALSE);
			GetDlgItem(IDC_SPIN_STARTDELAY)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_FILTER)->EnableWindow(FALSE);
			GetDlgItem(IDC_SPIN_FILTER)->EnableWindow(FALSE);

			GetDlgItem(IDC_EDIT_COL_SIZE)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_ROW_SIZE)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_COL_START)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_ROW_START)->EnableWindow(FALSE);

			GetDlgItem(IDC_COMBO_TRIGGERMODE)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_FIXED_FRAME_RATE)->EnableWindow(FALSE);
			GetDlgItem(IDC_SPIN_FIXED_FRAME_RATE)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_TRIGGERMETHOD)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_TRIGGERDELAY)->EnableWindow(FALSE);
			GetDlgItem(IDC_SPIN_TRIGGERDELAY)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_CAPTURE)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_PREVIEW)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_TYPE)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_PERIOD)->EnableWindow(FALSE);
			GetDlgItem(IDC_SPIN_PERIOD)->EnableWindow(FALSE);
			GetDlgItem(IDC_EDIT_FILTER2)->EnableWindow(FALSE);
			GetDlgItem(IDC_SPIN_FILTER2)->EnableWindow(FALSE);
		}
	}
	else
	{
		m_one = 0;
		KSJ3D_StopAcquisition(m_nDeviceCurSel);
		KSJ3D_RegisterLiveImageCB(m_nDeviceCurSel, NULL, this);
		if (m_3dDataType)
		{
			KSJ3D_RegisterPointCloudDataCB(m_nDeviceCurSel, NULL, this);
			KSJ3D_RegisterPointCloudImageCB(m_nDeviceCurSel, NULL, this);

		}
		else
		{
			KSJ3D_RegisterProfileDataCB(m_nDeviceCurSel, NULL, this);
			KSJ3D_RegisterProfileImageCB(m_nDeviceCurSel, NULL, this);
		}

		GetDlgItem(IDC_COMBO_STARTMODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_LINE)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_LINE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_STARTDELAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_STARTDELAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FILTER)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_FILTER)->EnableWindow(TRUE);

		GetDlgItem(IDC_EDIT_COL_SIZE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ROW_SIZE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_COL_START)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_ROW_START)->EnableWindow(TRUE);

		GetDlgItem(IDC_COMBO_TRIGGERMODE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_FIXED_FRAME_RATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_FIXED_FRAME_RATE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_TRIGGERMETHOD)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_TRIGGERDELAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_TRIGGERDELAY)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_PREVIEW)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_CAPTURE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_TYPE)->EnableWindow(TRUE);
		GetDlgItem(IDC_EDIT_PERIOD)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_PERIOD)->EnableWindow(TRUE);
		OnCbnSelchangeComboTriggermode();
		OnCbnSelchangeComboStartmode();
	}
}


void CKSJShow3DDlg::OnEnChangeEditPeriod()
{
	if (m_nDeviceCurSel == -1)   return;
	m_nDataTriggerDivider = GetDlgItemInt(IDC_EDIT_PERIOD);
	KSJ3D_SetDataTriggerExternalTriggerParameters(m_nDeviceCurSel, m_nDataTriggerDivider, m_nDataTriggerDelay, m_nDataTriggerFilter, m_DataTriggerCondition);
}


void CKSJShow3DDlg::Readini(HANDLE hKSJIni)
{
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

	TCHAR szStartEndMode[3][32] =
	{
		_T("disable"),
		_T("Rising Edge"),
		_T("Falling Edge"),
	};
	int i;
	CComboBox    *pComboBox;
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_STARTMODE);
	if (!m_init)
	{
		//启动触发
		pComboBox->ResetContent();
		
		for (i = 0; i < 3; i++)
		{
			pComboBox->AddString(szStartEndMode[i]);
		}
	}

	unsigned long usvalue = 0;
	int nvalue, nvalue2, nvalue3, nvalue4;
	float fvalue;
	TCHAR szText[64] = { 0 };
	TCHAR szTemp[64];

	KSJINI_GetDWORD(hKSJIni, _T("StartTrigger"), _T("Mode"), 0, (DWORD*)&nvalue);
	pComboBox->SetCurSel(nvalue);
	OnCbnSelchangeComboStartmode();
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_STARTDELAY))->SetRange32(0, 65535);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_FILTER))->SetRange32(0, 65535);
	KSJ3D_GetStartTriggerParameters(m_nDeviceCurSel, &nvalue, &nvalue2);
	KSJINI_GetDWORD(hKSJIni, _T("StartTrigger"), _T("Delay"), nvalue, (DWORD*)&m_nStartTriggerDelay);
	KSJINI_GetDWORD(hKSJIni, _T("StartTrigger"), _T("Filter"), nvalue2, (DWORD*)&m_StartTriggerFilter);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_STARTDELAY))->SetPos32(m_nStartTriggerDelay);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_FILTER))->SetPos32(m_StartTriggerFilter);
	KSJ3D_SetStartTriggerParameters(m_nDeviceCurSel, m_StartTriggerFilter, m_nStartTriggerDelay);

	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_LINE))->SetRange32(0, 65535);
	KSJINI_GetDWORD(hKSJIni, _T("StartTrigger"), _T("MultiLine"), 500, (DWORD*)&m_nNumberOfProfiles);
	KSJ3D_SetMaxNumberOfProfilesToCapture(m_nDeviceCurSel, m_nNumberOfProfiles);
	_stprintf_s(szTemp, _T("%d"), m_nNumberOfProfiles);
	GetDlgItem(IDC_EDIT_LINE)->SetWindowText(szTemp);

	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGERMODE);//数据触发
	if (!m_init)
	{
		pComboBox->ResetContent();
		for (i = 0; i < 3; i++)
		{
			pComboBox->AddString(szTriggerMode[i]);
		}
	}

	KSJINI_GetDWORD(hKSJIni, _T("DataTrigger"), _T("Mode"), 0, (DWORD*)&m_DataTriggerMode);
	pComboBox->SetCurSel(m_DataTriggerMode);
	KSJ3D_SetDataTriggerMode(m_nDeviceCurSel, m_DataTriggerMode);
	OnCbnSelchangeComboTriggermode();
	KSJ3D_GetDataTriggerExternalTriggerParameters(m_nDeviceCurSel, &nvalue, &nvalue2, &nvalue3, (KSJ3D_TRIGGER_EDGE_MODE*)&nvalue4);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_PERIOD))->SetRange32(1, 65535);
	KSJINI_GetDWORD(hKSJIni, _T("DataTrigger"), _T("IntervalOfEveryPulse"), nvalue, (DWORD*)&m_nDataTriggerDivider);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_PERIOD))->SetPos32(m_nDataTriggerDivider);

	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_TRIGGERDELAY))->SetRange32(0, 65535);
	KSJINI_GetDWORD(hKSJIni, _T("DataTrigger"), _T("Delay"), nvalue2, (DWORD*)&m_nDataTriggerDelay);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_TRIGGERDELAY))->SetPos32(m_nDataTriggerDelay);

	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_FILTER2))->SetRange32(0, 65535);
	KSJINI_GetDWORD(hKSJIni, _T("DataTrigger"), _T("Filter"), nvalue3, (DWORD*)&m_nDataTriggerFilter);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_FILTER2))->SetPos32(m_nDataTriggerFilter);

	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TRIGGERMETHOD);
	if (!m_init)
	{
		pComboBox->ResetContent();
		for (i = 0; i < 2; i++)
		{
			pComboBox->AddString(g_szTriggerMethod[i]);
		}
	}

	KSJINI_GetDWORD(hKSJIni, _T("DataTrigger"), _T("Method"), nvalue4, (DWORD*)&m_DataTriggerCondition);
	pComboBox->SetCurSel(m_DataTriggerCondition);
	OnCbnSelchangeComboTriggermethod();

	KSJ3D_SetDataTriggerExternalTriggerParameters(m_nDeviceCurSel, m_nDataTriggerDivider, m_nDataTriggerDelay, m_nDataTriggerFilter, m_DataTriggerCondition);
	float fMin, fMax, fCur;
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_FIXED_FRAME_RATE))->SetRange32(1, 20000);
	KSJ3D_GetDataTriggerInternalFrequency(m_nDeviceCurSel, &nvalue);
	KSJINI_GetDWORD(hKSJIni, _T("DataTrigger"), _T("FrameRate"), nvalue, (DWORD*)&m_nInternalFrequencyHz);
	SetDlgItemInt(IDC_EDIT_FIXED_FRAME_RATE, m_nInternalFrequencyHz);
	KSJ3D_SetDataTriggerInternalFrequency(m_nDeviceCurSel, m_nInternalFrequencyHz);

	int    nMin, nMax, nCur;//相机曝光增益、视场
	
	KSJ3D_GetExposureTimeRange(m_nDeviceCurSel, &fMin, &fMax);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_EXPOSURE))->SetRange32(fMin, fMax);
	KSJ3D_GetExposureTime(m_nDeviceCurSel, &fCur);
	_stprintf_s(szText, _T("%.3f"), fCur);
	KSJINI_GetString(hKSJIni, _T("Param"), _T("Exposure"), szText, szTemp);
	m_fExpTimeMS = _tstof (szTemp);
	GetDlgItem(IDC_EDIT_EXPOSURE)->SetWindowText(szTemp);
	KSJ3D_SetExposureTime(m_nDeviceCurSel, m_fExpTimeMS);
	KSJ3D_GetGainRange(m_nDeviceCurSel, &nMin, &nMax);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_GAIN))->SetRange32(nMin, nMax);
	KSJ3D_GetGain(m_nDeviceCurSel, &nCur);
	KSJINI_GetDWORD(hKSJIni, _T("Param"), _T("Gain"), nCur, (DWORD*)&m_nGain);
	SetDlgItemInt(IDC_EDIT_GAIN, m_nGain);
	KSJ3D_SetGain(m_nDeviceCurSel, m_nGain);
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


	KSJINI_GetDWORD(hKSJIni, _T("3D"), _T("T"), 10, (DWORD*)&m_nThreshold);//3d参数
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_T))->SetRange32(0, 255);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_T))->SetPos32(m_nThreshold);
	SetDlgItemInt(IDC_EDIT_T, m_nThreshold);
	KSJ3D_Set3DLaserLineBrightnessThreshold(m_nDeviceCurSel, m_nThreshold);
	
	KSJINI_GetString(hKSJIni, _T("3D"), _T("Y"), _T("0.1"), szTemp);
	m_fProfiley = _tstof (szTemp);
	KSJ3D_SetYResolution(m_nDeviceCurSel, m_fProfiley);
	GetDlgItem(IDC_EDIT_Y)->SetWindowText(szTemp);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_Y))->SetRange32(0, 100);
	KSJ3D_GetZMap(m_nDeviceCurSel, &fMin, &fMax);
	_stprintf_s(szText, _T("%.1f"), fMin);
	KSJINI_GetString(hKSJIni, _T("3D"), _T("LOW"), szText, szTemp);
	m_fZMin = _tstof (szTemp);
	GetDlgItem(IDC_EDIT_LOW)->SetWindowText(szTemp);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_LOW))->SetRange32(0, 255);
	_stprintf_s(szText, _T("%.1f"), fMax);
	KSJINI_GetString(hKSJIni, _T("3D"), _T("HIGH"), szText, szTemp);
	m_fZMax = _tstof (szTemp);
	GetDlgItem(IDC_EDIT_HIGH2)->SetWindowText(szTemp);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_HIGH2))->SetRange32(0, 255);

	KSJINI_GetDWORD(hKSJIni, _T("3D"), _T("TYPE"), 0, (DWORD*)&m_3dDataType);
	if (!m_init)
	{
		pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TYPE);
		CString strtype[2] = { _T("轮廓"), _T("3D") };
		for (int i = 0; i < 2; i++)
		{
			pComboBox->AddString(strtype[i]);
		}

		pComboBox->SetCurSel(m_3dDataType);
		m_init = 1;
	}
}


void CKSJShow3DDlg::Writeini(HANDLE hKSJIni)
{
	int    nCur;
	unsigned short    usCur;
	TCHAR szText[32];

	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_STARTMODE);
	int nIndex = pComboBox->GetCurSel();
	KSJINI_SetInt(hKSJIni, _T("StartTrigger"), _T("Mode"), nIndex);
	KSJINI_SetInt(hKSJIni, _T("StartTrigger"), _T("Delay"), m_nStartTriggerDelay);
	KSJINI_SetInt(hKSJIni, _T("StartTrigger"), _T("Filter"), m_StartTriggerFilter);
	KSJINI_SetInt(hKSJIni, _T("StartTrigger"), _T("MultiLine"), m_nNumberOfProfiles);

	GetDlgItemText(IDC_EDIT_EXPOSURE, szText, 32);
	KSJINI_SetString(hKSJIni, _T("Param"), _T("Exposure"), szText);
	KSJINI_SetInt(hKSJIni, _T("Param"), _T("Gain"), m_nGain);
	KSJINI_SetInt(hKSJIni, _T("Param"), _T("ColStart"), m_nColStart);
	KSJINI_SetInt(hKSJIni, _T("Param"), _T("ColSize"), m_nColSize);
	KSJINI_SetInt(hKSJIni, _T("Param"), _T("RowStart"), m_nRowStart);
	KSJINI_SetInt(hKSJIni, _T("Param"), _T("RowSize"), m_nRowSize);

	KSJINI_SetInt(hKSJIni, _T("DataTrigger"), _T("Mode"), m_DataTriggerMode);
	KSJINI_SetInt(hKSJIni, _T("DataTrigger"), _T("Method"), m_DataTriggerCondition);
	KSJINI_SetInt(hKSJIni, _T("DataTrigger"), _T("FrameRate"), m_nInternalFrequencyHz);
	KSJINI_SetInt(hKSJIni, _T("DataTrigger"), _T("Delay"), m_nDataTriggerDelay);
	KSJINI_SetInt(hKSJIni, _T("DataTrigger"), _T("IntervalOfEveryPulse"), m_nDataTriggerDivider);
	KSJINI_SetInt(hKSJIni, _T("DataTrigger"), _T("Filter"), m_nDataTriggerFilter);
	
	//3d参数
	GetDlgItemText(IDC_EDIT_Y, szText, 32);
	KSJINI_SetString(hKSJIni, _T("3D"), _T("Y"), szText);
	nCur = GetDlgItemInt(IDC_EDIT_T);
	KSJINI_SetInt(hKSJIni, _T("3D"), _T("T"), nCur);
	GetDlgItemText(IDC_EDIT_LOW, szText, 32);
	KSJINI_SetString(hKSJIni, _T("3D"), _T("LOW"), szText);
	nCur = GetDlgItemInt(IDC_EDIT_HIGH2);
	GetDlgItemText(IDC_EDIT_HIGH2, szText, 32);
	KSJINI_SetString(hKSJIni, _T("3D"), _T("HIGH"), szText);
	KSJINI_SetInt(hKSJIni, _T("3D"), _T("TYPE"), m_3dDataType);
}


void CKSJShow3DDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (m_rcSrcImageWnd.PtInRect(point))
	{
		//m_pWndProfileImage->SetSelected(FALSE);
		m_pWndZmapImage->SetSelected(FALSE);
		m_pWndSrcImage->SetSelected(TRUE);
	}
	else if (m_rcZmapImageWnd.PtInRect(point))
	{
		//m_pWndProfileImage->SetSelected(FALSE);
		m_pWndZmapImage->SetSelected(TRUE);
		m_pWndSrcImage->SetSelected(FALSE);
	}
	//else if (m_rcProfileImageWnd.PtInRect(point))
	//{
	//	//m_pWndProfileImage->SetSelected(TRUE);
	//	m_pWndZmapImage->SetSelected(FALSE);
	//	m_pWndSrcImage->SetSelected(FALSE);
	//}

	CDialogEx::OnLButtonDown(nFlags, point);
}

LRESULT CKSJShow3DDlg::OnMsgMsg(WPARAM wParam, LPARAM lParam)
{
	GetDlgItem(IDC_STATIC_MSG)->SetWindowText(m_szMsg);

	return 0;
}

LRESULT CKSJShow3DDlg::OnMsgLost(WPARAM wParam, LPARAM lParam)
{
	GetDlgItem(IDC_STATIC_LOST)->SetWindowText(m_szMsg);

	return 0;
}

void __stdcall CKSJShow3DDlg::LiveImageCallback(int nChannel, unsigned char *pData, int nWidth, int nHeight, int nBitCount, void *lpContext)
{
	CKSJShow3DDlg* pMainDlg = (CKSJShow3DDlg*)lpContext;
	//TCHAR   szFileName[64];
	//_stprintf_s(szFileName, "%d.bmp", c++);

	//KSJ_HelperSaveToBmp(pData, nWidth, nHeight, nBitCount, szFileName);
	pMainDlg->ShowLiveImage(pData, nWidth, nHeight, nBitCount);
}

void CKSJShow3DDlg::ShowLiveImage(unsigned char *pData, int nWidth, int nHeight, int nBitCount)
{
	m_pWndSrcImage->LoadImage(pData, nWidth, nHeight, nBitCount);
}

void __stdcall CKSJShow3DDlg::ProfileDataCallback(int nChannel, int nPointNum, float fYMillimeters, int nProfileIndex, float *x, float *z, int nLostProfileNum, void *lpContext)
{
	CKSJShow3DDlg* pMainDlg = (CKSJShow3DDlg*)lpContext;
	pMainDlg->ProfileData(nChannel, nPointNum, fYMillimeters, nProfileIndex, x, z, nLostProfileNum);
}

void CKSJShow3DDlg::ProfileData(int nChannel, int nPointNum, float fYMillimeters, int nProfileIndex, float *x, float *z, int nLostProfileNum)
{
	if (m_one)
	{
		_tcscpy(m_szMsg, _T("结束一次采集"));
		PostMessage(WM_MSG, 0, 0);
		PostMessage(WM_ENDONE, 0, 0);
		m_one = 0;
	}
}

void __stdcall CKSJShow3DDlg::ProfileImageCallback(int nChannel, unsigned char *pData, int nWidth, int nHeight, int nBitCount, void *lpContext)
{
	CKSJShow3DDlg* pMainDlg = (CKSJShow3DDlg*)lpContext;

	pMainDlg->ShowProfileImage(pData, nWidth, nHeight, nBitCount);
}

void CKSJShow3DDlg::ShowProfileImage(unsigned char *pData, int nWidth, int nHeight, int nBitCount)
{
	m_pWndZmapImage->LoadImage(pData, nWidth, nHeight, nBitCount);
}

void __stdcall CKSJShow3DDlg::PointCloudDataCallback(int nChannel, int nTotalPointNum, int nProfileNum, float *x, float *y, float *z, int nTotalLostProfileNum, void *lpContext)
{
	CKSJShow3DDlg* pMainDlg = (CKSJShow3DDlg*)lpContext;
	pMainDlg->PointCloudData(nChannel, nTotalPointNum, nProfileNum, x, y, z, nTotalLostProfileNum);
}

void CKSJShow3DDlg::PointCloudData(int nChannel, int nTotalPointNum, int nProfileNum, float *x, float *y, float *z, int nTotalLostProfileNum)
{
	//TRACE("TotalPoint:%d, TotalLostProfileNum:%d\n", nTotalPointNum, nTotalLostProfileNum);
	if (nTotalLostProfileNum > 0)
	{
		_stprintf_s(m_szMsg, _T("3d丢帧%d"), nTotalLostProfileNum);
		PostMessage(WM_LOST, 0, 0);
	}
	else
	{
		_stprintf_s(m_szMsg, _T("未丢帧"));
		PostMessage(WM_LOST, 0, 0);
	}

#ifdef halcon12
	if (m_save)
	{
		_tcscpy(m_szMsg,  _T("om3保存中"));
		PostMessage(WM_MSG, 0, 0);
		SYSTEMTIME LocalTime;
		TCHAR szFileName[128] = { 0 };
		HObject Hobjx, Hobjy, Hobjz;
		gen_image1(&Hobjx, "real", m_nColSize, m_nNumberOfProfiles, (long)x);//深拷贝
		gen_image1(&Hobjy, "real", m_nColSize, m_nNumberOfProfiles, (long)y);
		gen_image1(&Hobjz, "real", m_nColSize, m_nNumberOfProfiles, (long)z);
		HTuple hv_ObjectModel3D;
		XyzToObjectModel3d(Hobjx, Hobjy, Hobjz, &hv_ObjectModel3D);
		GetLocalTime(&LocalTime);
		_stprintf_s(szFileName,  _T("capture-%04d-%02d-%02d-%02d-%02d-%02d.om3"), LocalTime.wYear, LocalTime.wMonth, LocalTime.wDay, LocalTime.wHour, LocalTime.wMinute, LocalTime.wSecond);
		WriteObjectModel3d(hv_ObjectModel3D,  _T("om3"), szFileName,  _T("invert_normals"),  _T("false"));
		_tcscpy(m_szMsg,  _T("om3保存完毕"));
		PostMessage(WM_MSG, 0, 0);
	}
#else
	if (m_save)
	{
		_tcscpy(m_szMsg, _T("pcd保存中"));
		PostMessage(WM_MSG, 0, 0);
		SYSTEMTIME LocalTime;
		TCHAR szFileName[128] = { 0 };
		GetLocalTime(&LocalTime);
		_stprintf_s(szFileName, _T("capture-%04d-%02d-%02d-%02d-%02d-%02d.pcd"), LocalTime.wYear, LocalTime.wMonth, LocalTime.wDay, LocalTime.wHour, LocalTime.wMinute, LocalTime.wSecond);
		KSJ3D_HelperSaveToPCD(m_nColSize, m_nNumberOfProfiles, x, y, z, szFileName);
		_tcscpy(m_szMsg, _T("pcd保存完毕"));
		PostMessage(WM_MSG, 0, 0);
	}
#endif
	if (m_one)
	{
		_tcscpy(m_szMsg, _T("结束一次采集"));
		PostMessage(WM_MSG, 0, 0);
		PostMessage(WM_ENDONE, 0, 0);
		m_one = 0;
	}
}

void __stdcall CKSJShow3DDlg::CloudImageCallback(int nChannel, unsigned char *pData, int nWidth, int nHeight, int nBitCount, void *lpContext)
{
	CKSJShow3DDlg* pMainDlg = (CKSJShow3DDlg*)lpContext;

	pMainDlg->ShowZmapImage(pData, nWidth, nHeight, nBitCount);
}

void CKSJShow3DDlg::ShowZmapImage(unsigned char *pData, int nWidth, int nHeight, int nBitCount)
{
	m_pWndZmapImage->LoadImage(pData, nWidth, nHeight, nBitCount);
}

void CKSJShow3DDlg::OnTimer(UINT_PTR nIDEvent)
{
	if (nIDEvent == SYSTEM_TIMER_INDEX)
	{
		float fTransmissionRate = 0.0f;
		float fExternalTriggerRateHz = 0.0f;
		
		KSJ3D_GetTransmissionRate(m_nDeviceCurSel, &fTransmissionRate);
		KSJ3D_GetExternalTriggerRate(m_nDeviceCurSel, &fExternalTriggerRateHz);

		CString strMessage;
		strMessage.Format(_T("帧率: %.2f， 触发帧率: %.2f"), fTransmissionRate, fExternalTriggerRateHz);
		GetDlgItem(IDC_STATIC_MSG2)->SetWindowText(strMessage);
	}

	CDialogEx::OnTimer(nIDEvent);
}


void CKSJShow3DDlg::OnEnChangeEditT()
{
	if (m_nDeviceCurSel == -1)   return;
	m_nThreshold = GetDlgItemInt(IDC_EDIT_T);
	KSJ3D_Set3DLaserLineBrightnessThreshold(m_nDeviceCurSel, m_nThreshold);
}


void CKSJShow3DDlg::OnEnChangeEditY()
{
	if (m_nDeviceCurSel == -1)   return;
	TCHAR str[32];
	GetDlgItem(IDC_EDIT_Y)->GetWindowText(str, 32);
	m_fProfiley = _tstof (str);
	KSJ3D_SetYResolution(m_nDeviceCurSel, m_fProfiley);
}


void CKSJShow3DDlg::OnEnChangeEditLow()
{
	if (m_nDeviceCurSel == -1)   return;
	TCHAR str[32];
	GetDlgItem(IDC_EDIT_LOW)->GetWindowText(str, 32);
	m_fZMin = _tstof (str);
	KSJ3D_SetZMap(m_nDeviceCurSel, m_fZMin, m_fZMax);
}


void CKSJShow3DDlg::OnEnChangeEditHigh2()
{
	if (m_nDeviceCurSel == -1)   return;
	TCHAR str[32];
	GetDlgItem(IDC_EDIT_HIGH2)->GetWindowText(str, 32);
	m_fZMax = _tstof (str);
	KSJ3D_SetZMap(m_nDeviceCurSel, m_fZMin, m_fZMax);
}


void CKSJShow3DDlg::OnEnChangeEditFilter2()
{
	if (m_nDeviceCurSel == -1)   return;
	m_nDataTriggerFilter = GetDlgItemInt(IDC_EDIT_FILTER2);
	KSJ3D_SetDataTriggerExternalTriggerParameters(m_nDeviceCurSel, m_nDataTriggerDivider, m_nDataTriggerDelay, m_nDataTriggerFilter, m_DataTriggerCondition);
}


void CKSJShow3DDlg::OnEnChangeEditLine()
{
	if (m_nDeviceCurSel == -1)   return;
	m_nNumberOfProfiles = GetDlgItemInt(IDC_EDIT_LINE);
	KSJ3D_SetMaxNumberOfProfilesToCapture(m_nDeviceCurSel, m_nNumberOfProfiles);
}


LRESULT CKSJShow3DDlg::OnMsgEndone(WPARAM wParam, LPARAM lParam)
{
	((CButton*)GetDlgItem(IDC_CHECK_ONE))->SetCheck(FALSE);
	OnBnClickedCheckOne();
	return 0;
}

void CKSJShow3DDlg::OnBnClickedCheckSave()
{
	m_save = ((CButton*)GetDlgItem(IDC_CHECK_SAVE))->GetCheck();
}


void CKSJShow3DDlg::OnBnClickedButtonSetroi()
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
}



void CKSJShow3DDlg::OnDeltaposSpinExposure(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	*pResult = 0;
	float fMin, fMax;
	KSJ3D_GetExposureTimeRange(m_nDeviceCurSel, &fMin, &fMax);
	if (pNMUpDown->iDelta == -1)
	{
		m_fExpTimeMS = m_fExpTimeMS - 0.001;
	}
	else if (pNMUpDown->iDelta == 1)
	{
		m_fExpTimeMS = m_fExpTimeMS + 0.001;
	}
	
	if (m_fExpTimeMS < fMin) m_fExpTimeMS = fMin;
	if (m_fExpTimeMS > fMax) m_fExpTimeMS = fMax;
	TCHAR szText[32];
	_stprintf_s(szText, _T("%.3f"), m_fExpTimeMS);
	GetDlgItem(IDC_EDIT_EXPOSURE)->SetWindowText(szText);
}


void CKSJShow3DDlg::OnDeltaposSpinY(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	if (pNMUpDown->iDelta == -1 && m_fProfiley > 0.01)
	{
		m_fProfiley = m_fProfiley - 0.01;
	}
	else if (pNMUpDown->iDelta == 1 && m_fProfiley < 100)
	{
		m_fProfiley = m_fProfiley + 0.01;
	}
	TCHAR szText[32];
	_stprintf_s(szText, _T("%.2f"), m_fProfiley);
	GetDlgItem(IDC_EDIT_Y)->SetWindowText(szText);
}


void CKSJShow3DDlg::OnDeltaposSpinLow(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	if (pNMUpDown->iDelta == -1 && m_fZMin > 0.1)
	{
		m_fZMin = m_fZMin - 0.1;
	}
	else if (pNMUpDown->iDelta == 1 && m_fZMin < 255)
	{
		m_fZMin = m_fZMin + 0.1;
	}
	TCHAR szText[32];
	_stprintf_s(szText, _T("%.1f"), m_fZMin);
	GetDlgItem(IDC_EDIT_LOW)->SetWindowText(szText);
}


void CKSJShow3DDlg::OnDeltaposSpinHigh2(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMUPDOWN pNMUpDown = reinterpret_cast<LPNMUPDOWN>(pNMHDR);
	// TODO: 在此添加控件通知处理程序代码
	*pResult = 0;
	if (pNMUpDown->iDelta == -1 && m_fZMax > 0.1)
	{
		m_fZMax = m_fZMax - 0.1;
	}
	else if (pNMUpDown->iDelta == 1 && m_fZMax < 255)
	{
		m_fZMax = m_fZMax + 0.1;
	}
	TCHAR szText[32];
	_stprintf_s(szText, _T("%.1f"), m_fZMax);
	GetDlgItem(IDC_EDIT_HIGH2)->SetWindowText(szText);
}


void CKSJShow3DDlg::OnBnClickedButtonLoadini()
{
	CString strFile;
	HANDLE hKSJIni;
	CFileDialog    dlgFile(TRUE, NULL, NULL, OFN_HIDEREADONLY, _T("Describe Files (*.ini)|*.ini||"), NULL);

	if (dlgFile.DoModal())
	{
		strFile = dlgFile.GetPathName();
		KSJINI_Init(&hKSJIni, strFile.GetString());
		Readini(hKSJIni);
		KSJINI_UnInit(&hKSJIni);
	}
}


void CKSJShow3DDlg::OnBnClickedButtonSaveini()
{
	HANDLE hKSJIni;
	CString path;
	TCHAR strini[64];
	GetModuleFileName(NULL, path.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	path.ReleaseBuffer();
	int pos = path.ReverseFind('\\');
	path = path.Left(pos);
	GetDlgItem(IDC_EDIT_ININAME)->GetWindowText(strini,64);
	if (_tcslen(strini) == 0)
	{
		MessageBox(_T("配置文件名不得为空"));
		return;
	}
	path += _T("\\");
	path += strini;
	path += _T(".ini");
	KSJINI_Init(&hKSJIni, path.GetString());
	Writeini(hKSJIni);
	KSJINI_UnInit(&hKSJIni);
}
