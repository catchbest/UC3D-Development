
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
	m_nSystemTimer = 0;
	m_one = 0;
	m_save = 0;
	m_bIs3DCamera = false;

	m_nNumberOfProfiles = 100;
	memset(m_szMsg, 0, sizeof(m_szMsg));
	m_nTotalLost = 0;
}

void CKSJShow3DDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_TRACE, m_ListTrace);
}

BEGIN_MESSAGE_MAP(CKSJShow3DDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_CBN_SELCHANGE(IDC_COMBO_TYPE, &CKSJShow3DDlg::OnCbnSelchangeComboType)
	ON_WM_DESTROY()
	ON_BN_CLICKED(IDC_CHECK_CAPTURE, &CKSJShow3DDlg::OnBnClickedCheckCapture)
	ON_BN_CLICKED(IDC_CHECK_PREVIEW, &CKSJShow3DDlg::OnBnClickedCheckPreview)
	ON_BN_CLICKED(IDC_CHECK_ONE, &CKSJShow3DDlg::OnBnClickedCheckOne)
	ON_MESSAGE(WM_MSG, &CKSJShow3DDlg::OnMsgMsg)
	ON_MESSAGE(WM_LOST, &CKSJShow3DDlg::OnMsgLost)
	ON_MESSAGE(WM_ENDONE, &CKSJShow3DDlg::OnMsgEndone)
	ON_MESSAGE(WM_PROFILE, &CKSJShow3DDlg::OnMsgProfile)
	ON_MESSAGE(MSG_TRACE, &CKSJShow3DDlg::OnTraceMsg)
	ON_MESSAGE(MSG_CLEAR_TRACE, &CKSJShow3DDlg::OnClearMsg)
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_CHECK_SAVE, &CKSJShow3DDlg::OnBnClickedCheckSave)
	ON_BN_CLICKED(IDC_BUTTON_LOADINI, &CKSJShow3DDlg::OnBnClickedButtonLoadini)
	ON_BN_CLICKED(IDC_BUTTON_SAVEINI, &CKSJShow3DDlg::OnBnClickedButtonSaveini)
	ON_WM_MOUSEWHEEL()
	ON_CBN_SELCHANGE(IDC_COMBO_DEVICE_LIST, &CKSJShow3DDlg::OnCbnSelchangeComboDeviceList)
	ON_BN_CLICKED(IDC_BUTTON_STARTTRIGGER, &CKSJShow3DDlg::OnBnClickedButtonStarttrigger)
	ON_BN_CLICKED(IDC_BUTTON_ROI, &CKSJShow3DDlg::OnBnClickedButtonRoi)
	ON_BN_CLICKED(IDC_BUTTON_EG, &CKSJShow3DDlg::OnBnClickedButtonEg)
	ON_BN_CLICKED(IDC_BUTTON_DATATRIGGER, &CKSJShow3DDlg::OnBnClickedButtonDatatrigger)
	ON_BN_CLICKED(IDC_BUTTON_LOG, &CKSJShow3DDlg::OnBnClickedButtonLog)
	ON_EN_CHANGE(IDC_EDIT_LINE, &CKSJShow3DDlg::OnEnChangeEditLine)
	ON_BN_CLICKED(IDC_CHECK_LOG, &CKSJShow3DDlg::OnBnClickedCheckLog)
	
	ON_BN_CLICKED(IDC_BUTTON_SERIALS, &CKSJShow3DDlg::OnBnClickedButtonSerials)
END_MESSAGE_MAP()

LRESULT CKSJShow3DDlg::OnTraceMsg(WPARAM wParam, LPARAM lParam)
{
	CString msg = (LPCTSTR)lParam;
	int index = msg.Find("(");
	SYSTEMTIME st;
	CString strTime;
	GetLocalTime(&st);
	strTime.Format("%02d:%02d:%02d:%03d: ", st.wHour, st.wMinute, st.wSecond, st.wMilliseconds);
	m_ListTrace.AddString(strTime + msg.Left(index));
	return 0;
}

LRESULT CKSJShow3DDlg::OnClearMsg(WPARAM wParam, LPARAM lParam)
{
	m_ListTrace.ResetContent();
	g_nTraceNum = 0;
	return 0;
}


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
	TRACE_API(KSJ3D_Inital(), this);
	CComboBox *pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TYPE);
	pComboBox->ResetContent();
	CString strtype[2] = { _T("轮廓"), _T("3D") };
	for (int i = 0; i < 2; i++)
	{
		pComboBox->AddString(strtype[i]);
	}

	int pos;
	GetModuleFileName(NULL, m_path.GetBufferSetLength(MAX_PATH + 1), MAX_PATH);
	m_path.ReleaseBuffer();
	pos = m_path.ReverseFind('\\');
	m_path = m_path.Left(pos + 1);
	CString path = m_path;
	path += _T("KSJApi.ini");
	int log = GetPrivateProfileInt("Manage", "LogEnable", 1, path.GetString());
	((CButton*)GetDlgItem(IDC_CHECK_LOG))->SetCheck(log);
	
	m_nDeviceCurSel = 0;
	UpdateDeviceList();//获取相机的列表插入到界面上放下拉框
	m_TriggerDlg.Create(IDD_DIALOG_TRIGGER, this);
	m_RoiDlg.Create(IDD_DIALOG_ROI, this);
	m_ParamDlg.Create(IDD_DIALOG_PARAM, this);
	m_StartTriggerDlg.Create(IDD_DIALOG_START_TRIGGER, this);
	UpdateInterface();

	GetDlgItem(IDC_BUTTON_STARTTRIGGER)->MoveWindow(40, 110, 170, 25);
	GetDlgItem(IDC_BUTTON_ROI)->MoveWindow(40, 150, 170, 25);
	GetDlgItem(IDC_BUTTON_EG)->MoveWindow(40, 190, 170, 25);
	GetDlgItem(IDC_BUTTON_DATATRIGGER)->MoveWindow(40, 230, 170, 25);
	GetDlgItem(IDC_BUTTON_LOG)->MoveWindow(40, 270, 170, 25);

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
	m_pWnd3DImage = new CKSJImageWnd(this, -1);
	m_pWnd3DImage->Create(NULL, TEXT(""), WS_CHILD | WS_BORDER | WS_HSCROLL | WS_VSCROLL | WS_VISIBLE, m_rcZmapImageWnd, this, 0, NULL);   // 
	m_pWnd3DImage->ShowWindow(SW_SHOW);

	m_pWndZmapImage = new CKSJProfileWnd(this, -1);
	m_pWndZmapImage->Create(NULL, TEXT(""), WS_CHILD | WS_BORDER | WS_HSCROLL | WS_VSCROLL | WS_VISIBLE, m_rcZmapImageWnd, this, 0, NULL);   // 
	m_pWndZmapImage->ShowWindow(SW_SHOW);

	OnCbnSelchangeComboType();

	m_nSystemTimer = SetTimer(SYSTEM_TIMER_INDEX, 1000, NULL);
	CRect rect;
	rect.left = 730;
	rect.top = 690;
	rect.right = 1130;
	rect.bottom = 720;
	GetDlgItem(IDC_STATIC_MSG)->MoveWindow(rect);
	rect.left = 270;
	rect.top = 730;
	rect.right = 1230;
	rect.bottom = 890;
	GetDlgItem(IDC_LIST_TRACE)->MoveWindow(rect);
	m_ListTrace.SetHorizontalExtent(1100);
	GetDlgItem(IDC_STATIC_PROFILE)->MoveWindow(40, 610, 170, 25);
	KSJ3D_GetReturnCodeString(0);
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

	TRACE_API(KSJ3D_UnInitial(), this);

	m_pWnd3DImage->DestroyWindow();
	delete m_pWnd3DImage;
	m_pWnd3DImage = NULL;

	m_pWndZmapImage->DestroyWindow();
	delete m_pWndZmapImage;
	m_pWndZmapImage = NULL;

	m_pWndSrcImage->DestroyWindow();
	delete m_pWndSrcImage;
	m_pWndSrcImage = NULL;
}

