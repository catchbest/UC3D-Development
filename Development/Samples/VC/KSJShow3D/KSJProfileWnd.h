#if !defined(AFX_KSJPROFILEWND_H__DD2F42A5_819F_4D99_A3CD_144154F3684A__INCLUDED_)
#define AFX_KSJPROFILEWND_H__DD2F42A5_819F_4D99_A3CD_144154F3684A__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// KSJPROFILEWND.h : header file
//
#define WM_PROFILE WM_USER + 103

/////////////////////////////////////////////////////////////////////////////
// CKSJProfileWnd window
// ��Ӧ��귽ʽ
// ��ΪKMM_NORMALʱ�����������²��ƶ����ʱ���ͼ�����λ�õ��϶�
// ��ΪKMM_DRAWʱ��
// �����ͼ����Ϊָ��ͼ�Σ���ʼ�µ�ͼ�λ��Ƽ���ӱ༭�㡣
// ���û��ָ��ͼ�Σ�����갴��ʱ���ͼ�ν���HIT���
// ��갴�²��ƶ�ʱ���HIT��ͼ�ν����ƶ�
// ����ƶ�ʱ����Բ���ͼ�ν���PASS��⣬
// ��갴��û�м�⵽HITʱ���Ὺʼһ����ͼ�εĻ��ơ�
// �����ڻ���ʱ��������ƶ�ʱ��
struct fPoint
{
	float x;
	float z;
};

class CKSJProfileWnd : public CWnd
{
protected:
	DECLARE_DYNCREATE(CKSJProfileWnd)

// Construction
public:
	CKSJProfileWnd( CWnd *pParent = NULL, int nIndex = 0 );
	virtual ~CKSJProfileWnd();
private:
	CWnd    *m_pParent;

public:
	void ConnectCamera(int nIndex);

private:
	int           m_nIndex;    // ��m_nIndex��Ϊ-1ʱ����ʾ�˴�����Ϊ�����Ԥ������ʹ�ã�Ҳ����KSJ_PreviewStart����ʾ�ڴ˴�����
	                           // ��=-1ʱ����ʾ��ͨ��LoadImage����ʾһ��ͼƬ�Ĵ��ڡ�
	bool		  m_bProfile;
	float		  *m_pfX;
	float		  *m_pfZ;
	fPoint		  *m_pfImage;
	int			  m_pcount;
	int			  m_nxScale, m_nzScale, m_nPointNum, m_nColStart;
	float		  m_fZMin, m_fZMax, m_fXMin, m_fXMax;
	HPEN		  m_hPen, m_hPeng, m_hPenr;
	int nXmin, nXmax, nZmin, nZmax;
	float fxmin, fxmax, fzmin, fzmax;
	HBRUSH		  m_hbrush, m_hbrushr;
public:
	void		  SetScale(int x, int z);
	void		  SetPointNum(int nColStart, int nPointNum);
	void          SetProfile(float* pfX, float* pfZ, float fXMax, float fZMin, float fZMax);
	int           SetImageInfo(int nCaptureWidth, int nCaptureHeight, int nBitCount);
	BOOL          MyPreTranslateMessage(MSG* pMsg);
	fPoint	      FindPoint(fPoint p);
private:
	BOOL          m_bOpenFromFile;                                                      // ����ǵ����򿪵�ͼ���ļ�
	TCHAR         m_szOpenFileFullName[MAX_PATH];
	fPoint		  m_fDblImage;
private:
	BYTE          *m_pImageBmpData;
	BITMAPINFO    *m_pImageBmpInfo;

	CRITICAL_SECTION m_csImageBmpData;    // !!�����ڶ�ȡ�в����Խ��еĲ�������Ҫ��m_csCatchOneFrame�ٽ���!!

public:
	void          SetSelected( BOOL bSelect );
	bool          GetSelected() { return m_bSelected; };
private:
	BOOL          m_bSelected;


private:
	HANDLE          m_hZoom;


public:
	void            ZoomIn();
	void            ZoomOut();

private:
	double			m_dZoomFactor;
	double			m_dShowZoomH;					// ͼ������سߴ���ͼ����ʾ�ĳߴ��ˮƽ���ű��� = m_nShowWidth  / m_nPreviewWidth;
	double			m_dShowZoomV;					// ͼ������سߴ���ͼ����ʾ�ĳߴ�Ĵ�ֱ���ű��� = m_nShowHeight / m_nPreviewHeight;

	// Ԥ��ͼ����ʾ�ڴ��ڵ�λ��
	int				m_nShowOffsetH;     // Ԥ��ͼ�����Ͻ��൱�ڿͻ������Ͻǵ�ˮƽƫ�ƣ�
	int				m_nShowOffsetV;     // Ԥ��ͼ�����Ͻ��൱�ڿͻ������ϽǵĴ�ֱƫ�ƣ�
	int				m_nShowWidth;       // ��ͼ����ʾ�Ŀ�ȣ�ע�ⲻ��ͼ��Ŀ�ȣ�������ʾ�Ŀ�ȣ�
	int				m_nShowHeight;	    // ��ͼ����ʾ�ĸ߶ȣ�ע�ⲻ��ͼ��ĸ߶ȣ�������ʾ�ĸ߶ȣ�

	// ���������
	CPoint			m_ptOffset;		    // ��������ˮƽ����ֱλ�ã� ���ù�����������갴�º��ƶ�ͼ�� ��
	// int             m_nHScrollPos;   // �Ժ���������ֵ�滻m_ptOffset,�����һЩ
	// int             m_nVScrollPos;


	CPoint          m_ptLButtonDownInClientPre;
	CPoint          m_ptLButtonDownInImage;


// Attributes
public:
// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CKSJProfileWnd)
	//}}AFX_VIRTUAL

	// Generated message map functions
protected:
	//{{AFX_MSG(CKSJProfileWnd)
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

public:
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDblClk(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg int  OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnContextMenu(CWnd* /*pWnd*/, CPoint /*point*/);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg void OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);


public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual void PreSubclassWindow();
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);

private:
	void DrawSelectRect();
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnRButtonDown(UINT nFlags, CPoint point);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_KSJPROFILEWND_H__DD2F42A5_819F_4D99_A3CD_144154F3684A__INCLUDED_)
