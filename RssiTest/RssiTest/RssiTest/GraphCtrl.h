#pragma once

#include <afxtempl.h>


#define GRAPH_LINE		10		//���ΰ��� ����

typedef  struct					//����Ȯ��
{
	CString		Name;			//�̸�
	float		RangeMax;		//MAX ����
	float		RangeMin;		//MIN ����
	float		RangeAll;		//��ü ����
	int			Check;			//��뿩��
	COLORREF	Color;			//����
}NAME_SATATE;

typedef struct
{
	int SideWidth;			//�׷��� ��ü �ʺ�
	int SideHeight;			//�׷��� ��ü ����
	int GraphWidth;			//�׷��� �׸��� �ʺ�
	int GraphHeight;		//�׷��� �׸��� ����
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

	CRect	m_rectClient;						//���Ĺڽ� ��Ʈ��
	CRect	m_SideRect, m_GraphRect;			//��ü����, �׷����� �׷����� ���� 

	CBrush	m_RectBrush, *m_pRectBrush;			//ĵ���� �귯��
	CPen	m_RectPen, *m_pRectPen;				//ĵ���� �� 
	CPen	m_GridPen, *m_pGridPen;				//�׸��� �� 
	CFont	m_Xfont, m_Yfont, *m_pOldFont ;
	COLORREF	m_GraphColor;					//��� ����
	COLORREF	m_GridColor;					//�׸��� ����
	
	//----------------------------------------------------//
	CDC		m_dcPlot;							//�׷��� 
	CBitmap m_bitmapPlot, *m_pbitmapOldPlot;
	//----------------------------------------------------//

//	CPen	m_GraphPen, *m_pGraphPen;			//�׷��� �� 

	


	int m_SideWidth;			//�ܰ� �ʺ�
	int m_SideHeight;			//�ܰ� ����
	int m_GraphWidth;			//�׷��� �ʺ�
	int m_GraphHeight;			//�׷��� ����
	int m_YWidth;				//Y�� ����
	int m_XWidth;				//Y�� ����

	int	m_Num;					//üũȮ�ΰ���(���׷��� ����)
	float	m_NewData[GRAPH_LINE];		//������ ��						������ŭ �Ҵ�
	CPoint m_Point_S[GRAPH_LINE], m_Point_E[GRAPH_LINE];				
	NAME_SATATE m_sNameData[GRAPH_LINE];								
	CArray <CPoint, CPoint&> m_ArrayPoint[GRAPH_LINE];					
	void InitValue();


public:
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};


