#pragma once

#include <afxtempl.h>


#define GRAPH_LINE		10		//라인갯수 설정

typedef  struct					//상태확인
{
	CString		Name;			//이름
	float		RangeMax;		//MAX 범위
	float		RangeMin;		//MIN 범위
	float		RangeAll;		//전체 범위
	int			Check;			//사용여부
	COLORREF	Color;			//색상
}NAME_SATATE;

typedef struct
{
	int SideWidth;			//그래프 전체 너비
	int SideHeight;			//그래프 전체 높이
	int GraphWidth;			//그래프 그리드 너비
	int GraphHeight;		//그래프 그리드 높이
}GRAPH_SIZE;


// GraphCtrl
class GraphCtrl : public CWnd
{
	DECLARE_DYNAMIC(GraphCtrl)

public:
	GraphCtrl();
	virtual ~GraphCtrl();

	CWnd *pGraphWnd;

	void InitGraph();
	void InitData(UINT number, CString title, float data_Max, float data_Min, COLORREF Color, BOOL check);
	void DrawRect();
	virtual BOOL Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID=NULL);
	
	void LineDraw(UINT number, CDC &dc);
	void LineAdd(UINT number, float data);
	void LineColor(UINT number, COLORREF Color);
	void LineClear(UINT number);
	void BackgroundColor(COLORREF Color);
	void GridColor(COLORREF Color);
	void CaptureGraph(UINT GTAPH_ID, CString path);
	void AllClear();

	BOOL		m_SettingFlg;
	GRAPH_SIZE	size;
	CPoint		GraphPoint;
	




protected:
	DECLARE_MESSAGE_MAP()

	CDC MemDC;
	CBitmap Bitmap, *pOldBmp;

	CRect	m_rectClient;						//픽쳐박스 컨트롤
	CRect	m_SideRect, m_GraphRect;			//전체영역, 그래프가 그려지는 영역 

	CBrush	m_RectBrush, *m_pRectBrush;			//캔버스 브러시
	CPen	m_RectPen, *m_pRectPen;				//캔버스 펜 
	CPen	m_GridPen, *m_pGridPen;				//그리드 펜 
	CFont	m_Xfont, m_Yfont, *m_pOldFont ;
	COLORREF	m_GraphColor;					//배경 색상
	COLORREF	m_GridColor;					//그리드 색상
	
	//----------------------------------------------------//
	CDC		m_dcPlot;							//그래프 
	CBitmap m_bitmapPlot, *m_pbitmapOldPlot;
	//----------------------------------------------------//

//	CPen	m_GraphPen, *m_pGraphPen;			//그래프 펜 

	


	int m_SideWidth;			//외곽 너비
	int m_SideHeight;			//외곽 높이
	int m_GraphWidth;			//그래프 너비
	int m_GraphHeight;			//그래프 높이
	int m_YWidth;				//Y축 간격
	int m_XWidth;				//Y축 간격

	int	m_Num;					//체크확인갯수(사용그래프 갯수)
	float	m_NewData[GRAPH_LINE];		//데이터 값						갯수만큼 할당
	CPoint m_Point_S[GRAPH_LINE], m_Point_E[GRAPH_LINE];				
	NAME_SATATE m_sNameData[GRAPH_LINE];								
	CArray <CPoint, CPoint&> m_ArrayPoint[GRAPH_LINE];					
	void InitValue();


public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


