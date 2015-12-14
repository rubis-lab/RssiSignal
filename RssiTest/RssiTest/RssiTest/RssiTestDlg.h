
// RssiTestDlg.h : ��� ����
//

#pragma once
#include "GraphCtrl.h"		//�׷��� Ŭ����
#include "afxwin.h"

// CRssiTestDlg ��ȭ ����
class CRssiTestDlg : public CDialog
{
// �����Դϴ�.
public:
	CRssiTestDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.

	//------------------------�׷��� ����-----------------------//
	GraphCtrl	m_Graph;

	void		InitChart();					//�׷��� �ʱ�ȭ
	void		ChartDraw();					//�׷��� �׸���
	//------------------------�׷��� ����-----------------------//

	void		InitPos();						//��ġ �ʱ�ȭ
	BOOL		m_SizeFlg;						//������ �÷���

	



// ��ȭ ���� �������Դϴ�.
	enum { IDD = IDD_RssiTest_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV �����Դϴ�.


// �����Դϴ�.
protected:
	HICON m_hIcon;

	// ������ �޽��� �� �Լ�
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
