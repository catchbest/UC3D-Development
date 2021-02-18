#include "stdafx.h"
#include "KSJProfileWnd.h"
#include "KSJShow3DDlg.h"
#include <vector>
using namespace std;
#ifdef _DEBUG
//#define new DEBUG_NEW
//#undef THIS_FILE
//static TCHAR THIS_FILE[] = __FILE__;
#endif

#define WIDTHBYTES(i)    ( ( i+31 ) / 32 * 4) 

#define  ZOOM_INDEX_MIN         0
#define  ZOOM_INDEX_MAX			22
#define  ZOOM_INDEX_DEFAULT     12			// 实际尺寸显示时1:1比例时，缩放比例因子的索引




IMPLEMENT_DYNCREATE(CKSJProfileWnd, CWnd)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
CKSJProfileWnd::CKSJProfileWnd( CWnd *pParent, int nIndex ) : 
m_pParent( pParent ), 
m_nIndex( nIndex ),
m_pImageBmpInfo( NULL ),
m_pImageBmpData( NULL ),
m_bSelected( FALSE ),
m_hZoom(NULL),
m_dZoomFactor( 1.0 ),
m_dShowZoomH( 1.0 ),
m_dShowZoomV( 1.0 ),
m_pfX(NULL),
m_pfZ(NULL),
m_pfImage(NULL),
m_nxScale(10),
m_nzScale(10),
m_nPointNum(0),
m_bOpenFromFile( FALSE )
{
	InitializeCriticalSection(&m_csImageBmpData);

	m_ptOffset.x = m_ptOffset.y = 0;

	m_pImageBmpInfo    = (LPBITMAPINFO)new TCHAR[ sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD) ];
	memset( (VOID*)m_pImageBmpInfo, 0, sizeof(BITMAPINFOHEADER) + 256 * sizeof(RGBQUAD) );

	if( m_pImageBmpInfo == NULL )
	{
		return ;
	}

	m_pImageBmpInfo->bmiHeader.biSize			= sizeof(tagBITMAPINFOHEADER);
	m_pImageBmpInfo->bmiHeader.biPlanes			= 1;
	m_pImageBmpInfo->bmiHeader.biCompression	= BI_RGB;
	m_pImageBmpInfo->bmiHeader.biSizeImage		= 0;
	m_pImageBmpInfo->bmiHeader.biXPelsPerMeter	= 0;
	m_pImageBmpInfo->bmiHeader.biYPelsPerMeter	= 0;
	m_pImageBmpInfo->bmiHeader.biClrUsed		= 0;
	m_pImageBmpInfo->bmiHeader.biClrImportant	= 0;

	m_pImageBmpInfo->bmiHeader.biWidth			= 0;
	m_pImageBmpInfo->bmiHeader.biHeight			= 0;
	m_pImageBmpInfo->bmiHeader.biBitCount		= 0;
	m_pImageBmpInfo->bmiHeader.biSizeImage      = 0;  // m_nWidth * m_nHeight * ( m_nBitCount >> 3 );

	for ( int i=0; i<256; i++ )
	{
		m_pImageBmpInfo->bmiColors[i].rgbBlue	   = (BYTE) i;
		m_pImageBmpInfo->bmiColors[i].rgbGreen	   = (BYTE) i;
		m_pImageBmpInfo->bmiColors[i].rgbRed	   = (BYTE) i;
		m_pImageBmpInfo->bmiColors[i].rgbReserved  = (BYTE) i;
	}

	KSJIZM_Init(&m_hZoom, 0, 0, 0, 0 );

	KSJIZM_SetZoomMode(m_hZoom, ZM_FITIMG);

	memset( m_szOpenFileFullName, 0, MAX_PATH * sizeof( TCHAR ) );
	m_hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
	m_hPeng = CreatePen(PS_SOLID, 1, RGB(44, 173, 66));
	m_hPenr = CreatePen(PS_SOLID, 1, RGB(255, 0, 0));
	m_hbrush = CreateSolidBrush(RGB(44, 173, 66));
	m_hbrushr = CreateSolidBrush(RGB(255, 0, 0));
	m_fDblImage.x = -1;
	m_fDblImage.z = -1;
}

void CKSJProfileWnd::ConnectCamera(int nIndex)
{
	m_nIndex = nIndex;
}


