//==================================================
//ソケットクライアントクラス
//
//author: Yutaro ISHIDA
//==================================================


#include "stdafx.h"
#include "socket_client.h"


//==================================================
//コンストラクタ
//==================================================
SocketClient::SocketClient(string ipaddr, int port)
{
	//winsock2の初期化
	WSAStartup(MAKEWORD(2, 0), &wsa_data_);

	//ソケットの生成
	socket_client_ = socket(AF_INET, SOCK_STREAM, 0);

	//構造体に情報を書き込む
	sockaddr_in_client_.sin_family = AF_INET;
	sockaddr_in_client_.sin_addr.S_un.S_addr = inet_addr(ipaddr.c_str());
	sockaddr_in_client_.sin_port = htons(port);
}


//==================================================
//デストラクタ
//==================================================
SocketClient::~SocketClient()
{
}


//==================================================
//接続する関数
//==================================================
bool SocketClient::open_socket(void)
{
	cout << "[INFO]: [socket_client]: Open socket" << endl;

	if (connect(socket_client_, (struct sockaddr *)&sockaddr_in_client_, sizeof(sockaddr_in_client_)) == SOCKET_ERROR)
	{
		cout << "[ERROR]: [socket_client]: Open socket connect FAILURE" << endl;
		return false;
	}

	u_long argp = 1;
	ioctlsocket(socket_client_, FIONBIO, &argp);

	FD_ZERO(&fds_);
	FD_SET(socket_client_, &fds_);

	tv_.tv_sec = 10;
	tv_.tv_usec = 0;

	cout << "[INFO]: [socket_client]: Open socket SUCCESS" << endl;
	return true;
}


//==================================================
//切断する関数
//==================================================
bool SocketClient::close_socket(void)
{
	cout << "[INFO]: [socket_client]: Close socket" << endl;

	if (closesocket(socket_client_) == SOCKET_ERROR) {
		cout << "[ERROR]: [socket_client]: Close socket closesocket FAILURE " << endl;
		return false;
	}

	WSACleanup();

	cout << "[INFO]: [socket_client]: Close socket SUCCESS" << endl;
	return true;
}


//==================================================
//送信する関数
//==================================================
bool SocketClient::snd(float snd_data[], size_t snd_data_size)
{
	tv_.tv_sec = 10;
	tv_.tv_usec = 0;

	if (select(socket_client_ + 1, NULL, &fds_, NULL, &tv_) == 0)
	{
		cout << "[ERROR]: [socket_client]: Send time-out FAILURE" << endl;
		return false;
	}

	if (send(socket_client_, (char *)snd_data, snd_data_size, 0) == SOCKET_ERROR)
	{
		cout << "[ERROR]: [socket_client]: Send FAILURE" << endl;
		return false;
	}

	return true;
}


//==================================================
//受信する関数
//==================================================
bool SocketClient::rcv(float rcv_data[], size_t rcv_data_size)
{
	int buf_cnt_rcv_data_size = 0;
	int cnt_rcv_data_size = 0;

	tv_.tv_sec = 10;
	tv_.tv_usec = 0;

	if (select(socket_client_ + 1, &fds_, NULL, NULL, &tv_) == 0)
	{
		cout << "[ERROR]: [socket_client]: Recieve time-out FAILURE" << endl;
		return false;
	}
	
	int cnt_rcv = 0;

	while (rcv_data_size - cnt_rcv_data_size > 0) {
		buf_cnt_rcv_data_size = recv(socket_client_, (char *)rcv_data + cnt_rcv_data_size, rcv_data_size - cnt_rcv_data_size, 0);
		
		if (buf_cnt_rcv_data_size == SOCKET_ERROR)
		{
			cout << "[ERROR]: [socket_client]: Recieve FAILURE" << endl;
			return false;
		}

		cnt_rcv_data_size += buf_cnt_rcv_data_size;

		if (cnt_rcv >= 1000)
		{
			cout << "[ERROR]: [socket_client]: Recieve FAILURE" << endl;
			return false;
		}
		else
		{
			cnt_rcv++;
		}
	}
	
	return true;
}