void CKSJShow3DDlg::UpdateDeviceList()
{
	m_ListTrace.ResetContent();
	
	TRACE_API(KSJ3D_UnInitial(), this);
	TRACE_API(KSJ3D_Inital(), this);
	TRACE_API(KSJ3D_GetDeviceCount(&m_nDeviceNum), this);//获取电脑连接的相机数量

	if (m_nDeviceNum == 0)//如果未连接相机则跳出
	{
		GetDlgItem(IDC_STATIC_DEVICENUM)->SetWindowText(_T("未查询到相机"));
		m_nDeviceCurSel = -1;
		GetDlgItem(IDC_BUTTON_STARTTRIGGER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_ROI)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_EG)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_DATATRIGGER)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LOG)->EnableWindow(FALSE);

		GetDlgItem(IDC_CHECK_PREVIEW)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_ONE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_CAPTURE)->EnableWindow(FALSE);

		GetDlgItem(IDC_EDIT_LINE)->EnableWindow(FALSE);
		GetDlgItem(IDC_SPIN_LINE)->EnableWindow(FALSE);
		GetDlgItem(IDC_COMBO_TYPE)->EnableWindow(FALSE);
		GetDlgItem(IDC_CHECK_SAVE)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_LOADINI)->EnableWindow(FALSE);
		GetDlgItem(IDC_BUTTON_SAVEINI)->EnableWindow(FALSE);
		GetDlgItem(IDC_EDIT_ININAME)->EnableWindow(FALSE);
		return;
	}
	CComboBox    *pComboBox = NULL;
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_DEVICE_LIST);
	pComboBox->ResetContent();
	char CameraName[64];
	char szOpen[64];
	bool bIs3DCamera;
	int i;
	for (i = 0; i < m_nDeviceNum; i++)//获取每个相机的参数插入到顶部下拉框中
	{
		m_DeviceInfo[i].nIndex = i;
		TRACE_API(KSJ3D_GetDeviceInformation(i, &m_DeviceInfo[i].nDeviceType, &(m_DeviceInfo[i].nSerials), &(m_DeviceInfo[i].wFirmwareVersion), &(m_DeviceInfo[i].wFpgaVersion)), this);
		TRACE_API(KSJ3D_Is3DCamera(i, &bIs3DCamera), this);
		if (bIs3DCamera)
		{
			TRACE_API(KSJ3D_GetCameraName(i, CameraName), this);
		}
		else strcpy(CameraName, g_szDeviceType[m_DeviceInfo[i].nDeviceType]);
		if (!CheckCameraName(CameraName)) strcpy(CameraName, "3DCamera");
		BYTE btMajVersion = (m_DeviceInfo[i].wFirmwareVersion & 0xFF00) >> 8;		// 得到主版本号
		BYTE btMinVersion = m_DeviceInfo[i].wFirmwareVersion & 0x00FF;				// 得到副版本号

		BYTE btFpgaMajVersion = (m_DeviceInfo[i].wFpgaVersion & 0xFF00) >> 8;		// 得到主版本号
		BYTE btFpgaMinVersion = m_DeviceInfo[i].wFpgaVersion & 0x00FF;				// 得到副版本号

		TCHAR  szMenuItem[256] = { '\0' };
		sprintf_s(szMenuItem, _T("(%s)-Serials(%d)-Fw(%d.%d)-Fpga(%d.%d)"),  CameraName, m_DeviceInfo[i].nSerials, btMajVersion, btMinVersion, btFpgaMajVersion, btFpgaMinVersion);
		pComboBox->AddString(szMenuItem);
	}
	
	pComboBox->SetCurSel(m_nDeviceCurSel);
	SetDlgItemInt(IDC_EDIT_SERIALS, m_DeviceInfo[m_nDeviceCurSel].nSerials);
	

	GetDlgItem(IDC_STATIC_DEVICENUM)->SetWindowText(_T("相机连接成功"));
}