CKSJProfileWnd::~CKSJProfileWnd()
{
	if (m_pImageBmpData != NULL)
	{
		delete[] m_pImageBmpData;
		m_pImageBmpData = NULL;
	}

	if( m_pImageBmpInfo )
	{
		delete [] m_pImageBmpInfo;
		m_pImageBmpInfo = NULL;
	}

	if (m_hZoom)
	{
		KSJIZM_UnInit(&m_hZoom);
		m_hZoom = NULL;
	}

	if (m_pfX != NULL)
	{
		delete[] m_pfX;
		m_pfX = NULL;
	}

	if (m_pfZ != NULL)
	{
		delete[] m_pfZ;
		m_pfZ = NULL;
	}

	if (m_pfImage != NULL)
	{
		delete[] m_pfImage;
		m_pfImage = NULL;
	}

	::DeleteObject((HGDIOBJ)m_hPen);
	::DeleteObject((HGDIOBJ)m_hPeng);
	::DeleteObject((HGDIOBJ)m_hPenr);
	::DeleteObject((HGDIOBJ)m_hbrush);
	::DeleteObject((HGDIOBJ)m_hbrushr);
	DeleteCriticalSection(&m_csImageBmpData);
}


BEGIN_MESSAGE_MAP(CKSJProfileWnd, CWnd)
	//{{AFX_MSG_MAP(CKSJProfileWnd)
	//}}AFX_MSG_MAP
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_WM_CREATE()
	ON_WM_CONTEXTMENU()
	ON_WM_ERASEBKGND()
	ON_WM_HSCROLL()
	ON_WM_VSCROLL()
	ON_WM_SIZE()
	ON_WM_RBUTTONDOWN()
END_MESSAGE_MAP()

int CKSJProfileWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

#ifndef SHOW_HVSCROLL
	EnableScrollBarCtrl(SB_HORZ, FALSE);
	EnableScrollBarCtrl(SB_VERT, FALSE);
#else
	EnableScrollBarCtrl(SB_HORZ, TRUE);
	EnableScrollBarCtrl(SB_VERT, TRUE);
#endif

	return 0;
}

void CKSJProfileWnd::SetSelected(BOOL bSelected)
{
	m_bSelected = bSelected;

	SetFocus();

	Invalidate();
}

void CKSJProfileWnd::SetScale(int x, int z)
{
	m_nxScale = x;
	m_nzScale = z;
}

void CKSJProfileWnd::SetPointNum(int nColStart,int nPointNum)
{
	if (nColStart == m_nColStart && nPointNum == m_nPointNum) return;
	if (m_pfX != NULL)
	{
		delete[] m_pfX;
		m_pfX = NULL;
	}

	if (m_pfZ != NULL)
	{
		delete[] m_pfZ;
		m_pfZ = NULL;
	}

	if (m_pfImage != NULL)
	{
		delete[] m_pfImage;
		m_pfImage = NULL;
	}
	m_pfX = new float[nPointNum];
	m_pfZ = new float[nPointNum];
	m_nPointNum = nPointNum;
	m_nColStart = nColStart;
	m_pfImage = new fPoint[nPointNum];
	m_pcount = 0;
}

void CKSJProfileWnd::SetProfile(float* pfX, float* pfZ, float fXMax, float fZMin, float fZMax)
{

	if (TryEnterCriticalSection(&m_csImageBmpData)) // 正在使用这个图更新界面的时候，就不处理这个更新
	{
		memcpy(m_pfX, pfX, sizeof(float)*m_nPointNum);
		memcpy(m_pfZ, pfZ, sizeof(float)*m_nPointNum);
		m_fXMax = fXMax;
		m_fZMin = fZMin;
		m_fZMax = fZMax;
		m_dZoomFactor = 1.0;
		m_dShowZoomH = 1.0;
		m_dShowZoomV = 1.0;
		m_ptOffset.x = m_ptOffset.y = 0;

		LeaveCriticalSection(&m_csImageBmpData);

		Invalidate();
	}
}

// 当显示的图像大小发生改变时，必须调用
int CKSJProfileWnd::SetImageInfo(int nWidth, int nHeight, int nBitCount)
{
	if (m_nIndex != -1)
	{
		if (m_pImageBmpData != NULL)
		{
			delete[] m_pImageBmpData;
			m_pImageBmpData = NULL;
		}

		KSJIZM_SetImageInfo(m_hZoom, nWidth, nHeight);

		return RET_SUCCESS;
	}


	if (m_pImageBmpData != NULL &&
		m_pImageBmpInfo->bmiHeader.biWidth == nWidth &&
		m_pImageBmpInfo->bmiHeader.biHeight == nHeight &&
		m_pImageBmpInfo->bmiHeader.biBitCount == nBitCount)
		return RET_SUCCESS;

	m_pImageBmpInfo->bmiHeader.biWidth     = nWidth;
	m_pImageBmpInfo->bmiHeader.biHeight    = nHeight;
	m_pImageBmpInfo->bmiHeader.biBitCount  = nBitCount;
	m_pImageBmpInfo->bmiHeader.biSizeImage = nWidth * nHeight * (nBitCount >> 3);

	if (m_pImageBmpData != NULL)
	{
		delete[] m_pImageBmpData;
		m_pImageBmpData = NULL;
	}

	m_pImageBmpData = new BYTE[m_pImageBmpInfo->bmiHeader.biSizeImage];
	memset(m_pImageBmpData, 0, m_pImageBmpInfo->bmiHeader.biSizeImage);
	KSJIZM_SetImageInfo(m_hZoom, m_pImageBmpInfo->bmiHeader.biWidth, m_pImageBmpInfo->bmiHeader.biHeight);

	TCHAR szBuf[256] = { 0 };
	_stprintf_s(szBuf, 256, _T("CKSJProfileWnd::SetImageInfo::KSJIZM_SetImageInfo( %d,%d )\n"), m_pImageBmpInfo->bmiHeader.biWidth, m_pImageBmpInfo->bmiHeader.biHeight);
	return RET_SUCCESS;
}


