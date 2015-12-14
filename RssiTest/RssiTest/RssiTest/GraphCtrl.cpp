// GraphCtrl.cpp : ���� �����Դϴ�.
//

#include "stdafx.h"
#include "GraphCtrl.h"
#include "Math.h"


// GraphCtrl

IMPLEMENT_DYNAMIC(GraphCtrl, CWnd)

GraphCtrl::GraphCtrl()
{
	InitValue();
}

GraphCtrl::~GraphCtrl()
{
}


BEGIN_MESSAGE_MAP(GraphCtrl, CWnd)
	ON_WM_PAINT()
	ON_WM_SIZE()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()



// GraphCtrl �޽��� ó�����Դϴ�.

void GraphCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	// �׸��� �޽����� ���ؼ��� CWnd::OnPaint()��(��) ȣ������ ���ʽÿ�.

	if (MemDC.GetSafeHdc() != NULL)
	{
		dc.BitBlt(0, 0, m_SideWidth, m_SideHeight, &MemDC, 0, 0, SRCCOPY);			//��ü �׸���
	}
}

void GraphCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
	GetClientRect(m_rectClient) ;			

	m_SideWidth = m_rectClient.Width();			//�׷��� ��ü �ʺ�
	m_SideHeight = m_rectClient.Height();		//�׷��� ��ü ����

	m_GraphWidth = m_SideWidth-20;				//�׷��� �׸��� �ʺ�
	m_GraphHeight = m_SideHeight-40;			//�׷��� �׸��� ����
	
	
	size.SideWidth = m_SideWidth;
	size.SideHeight = m_SideHeight;
	size.SideWidth = m_GraphWidth;
	size.SideHeight = m_GraphHeight;
}

BOOL GraphCtrl::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID) 
{
	BOOL result ;
	static CString className = AfxRegisterWndClass(CS_HREDRAW | CS_VREDRAW) ;

//	result = CWnd::CreateEx(WS_EX_CLIENTEDGE | WS_EX_STATICEDGE, 
	result = CWnd::CreateEx(WS_EX_WINDOWEDGE , 
                          className, NULL, dwStyle, 
                          rect.left, rect.top, rect.right-rect.left, rect.bottom-rect.top,
                          pParentWnd->GetSafeHwnd(), (HMENU)nID) ;
	if (result != 0)
		DrawRect() ;

	pGraphWnd = pParentWnd;

  return result ;
}

void GraphCtrl::InitGraph()
{
	for(int i=0; i<GRAPH_LINE; i++)
	{
		m_Point_E[i].x = (long) m_GraphRect.right-1;
		m_ArrayPoint[i].RemoveAll();
	}
	DrawRect();
}
void GraphCtrl::InitData(UINT number, CString title, float data_Max, float data_Min, COLORREF Color, BOOL check)
{
	if(number<1)	return;
	else
	{
		UINT GraphNum = number-1;

		m_sNameData[GraphNum].Name = title;
		m_sNameData[GraphNum].RangeMax = data_Max;
		m_sNameData[GraphNum].RangeMin = data_Min;
		m_sNameData[GraphNum].RangeAll = m_sNameData[GraphNum].RangeMax-m_sNameData[GraphNum].RangeMin;
		m_sNameData[GraphNum].Color = Color;
		m_sNameData[GraphNum].Check = check;

		if(check)	m_Num++;
	}
}

void GraphCtrl::BackgroundColor(COLORREF Color)
{
	m_GraphColor = Color;
	DrawRect();
}

void GraphCtrl::GridColor(COLORREF Color)
{
	m_GridColor = Color;
	DrawRect();
}

void GraphCtrl::InitValue()
{
	m_YWidth=40;				//������ Y�� ���� 
	m_XWidth=5;				//�׷����� X�� ����

	m_Num=0;					//�׷��� 1�� ���
	for(int i=0; i<GRAPH_LINE; i++)
	{
		m_sNameData[i].RangeMax = 0;
		m_sNameData[i].RangeMin = 0;
		m_sNameData[i].RangeAll = 0;
		m_sNameData[i].Color = RGB(255, 0, 0);
	}

	m_GraphColor = RGB(255, 255, 255);				//��� ����
	m_GridColor = RGB(0, 0, 0);						//��� ����
	
	m_SettingFlg=false;

	//------------------------------�ܰ� ���̵� ����------------------------------------//
	m_SideRect.left = 0;									//���̾�α׿� X����(50)	
	m_SideRect.top = 0;										//���̾�α׿� Y����(50)
	m_SideRect.right = m_SideWidth;		//���̾�α׿� X����(50) + �ܰ��ʺ�(900)
	m_SideRect.bottom = m_SideHeight;		//���̾�α׿� Y����(50) + �ܰ�����(340)
	//------------------------------�ܰ� ���̵� ����---------------------------------------//	

	//---------------------------------�׷��� ����---------------------------------------//
	m_GraphRect.left = m_SideRect.left + (m_YWidth * m_Num);//X��(50) + Y�࿵�� * ����
	m_GraphRect.top = m_SideRect.top + 10;					//���̾�α׿� Y����(50) + �׷�������(10)
	m_GraphRect.right = m_SideRect.right - 10;				//�ܰ��ʺ� - �׷�������(10)
	m_GraphRect.bottom = m_GraphRect.top + m_GraphHeight;	//���̾�α׿� Y����(50) + �׷��� �׷��� ����
	//---------------------------------�׷��� ����---------------------------------------//
	
	for(int i=0; i<GRAPH_LINE; i++)		m_Point_E[i].x = (long) m_GraphRect.right-1;
}

