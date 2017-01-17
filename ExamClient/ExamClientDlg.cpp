
// ExamClientDlg.cpp : ���� ����
//

#include "stdafx.h"
#include "ExamClient.h"
#include "ExamClientDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CExamClientDlg ��ȭ ����



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


// CExamClientDlg �޽��� ó����

BOOL CExamClientDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// �� ��ȭ ������ �������� �����մϴ�.  ���� ���α׷��� �� â�� ��ȭ ���ڰ� �ƴ� ��쿡��
	//  �����ӿ�ũ�� �� �۾��� �ڵ����� �����մϴ�.
	SetIcon(m_hIcon, TRUE);			// ū �������� �����մϴ�.
	SetIcon(m_hIcon, FALSE);		// ���� �������� �����մϴ�.
	mh_socket = socket(AF_INET, SOCK_STREAM, 0);//���� ����
	struct sockaddr_in srv_addr;
	memset(&srv_addr, 0, sizeof(struct sockaddr_in));
	srv_addr.sin_family = AF_INET;
	srv_addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	srv_addr.sin_port = htons(18000);
	WSAAsyncSelect(mh_socket, m_hWnd, 25001, FD_CONNECT);
	m_connect_flag = 1;//������ ���¸� ��Ÿ���� �÷��װ�
	
	AddEventString("������ ������ �õ��մϴ�...");

	connect(mh_socket, (LPSOCKADDR)&srv_addr, sizeof(srv_addr));
	//������ ����ǰ�(listen) �־�ߵ�, Ŀ��Ʈ �Լ��� �����ϸ� �ִ� 28�� ���� ������� ���¿� ����
	
	return TRUE;  // ��Ŀ���� ��Ʈ�ѿ� �������� ������ TRUE�� ��ȯ�մϴ�.
}

// ��ȭ ���ڿ� �ּ�ȭ ���߸� �߰��� ��� �������� �׸�����
//  �Ʒ� �ڵ尡 �ʿ��մϴ�.  ����/�� ���� ����ϴ� MFC ���� ���α׷��� ��쿡��
//  �����ӿ�ũ���� �� �۾��� �ڵ����� �����մϴ�.

void CExamClientDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // �׸��⸦ ���� ����̽� ���ؽ�Ʈ�Դϴ�.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// Ŭ���̾�Ʈ �簢������ �������� ����� ����ϴ�.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// �������� �׸��ϴ�.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// ����ڰ� �ּ�ȭ�� â�� ���� ���ȿ� Ŀ���� ǥ�õǵ��� �ý��ۿ���
//  �� �Լ��� ȣ���մϴ�.
HCURSOR CExamClientDlg::OnQueryDragIcon(){

	return static_cast<HCURSOR>(m_hIcon);
}

void CExamClientDlg::OnBnClickedSendBtn()//���� ��ư
{
	CString str;
	GetDlgItemText(IDC_CHAT_EDIT, str);

	if (m_connect_flag == 2) {
		SendFrameData(mh_socket, 1, str.GetLength()+1, (char *)(const char *)str);
		
		SetDlgItemText(IDC_CHAT_LIST, str);
		GotoDlgCtrl(GetDlgItem(IDC_CHAT_EDIT));//Ư�� ��ȭ���� ��Ʈ�ѷ� ������. -> ����
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
		AddEventString("������ ������ �����߽��ϴ�.~");
	}
	else {
		m_connect_flag = 2;
		WSAAsyncSelect(mh_socket, m_hWnd, 25002, FD_READ | FD_CLOSE);
		AddEventString("������ �����߽��ϴ�.");
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
	// TODO: ���⿡ �޽��� ó���� �ڵ带 �߰��մϴ�.
}

void CExamClientDlg::SendFrameData(SOCKET ah_socket, char a_message_id, unsigned short int a_body_size, char* ap_send_data)//����� �������� content�� ������ �Լ�
{
	char *p_send_data = new char[4 + a_body_size];//��� 4 byte
	*p_send_data = 27;//��� ����
	*(p_send_data + 1) = a_message_id;
	*(unsigned short *)(p_send_data + 2) = a_body_size;//�Ͻ������� char*�� short*�� �°� ������ ����

	memcpy(p_send_data + 4, ap_send_data, a_body_size);//�����͸� ����
	send(ah_socket, p_send_data, a_body_size + 4, 0);//send(����ȭ�Լ�->�� ����ɶ�����) Ŭ���̾�Ʈ���� ����
	delete[] p_send_data;//������ ����
}


afx_msg LRESULT CExamClientDlg::On25002(WPARAM wParam, LPARAM lParam)
{
	CString str;
	if (WSAGETSELECTEVENT(lParam) == FD_READ) {
		WSAAsyncSelect(wParam, m_hWnd, 25002, FD_CLOSE);//-->�񵿱⿡�� FD_READ�� ���ܽ�Ŵ
														//���� �Ծ࿡ ����..
		char key;
		recv(wParam, &key, 1, 0);//1����Ʈ �о key���� ����. 4���� ���ڴ� recv �ɼǼ��� 0:default
		if (key == 27) {//�ڽ��� Ŭ���̾�Ʈ���� ���� ���� Ȯ��
			char message_id;
			recv(wParam, &message_id, 1, 0);
			unsigned short body_size;
			recv(wParam, (char *)&body_size, 2, 0);
			char* p_body_data = NULL;
			if (body_size > 0) {				
				p_body_data = new char[body_size];
				int total = 0, x, retry = 0;
				while (total < body_size) {//recv�� ������ ����� �°� content�� ����
					x = recv(wParam, p_body_data + total, body_size - total, 0);//������content ����, ���� �ּҴ� +total, �޴� ������� -total
					if (x == SOCKET_ERROR)break;//client�� ������ �����Ͽ� recv�� �������� ��ȯ�ϸ� break�ɼ� �ֵ���
					total = total + x;
					if (total < body_size) {
						Sleep(50);//5�и� ������ ��� �ڽ��� ���� �ӵ��� ������� ������ ���� ������ ���� ���� �� �ֵ��� �Ѵ�.
						retry++;
						if (retry > 5)break;//��õ� 5�� ������ break
					}
				}
			}
			if (message_id == 1) {
				AddEventString(p_body_data);
			}
			if (p_body_data != NULL)delete[] p_body_data;
			
			WSAAsyncSelect(wParam, m_hWnd, 25002, FD_READ | FD_CLOSE);
		}
		//���� ���� ���ο� �����ϱ�		
	}
	else {//FD_CLOSE
		closesocket(mh_socket);//Ŭ���̾�Ʈ ���� Ŭ����
		mh_socket = INVALID_SOCKET;
		m_connect_flag = 0;//���� ���� flag ����
		AddEventString("������ ������ ���� �߽��ϴ�.~");
	}
	return 0;
}