/////////////////////////////////////////////////////////////////////////////
// CKSJProfileWnd message handlers
BOOL CKSJProfileWnd::OnEraseBkgnd(CDC* pDC)
{
	return TRUE;
	return CWnd::OnEraseBkgnd(pDC);
}

void CKSJProfileWnd::DrawSelectRect()
{
	COLORREF    crFrameNoraml   = GetSysColor(COLOR_WINDOWFRAME);
	COLORREF    crFrameSelected = GetSysColor(COLOR_BTNHIGHLIGHT);
	int         nFrameX = GetSystemMetrics(SM_CXFRAME);

	HDC   hWndDC = ::GetWindowDC(m_hWnd);
	int   nWndDC = ::SaveDC(hWndDC);

	RECT  rtWnd;
	GetWindowRect(&rtWnd);

	int   nWndW = rtWnd.right - rtWnd.left;
	int   nWndH = rtWnd.bottom - rtWnd.top;

	HPEN  hPen;
	if (m_bSelected)
		hPen = ::CreatePen(PS_SOLID, 1, RGB(0, 255, 0));
	else
		hPen = ::CreatePen(PS_SOLID, 1, crFrameNoraml);       // RGB(44, 173, 66));

	HPEN hPenOld = (HPEN)::SelectObject(hWndDC, hPen);

	::SetBkMode(hWndDC, TRANSPARENT);
	::MoveToEx(hWndDC, 0, 0, NULL);
	::LineTo(hWndDC, nWndW - 1, 0);
	::MoveToEx(hWndDC, 0, 0, NULL);
	::LineTo(hWndDC, 0, nWndH - 1);

	::MoveToEx(hWndDC, nWndW - 1, 0, NULL);
	::LineTo(hWndDC, nWndW - 1, nWndH - 1);
	::MoveToEx(hWndDC, 0, nWndH - 1, NULL);
	::LineTo(hWndDC, nWndW - 1, nWndH - 1);

	::SelectObject(hWndDC, hPenOld);
	::DeleteObject(hPen);

	::RestoreDC(hWndDC, nWndDC);
	::ReleaseDC(m_hWnd, hWndDC);

}

