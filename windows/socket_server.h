//==================================================
//�\�P�b�g�T�[�o�N���X�̃w�b�_
//
//author: Yutaro ISHIDA
//==================================================


#pragma once


#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <iostream>
#include <sstream>
#include <fstream>
#include <vector>

#define _WINSOCK_DEPRECATED_NO_WARNINGS

//�ȉ�2�s�C�K��windows.h����ɑ��点�邱��
#include <winsock2.h>
#pragma comment(lib, "wsock32.lib")


using namespace std;


//==================================================
//�p�����[�^
//==================================================


//==================================================
//�O���[�o��
//==================================================


//==================================================
//Socket�N���C�A���g�N���X
//==================================================
class SocketServer
{
private:
	WSADATA wsa_data_;

	struct sockaddr_in sockaddr_in_server_;
	SOCKET socket_server_;

	struct sockaddr_in sockaddr_in_client_;
	SOCKET socket_client_;

	fd_set fds_;

	struct timeval tv_;


public:
	//==================================================
	//�R���X�g���N�^
	//==================================================
	SocketServer(int port);


	//==================================================
	//�f�X�g���N�^
	//==================================================
	~SocketServer();


	//==================================================
	//�ڑ�����֐�
	//==================================================
	bool open_socket(void);


	//==================================================
	//�ؒf����֐�
	//==================================================
	bool close_socket(void);


	//==================================================
	//���M����֐�
	//==================================================
	bool snd(float snd_data[], size_t snd_data_size);


	//==================================================
	//��M����֐�
	//==================================================
	bool rcv(float rcv_data[], size_t rcv_data_size);
};