void CKSJShow3DDlg::UpdateInterface()//获取选中相机的数据显示在界面上
{
	if (m_nDeviceCurSel == -1)   return;
	for (int i = 0; i < m_nDeviceNum; i++)
	{
		if (i != m_nDeviceCurSel) TRACE_API(KSJ3D_DeviceClose(i), this);
	}
	int nRet = KSJ3D_DeviceOpen(m_nDeviceCurSel);
	if (nRet == RET_SUCCESS)
	{
		TRACE_API(KSJ3D_Is3DCamera(m_nDeviceCurSel, &m_bIs3DCamera), this);
		if (m_bIs3DCamera)
		{
			char CameraName[64];
			TRACE_API(KSJ3D_GetCameraName(m_nDeviceCurSel, CameraName), this);
			if (!CheckCameraName(CameraName)) strcpy(CameraName, "3DCamera");
			CString path;
			CString str;
			str.Format(_T("\\param_%s_%d.ini"), CameraName, m_DeviceInfo[m_nDeviceCurSel].nSerials);
			path = m_path + str;
			KSJINI_Init(&m_hKSJIni, path);

			GetDlgItem(IDC_STATIC_DEVICENUM)->SetWindowText(_T("相机连接成功"));
			TRACE_API(KSJ3D_LaserModeSet(m_nDeviceCurSel, LM_FLASH), this);
			TRACE_API(KSJ3D_RegisterLiveImageCB(m_nDeviceCurSel, NULL, this), this);
			TRACE_API(KSJ3D_RegisterProfileDataCB(m_nDeviceCurSel, NULL, this), this);
			TRACE_API(KSJ3D_RegisterProfileImageCB(m_nDeviceCurSel, NULL, this), this);
			TRACE_API(KSJ3D_RegisterPointCloudDataCB(m_nDeviceCurSel, NULL, this), this);
			TRACE_API(KSJ3D_RegisterPointCloudImageCB(m_nDeviceCurSel, NULL, this), this);

			TRACE_API(KSJ3D_SetObliterateInvalidData(m_nDeviceCurSel, false), this);
			
			Readini(m_hKSJIni);
			
			GetDlgItem(IDC_BUTTON_STARTTRIGGER)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_ROI)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_EG)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_DATATRIGGER)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_LOG)->EnableWindow(TRUE);

			GetDlgItem(IDC_CHECK_PREVIEW)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_ONE)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_CAPTURE)->EnableWindow(TRUE);

			GetDlgItem(IDC_EDIT_LINE)->EnableWindow(TRUE);
			GetDlgItem(IDC_SPIN_LINE)->EnableWindow(TRUE);
			GetDlgItem(IDC_COMBO_TYPE)->EnableWindow(TRUE);
			GetDlgItem(IDC_CHECK_SAVE)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_LOADINI)->EnableWindow(TRUE);
			GetDlgItem(IDC_BUTTON_SAVEINI)->EnableWindow(TRUE);
			GetDlgItem(IDC_EDIT_ININAME)->EnableWindow(TRUE);

			m_StartTriggerDlg.EnableWindow(TRUE);
			m_RoiDlg.EnableWindow(TRUE);
			m_ParamDlg.EnableWindow(TRUE);
			m_TriggerDlg.EnableWindow(TRUE);

			return;
		}
		else GetDlgItem(IDC_STATIC_DEVICENUM)->SetWindowText(_T("相机非3d相机"));
		
	}
	else GetDlgItem(IDC_STATIC_DEVICENUM)->SetWindowText(_T("相机已被其他进程打开")); 
	
	GetDlgItem(IDC_BUTTON_STARTTRIGGER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_ROI)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_EG)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_DATATRIGGER)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LOG)->EnableWindow(FALSE);

	GetDlgItem(IDC_CHECK_PREVIEW)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_ONE)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_CAPTURE)->EnableWindow(FALSE);

	GetDlgItem(IDC_EDIT_LINE)->EnableWindow(FALSE);
	GetDlgItem(IDC_SPIN_LINE)->EnableWindow(FALSE);
	GetDlgItem(IDC_COMBO_TYPE)->EnableWindow(FALSE);
	GetDlgItem(IDC_CHECK_SAVE)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_LOADINI)->EnableWindow(FALSE);
	GetDlgItem(IDC_BUTTON_SAVEINI)->EnableWindow(FALSE);
	GetDlgItem(IDC_EDIT_ININAME)->EnableWindow(FALSE);
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

	if (m_3dDataType)
	{
		m_rcSrcImageWnd.left = 280;
		m_rcSrcImageWnd.top = 110;
		m_rcSrcImageWnd.right = 680;
		m_rcSrcImageWnd.bottom = 680;
		m_pWndSrcImage->MoveWindow(m_rcSrcImageWnd);
		m_rcZmapImageWnd = m_rcSrcImageWnd;
		m_rcZmapImageWnd.left += 450;
		m_rcZmapImageWnd.right += 450;
		m_pWnd3DImage->MoveWindow(m_rcZmapImageWnd);
		m_pWnd3DImage->ShowWindow(TRUE);
		m_pWndZmapImage->ShowWindow(FALSE);
	}
	else
	{
		m_rcSrcImageWnd.left = 280;
		m_rcSrcImageWnd.top = 110;
		m_rcSrcImageWnd.right = 1130;
		m_rcSrcImageWnd.bottom = 390;
		m_pWndSrcImage->MoveWindow(m_rcSrcImageWnd);
		m_rcZmapImageWnd = m_rcSrcImageWnd;
		m_rcZmapImageWnd.top += 290;
		m_rcZmapImageWnd.bottom += 290;
		m_pWndZmapImage->MoveWindow(m_rcZmapImageWnd);

		m_pWnd3DImage->ShowWindow(FALSE);
		m_pWndZmapImage->ShowWindow(TRUE);
	}
}