void GraphCtrl::DrawRect()
{
	//------------------------------�ܰ� ���̵� ����------------------------------------//
	m_SideRect.left = 0;									//���̾�α׿� X����(0)	
	m_SideRect.top = 0;										//���̾�α׿� Y����(0)
	m_SideRect.right = m_SideWidth;			//���̾�α׿� X����(50) + �ܰ��ʺ�(900)
	m_SideRect.bottom = m_SideHeight;		//���̾�α׿� Y����(50) + �ܰ�����(340)
	//------------------------------�ܰ� ���̵� ����---------------------------------------//	

	//---------------------------------�׷��� ����---------------------------------------//
	m_GraphRect.left = m_SideRect.left + (m_YWidth * m_Num);//X��(50) + Y�࿵�� * ����
	m_GraphRect.top = m_SideRect.top + 10;					//���̾�α׿� Y����(50) + �׷�������(10)
	m_GraphRect.right = m_SideRect.right - 10;				//�ܰ��ʺ� - �׷�������(10)
	m_GraphRect.bottom = m_GraphRect.top + m_GraphHeight;	//���̾�α׿� Y����(50) + �׷��� �׷��� ����
	//---------------------------------�׷��� ����---------------------------------------//
	m_GraphWidth = m_GraphRect.right-m_GraphRect.left;


	CClientDC dc(this);									//������ �� 

	if(MemDC.GetSafeHdc() == NULL)
	{
		MemDC.CreateCompatibleDC(&dc);
		Bitmap.CreateCompatibleBitmap(&dc, m_SideWidth, m_SideHeight);
		pOldBmp = (CBitmap *)MemDC.SelectObject  (&Bitmap);			
	}

	//--------------�� �귯�� ����---------------//
	m_RectBrush.CreateSolidBrush(m_GraphColor);			//��� �귯�� ���� 
	m_RectPen.CreatePen(PS_SOLID,0, m_GridColor);	//��� �� ���� 
	m_GridPen.CreatePen(PS_DOT,0, m_GridColor);		//�׸��� �� ���� 

	m_pRectBrush=MemDC.SelectObject(&m_RectBrush);
	m_pRectPen=MemDC.SelectObject(&m_RectPen);
	
	MemDC.SetBkMode(TRANSPARENT);
	//--------------�� �귯�� ����---------------//

	MemDC.Rectangle(m_SideRect.left, m_SideRect.top, m_SideRect.right, m_SideRect.bottom);
	MemDC.Rectangle(m_GraphRect.left, m_GraphRect.top, m_GraphRect.right, m_GraphRect.bottom+1);

	//---------------------------------Y�� �׸���(�տ� �̸�)--------------------------------//
	for(int i=0; i<m_Num; i++)
	{
		MemDC.MoveTo(m_SideRect.left + (m_YWidth * i), m_GraphRect.top);
		MemDC.LineTo(m_SideRect.left + (m_YWidth * i), m_GraphRect.bottom);
	}
	//---------------------------------Y�� �׸���--------------------------------//


	//---------------------------------X�� �׸��� �׸���--------------------------------//
	m_pGridPen=MemDC.SelectObject(&m_GridPen);

	for(int j=0; j<4; j++)
	{

		for(int k=1; k<=m_Num; k++)
		{
			MemDC.MoveTo(m_SideRect.left + (m_YWidth * k), m_GraphHeight/4 * j + m_GraphRect.top);
			MemDC.LineTo(m_SideRect.left + (m_YWidth * k)+5, m_GraphHeight/4 * j + m_GraphRect.top);
		}

		MemDC.MoveTo(m_GraphRect.left, m_GraphHeight/4 * j + m_GraphRect.top);
		MemDC.LineTo(m_GraphRect.right, m_GraphHeight/4 * j + m_GraphRect.top);
	}	
	//---------------------------------X�� �׸��� �׸���--------------------------------//
	


	//--------------------��Ʈ����----------------------//
	m_Yfont.CreateFont (14, 0, 0, 0, 100, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS, "Arial") ;
	m_Xfont.CreateFont (10, 0, 0, 0, 100, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS, "Arial") ;
	
	MemDC.SetTextColor (RGB(128,128,128)) ;
	MemDC.SetTextAlign (TA_CENTER|TA_BASELINE);
	//--------------------��Ʈ����----------------------//


	//--------------------Y�� �̸�---------------------//
	m_pOldFont = MemDC.SelectObject(&m_Yfont);
	for(int k=1; k<=m_Num; k++)
	{ 
		MemDC.SetTextColor (m_sNameData[k-1].Color) ;
		MemDC.SetTextAlign (TA_CENTER|TA_BASELINE);
		MemDC.TextOut (m_SideRect.left + (m_YWidth * k)-18 , m_GraphRect.bottom+20, m_sNameData[k-1].Name);
	}
	MemDC.SelectObject(m_pOldFont) ;			//��Ʈ
	//--------------------Y�� �̸�---------------------//
		

	//--------------------Y�� ������ ��--------------------//
	m_pOldFont = MemDC.SelectObject(&m_Xfont);
	for(int k=0; k<m_Num; k++)
	{
		MemDC.SetTextColor (m_sNameData[k].Color) ;
		MemDC.SetTextAlign (TA_CENTER|TA_BASELINE);

		for(int j=0; j<5; j++)
		{
			CString str;
			str.Format("%.1f", m_sNameData[k].RangeMax -  m_sNameData[k].RangeAll/4*j);
			MemDC.TextOut (m_SideRect.left + (m_YWidth * (k+1))-48 + m_SideRect.left+30, m_GraphHeight/4 * j + m_GraphRect.top+4, str);			
		}
	}
	//--------------------Y�� ������ ��--------------------//
	
	
	//--------------------���� ������ ���� �׸���------------------//
	if(m_SettingFlg)
	{
		CPen GraphPens[GRAPH_LINE];
		CPen *pOldPens[GRAPH_LINE];

		for(int i=0; i<GRAPH_LINE; i++)
		{
			GraphPens[i].CreatePen(PS_SOLID, 1, m_sNameData[i].Color);
			pOldPens[i] = MemDC.SelectObject(&GraphPens[i]);
			LineDraw(i, MemDC);
			MemDC.SelectObject(pOldPens[i]);
		}
	}
	//--------------------���� ������ ���� �׸���------------------//

	MemDC.SelectObject(m_pRectBrush);		
	MemDC.SelectObject(m_pRectPen);
	MemDC.SelectObject(m_pGridPen);			
	MemDC.SelectObject(m_pOldFont) ;				


	//--------------�� �귯�� ����---------------//
	if(m_GridPen.m_hObject)	
		m_GridPen.DeleteObject();
	if(m_RectPen.m_hObject)
		m_RectPen.DeleteObject();
	if(m_RectBrush.m_hObject)
		m_RectBrush.DeleteObject();
	if(m_Yfont.m_hObject)
		m_Yfont.DeleteObject();
	if(m_Xfont.m_hObject)	
		m_Xfont.DeleteObject();
	//--------------�� �귯�� ����---------------//

	Invalidate(false);	
}

