
// ExamClient.h : PROJECT_NAME ���� ���α׷��� ���� �� ��� �����Դϴ�.
//

#pragma once

#ifndef __AFXWIN_H__
	#error "PCH�� ���� �� ������ �����ϱ� ���� 'stdafx.h'�� �����մϴ�."
#endif

#include "resource.h"		// �� ��ȣ�Դϴ�.


// CExamClientApp:
// �� Ŭ������ ������ ���ؼ��� ExamClient.cpp�� �����Ͻʽÿ�.
//

class CExamClientApp : public CWinApp
{
public:
	CExamClientApp();

// �������Դϴ�.
public:
	virtual BOOL InitInstance();

// �����Դϴ�.

	DECLARE_MESSAGE_MAP()
};

extern CExamClientApp theApp;