void CKSJShow3DDlg::OnBnClickedCheckCapture()
{
	if (m_nDeviceCurSel == -1)   return;
	PostMessage(MSG_CLEAR_TRACE, 0, 0);
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_CHECK_CAPTURE))->GetCheck();
	if (bCheck)
	{
		TRACE_API(KSJ3D_RegisterLiveImageCB(m_nDeviceCurSel, LiveImageCallback, this), this);
		if (m_3dDataType)
		{
			m_nTotalLost = 0;
			TRACE_API(KSJ3D_RegisterPointCloudDataCB(m_nDeviceCurSel, PointCloudDataCallback, this), this);
			TRACE_API(KSJ3D_RegisterPointCloudImageCB(m_nDeviceCurSel, CloudImageCallback, this), this);
			m_pWnd3DImage->SetImageInfo(m_RoiDlg.m_nColSize, m_nNumberOfProfiles, 8);
		}
		else
		{
			TRACE_API(KSJ3D_RegisterProfileDataCB(m_nDeviceCurSel, ProfileDataCallback, this), this);
			TRACE_API(KSJ3D_RegisterProfileImageCB(m_nDeviceCurSel, ProfileImageCallback, this), this);
			
			float fZSize = m_RoiDlg.m_fZMax - m_RoiDlg.m_fZMin;
			int nHeight = fZSize / (m_RoiDlg.m_fFovFar / m_RoiDlg.m_nMaxColSize);
			m_pWndZmapImage->SetImageInfo(m_RoiDlg.m_nMaxColSize, nHeight, 8);
			m_pWndZmapImage->SetPointNum(m_RoiDlg.m_nColStart, m_RoiDlg.m_nColSize);
		}

		if (KSJ3D_StartAcquisition(m_nDeviceCurSel) == RET_SUCCESS)
		{
			GetDlgItem(IDC_STATIC_MSG)->SetWindowText(_T("开始采集"));
			m_StartTriggerDlg.EnableWindow(FALSE);
			m_StartTriggerDlg.SetEnable(FALSE);
			m_RoiDlg.EnableWindow(FALSE);
			m_RoiDlg.SetEnable(FALSE);
			m_TriggerDlg.EnableWindow(FALSE);
			m_TriggerDlg.SetEnable(FALSE);
			GetDlgItem(IDC_EDIT_LINE)->EnableWindow(FALSE);
			GetDlgItem(IDC_SPIN_LINE)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_TYPE)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_DEVICE_LIST)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_PREVIEW)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_ONE)->EnableWindow(FALSE);
		}
	}
	else
	{
		TRACE_API(KSJ3D_StopAcquisition(m_nDeviceCurSel), this);
		TRACE_API(KSJ3D_RegisterLiveImageCB(m_nDeviceCurSel, NULL, this), this);
		if (m_3dDataType)
		{
			TRACE_API(KSJ3D_RegisterPointCloudDataCB(m_nDeviceCurSel, NULL, this), this);
			TRACE_API(KSJ3D_RegisterPointCloudImageCB(m_nDeviceCurSel, NULL, this), this);
		}
		else
		{
			TRACE_API(KSJ3D_RegisterProfileDataCB(m_nDeviceCurSel, NULL, this), this);
			TRACE_API(KSJ3D_RegisterProfileImageCB(m_nDeviceCurSel, NULL, this), this);
		}

		m_StartTriggerDlg.EnableWindow(TRUE);
		m_StartTriggerDlg.SetEnable(TRUE);
		m_RoiDlg.EnableWindow(TRUE);
		m_RoiDlg.SetEnable(TRUE);
		m_TriggerDlg.EnableWindow(TRUE);
		m_TriggerDlg.SetEnable(TRUE);
		GetDlgItem(IDC_EDIT_LINE)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_LINE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_TYPE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_DEVICE_LIST)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_PREVIEW)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_ONE)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_MSG)->SetWindowText(_T("结束采集"));
	}
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
		int high = 300;
		if (type == 1)
		{
			GetDlgItem(IDC_BUTTON_STARTTRIGGER)->MoveWindow(40, 110, 170, 25);
			GetDlgItem(IDC_BUTTON_ROI)->MoveWindow(40, 150 + high, 170, 25);
			GetDlgItem(IDC_BUTTON_EG)->MoveWindow(40, 190 + high, 170, 25);
			GetDlgItem(IDC_BUTTON_DATATRIGGER)->MoveWindow(40, 230 + high, 170, 25);
			GetDlgItem(IDC_BUTTON_LOG)->MoveWindow(40, 270 + high, 170, 25);
			m_StartTriggerDlg.MoveWindow(20, 140, 250, 300);
			m_StartTriggerDlg.ShowWindow(SW_SHOW);
			PostMessage(MSG_CLEAR_TRACE, 0, 0);
		}
		else if (type == 2)
		{
			GetDlgItem(IDC_BUTTON_STARTTRIGGER)->MoveWindow(40, 110, 170, 25);
			GetDlgItem(IDC_BUTTON_ROI)->MoveWindow(40, 150, 170, 25);
			GetDlgItem(IDC_BUTTON_EG)->MoveWindow(40, 190 + high, 170, 25);
			GetDlgItem(IDC_BUTTON_DATATRIGGER)->MoveWindow(40, 230 + high, 170, 25);
			GetDlgItem(IDC_BUTTON_LOG)->MoveWindow(40, 270 + high, 170, 25);
			m_RoiDlg.MoveWindow(20, 180, 250, 300);
			m_RoiDlg.ShowWindow(SW_SHOW);
			PostMessage(MSG_CLEAR_TRACE, 0, 0);
		}
		else if (type == 3)
		{
			GetDlgItem(IDC_BUTTON_STARTTRIGGER)->MoveWindow(40, 110, 170, 25);
			GetDlgItem(IDC_BUTTON_ROI)->MoveWindow(40, 150, 170, 25);
			GetDlgItem(IDC_BUTTON_EG)->MoveWindow(40, 190, 170, 25);
			GetDlgItem(IDC_BUTTON_DATATRIGGER)->MoveWindow(40, 230 + high, 170, 25);
			GetDlgItem(IDC_BUTTON_LOG)->MoveWindow(40, 270 + high, 170, 25);
			m_ParamDlg.MoveWindow(20, 220, 250, 300);
			m_ParamDlg.ShowWindow(SW_SHOW);
			PostMessage(MSG_CLEAR_TRACE, 0, 0);
		}
		else if (type == 4)
		{
			GetDlgItem(IDC_BUTTON_STARTTRIGGER)->MoveWindow(40, 110, 170, 25);
			GetDlgItem(IDC_BUTTON_ROI)->MoveWindow(40, 150, 170, 25);
			GetDlgItem(IDC_BUTTON_EG)->MoveWindow(40, 190, 170, 25);
			GetDlgItem(IDC_BUTTON_DATATRIGGER)->MoveWindow(40, 230, 170, 25);
			GetDlgItem(IDC_BUTTON_LOG)->MoveWindow(40, 270 + high, 170, 25);
			m_TriggerDlg.MoveWindow(20, 260, 250, 300);
			m_TriggerDlg.ShowWindow(SW_SHOW);
			PostMessage(MSG_CLEAR_TRACE, 0, 0);

		}
		else if (type == 5)
		{
			GetDlgItem(IDC_BUTTON_STARTTRIGGER)->MoveWindow(40, 110, 170, 25);
			GetDlgItem(IDC_BUTTON_ROI)->MoveWindow(40, 150, 170, 25);
			GetDlgItem(IDC_BUTTON_EG)->MoveWindow(40, 190, 170, 25);
			GetDlgItem(IDC_BUTTON_DATATRIGGER)->MoveWindow(40, 230, 170, 25);
			GetDlgItem(IDC_BUTTON_LOG)->MoveWindow(40, 270, 170, 25);
			GetDlgItem(IDC_CHECK_LOG)->MoveWindow(40, 310, 60, 20);
			GetDlgItem(IDC_CHECK_LOG)->ShowWindow(SW_SHOW);
			PostMessage(MSG_CLEAR_TRACE, 0, 0);
		}
	}
	else
	{
		GetDlgItem(IDC_BUTTON_STARTTRIGGER)->MoveWindow(40, 110, 170, 25);
		GetDlgItem(IDC_BUTTON_ROI)->MoveWindow(40, 150, 170, 25);
		GetDlgItem(IDC_BUTTON_EG)->MoveWindow(40, 190, 170, 25);
		GetDlgItem(IDC_BUTTON_DATATRIGGER)->MoveWindow(40, 230, 170, 25);
		GetDlgItem(IDC_BUTTON_LOG)->MoveWindow(40, 270, 170, 25);
	}
}


