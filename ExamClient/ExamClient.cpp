
// ExamClient.cpp : ���� ���α׷��� ���� Ŭ���� ������ �����մϴ�.
//

#include "stdafx.h"
#include "ExamClient.h"
#include "ExamClientDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CExamClientApp

BEGIN_MESSAGE_MAP(CExamClientApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CExamClientApp ����

CExamClientApp::CExamClientApp()
{
	// TODO: ���⿡ ���� �ڵ带 �߰��մϴ�.
	// InitInstance�� ��� �߿��� �ʱ�ȭ �۾��� ��ġ�մϴ�.
}


// ������ CExamClientApp ��ü�Դϴ�.

CExamClientApp theApp;


// CExamClientApp �ʱ�ȭ

BOOL CExamClientApp::InitInstance()
{
	CWinApp::InitInstance();
	WSADATA temp;
	WSAStartup(0x0202, &temp);//���� ����� �ش� �ڵ� ���� ���� ������****

	CExamClientDlg dlg;
	m_pMainWnd = &dlg;
	dlg.DoModal();
	
	WSACleanup();//������ �����->���� �ִ� �� �� ���� ����
	return FALSE;
}

