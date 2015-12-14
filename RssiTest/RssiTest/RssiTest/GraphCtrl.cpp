// GraphCtrl.cpp : 구현 파일입니다.
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



// GraphCtrl 메시지 처리기입니다.

void GraphCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	// 그리기 메시지에 대해서는 CWnd::OnPaint()을(를) 호출하지 마십시오.

	if (MemDC.GetSafeHdc() != NULL)
	{
		dc.BitBlt(0, 0, m_SideWidth, m_SideHeight, &MemDC, 0, 0, SRCCOPY);			//전체 그리기
	}
}

void GraphCtrl::OnSize(UINT nType, int cx, int cy)
{
	CWnd::OnSize(nType, cx, cy);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
	GetClientRect(m_rectClient) ;			

	m_SideWidth = m_rectClient.Width();			//그래프 전체 너비
	m_SideHeight = m_rectClient.Height();		//그래프 전체 높이

	m_GraphWidth = m_SideWidth-20;				//그래프 그리드 너비
	m_GraphHeight = m_SideHeight-40;			//그래프 그리드 높이
	
	
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
	m_YWidth=40;				//범례의 Y축 간격 
	m_XWidth=5;				//그래프의 X축 간격

	m_Num=0;					//그래프 1개 사용
	for(int i=0; i<GRAPH_LINE; i++)
	{
		m_sNameData[i].RangeMax = 0;
		m_sNameData[i].RangeMin = 0;
		m_sNameData[i].RangeAll = 0;
		m_sNameData[i].Color = RGB(255, 0, 0);
	}

	m_GraphColor = RGB(255, 255, 255);				//배경 색상
	m_GridColor = RGB(0, 0, 0);						//배경 색상
	
	m_SettingFlg=false;

	//------------------------------외곽 사이드 영역------------------------------------//
	m_SideRect.left = 0;									//다이얼로그와 X여백(50)	
	m_SideRect.top = 0;										//다이얼로그와 Y여백(50)
	m_SideRect.right = m_SideWidth;		//다이얼로그와 X여백(50) + 외곽너비(900)
	m_SideRect.bottom = m_SideHeight;		//다이얼로그와 Y여백(50) + 외곽높이(340)
	//------------------------------외곽 사이드 영역---------------------------------------//	

	//---------------------------------그래프 영역---------------------------------------//
	m_GraphRect.left = m_SideRect.left + (m_YWidth * m_Num);//X축(50) + Y축영역 * 갯수
	m_GraphRect.top = m_SideRect.top + 10;					//다이얼로그와 Y여백(50) + 그래프여백(10)
	m_GraphRect.right = m_SideRect.right - 10;				//외곽너비 - 그래프여백(10)
	m_GraphRect.bottom = m_GraphRect.top + m_GraphHeight;	//다이얼로그와 Y여백(50) + 그래프 그래프 높이
	//---------------------------------그래프 영역---------------------------------------//
	
	for(int i=0; i<GRAPH_LINE; i++)		m_Point_E[i].x = (long) m_GraphRect.right-1;
}