void CKSJShow3DDlg::hidecontrol()
{
	m_TriggerDlg.ShowWindow(SW_HIDE);
	m_RoiDlg.ShowWindow(SW_HIDE);
	m_ParamDlg.ShowWindow(SW_HIDE);
	m_StartTriggerDlg.ShowWindow(SW_HIDE);
	GetDlgItem(IDC_CHECK_LOG)->ShowWindow(SW_HIDE);
}


void CKSJShow3DDlg::OnBnClickedCheckPreview()
{
	if (m_nDeviceCurSel == -1)   return;
	PostMessage(MSG_CLEAR_TRACE, 0, 0);
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_CHECK_PREVIEW))->GetCheck();
	if (bCheck)
	{
		TRACE_API(KSJ3D_SetDataTriggerMode(m_nDeviceCurSel, DTM_FREE_RUN), this);
		TRACE_API(KSJ3D_SetStartTrigger(m_nDeviceCurSel, STS_INPUT_0, false, TEM_FALLING_EDGE), this);
		m_pWndSrcImage->SetImageInfo(m_RoiDlg.m_nColSize, m_RoiDlg.m_nRowSize, 8);
		TRACE_API(KSJ3D_RegisterLiveImageCB(m_nDeviceCurSel, LiveImageCallback, this), this);
		if (KSJ3D_StartAcquisition(m_nDeviceCurSel) == RET_SUCCESS)
		{
			m_triggermode = m_TriggerDlg.m_DataTriggerMode;
			m_startmode = m_StartTriggerDlg.m_StartTriggerCondition;
			GetDlgItem(IDC_STATIC_MSG)->SetWindowText(_T("开始预览"));
			m_StartTriggerDlg.EnableWindow(FALSE);
			m_StartTriggerDlg.SetEnable(FALSE);
			m_RoiDlg.EnableWindow(FALSE);
			m_RoiDlg.SetEnable(FALSE);
			m_TriggerDlg.EnableWindow(FALSE);
			m_TriggerDlg.SetEnable(FALSE);
			GetDlgItem(IDC_EDIT_LINE)->EnableWindow(FALSE);
			GetDlgItem(IDC_SPIN_LINE)->EnableWindow(FALSE); 
			GetDlgItem(IDC_COMBO_TYPE)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_DEVICE_LIST)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_ONE)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_CAPTURE)->EnableWindow(FALSE);
		}
	}
	else
	{
		TRACE_API(KSJ3D_StopAcquisition(m_nDeviceCurSel), this);
		TRACE_API(KSJ3D_RegisterLiveImageCB(m_nDeviceCurSel, NULL, this), this);
		m_StartTriggerDlg.EnableWindow(TRUE);
		m_StartTriggerDlg.SetEnable(TRUE);
		m_RoiDlg.EnableWindow(TRUE);
		m_RoiDlg.SetEnable(TRUE);
		m_TriggerDlg.EnableWindow(TRUE);
		m_TriggerDlg.SetEnable(TRUE);
		GetDlgItem(IDC_EDIT_LINE)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_LINE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_TYPE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_DEVICE_LIST)->EnableWindow(TRUE);
		TRACE_API(KSJ3D_SetDataTriggerMode(m_nDeviceCurSel, m_triggermode), this);
		KSJ3D_TRIGGER_EDGE_MODE StartTriggerCondition = TEM_FALLING_EDGE;
		if (m_startmode != 0) StartTriggerCondition = (KSJ3D_TRIGGER_EDGE_MODE)(m_startmode - 1);
		else StartTriggerCondition = (KSJ3D_TRIGGER_EDGE_MODE)m_startmode;

		if (m_startmode == 0)
		{
			TRACE_API(KSJ3D_SetStartTrigger(m_nDeviceCurSel, STS_INPUT_0, false, StartTriggerCondition), this);
		}
		else KSJ3D_SetStartTrigger(m_nDeviceCurSel, STS_INPUT_0, true, StartTriggerCondition);
		GetDlgItem(IDC_CHECK_ONE)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_CAPTURE)->EnableWindow(TRUE);
		GetDlgItem(IDC_STATIC_MSG)->SetWindowText(_T("停止预览"));
	}
}

