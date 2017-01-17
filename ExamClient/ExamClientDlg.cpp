
// ExamClientDlg.cpp : 구현 파일
//

#include "stdafx.h"
#include "ExamClient.h"
#include "ExamClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CExamClientDlg 대화 상자



CExamClientDlg::CExamClientDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_EXAMCLIENT_DIALOG, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CExamClientDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CHAT_LIST, m_chat_list);
}

BEGIN_MESSAGE_MAP(CExamClientDlg, CDialogEx)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_BN_CLICKED(IDC_SEND_BTN, &CExamClientDlg::OnBnClickedSendBtn)
	ON_MESSAGE(25001, &CExamClientDlg::On25001)
	ON_WM_DESTROY()
	ON_MESSAGE(25002, &CExamClientDlg::On25002)
END_MESSAGE_MAP()


// CExamClientDlg 메시지 처리기

BOOL CExamClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.
	mh_socket = socket(AF_INET, SOCK_STREAM, 0);//소켓 생성
	struct sockaddr_in srv_addr;
	memset(&srv_addr, 0, sizeof(struct sockaddr_in));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	srv_addr.sin_port = htons(18000);
	WSAAsyncSelect(mh_socket, m_hWnd, 25001, FD_CONNECT);
	m_connect_flag = 1;//접속중 상태를 나타내는 플래그값
	
	AddEventString("서버에 접속을 시도합니다...");

	connect(mh_socket, (LPSOCKADDR)&srv_addr, sizeof(srv_addr));
	//서버가 실행되고(listen) 있어야됨, 커넥트 함수가 실패하면 최대 28초 동안 응답없음 상태에 빠짐
	
	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 응용 프로그램의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CExamClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CExamClientDlg::OnQueryDragIcon(){

	return static_cast<HCURSOR>(m_hIcon);
}

void CExamClientDlg::OnBnClickedSendBtn()//전송 버튼
{
	CString str;
	GetDlgItemText(IDC_CHAT_EDIT, str);

	if (m_connect_flag == 2) {
		SendFrameData(mh_socket, 1, str.GetLength()+1, (char *)(const char *)str);
		
		SetDlgItemText(IDC_CHAT_LIST, str);
		GotoDlgCtrl(GetDlgItem(IDC_CHAT_EDIT));//특정 대화상자 컨트롤로 보낸다. -> 반전
	}
}

void CExamClientDlg::AddEventString(const char *ap_string) {
	while (m_chat_list.GetCount() > 500) {
		m_chat_list.DeleteString(0);
	}
	int index = m_chat_list.InsertString(-1, ap_string);
	m_chat_list.SetCurSel(index);
}

afx_msg LRESULT CExamClientDlg::On25001(WPARAM wParam, LPARAM lParam) {
	if (WSAGETSELECTERROR(lParam)) {
		m_connect_flag = 0;
		closesocket(mh_socket);
		mh_socket = INVALID_SOCKET;
		AddEventString("서버에 접속을 실패했습니다.~");
	}
	else {
		m_connect_flag = 2;
		WSAAsyncSelect(mh_socket, m_hWnd, 25002, FD_READ | FD_CLOSE);
		AddEventString("서버에 접속했습니다.");
	}
	return 0;
}


void CExamClientDlg::OnDestroy()
{
	CDialogEx::OnDestroy();
	if (mh_socket != INVALID_SOCKET) {
		closesocket(mh_socket);
		mh_socket = INVALID_SOCKET;
	}
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}

void CExamClientDlg::SendFrameData(SOCKET ah_socket, char a_message_id, unsigned short int a_body_size, char* ap_send_data)//헤더와 데이터의 content를 보내는 함수
{
	char *p_send_data = new char[4 + a_body_size];//헤더 4 byte
	*p_send_data = 27;//헤더 구성
	*(p_send_data + 1) = a_message_id;
	*(unsigned short *)(p_send_data + 2) = a_body_size;//일시적으로 char*를 short*에 맞게 변위를 조절

	memcpy(p_send_data + 4, ap_send_data, a_body_size);//데이터를 복사
	send(ah_socket, p_send_data, a_body_size + 4, 0);//send(동기화함수->다 진행될때까지) 클라이언트에게 전송
	delete[] p_send_data;//전송후 삭제
}


afx_msg LRESULT CExamClientDlg::On25002(WPARAM wParam, LPARAM lParam)
{
	CString str;
	if (WSAGETSELECTEVENT(lParam) == FD_READ) {
		WSAAsyncSelect(wParam, m_hWnd, 25002, FD_CLOSE);//-->비동기에서 FD_READ를 제외시킴
														//정한 규약에 따라..
		char key;
		recv(wParam, &key, 1, 0);//1바이트 읽어서 key값에 설정. 4번쨰 인자는 recv 옵션설정 0:default
		if (key == 27) {//자신의 클라이언트에서 보낸 건지 확인
			char message_id;
			recv(wParam, &message_id, 1, 0);
			unsigned short body_size;
			recv(wParam, (char *)&body_size, 2, 0);
			char* p_body_data = NULL;
			if (body_size > 0) {				
				p_body_data = new char[body_size];
				int total = 0, x, retry = 0;
				while (total < body_size) {//recv가 수신한 사이즈에 맞게 content를 받음
					x = recv(wParam, p_body_data + total, body_size - total, 0);//데이터content 받음, 시작 주소는 +total, 받는 사이즈는 -total
					if (x == SOCKET_ERROR)break;//client가 비정상 종료하여 recv가 에러값을 반환하면 break될수 있도록
					total = total + x;
					if (total < body_size) {
						Sleep(50);//5밀리 세컨드 쉬어서 자신의 빠른 속도를 어느정도 조절해 수신 버퍼의 양을 높힐 수 있도록 한다.
						retry++;
						if (retry > 5)break;//재시도 5번 넘으면 break
					}
				}
			}
			if (message_id == 1) {
				AddEventString(p_body_data);
			}
			if (p_body_data != NULL)delete[] p_body_data;
			
			WSAAsyncSelect(wParam, m_hWnd, 25002, FD_READ | FD_CLOSE);
		}
		//현상 말고 원인에 집중하기		
	}
	else {//FD_CLOSE
		closesocket(mh_socket);//클라이언트 소켓 클로즈
		mh_socket = INVALID_SOCKET;
		m_connect_flag = 0;//연결 해제 flag 변경
		AddEventString("서버가 연결을 해제 했습니다.~");
	}
	return 0;
}