void CKSJProfileWnd::OnPaint()
{
	CPaintDC dc(this); // device context for painting

	if (FALSE == IsWindowVisible())    return;

	DrawSelectRect();

	if (m_hZoom == NULL)    return;

	EnterCriticalSection(&m_csImageBmpData);

	HDC   hDC = dc.GetSafeHdc();

	HDC   hMemClientDC = ::CreateCompatibleDC(hDC);
	int   nMemClientDC = ::SaveDC(hMemClientDC);


	RECT  rtClient;
	::GetClientRect(m_hWnd, &rtClient);

	int nClientW = rtClient.right - rtClient.left;
	int nClientH = rtClient.bottom - rtClient.top;

	HBITMAP hMemClientBmp    = ::CreateCompatibleBitmap(hDC, nClientW, nClientH);
	HBITMAP hMemClientBmpOld = (HBITMAP)::SelectObject(hMemClientDC, hMemClientBmp);

	HBRUSH   hMemClientBrush = ::CreateSolidBrush(RGB(112, 128, 144));
	HBRUSH   hMemClientBrushOld = (HBRUSH)::SelectObject(hMemClientDC, hMemClientBrush);// ::GetStockObject(BLACK_BRUSH));

	::Rectangle(hMemClientDC, 0, 0, nClientW, nClientH);

	KSJIZM_GetPosition(m_hZoom, &m_nShowOffsetH, &m_nShowOffsetV, &m_nShowWidth, &m_nShowHeight);
	TCHAR szBuf[256] = { 0 };
	_stprintf_s(szBuf, 256, _T("CKSJProfileWnd::OnPaint( %d,  %d,%d,%d,%d)\n"), m_nIndex, m_nShowOffsetH, m_nShowOffsetV, m_nShowWidth, m_nShowHeight);

	::SetStretchBltMode(hMemClientDC, COLORONCOLOR);
		
	if (m_pImageBmpData != NULL)
	{
		char szText[32];
		SetBkMode(hMemClientDC,TRANSPARENT);
			
		SetTextColor(hMemClientDC, RGB(255, 255, 255)); //设置颜色

		float xstep, zstep;
		int nWidth,nHeigth,nx,nz,nXoffset,nZoffset;

		int i;
		
		SelectObject(hMemClientDC, m_hPen);
		::StretchDIBits(hMemClientDC, m_nShowOffsetH, m_nShowOffsetV, m_nShowWidth, m_nShowHeight, 0, 0, m_pImageBmpInfo->bmiHeader.biWidth, m_pImageBmpInfo->bmiHeader.biHeight, m_pImageBmpData, m_pImageBmpInfo, DIB_RGB_COLORS, SRCCOPY);
		


		if ((m_nShowOffsetH + m_nShowWidth <= nClientW) && m_nShowOffsetH >= 0)
		{
			nXmin = 0;
			nXmax = m_nShowWidth;
			nWidth = m_nShowWidth;
			nXoffset = m_nShowOffsetH;
		}
		else
		{
			nXmin = -m_nShowOffsetH;
			nXmax = -m_nShowOffsetH + nClientW;
			nWidth = nClientW;
			nXoffset = 0;
		}

		fxmin = (float)nXmin / m_nShowWidth * m_fXMax;
		fxmax = (float)nXmax / m_nShowWidth * m_fXMax;
		xstep = m_nxScale / m_fXMax*m_nShowWidth;
		nx = m_fXMax / m_nxScale;

		if ((m_nShowOffsetV + m_nShowHeight <= nClientH) && m_nShowOffsetV >= 0)
		{
			nZmin = 0;
			nZmax = m_nShowHeight;
			nHeigth = m_nShowHeight;
			nZoffset = m_nShowOffsetV;
		}
		else
		{
			nZmin = m_nShowHeight + m_nShowOffsetV - nClientH;
			nZmax = m_nShowHeight + m_nShowOffsetV;
			nZoffset = 0;
			nHeigth = nClientH;
		}


		float fZSize = m_fZMax - m_fZMin;
		fzmin = (float)nZmin / m_nShowHeight * fZSize + m_fZMin;
		fzmax = (float)nZmax / m_nShowHeight * fZSize + m_fZMin;
		float foffset = 0;
		int n = m_fZMin / m_nzScale;
		int m = (int)m_fZMin%m_nzScale;
		if (m > 1) n++;
		foffset = n * 10 - m_fZMin;
		int noffset = foffset / fZSize * m_nShowHeight;
		zstep = m_nzScale / fZSize*m_nShowHeight;
		nz = fZSize / m_nzScale;
		for (i = 0; i < nx; i++)//x轴坐标绘制
		{
			MoveToEx(hMemClientDC, m_nShowOffsetH + xstep*(i + 1), nZoffset + nHeigth, NULL);
			LineTo(hMemClientDC, m_nShowOffsetH + xstep*(i + 1), nZoffset + nHeigth - m_nShowHeight);
			sprintf(szText, "%d", (i + 1)*m_nxScale);
			TextOut(hMemClientDC, m_nShowOffsetH + xstep*(i + 1), nZoffset + nHeigth - 20, szText, strlen(szText));
		}

		for (i = 0; i < nz; i++)//z轴坐标绘制
		{
			MoveToEx(hMemClientDC, nXoffset, m_nShowOffsetV + m_nShowHeight - zstep*(i + 1) - noffset, NULL);
			LineTo(hMemClientDC, nXoffset + m_nShowWidth, m_nShowOffsetV + m_nShowHeight - zstep*(i + 1) - noffset);
			sprintf(szText, "%.1f", (i + 1) * m_nzScale + m_fZMin + foffset);
			TextOut(hMemClientDC, nXoffset + 10, m_nShowOffsetV + m_nShowHeight - zstep*(i + 1), szText, strlen(szText));
		}

		SelectObject(hMemClientDC, m_hPenr);
		SelectObject(hMemClientDC, m_hbrushr);
		int nValueCurZ = 0;
		int nValueCurX = 0;
		float fxScale, fzScale;
		fxScale = (float)m_nShowWidth / nClientW;
		fzScale = (float)m_nShowHeight / nClientH;
		m_pcount = 0;
		int x, y;
		for (i = 0; i < m_nPointNum; ++i)//绘制线
		{
			if (m_pfZ[i] != -1000)
			{
				if (m_pfZ[i] >= fzmin && m_pfZ[i] <= fzmax && m_pfX[i] >= fxmin && m_pfX[i] <= fxmax)
				{
					nValueCurZ = int((nZmax - nZmin)*(m_pfZ[i] - fzmin) / (fzmax - fzmin) + 0.5f);
					nValueCurX = int((nXmax - nXmin)*(m_pfX[i] - fxmin) / (fxmax - fxmin) + 0.5f);

					if (m_nShowOffsetH >0)x = nValueCurX + m_nShowOffsetH;
					else x = nValueCurX;
					if (m_nShowOffsetV >0)y = nZmax - nZmin - nValueCurZ + m_nShowOffsetV;
					else y = nZmax - nZmin - nValueCurZ;
					int size = 1;
					Ellipse(hMemClientDC, x - size, y - size, x + size, y + size);
					m_pfImage[m_pcount].x = m_pfX[i];
					m_pfImage[m_pcount].z = m_pfZ[i];
					m_pcount++;
				}
			}
		}
		SelectObject(hMemClientDC, m_hPeng);
		SelectObject(hMemClientDC, m_hbrush);

		if (m_fDblImage.x >= 0)
		{
			nValueCurZ = int((nZmax - nZmin)*(m_fDblImage.z - fzmin) / (fzmax - fzmin) + 0.5f);
			nValueCurX = int((nXmax - nXmin)*(m_fDblImage.x - fxmin) / (fxmax - fxmin) + 0.5f);

			if (m_nShowOffsetH > 0)x = nValueCurX + m_nShowOffsetH;
			else x = nValueCurX;
			if (m_nShowOffsetV > 0)y = nZmax - nZmin - nValueCurZ + m_nShowOffsetV;
			else y = nZmax - nZmin - nValueCurZ;
			int size = 3;
			if (fxScale > 1 && fxScale <= 2) size = 5;
			else if (fxScale > 2) size = 7;
			Ellipse(hMemClientDC, x - size, y - size, x + size, y + size);
			MoveToEx(hMemClientDC, x, m_nShowOffsetV, NULL);
			LineTo(hMemClientDC, x, m_nShowOffsetV + m_nShowHeight);
		}
	}


	::SetStretchBltMode(hDC, COLORONCOLOR);
	::BitBlt(hDC, 0, 0, nClientW, nClientH, hMemClientDC, 0, 0, SRCCOPY);
	
	LeaveCriticalSection(&m_csImageBmpData);

	::SelectObject(hMemClientDC, hMemClientBrushOld);
	::DeleteObject(hMemClientBrush);

	::SelectObject(hMemClientDC, hMemClientBmpOld);
	::DeleteObject(hMemClientBmp);

	::RestoreDC(hMemClientDC, nMemClientDC);
	::DeleteDC(hMemClientDC);
}

