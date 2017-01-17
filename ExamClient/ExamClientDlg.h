
// ExamClientDlg.h : ��� ����
//

#pragma once
#include "afxwin.h"


// CExamClientDlg ��ȭ ����
class CExamClientDlg : public CDialogEx
{
private:
	SOCKET mh_socket = INVALID_SOCKET;//��Ʈ�� ��ȭ�� ȯ�濡 ����������� (~0)
	char m_connect_flag = 0;// 0:��������, 1:������, 2:���ӵ�
// �����Դϴ�.
public:
	CExamClientDlg(CWnd* pParent = NULL);	// ǥ�� �������Դϴ�.
// ��ȭ ���� �������Դϴ�.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXAMCLIENT_DIALOG };
#endif
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
private:
	CListBox m_chat_list;
public:
	afx_msg void OnBnClickedSendBtn();
	void AddEventString(const char * ap_string);
protected:
	afx_msg LRESULT On25001(WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnDestroy();
	void SendFrameData(SOCKET ah_socket, char a_message_id, unsigned short int a_body_size, char * ap_send_data);
protected:
	afx_msg LRESULT On25002(WPARAM wParam, LPARAM lParam);
};
