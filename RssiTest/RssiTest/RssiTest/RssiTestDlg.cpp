
// RssiTestDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "RssiTest.h"
#include "RssiTestDlg.h"
#include "rssi.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CRssiTestDlg ��ȭ ����




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


// CRssiTestDlg �޽��� ó����

BOOL CRssiTestDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�. ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.

	// TODO: ���⿡ �߰� �ʱ�ȭ �۾��� �߰��մϴ�.

	InitPos();
	InitChart();
	ChartDraw();

	m_SizeFlg = true;
	OnStnDblclickGraph();

	m_nOptimalAP = 0;
		
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�. ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CRssiTestDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CRssiTestDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}
void CRssiTestDlg::InitPos()
{
	int cx = ::GetSystemMetrics(SM_CXMAXIMIZED);		// ������ ���� ����/2
	int cy = ::GetSystemMetrics(SM_CYMAXIMIZED);		// ������ ���� ����/2
	int mx = 800;										// ���̾�α� Xũ��
	int my = 670;										// ���̾�α� Yũ��
	int posx = (cx-mx)/2;								// ���̾�α� X������
	int posy = (cy-my)/2;								// ���̾�α� Y������

	MoveWindow(posx, posy, mx, my, NULL);				// ���̾�α� ũ�� ����

	m_Static_Graph.SetWindowPos(NULL, 10, 10, mx-25, my-50,  SWP_SHOWWINDOW  );
}

void CRssiTestDlg::InitChart()
{
	CRect rect;											// ������Ʈ���� �������� 
	GetDlgItem(IDC_GRAPH)->GetWindowRect(rect) ;	
	ScreenToClient(rect) ;

	// create the control
	m_Graph.Create(WS_VISIBLE | WS_CHILD, rect, this) ;	// �׷��� ��µ� ���� ���� 
}

void CRssiTestDlg::ChartDraw()
{
	m_Graph.InitGraph();								// �ʱ�ȭ 
	m_Graph.BackgroundColor(RGB(255, 255, 255));				// �׷��� ��� ��������
	m_Graph.GridColor(RGB(200, 200, 200));				// �׷��� �׸��� ��������

	m_Graph.InitData(1, "RSSI", 0, -100, RGB(255, 0, 0), true);		// ����1 ����, �ִ�, �ּ�, ����,ǥ��		

	m_Graph.m_SettingFlg = true;											// ��� ���� �Ϸ�
	m_Graph.DrawRect();
}


void CRssiTestDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰� ��/�Ǵ� �⺻���� ȣ���մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
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
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	
	createRssi();
	SetTimer(1, 100, NULL);
	SetTimer(2, 5000, NULL);
}

void CRssiTestDlg::OnBnClickedCapture()
{
	// TODO: ���⿡ ��Ʈ�� �˸� ó���� �ڵ带 �߰��մϴ�.
	destroyRssi();
	m_Graph.CaptureGraph(IDC_GRAPH, "Capture.bmp");
}