fPoint CKSJProfileWnd::FindPoint(fPoint p)
{
	fPoint ret;
	ret.x = ret.z = -1;
	vector<fPoint> vec;
	int i,index=-1,sub=65535;
	for (i = 0; i < m_pcount;i++)
	{
		if (abs(p.x - m_pfImage[i].x) < 0.1) vec.push_back(m_pfImage[i]);
	}

	for (i = 0; i < vec.size();i++)
	{
		if (sub > abs(vec[i].z - p.z))
		{
			sub = abs(vec[i].z - p.z);
			index = i;
		}
	}

	if (index != -1) return vec[index];
	else return ret;
}

void CKSJProfileWnd::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	if (m_hZoom == NULL)    return;
	if (m_pImageBmpData == NULL) return;
	POINT    ptScreen = point;
	ClientToScreen(&ptScreen);
	POINT    ptParentClient = ptScreen;
	((CKSJShow3DDlg*)GetParent())->ScreenToClient(&ptParentClient);
	SetFocus();
	POINT    ptImage;
	if (KSJIZM_RET_SUCCESS != KSJIZM_MapPointClientToImage(m_hZoom, point.x, point.y, (int*)&ptImage.x, (int*)&ptImage.y))
		return;

	float data[2];
	EnterCriticalSection(&m_csImageBmpData);
	fPoint convert;
	int nValueCurX, nValueCurZ;

	if (m_nShowOffsetH > 0) nValueCurX = point.x - m_nShowOffsetH;
	else nValueCurX = point.x;
	convert.x = nValueCurX*(fxmax - fxmin) / (nXmax - nXmin) + fxmin;

	if (m_nShowOffsetV > 0)nValueCurZ = nZmax - nZmin - point.y + m_nShowOffsetV;
	else nValueCurZ = nZmax - nZmin - point.y;
	convert.z = nValueCurZ * (fzmax - fzmin) / (nZmax - nZmin) + fzmin;

	fPoint ret = FindPoint(convert);
	if (ret.x > 0 && ret.z > 0)
	{
		data[0] = (float)ret.x;
		data[1] = (float)ret.z;

		GetParent()->SendMessage(WM_PROFILE, nFlags, (LPARAM)data);
		
		m_fDblImage = ret;
		Invalidate();
		UpdateWindow();
	}

	LeaveCriticalSection(&m_csImageBmpData);
	CWnd::OnLButtonDblClk(nFlags, point);
}