void GraphCtrl::DrawRect()
{
	//------------------------------외곽 사이드 영역------------------------------------//
	m_SideRect.left = 0;									//다이얼로그와 X여백(0)	
	m_SideRect.top = 0;										//다이얼로그와 Y여백(0)
	m_SideRect.right = m_SideWidth;			//다이얼로그와 X여백(50) + 외곽너비(900)
	m_SideRect.bottom = m_SideHeight;		//다이얼로그와 Y여백(50) + 외곽높이(340)
	//------------------------------외곽 사이드 영역---------------------------------------//	

	//---------------------------------그래프 영역---------------------------------------//
	m_GraphRect.left = m_SideRect.left + (m_YWidth * m_Num);//X축(50) + Y축영역 * 갯수
	m_GraphRect.top = m_SideRect.top + 10;					//다이얼로그와 Y여백(50) + 그래프여백(10)
	m_GraphRect.right = m_SideRect.right - 10;				//외곽너비 - 그래프여백(10)
	m_GraphRect.bottom = m_GraphRect.top + m_GraphHeight;	//다이얼로그와 Y여백(50) + 그래프 그래프 높이
	//---------------------------------그래프 영역---------------------------------------//
	m_GraphWidth = m_GraphRect.right-m_GraphRect.left;


	CClientDC dc(this);									//윈도우 디씨 

	if(MemDC.GetSafeHdc() == NULL)
	{
		MemDC.CreateCompatibleDC(&dc);
		Bitmap.CreateCompatibleBitmap(&dc, m_SideWidth, m_SideHeight);
		pOldBmp = (CBitmap *)MemDC.SelectObject  (&Bitmap);			
	}

	//--------------펜 브러시 설정---------------//
	m_RectBrush.CreateSolidBrush(m_GraphColor);			//배경 브러시 설정 
	m_RectPen.CreatePen(PS_SOLID,0, m_GridColor);	//배경 펜 설정 
	m_GridPen.CreatePen(PS_DOT,0, m_GridColor);		//그리드 펜 설정 

	m_pRectBrush=MemDC.SelectObject(&m_RectBrush);
	m_pRectPen=MemDC.SelectObject(&m_RectPen);
	
	MemDC.SetBkMode(TRANSPARENT);
	//--------------펜 브러시 설정---------------//

	MemDC.Rectangle(m_SideRect.left, m_SideRect.top, m_SideRect.right, m_SideRect.bottom);
	MemDC.Rectangle(m_GraphRect.left, m_GraphRect.top, m_GraphRect.right, m_GraphRect.bottom+1);

	//---------------------------------Y축 그리기(앞에 이름)--------------------------------//
	for(int i=0; i<m_Num; i++)
	{
		MemDC.MoveTo(m_SideRect.left + (m_YWidth * i), m_GraphRect.top);
		MemDC.LineTo(m_SideRect.left + (m_YWidth * i), m_GraphRect.bottom);
	}
	//---------------------------------Y축 그리기--------------------------------//


	//---------------------------------X축 그리드 그리기--------------------------------//
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
	//---------------------------------X축 그리드 그리기--------------------------------//
	


	//--------------------폰트설정----------------------//
	m_Yfont.CreateFont (14, 0, 0, 0, 100, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS, "Arial") ;
	m_Xfont.CreateFont (10, 0, 0, 0, 100, FALSE, FALSE, 0, ANSI_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, DEFAULT_QUALITY, DEFAULT_PITCH|FF_SWISS, "Arial") ;
	
	MemDC.SetTextColor (RGB(128,128,128)) ;
	MemDC.SetTextAlign (TA_CENTER|TA_BASELINE);
	//--------------------폰트설정----------------------//


	//--------------------Y축 이름---------------------//
	m_pOldFont = MemDC.SelectObject(&m_Yfont);
	for(int k=1; k<=m_Num; k++)
	{ 
		MemDC.SetTextColor (m_sNameData[k-1].Color) ;
		MemDC.SetTextAlign (TA_CENTER|TA_BASELINE);
		MemDC.TextOut (m_SideRect.left + (m_YWidth * k)-18 , m_GraphRect.bottom+20, m_sNameData[k-1].Name);
	}
	MemDC.SelectObject(m_pOldFont) ;			//폰트
	//--------------------Y축 이름---------------------//
		

	//--------------------Y축 데이터 값--------------------//
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
	//--------------------Y축 데이터 값--------------------//
	
	
	//--------------------실제 데이터 라인 그리기------------------//
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
	//--------------------실제 데이터 라인 그리기------------------//

	MemDC.SelectObject(m_pRectBrush);		
	MemDC.SelectObject(m_pRectPen);
	MemDC.SelectObject(m_pGridPen);			
	MemDC.SelectObject(m_pOldFont) ;				


	//--------------펜 브러시 해제---------------//
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
	//--------------펜 브러시 해제---------------//

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
	m_Point_E[number].y = (long) m_GraphHeight - m_GraphHeight / m_sNameData[number].RangeAll * (m_NewData[number]-m_sNameData[number].RangeMin) + m_GraphRect.top;				//세로계산 

	CPoint point(m_Point_E[number].x, m_Point_E[number].y);
	m_ArrayPoint[number].Add(point);

	int size = m_ArrayPoint[number].GetSize();
	int result = m_GraphWidth / m_XWidth;

	if(size >= 2)
	{
		for(int i=1; i<size ; i++)	m_ArrayPoint[number][i-1].x = m_ArrayPoint[number][i-1].x-m_XWidth;
		for(int i=1; i<size ; i++)
		{	
			//if(m_ArrayPoint[number][i].y <= m_GraphRect.top)	m_ArrayPoint[number][i].y = m_GraphRect.top;			//전체영역과 그래프 상단 사이
			//if(m_ArrayPoint[number][i].y >= m_GraphRect.bottom)	m_ArrayPoint[number][i].y = m_GraphRect.bottom;		//전체영역과 그래프 하단 사이

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


    // 전체 화면 사이즈
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

    //스크린 DC를 얻는다.
    ScreenDC.CreateDC("DISPLAY", NULL, NULL, NULL); 
  
    //스크린 DC와 호환되는 DC 를 만든다
    memDC.CreateCompatibleDC(&ScreenDC); 

    //스크린 DC와 호환되는 비트맵을 만든다.
    m_Bitmap.CreateCompatibleBitmap(&ScreenDC, cx, cy);

	// 비트맵 DC를 선택한다.
	CBitmap* pOldBitmap = memDC.SelectObject(&m_Bitmap);

	BYTE *p_image_data = NULL;

	HBITMAP h_bitmap = ::CreateDIBSection(ScreenDC, &dib_define, DIB_RGB_COLORS, (void **)&p_image_data, 0, 0);			
	HBITMAP h_old_bitmap = (HBITMAP)::SelectObject(memDC, h_bitmap);													

	memDC.BitBlt(0, 0, m_SideWidth, m_SideHeight, &ScreenDC, rect.left, rect.top, SRCCOPY);								
	

	// 본래의 비트맵으로 복구한다.
	::SelectObject(memDC, h_old_bitmap); 
	
	// 가상 DC를 제거한다.
	
	DeleteDC(memDC);
	DeleteDC(ScreenDC);


	// DIB 파일의 내용을 구성한다.
	BITMAPFILEHEADER dib_format_layout;
	ZeroMemory(&dib_format_layout, sizeof(BITMAPFILEHEADER));
	dib_format_layout.bfType = *(WORD*)"BM";
	dib_format_layout.bfSize = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER) + dib_define.bmiHeader.biSizeImage;
	dib_format_layout.bfOffBits = sizeof(BITMAPFILEHEADER) + sizeof(BITMAPINFOHEADER);									

	// DIB 파일을 생성한다.
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