void CKSJShow3DDlg::OnBnClickedCheckOne()
{
	if (m_nDeviceCurSel == -1)   return;
	PostMessage(MSG_CLEAR_TRACE, 0, 0);
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_CHECK_ONE))->GetCheck();
	if (bCheck)
	{
		GetDlgItem(IDC_STATIC_MSG)->SetWindowText(_T("开始一次采集"));
		m_one = 1;
		TRACE_API(KSJ3D_RegisterLiveImageCB(m_nDeviceCurSel, LiveImageCallback, this), this);
		if (m_3dDataType)
		{
			TRACE_API(KSJ3D_RegisterPointCloudDataCB(m_nDeviceCurSel, PointCloudDataCallback, this), this);
			TRACE_API(KSJ3D_RegisterPointCloudImageCB(m_nDeviceCurSel, CloudImageCallback, this), this);
			m_pWnd3DImage->SetImageInfo(m_RoiDlg.m_nColSize, m_nNumberOfProfiles, 8);
		}
		else
		{
			TRACE_API(KSJ3D_RegisterProfileDataCB(m_nDeviceCurSel, ProfileDataCallback, this), this);
			TRACE_API(KSJ3D_RegisterProfileImageCB(m_nDeviceCurSel, CloudImageCallback, this), this);
			float fZSize = m_RoiDlg.m_fZMax - m_RoiDlg.m_fZMin;
			int nHeight = fZSize / (m_RoiDlg.m_fFovFar / m_RoiDlg.m_nMaxColSize);
			m_pWndZmapImage->SetImageInfo(m_RoiDlg.m_nMaxColSize, nHeight, 8);
			m_pWndZmapImage->SetPointNum(m_RoiDlg.m_nColStart, m_RoiDlg.m_nColSize);
		}

		if (KSJ3D_StartAcquisition(m_nDeviceCurSel) == RET_SUCCESS)
		{
			m_StartTriggerDlg.EnableWindow(FALSE);
			m_StartTriggerDlg.SetEnable(FALSE);
			m_RoiDlg.EnableWindow(FALSE);
			m_RoiDlg.SetEnable(FALSE);
			m_TriggerDlg.EnableWindow(FALSE);
			m_TriggerDlg.SetEnable(FALSE);
			GetDlgItem(IDC_EDIT_LINE)->EnableWindow(FALSE);
			GetDlgItem(IDC_SPIN_LINE)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_TYPE)->EnableWindow(FALSE);
			GetDlgItem(IDC_COMBO_DEVICE_LIST)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_PREVIEW)->EnableWindow(FALSE);
			GetDlgItem(IDC_CHECK_CAPTURE)->EnableWindow(FALSE);
		}
	}
	else
	{
		m_one = 0;
		TRACE_API(KSJ3D_StopAcquisition(m_nDeviceCurSel), this);
		TRACE_API(KSJ3D_RegisterLiveImageCB(m_nDeviceCurSel, NULL, this), this);
		if (m_3dDataType)
		{
			TRACE_API(KSJ3D_RegisterPointCloudDataCB(m_nDeviceCurSel, NULL, this), this);
			TRACE_API(KSJ3D_RegisterPointCloudImageCB(m_nDeviceCurSel, NULL, this), this);

		}
		else
		{
			TRACE_API(KSJ3D_RegisterProfileDataCB(m_nDeviceCurSel, NULL, this), this);
			TRACE_API(KSJ3D_RegisterProfileImageCB(m_nDeviceCurSel, NULL, this), this);
		}

		m_StartTriggerDlg.EnableWindow(TRUE);
		m_StartTriggerDlg.SetEnable(TRUE);
		m_RoiDlg.EnableWindow(TRUE);
		m_RoiDlg.SetEnable(TRUE);
		m_TriggerDlg.EnableWindow(TRUE);
		m_TriggerDlg.SetEnable(TRUE);
		GetDlgItem(IDC_EDIT_LINE)->EnableWindow(TRUE);
		GetDlgItem(IDC_SPIN_LINE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_TYPE)->EnableWindow(TRUE);
		GetDlgItem(IDC_COMBO_DEVICE_LIST)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_PREVIEW)->EnableWindow(TRUE);
		GetDlgItem(IDC_CHECK_CAPTURE)->EnableWindow(TRUE);
	}
}


void CKSJShow3DDlg::Readini(HANDLE hKSJIni)
{
	m_StartTriggerDlg.m_nDeviceCurSel = m_nDeviceCurSel;
	m_RoiDlg.m_nDeviceCurSel = m_nDeviceCurSel;
	m_ParamDlg.m_nDeviceCurSel = m_nDeviceCurSel;
	m_TriggerDlg.m_nDeviceCurSel = m_nDeviceCurSel;
	int i;

	unsigned long usvalue = 0;
	int nvalue, nvalue2, nvalue3, nvalue4;
	float fvalue;
	TCHAR szText[64] = { 0 };
	TCHAR szTemp[64];
	
	KSJINI_GetDWORD(hKSJIni, _T("3D"), _T("TYPE"), 0, (DWORD*)&m_3dDataType);
	CComboBox    *pComboBox;
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_TYPE);
	pComboBox->SetCurSel(m_3dDataType);
	((CSpinButtonCtrl*)GetDlgItem(IDC_SPIN_LINE))->SetRange32(0, 65536);
	KSJINI_GetDWORD(hKSJIni, _T("StartTrigger"), _T("MultiLine"), 500, (DWORD*)&m_nNumberOfProfiles);
	TRACE_API(KSJ3D_SetMaxNumberOfProfilesToCapture(m_nDeviceCurSel, m_nNumberOfProfiles), this);
	_stprintf_s(szTemp, _T("%d"), m_nNumberOfProfiles);
	GetDlgItem(IDC_EDIT_LINE)->SetWindowText(szTemp);
	m_StartTriggerDlg.Readini(hKSJIni);
	m_ParamDlg.Readini(hKSJIni);
	m_RoiDlg.Readini(hKSJIni);
	m_TriggerDlg.Readini(hKSJIni);
	
}


void CKSJShow3DDlg::Writeini(HANDLE hKSJIni)
{
	TCHAR szText[32];
	int nIndex=0;
	m_StartTriggerDlg.Writeini(hKSJIni);
	KSJINI_SetInt(hKSJIni, _T("StartTrigger"), _T("MultiLine"), m_nNumberOfProfiles);
	m_ParamDlg.Writeini(hKSJIni);
	m_RoiDlg.Writeini(hKSJIni);
	m_TriggerDlg.Writeini(hKSJIni);
	KSJINI_SetInt(hKSJIni, _T("3D"), _T("TYPE"), m_3dDataType);
}

 
void CKSJShow3DDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (m_rcSrcImageWnd.PtInRect(point))
	{
		m_pWndZmapImage->SetSelected(FALSE);
		m_pWndSrcImage->SetSelected(TRUE);
		m_pWnd3DImage->SetSelected(FALSE);
	}
	else if (m_rcZmapImageWnd.PtInRect(point))
	{
		if (m_3dDataType)
		{
			m_pWnd3DImage->SetSelected(TRUE);
			m_pWndSrcImage->SetSelected(FALSE);
		}
		else
		{
			m_pWndZmapImage->SetSelected(TRUE);
			m_pWndSrcImage->SetSelected(FALSE);
		}
		
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}

LRESULT CKSJShow3DDlg::OnMsgMsg(WPARAM wParam, LPARAM lParam)
{
	GetDlgItem(IDC_STATIC_MSG)->SetWindowText(m_szMsg);

	return 0;
}

LRESULT CKSJShow3DDlg::OnMsgLost(WPARAM wParam, LPARAM lParam)
{
	int nTotalLostProfileNum = int(wParam);
	int nTotal = int(lParam);
	TCHAR szLost[32];
	_stprintf_s(szLost, _T("采集丢帧:%d"), nTotalLostProfileNum);
	GetDlgItem(IDC_STATIC_LOST)->SetWindowText(szLost);
	_stprintf_s(szLost, _T("总丢帧:%d"), nTotal);
	GetDlgItem(IDC_STATIC_TOTALLOST)->SetWindowText(szLost);
	
	return 0;
}