void GraphCtrl::AllClear()
{
	for(int i=0; i<GRAPH_LINE; i++)
		m_ArrayPoint[i].RemoveAll();
}

void GraphCtrl::LineAdd(UINT number, float data)
{
	if(number<1)	return;
	else
	{
		UINT GraphNum = number-1;

		m_NewData[GraphNum] = data;
		LineColor(number, m_sNameData[GraphNum].Color);
//		DrawRect();
	}
}

void GraphCtrl::LineClear(UINT number)
{
	if(number<1)	return;
	else
	{
		UINT GraphNum = number-1;

		m_ArrayPoint[GraphNum].RemoveAll();
	}
}

void GraphCtrl::LineColor(UINT number, COLORREF Color)
{
	if(number<1)	return;
	else
	{
		UINT GraphNum = number-1;

		m_sNameData[GraphNum].Color = Color;
	}
}



void GraphCtrl::LineDraw(UINT number, CDC &dc)
{
	m_Point_E[number].y = (long) m_GraphHeight - m_GraphHeight / m_sNameData[number].RangeAll * (m_NewData[number]-m_sNameData[number].RangeMin) + m_GraphRect.top;				//���ΰ�� 

	CPoint point(m_Point_E[number].x, m_Point_E[number].y);
	m_ArrayPoint[number].Add(point);

	int size = m_ArrayPoint[number].GetSize();
	int result = m_GraphWidth / m_XWidth;

	if(size >= 2)
	{
		for(int i=1; i<size ; i++)	m_ArrayPoint[number][i-1].x = m_ArrayPoint[number][i-1].x-m_XWidth;
		for(int i=1; i<size ; i++)
		{	
			//if(m_ArrayPoint[number][i].y <= m_GraphRect.top)	m_ArrayPoint[number][i].y = m_GraphRect.top;			//��ü������ �׷��� ��� ����
			//if(m_ArrayPoint[number][i].y >= m_GraphRect.bottom)	m_ArrayPoint[number][i].y = m_GraphRect.bottom;		//��ü������ �׷��� �ϴ� ����

		 	dc.MoveTo(m_ArrayPoint[number][i-1]);
			dc.LineTo(m_ArrayPoint[number][i]);
		}
	}
	if(size>result)	m_ArrayPoint[number].RemoveAt(0,1);
}