void CKSJProfileWnd::OnLButtonDown(UINT nFlags, CPoint point)
{
	if (m_hZoom == NULL)    return;

	// 如果此窗口还没有被选中，则这次鼠标点击只是用于选中图像，并把另一个选中的窗口设置为未选中状态
	if ( !m_bSelected )
	{
		POINT    ptScreen = point;
		ClientToScreen(&ptScreen);

		POINT    ptParentClient = ptScreen;
		((CKSJShow3DDlg*)GetParent())->ScreenToClient(&ptParentClient);
		
		
		// 转换为相对父窗口的客户区坐标
		GetParent()->SendMessage(WM_LBUTTONDOWN, nFlags, ptParentClient.x | (ptParentClient.y << 16));

		return;
	}

	SetFocus();
	// 判断鼠标点击点是否在图像内，如不在则不处理
	POINT    ptImage;

	if (KSJIZM_RET_SUCCESS != KSJIZM_MapPointClientToImage(m_hZoom, point.x, point.y, (int*)&ptImage.x, (int*)&ptImage.y))
		return;

	m_ptLButtonDownInClientPre = point;
	m_ptLButtonDownInImage     = ptImage;
	CWnd::OnLButtonDown(nFlags, point);
}

void CKSJProfileWnd::OnRButtonDown(UINT nFlags, CPoint point)
{
	if (m_hZoom == NULL) return;
	KSJIZM_SetZoomMode(m_hZoom, ZM_FITIMG);
	Invalidate();
	UpdateWindow();
	CWnd::OnRButtonDown(nFlags, point);
}


// point相对窗口左上角的相对位置
void CKSJProfileWnd::OnMouseMove(UINT nFlags, CPoint point)
{
	if (!m_bSelected)    return;
	if (m_hZoom == NULL) return;

	// 判断鼠标点击点是否在图像内，如不在则不处理
	POINT    ptImage;
	if (KSJIZM_RET_SUCCESS != KSJIZM_MapPointClientToImage(m_hZoom, point.x, point.y, (int*)&ptImage.x, (int*)&ptImage.y))
		return;

	// 如果鼠标移动过程中鼠标的左键被按下，当显示的大小大于窗口的大小时，需要改变鼠标形状，并支持移动
	if( nFlags & MK_LBUTTON )
	{
		SetCursor(::LoadCursor(NULL, IDC_HAND));    // MAKEINTRESOURCE()

		m_ptOffset = (point - m_ptLButtonDownInClientPre);
		m_ptLButtonDownInClientPre = point;

		KSJIZM_Offset(m_hZoom, m_ptOffset.x, m_ptOffset.y);

		if (m_nIndex != -1)
		{
			KSJIZM_GetPosition(m_hZoom, &m_nShowOffsetH, &m_nShowOffsetV, &m_nShowWidth, &m_nShowHeight);
			//KSJ_PreviewSetPos(m_nIndex, m_hWnd, m_nShowOffsetH, m_nShowOffsetV, m_nShowWidth, m_nShowHeight);
		}
		else
		{
			Invalidate();
			UpdateWindow();
		}

		return;
	}

	CWnd::OnMouseMove(nFlags, point);
}


void CKSJProfileWnd::ZoomIn( )
{
	if (m_hZoom == NULL)    return;
	if (!m_bSelected) return;
	KSJIZM_SetZoomMode(m_hZoom, ZM_ZOOM);
	POINT ptClient;
    ::GetCursorPos(&ptClient);
	::ScreenToClient(m_hWnd, &ptClient);

	TCHAR szBuf[256] = { 0 };
	_stprintf_s(szBuf, 256, _T("CKSJProfileWnd::ZoomIn::ClientPos( %d,%d )"), ptClient.x, ptClient.y);
	OutputDebugString(szBuf);


	int nImageX, nImageY;
	if (KSJIZM_RET_SUCCESS != KSJIZM_MapPointClientToImage(m_hZoom, ptClient.x, ptClient.y, &nImageX, &nImageY))
		return;

	KSJIZM_ZoomIn(m_hZoom, nImageX, nImageY);
	Invalidate();
	UpdateWindow();
}


void  CKSJProfileWnd::ZoomOut()
{
	if (m_hZoom == NULL)    return;
	if (!m_bSelected) return;
	KSJIZM_SetZoomMode(m_hZoom, ZM_ZOOM);
	POINT ptClient;
	::GetCursorPos(&ptClient);
	::ScreenToClient(m_hWnd, &ptClient);

	TCHAR szBuf[256] = { 0 };
	_stprintf_s(szBuf, 256, _T("CKSJProfileWnd::ZoomOut::ClientPos( %d,%d )\n"), ptClient.x, ptClient.y);
	OutputDebugString(szBuf);


	int nImageX, nImageY;
	if (KSJIZM_RET_SUCCESS != KSJIZM_MapPointClientToImage(m_hZoom, ptClient.x, ptClient.y, &nImageX, &nImageY) )
		return;

	KSJIZM_ZoomOut(m_hZoom, nImageX, nImageY);
    Invalidate();
	UpdateWindow();
}


