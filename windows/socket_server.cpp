//==================================================
//ソケットサーバクラス
//
//author: Yutaro ISHIDA
//==================================================


#include "stdafx.h"
#include "socket_server.h"


//==================================================
//コンストラクタ
//==================================================
SocketServer::SocketServer(int port)
{
	//winsock2の初期化
	WSAStartup(MAKEWORD(2, 0), &wsa_data_);

	//ソケットの生成
	socket_server_ = socket(AF_INET, SOCK_STREAM, 0);

	//構造体に情報を書き込む
	sockaddr_in_server_.sin_family = AF_INET;
	sockaddr_in_server_.sin_addr.S_un.S_addr = htonl(INADDR_ANY);
	sockaddr_in_server_.sin_port = htons(port);
}


//==================================================
//デストラクタ
//==================================================
SocketServer::~SocketServer()
{
}


//==================================================
//接続する関数
//==================================================
bool SocketServer::open_socket(void)
{
	cout << "[INFO]: [socket_server]: Open socket" << endl;

	if(bind(socket_server_, (struct sockaddr *)&sockaddr_in_server_, sizeof(sockaddr_in_server_)) == SOCKET_ERROR)
	{
		cout << "[ERROR]: [socket_server]: Open socket bind FAILURE" << endl;
		return false;
	}

	if (listen(socket_server_, 1) == SOCKET_ERROR) //1クライアントまで接続可能
	{
		cout << "[ERROR]: [socket_server]: Open socket listen FAILURE" << endl;
		return false;
	}

	cout << "[INFO]: [socket_server]: Wait client" << endl;

	int address_len = sizeof(sockaddr_in_client_);
	socket_client_ = accept(socket_server_, (struct sockaddr *)&sockaddr_in_client_, &address_len);
	if (socket_client_ == SOCKET_ERROR)
	{
		cout << "[ERROR]: [socket_server]: Open socket accept FAILURE" << endl;
		return false;
	}

	u_long argp = 1;
	ioctlsocket(socket_client_, FIONBIO, &argp);

	FD_ZERO(&fds_);
	FD_SET(socket_server_, &fds_);
	FD_SET(socket_client_, &fds_);

	cout << "[INFO]: [socket_server]: Open socket SUCCESS" << endl;
	return true;
}


//==================================================
//切断する関数
//==================================================
bool SocketServer::close_socket(void)
{
	cout << "[INFO]: [socket_server]: Close socket" << endl;

	if (closesocket(socket_client_) == SOCKET_ERROR || closesocket(socket_server_) == SOCKET_ERROR)
	{
		cout << "[ERROR]: [socket_server]: Close socket accept FAILURE" << endl;
		return false;
	}

	cout << "[INFO]: [socket_server]: Close socket SUCCESS" << endl;
	return true;
}


//==================================================
//送信する関数
//==================================================
bool SocketServer::snd(float snd_data[], size_t snd_data_size)
{
	tv_.tv_sec = 10;
	tv_.tv_usec = 0;

	if (select(socket_client_ + 1, NULL, &fds_, NULL, &tv_) == 0)
	{
		cout << "[ERROR]: [socket_server]: Send time-out FAILURE" << endl;
		return false;
	}

	if (send(socket_client_, (char *)snd_data, snd_data_size, 0) == SOCKET_ERROR)
	{
		cout << "[ERROR]: [socket_server]: Send FAILURE" << endl;
		return false;
	}

	return true;
}


//==================================================
//受信する関数
//==================================================
bool SocketServer::rcv(float rcv_data[], size_t rcv_data_size)
{
	int buf_cnt_rcv_data_size = 0;
	int cnt_rcv_data_size = 0;

	tv_.tv_sec = 10;
	tv_.tv_usec = 0;

	if (select(socket_client_ + 1, &fds_, NULL, NULL, &tv_) == 0)
	{
		cout << "[ERROR]: [socket_server]: Recieve time-out FAILURE" << endl;
		return false;
	}

	int cnt_rcv = 0;

	while (rcv_data_size - cnt_rcv_data_size > 0) {
		buf_cnt_rcv_data_size = recv(socket_client_, (char *)rcv_data + cnt_rcv_data_size, rcv_data_size - cnt_rcv_data_size, 0);

		if (buf_cnt_rcv_data_size == SOCKET_ERROR)
		{
			cout << "[ERROR]: [socket_server]: Recieve FAILURE" << endl;
			return false;
		}

		cnt_rcv_data_size += buf_cnt_rcv_data_size;

		if (cnt_rcv >= 10000)
		{
			cout << "[ERROR]: [socket_server]: Recieve FAILURE" << endl;
			return false;
		}
		else
		{
			cnt_rcv++;
		}
	}

	return true;
}