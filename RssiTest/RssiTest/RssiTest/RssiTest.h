
// RssiTest.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CRssiTestApp:
// �� Ŭ������ ������ ���ؼ��� RssiTest.cpp�� �����Ͻʽÿ�.
//

class CRssiTestApp : public CWinAppEx
{
public:
	CRssiTestApp();

// �������Դϴ�.
	public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CRssiTestApp theApp;