LRESULT CKSJShow3DDlg::OnMsgProfile(WPARAM wParam, LPARAM lParam)
{
	float *data = (float *)lParam;
	char szProfile[32];
	sprintf(szProfile, "轮廓坐标x:%.3f,z:%.3f", data[0], data[1]);
	GetDlgItem(IDC_STATIC_PROFILE)->SetWindowText(szProfile);
	return 0;
}

void __stdcall CKSJShow3DDlg::LiveImageCallback(int nChannel, unsigned char *pData, int nWidth, int nHeight, int nBitCount, void *lpContext)
{
	CKSJShow3DDlg* pMainDlg = (CKSJShow3DDlg*)lpContext;
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
	m_pWndZmapImage->SetProfile(x, z, m_RoiDlg.m_fFovFar, m_RoiDlg.m_fZMin, m_RoiDlg.m_fZMax);
#ifdef halcon12
	if (m_save)
	{
		float *y = new float[m_RoiDlg.m_nColSize];
		memset(y, 0, sizeof(float)* m_RoiDlg.m_nColSize);
		CString path;
		_tcscpy(m_szMsg, _T("om3保存中"));
		PostMessage(WM_MSG, 0, 0);
		SYSTEMTIME LocalTime;
		TCHAR szFileName[128] = { 0 };
		HObject Hobjx, Hobjy, Hobjz;
#ifdef _WIN64
		gen_image1(&Hobjx, "real", m_RoiDlg.m_nColSize, 1, (long long)x);//深拷贝
		gen_image1(&Hobjy, "real", m_RoiDlg.m_nColSize, 1, (long long)y);
		gen_image1(&Hobjz, "real", m_RoiDlg.m_nColSize, 1, (long long)z);

#else
		gen_image1(&Hobjx, "real", m_RoiDlg.m_nColSize, 1, (long)x);//深拷贝
		gen_image1(&Hobjy, "real", m_RoiDlg.m_nColSize, 1, (long)y);
		gen_image1(&Hobjz, "real", m_RoiDlg.m_nColSize, 1, (long)z);
#endif
		HTuple hv_ObjectModel3D;
		XyzToObjectModel3d(Hobjx, Hobjy, Hobjz, &hv_ObjectModel3D);
		GetLocalTime(&LocalTime);
		_stprintf_s(szFileName, _T("capture-%04d-%02d-%02d-%02d-%02d-%02d-%03d.om3"), LocalTime.wYear, LocalTime.wMonth, LocalTime.wDay, LocalTime.wHour, LocalTime.wMinute, LocalTime.wSecond, LocalTime.wMilliseconds);
		path = m_path + szFileName;
		WriteObjectModel3d(hv_ObjectModel3D, _T("om3"), path.GetString(), _T("invert_normals"), _T("false"));
		_tcscpy(m_szMsg, _T("om3保存完毕"));
		delete[]y;
		PostMessage(WM_MSG, 0, 0);
	}
#else
	if (m_save)
	{
		float *y = new float[m_RoiDlg.m_nColSize];
		memset(y, 0, sizeof(float)* m_RoiDlg.m_nColSize);
		CString path;
		_tcscpy(m_szMsg, _T("pcd保存中"));
		PostMessage(WM_MSG, 0, 0);
		SYSTEMTIME LocalTime;
		TCHAR szFileName[128] = { 0 };
		GetLocalTime(&LocalTime);
		_stprintf_s(szFileName, _T("capture-%04d-%02d-%02d-%02d-%02d-%02d-%03d.pcd"), LocalTime.wYear, LocalTime.wMonth, LocalTime.wDay, LocalTime.wHour, LocalTime.wMinute, LocalTime.wSecond, LocalTime.wMilliseconds);
		path = m_path + szFileName;
		TRACE_API(KSJ3D_HelperSaveToPCD(m_RoiDlg.m_nColSize, 1, x, y, z, path.GetString()), this);
		delete[]y;
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

void __stdcall CKSJShow3DDlg::ProfileImageCallback(int nChannel, unsigned char *pData, int nWidth, int nHeight, int nBitCount, void *lpContext)
{
	CKSJShow3DDlg* pMainDlg = (CKSJShow3DDlg*)lpContext;

	pMainDlg->ShowProfileImage(pData, nWidth, nHeight, nBitCount);
}

void CKSJShow3DDlg::ShowProfileImage(unsigned char *pData, int nWidth, int nHeight, int nBitCount)
{
	//m_pWndZmapImage->LoadImage(pData, nWidth, nHeight, nBitCount);
}

void __stdcall CKSJShow3DDlg::PointCloudDataCallback(int nChannel, int nTotalPointNum, int nProfileNum, float *x, float *y, float *z, int nTotalLostProfileNum, void *lpContext)
{
	CKSJShow3DDlg* pMainDlg = (CKSJShow3DDlg*)lpContext;
	pMainDlg->PointCloudData(nChannel, nTotalPointNum, nProfileNum, x, y, z, nTotalLostProfileNum);
}

void CKSJShow3DDlg::PointCloudData(int nChannel, int nTotalPointNum, int nProfileNum, float *x, float *y, float *z, int nTotalLostProfileNum)
{
	TRACE("TotalPoint:%d, TotalLostProfileNum:%d\n", nTotalPointNum, nTotalLostProfileNum);

	if (nTotalLostProfileNum > 0) m_nTotalLost += nTotalLostProfileNum;
	PostMessage(WM_LOST, nTotalLostProfileNum, m_nTotalLost);

#ifdef halcon12
	if (m_save)
	{
		CString path;
		_tcscpy(m_szMsg,  _T("om3保存中"));
		PostMessage(WM_MSG, 0, 0);
		SYSTEMTIME LocalTime;
		TCHAR szFileName[128] = { 0 };
		HObject Hobjx, Hobjy, Hobjz;
#ifdef _WIN64
		gen_image1(&Hobjx, "real", m_RoiDlg.m_nColSize, m_nNumberOfProfiles, (long long)x);//深拷贝
		gen_image1(&Hobjy, "real", m_RoiDlg.m_nColSize, m_nNumberOfProfiles, (long long)y);
		gen_image1(&Hobjz, "real", m_RoiDlg.m_nColSize, m_nNumberOfProfiles, (long long)z);

#else
		gen_image1(&Hobjx, "real", m_RoiDlg.m_nColSize, m_nNumberOfProfiles, (long)x);//深拷贝
		gen_image1(&Hobjy, "real", m_RoiDlg.m_nColSize, m_nNumberOfProfiles, (long)y);
		gen_image1(&Hobjz, "real", m_RoiDlg.m_nColSize, m_nNumberOfProfiles, (long)z);
#endif
		HTuple hv_ObjectModel3D;
		XyzToObjectModel3d(Hobjx, Hobjy, Hobjz, &hv_ObjectModel3D);
		GetLocalTime(&LocalTime);
		_stprintf_s(szFileName,  _T("capture-%04d-%02d-%02d-%02d-%02d-%02d-%03d.om3"), LocalTime.wYear, LocalTime.wMonth, LocalTime.wDay, LocalTime.wHour, LocalTime.wMinute, LocalTime.wSecond, LocalTime.wMilliseconds);
		path = m_path + szFileName;
		WriteObjectModel3d(hv_ObjectModel3D, _T("om3"), path.GetString(), _T("invert_normals"), _T("false"));
		ClearObjectModel3d(hv_ObjectModel3D);
		_tcscpy(m_szMsg,  _T("om3保存完毕"));
		PostMessage(WM_MSG, 0, 0);
	}
#else
	if (m_save)
	{
		CString path;
		_tcscpy(m_szMsg, _T("pcd保存中"));
		PostMessage(WM_MSG, 0, 0);
		SYSTEMTIME LocalTime;
		TCHAR szFileName[128] = { 0 };
		GetLocalTime(&LocalTime);
		_stprintf_s(szFileName, _T("capture-%04d-%02d-%02d-%02d-%02d-%02d-%03d.pcd"), LocalTime.wYear, LocalTime.wMonth, LocalTime.wDay, LocalTime.wHour, LocalTime.wMinute, LocalTime.wSecond, LocalTime.wMilliseconds);
		path = m_path + szFileName;
		TRACE_API(KSJ3D_HelperSaveToPCD(m_RoiDlg.m_nColSize, m_nNumberOfProfiles, x, y, z, path.GetString()), this);
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
	m_pWnd3DImage->LoadImage(pData, nWidth, nHeight, nBitCount);
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
		strMessage.Format(_T("采集帧率: %.2f， 触发频率: %.2f"), fTransmissionRate, fExternalTriggerRateHz);
		GetDlgItem(IDC_STATIC_MSG2)->SetWindowText(strMessage);
	}

	CDialogEx::OnTimer(nIDEvent);
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
	
	GetDlgItem(IDC_EDIT_ININAME)->GetWindowText(strini,64);
	if (_tcslen(strini) == 0)
	{
		MessageBox(_T("配置文件名不得为空"));
		return;
	}
	path = m_path;
	path += strini;
	path += _T(".ini");
	KSJINI_Init(&hKSJIni, path.GetString());
	Writeini(hKSJIni);
	KSJINI_UnInit(&hKSJIni);
}


BOOL CKSJShow3DDlg::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	if (zDelta > 0)
	{
		m_pWndSrcImage->ZoomIn();
		if (m_3dDataType) m_pWnd3DImage->ZoomIn();
		else m_pWndZmapImage->ZoomIn();
	}
	if (zDelta < 0)
	{
		m_pWndSrcImage->ZoomOut();
		if (m_3dDataType) m_pWnd3DImage->ZoomOut();
		else m_pWndZmapImage->ZoomOut();
	}
	return CDialogEx::OnMouseWheel(nFlags, zDelta, pt);
}


void CKSJShow3DDlg::OnCbnSelchangeComboDeviceList()
{
	CComboBox    *pComboBox = NULL;
	pComboBox = (CComboBox*)GetDlgItem(IDC_COMBO_DEVICE_LIST);
	m_nDeviceCurSel = pComboBox->GetCurSel();//下拉框重新选择相机
	SetDlgItemInt(IDC_EDIT_SERIALS, m_DeviceInfo[m_nDeviceCurSel].nSerials);
	UpdateDeviceList();
	UpdateInterface();
}



bool CKSJShow3DDlg::CheckCameraName(char * name)
{
	for (int i = 0; i < strlen(name);i++)
	{
		if (name[i]<0x20 || name[i]>0x7F) return false;
	}
	return true;
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


void CKSJShow3DDlg::OnBnClickedButtonLog()
{
	type = 5;
	if (se != type) se = type;
	else se = 0;
	SelectSet(type);
}


void CKSJShow3DDlg::OnEnChangeEditLine()
{
	if (m_nDeviceCurSel == -1)   return;
	m_nNumberOfProfiles = GetDlgItemInt(IDC_EDIT_LINE);
	TRACE_API(KSJ3D_SetMaxNumberOfProfilesToCapture(m_nDeviceCurSel, m_nNumberOfProfiles), this);
}


void CKSJShow3DDlg::OnBnClickedCheckLog()
{
	BOOL bCheck = ((CButton*)GetDlgItem(IDC_CHECK_LOG))->GetCheck();
	CString path = m_path;
	path += _T("KSJApi.ini");
	if (bCheck)
	{
		KSJ3D_LogSet(true, NULL);
		//WritePrivateProfileString("Manage", "LogEnable", "1", path.GetString());
		//MessageBox("打开LOG记录，重启程序生效，生成信息保存到执行程序目录KSJApiLog文件夹！");
	}
	else
	{
		KSJ3D_LogSet(false, NULL);
		//WritePrivateProfileString("Manage", "LogEnable", "0", path.GetString());
		//MessageBox("关闭LOG记录，重启程序生效！");
	}
}


void CKSJShow3DDlg::OnBnClickedButtonSerials()
{
	if (m_nDeviceCurSel == -1)   return;
	PostMessage(MSG_CLEAR_TRACE, 0, 0);
	TRACE_API(KSJ3D_SetSerialNumber(m_nDeviceCurSel, GetDlgItemInt(IDC_EDIT_SERIALS)), this);
	m_DeviceInfo[m_nDeviceCurSel].nSerials = GetDlgItemInt(IDC_EDIT_SERIALS);
	UpdateDeviceList();//更新下拉列表
	char CameraName[64];
	TRACE_API(KSJ3D_GetCameraName(m_nDeviceCurSel, CameraName), this);
	if (!CheckCameraName(CameraName)) strcpy(CameraName, "3DCamera");
	CString path;
	CString str;
	str.Format(_T("\\param_%s_%d.ini"), CameraName, m_DeviceInfo[m_nDeviceCurSel].nSerials);//更新需要保持的配置文件
	path = m_path + str;
	KSJINI_Init(&m_hKSJIni, path);
}