void GraphCtrl::CaptureGraph(UINT GTAPH_ID, CString path)
{
	int      cx,cy;
    CDC      memDC;
    CDC      ScreenDC;
    CBitmap  m_Bitmap;
    CPalette m_Pal;
	
	CRect rect;
	pGraphWnd->GetDlgItem(GTAPH_ID)->GetWindowRect(rect);


    // ��ü ȭ�� ������
    cx = GetSystemMetrics(SM_CXSCREEN);
    cy = GetSystemMetrics(SM_CYSCREEN);

	BITMAPINFO dib_define;
	dib_define.bmiHeader.biSize = sizeof(BITMAPINFOHEADER);
	dib_define.bmiHeader.biWidth = m_SideWidth;
	dib_define.bmiHeader.biHeight = m_SideHeight;
	dib_define.bmiHeader.biPlanes = 1;
	dib_define.bmiHeader.biBitCount = 24;
	dib_define.bmiHeader.biCompression = BI_RGB;
	dib_define.bmiHeader.biSizeImage = (((m_SideWidth * 24 + 31) & ~31) >> 3) * m_SideHeight;
	dib_define.bmiHeader.biXPelsPerMeter = 0;
	dib_define.bmiHeader.biYPelsPerMeter = 0;
	dib_define.bmiHeader.biClrImportant = 0;
	dib_define.bmiHeader.biClrUsed = 0;

    //��ũ�� DC�� ��´�.
    ScreenDC.CreateDC("DISPLAY", NULL, NULL, NULL); 
  
    //��ũ�� DC�� ȣȯ�Ǵ� DC �� �����
    memDC.CreateCompatibleDC(&ScreenDC); 

    //��ũ�� DC�� ȣȯ�Ǵ� ��Ʈ���� �����.
    m_Bitmap.CreateCompatibleBitmap(&ScreenDC, cx, cy);

	// ��Ʈ�� DC�� �����Ѵ�.
	CBitmap* pOldBitmap = memDC.SelectObject(&m_Bitmap);

	BYTE *p_image_data = NULL;

	HBITMAP h_bitmap = ::CreateDIBSection(ScreenDC, &dib_define, DIB_RGB_COLORS, (void **)&p_image_data, 0, 0);			
	HBITMAP h_old_bitmap = (HBITMAP)::SelectObject(memDC, h_bitmap);													

	memDC.BitBlt(0, 0, m_SideWidth, m_SideHeight, &ScreenDC, rect.left, rect.top, SRCCOPY);								
	

	// ������ ��Ʈ������ �����Ѵ�.
	::SelectObject(memDC, h_old_bitmap); 
	
	// ���� DC�� �����Ѵ�.
	
	DeleteDC(memDC);
	DeleteDC(ScreenDC);


	// DIB ������ ������ �����Ѵ�.
	BITMAPFILEHEADER dib_format_layout;
	ZeroMemory(&dib_format_layout, sizeof(BITMAPFILEHEADER));
	dib_format_layout.bfType = *(WORD*)"BM";
	dib_format_layout.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dib_define.bmiHeader.biSizeImage;
	dib_format_layout.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);									

	// DIB ������ �����Ѵ�.
	FILE *p_file = fopen(path, "wb");
	if(p_file != NULL)
	{
		fwrite(&dib_format_layout, 1, sizeof(BITMAPFILEHEADER), p_file);
		fwrite(&dib_define, 1, sizeof(BITMAPINFOHEADER), p_file);
		fwrite(p_image_data, 1, dib_define.bmiHeader.biSizeImage, p_file);
		fclose(p_file);
	}
	if(NULL != h_bitmap) DeleteObject(h_bitmap);
	memDC.SelectObject(pOldBitmap);
}
