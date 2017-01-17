
// ExamClientDlg.h : 헤더 파일
//

#pragma once
#include "afxwin.h"


// CExamClientDlg 대화 상자
class CExamClientDlg : public CDialogEx
{
private:
	SOCKET mh_socket = INVALID_SOCKET;//비트가 변화는 환경에 영향받지않음 (~0)
	char m_connect_flag = 0;// 0:접속해제, 1:접속중, 2:접속됨
// 생성입니다.
public:
	CExamClientDlg(CWnd* pParent = NULL);	// 표준 생성자입니다.
// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXAMCLIENT_DIALOG };
#endif
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