// 注意中文输入法
BOOL CKSJProfileWnd::MyPreTranslateMessage(MSG* pMsg)
{
	if (m_hZoom == NULL)    return FALSE;

	if( pMsg->message == WM_KEYDOWN) 
	{ 
		POINT ptClient;
		::GetCursorPos(&ptClient);
		::ScreenToClient(m_hWnd, &ptClient);
		int nImageX, nImageY;
		switch (pMsg->wParam)
		{ 
		case VK_OEM_PLUS:
		case VK_ADD:  
			KSJIZM_SetZoomMode(m_hZoom, ZM_ZOOM);
			KSJIZM_MapPointClientToImage(m_hZoom, ptClient.x, ptClient.y, &nImageX, &nImageY);
			KSJIZM_ZoomIn(m_hZoom, nImageX, nImageY);
			break;   // '+'

		case VK_OEM_MINUS:  
		case VK_SUBTRACT:   
			KSJIZM_SetZoomMode(m_hZoom, ZM_ZOOM);
			KSJIZM_MapPointClientToImage(m_hZoom, ptClient.x, ptClient.y, &nImageX, &nImageY);
			KSJIZM_ZoomOut(m_hZoom, nImageX, nImageY);
			break;
		case VK_MULTIPLY:   
			KSJIZM_SetZoomMode(m_hZoom, ZM_FITIMG);
			break;
		case VK_OEM_2:
		case VK_DIVIDE:      
			KSJIZM_SetZoomMode(m_hZoom, ZM_ACTUALSIZE);
			break;
		case 0x38: 
			if( GetKeyState(VK_SHIFT) & 0x8000  )
			{
				if (m_hZoom)
					KSJIZM_SetZoomMode(m_hZoom, ZM_FITIMG);
			}
			break; 

		default: 
			break;
		} 
	}

	Invalidate();
	UpdateWindow();

	return TRUE;
}

void CKSJProfileWnd::OnContextMenu(CWnd* pWnd, CPoint point)
{
#if 0
	CMenu menu;   
	VERIFY(menu.LoadMenu(IDR_MENU_IMAGE));   
	CMenu* pPopup = menu.GetSubMenu(0);   
	ASSERT(pPopup!=NULL);

	// menu.EnableMenuItem( ID_CONVERT_8BBP,   ( m_pImageBmpInfo->bmiHeader.biBitCount == 24     ? MF_ENABLED : MF_DISABLED)  | MF_BYCOMMAND );

	menu.CheckMenuItem( ID_ZOOM_FITIMAGE,   ( m_ZoomMode == ZM_FITIMG     ? MF_CHECKED : MF_UNCHECKED) | MF_BYCOMMAND );
	menu.CheckMenuItem( ID_ZOOM_FITWINDOW,  ( m_ZoomMode == ZM_FITWND     ? MF_CHECKED : MF_UNCHECKED) | MF_BYCOMMAND );
	menu.CheckMenuItem( ID_ZOOM_ACTUALSIZE, ( m_ZoomMode == ZM_ACTUALSIZE ? MF_CHECKED : MF_UNCHECKED) | MF_BYCOMMAND );
	menu.CheckMenuItem( ID_ZOOM_FITIMAGE,   ( m_ZoomMode == ZM_FITIMG     ? MF_CHECKED : MF_UNCHECKED) | MF_BYCOMMAND );
	menu.CheckMenuItem( ID_ZOOM_ZOOMIN,     ( m_ZoomMode == ZM_IN         ? MF_CHECKED : MF_UNCHECKED) | MF_BYCOMMAND );
	menu.CheckMenuItem( ID_ZOOM_ZOOMOUT,    ( m_ZoomMode == ZM_OUT        ? MF_CHECKED : MF_UNCHECKED) | MF_BYCOMMAND );


	menu.CheckMenuItem(ID_MOUSEMODE_NORMAL, (m_KMM == KMM_NORMAL ? MF_CHECKED : MF_UNCHECKED) | MF_BYCOMMAND);
	menu.CheckMenuItem(ID_MOUSEMODE_DRAW,   (m_KMM == KMM_DRAW ? MF_CHECKED : MF_UNCHECKED) | MF_BYCOMMAND);

	int   nCmd   =  pPopup->TrackPopupMenu( TPM_LEFTALIGN|TPM_LEFTBUTTON|TPM_RETURNCMD,point.x,point.y, this); 
	switch( nCmd )
	{
	case ID_ZOOM_FITIMAGE:
		if (m_hZoom)
			KSJIZM_SetZoomMode(m_hZoom, ZM_FITIMG);
		Invalidate();
		UpdateWindow();
		break;
	case ID_ZOOM_FITWINDOW:
		if (m_hZoom)
			KSJIZM_SetZoomMode(m_hZoom, ZM_FITWND);
			Invalidate();
		UpdateWindow();
		break;
	case ID_ZOOM_ACTUALSIZE:
		if (m_hZoom)
			KSJIZM_SetZoomMode(m_hZoom, ZM_ACTUALSIZE);
			Invalidate();
		UpdateWindow();
		break;
	case ID_ZOOM_ZOOMIN:
		if (m_hZoom)
			KSJIZM_SetZoomMode(m_hZoom, ZM_ZOOM);
			Invalidate();
		UpdateWindow();
		break;
	case ID_ZOOM_ZOOMOUT:
		if (m_hZoom)
			KSJIZM_SetZoomMode(m_hZoom, ZM_ZOOM);
		Invalidate();
		UpdateWindow();
		break;
	}
#endif
	return;
}




