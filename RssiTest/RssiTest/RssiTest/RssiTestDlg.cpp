
// RssiTestDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "RssiTest.h"
#include "RssiTestDlg.h"
#include "rssi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRssiTestDlg 대화 상자




CRssiTestDlg::CRssiTestDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CRssiTestDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CRssiTestDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_GRAPH, m_Static_Graph);
	DDX_Control(pDX, IDC_EDIT1, SSID);
	DDX_Control(pDX, IDC_EDIT2, DBM);	
	DDX_Control(pDX, IDC_LIST1, m_aplist);
}

BEGIN_MESSAGE_MAP(CRssiTestDlg, CDialog)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_WM_TIMER()
	ON_STN_DBLCLK(IDC_GRAPH, &CRssiTestDlg::OnStnDblclickGraph)	
	ON_BN_CLICKED(IDSTART, &CRssiTestDlg::OnBnClickedStart)
	ON_BN_CLICKED(IDCAPTURE, &CRssiTestDlg::OnBnClickedCapture)
END_MESSAGE_MAP()


// CRssiTestDlg 메시지 처리기

BOOL CRssiTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다. 응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	// TODO: 여기에 추가 초기화 작업을 추가합니다.

	InitPos();
	InitChart();
	ChartDraw();

	m_SizeFlg = true;
	OnStnDblclickGraph();

	m_nOptimalAP = 0;
		
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다. 문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CRssiTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CRssiTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CRssiTestDlg::InitPos()
{
	int cx = ::GetSystemMetrics(SM_CXMAXIMIZED);		// 윈도우 가로 길이/2
	int cy = ::GetSystemMetrics(SM_CYMAXIMIZED);		// 윈도우 세로 길이/2
	int mx = 800;										// 다이얼로그 X크기
	int my = 670;										// 다이얼로그 Y크기
	int posx = (cx-mx)/2;								// 다이얼로그 X시작점
	int posy = (cy-my)/2;								// 다이얼로그 Y시작점

	MoveWindow(posx, posy, mx, my, NULL);				// 다이얼로그 크기 조정

	m_Static_Graph.SetWindowPos(NULL, 10, 10, mx-25, my-50,  SWP_SHOWWINDOW  );
}

void CRssiTestDlg::InitChart()
{
	CRect rect;											// 픽쳐컨트롤을 영역구함 
	GetDlgItem(IDC_GRAPH)->GetWindowRect(rect) ;	
	ScreenToClient(rect) ;

	// create the control
	m_Graph.Create(WS_VISIBLE | WS_CHILD, rect, this) ;	// 그래프 출력될 영역 생성 
}

void CRssiTestDlg::ChartDraw()
{
	m_Graph.InitGraph();								// 초기화 
	m_Graph.BackgroundColor(RGB(255, 255, 255));				// 그래프 배경 색상지정
	m_Graph.GridColor(RGB(200, 200, 200));				// 그래프 그리드 색상지정

	m_Graph.InitData(1, "RSSI", 0, -100, RGB(255, 0, 0), true);		// 라인1 범례, 최대, 최소, 색상,표기		

	m_Graph.m_SettingFlg = true;											// 모든 세팅 완료
	m_Graph.DrawRect();
}


void CRssiTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if(nIDEvent == 1)
	{
		apList aps = getRssi();
		if(aps.bSuccess)
		{
			CEdit* pSSID = (CEdit*)GetDlgItem(IDC_EDIT1);
			CEdit* pDBM = (CEdit*)GetDlgItem(IDC_EDIT2);			

			pSSID->SetWindowText(aps.aps[m_nOptimalAP].ssid);

			CString szTmp;
			char tmp[64];
			sprintf(tmp, "%d", aps.aps[m_nOptimalAP].rssi);
			szTmp.SetString(tmp);		
			pDBM->SetWindowText(szTmp);

			m_aplist.ResetContent();
			char tmp2[1024];
			for(int i = 0; i < aps.nList; i++)
			{
				sprintf(tmp2, "%s      %d(dBm)", aps.aps[i].ssid, aps.aps[i].rssi);
				m_aplist.AddString(tmp2);
			}
			
			m_Graph.LineAdd(1, aps.aps[m_nOptimalAP].rssi);								
		}
		m_Graph.DrawRect();		
	}

	if(nIDEvent == 2)
	{
		m_nOptimalAP = getOptimalAP();		
	}

	CDialog::OnTimer(nIDEvent);
}

void CRssiTestDlg::OnStnDblclickGraph()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	m_Graph.DestroyWindow();
	if(!m_SizeFlg)
	{
		m_Static_Graph.SetWindowPos(NULL, 10, 10, 800-25, 600-50,  SWP_SHOWWINDOW  );
		m_SizeFlg = true;
	}
	else
	{
		m_Static_Graph.SetWindowPos(NULL, 10, 10, 800-25, 300,  SWP_SHOWWINDOW  );		
		m_SizeFlg = false;
	}
	InitChart();
	m_Graph.AllClear();
}

void CRssiTestDlg::OnBnClickedStart()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	
	createRssi();
	SetTimer(1, 100, NULL);
	SetTimer(2, 5000, NULL);
}

void CRssiTestDlg::OnBnClickedCapture()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
	destroyRssi();
	m_Graph.CaptureGraph(IDC_GRAPH, "Capture.bmp");
}
