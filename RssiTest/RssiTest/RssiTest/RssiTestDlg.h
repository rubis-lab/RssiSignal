
// RssiTestDlg.h : 헤더 파일
//

#pragma once
#include "GraphCtrl.h"		//그래프 클래스
#include "afxwin.h"

// CRssiTestDlg 대화 상자
class CRssiTestDlg : public CDialog
{
// 생성입니다.
public:
	CRssiTestDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.

	//------------------------그래프 관련-----------------------//
	GraphCtrl	m_Graph;

	void		InitChart();					//그래프 초기화
	void		ChartDraw();					//그래프 그리기
	//------------------------그래프 관련-----------------------//

	void		InitPos();						//위치 초기화
	BOOL		m_SizeFlg;						//사이즈 플래그

	



// 대화 상자 데이터입니다.
	enum { IDD = IDD_RssiTest_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	CStatic m_Static_Graph;
	afx_msg void OnStnDblclickGraph();
	afx_msg void OnBnClickedStart();
	afx_msg void OnBnClickedCapture();

	CEdit SSID;	
	CEdit DBM;
	CListBox m_aplist;
	
	int m_nOptimalAP;	
};