BOOL CKSJProfileWnd::PreTranslateMessage(MSG* pMsg)
{
	return CWnd::PreTranslateMessage(pMsg);
}

void CKSJProfileWnd::PreSubclassWindow()
{
	CWnd::PreSubclassWindow();
}

BOOL CKSJProfileWnd::PreCreateWindow(CREATESTRUCT& cs)
{
	// cs.lpszClass = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW, 0, (HBRUSH)::GetStockObject(BLACK_BRUSH), 0);
	return CWnd::PreCreateWindow(cs);
}

void CKSJProfileWnd::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);

	if (m_hZoom == NULL)    return;

	KSJIZM_SetClientInfo(m_hZoom, cx, cy);

	TCHAR szBuf[256] = { 0 };
	_stprintf_s(szBuf, 256, _T("CKSJProfileWnd::OnSize::KSJIZM_SetClientInfo( %d,%d )\n"), cx, cy);
	OutputDebugString(szBuf);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//
//    图像缩放及位置相关函数
//
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CKSJProfileWnd::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// if (m_bZoomVideo)    return;

	CRect    rtClient;
	GetClientRect(&rtClient);

	switch (nSBCode)
	{
	case SB_LEFT:				// Scroll to far left.
		SetScrollPos(SB_HORZ, GetScrollPos(SB_HORZ) - 10);
		break;
	case SB_LINELEFT:			// Scroll left.
		SetScrollPos(SB_HORZ, GetScrollPos(SB_HORZ) - 2);
		break;
	case SB_PAGELEFT:			// Scroll one page left.
		SetScrollPos(SB_HORZ, GetScrollPos(SB_HORZ) - rtClient.Width());		// 左侧空白区
		break;
	case SB_ENDSCROLL:			// End scroll.
		SetScrollPos(SB_HORZ, GetScrollPos(SB_HORZ) + 1);
		break;
	case SB_RIGHT:				// Scroll to far right.
		SetScrollPos(SB_HORZ, GetScrollPos(SB_HORZ) + 10);
		break;
	case SB_LINERIGHT:			// Scroll right.
		SetScrollPos(SB_HORZ, GetScrollPos(SB_HORZ) + 2);
		break;
	case SB_PAGERIGHT:			// Scroll one page right.
		SetScrollPos(SB_HORZ, GetScrollPos(SB_HORZ) + rtClient.Width());
		break;
	case SB_THUMBPOSITION:		// Scroll to absolute position. The current position is specified by the nPos parameter.
		SetScrollPos(SB_HORZ, nPos);
		break;
	case SB_THUMBTRACK:
		SetScrollPos(SB_HORZ, nPos);
		break;
	}

	// MoveWinAccordToScrollPos();

	CWnd::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CKSJProfileWnd::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	// if (m_bZoomVideo)    return;

	CRect    rtClient;
	GetClientRect(&rtClient);

	switch (nSBCode)
	{
	case SB_TOP:			//  Scroll to top. 
		SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) - 10);
		break;
	case SB_BOTTOM:		    // Scroll to bottom.
		SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) + 10);
		break;
	case SB_ENDSCROLL:	    // End scroll.
		break;
	case SB_LINEUP:		    // Scroll one line up.
		SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) - 2);
		break;
	case SB_LINEDOWN:	    // Scroll one line down.
		SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) + 2);
		break;
	case SB_PAGEUP:		    // Scroll one page up.
		SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) - rtClient.Height());		// 左侧空白区
		break;
	case SB_PAGEDOWN:	    // Scroll one page down.
		SetScrollPos(SB_VERT, GetScrollPos(SB_VERT) + rtClient.Height());		// 左侧空白区
		break;
	case SB_THUMBPOSITION:	// Scroll to the absolute position. The current position is provided in nPos.
		SetScrollPos(SB_VERT, nPos);
		break;
	case SB_THUMBTRACK:		// Drag scroll box to specified position. The current position is provided in nPos.
		SetScrollPos(SB_VERT, nPos);
		break;
	}

	// MoveWinAccordToScrollPos();
	CWnd::OnVScroll(nSBCode, nPos, pScrollBar);
}




