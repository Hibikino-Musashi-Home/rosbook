//==================================================
//ソケットクライアントクラス
//
//author: Yutaro ISHIDA
//==================================================


#include "common_pkg/socket_client.h"


//==================================================
//コンストラクタ
//==================================================
SocketClient::SocketClient(string ipaddr, int port){
    //ソケットの生成
    socket_client_ = socket(AF_INET, SOCK_STREAM, 0);

    //構造体に情報を書き込む
    sockaddr_in_client_.sin_family = AF_INET;
    sockaddr_in_client_.sin_addr.s_addr = inet_addr(ipaddr.c_str());
    sockaddr_in_client_.sin_port = htons(port);
}
 

//==================================================
//デストラクタ
//==================================================
SocketClient::~SocketClient(){
}


//==================================================
//接続する関数
//==================================================
bool SocketClient::open_socket(void){
    cout << "[INFO]: [socket_client]: Open socket" << endl;

    if(connect(socket_client_, (struct sockaddr *)&sockaddr_in_client_, sizeof(sockaddr_in_client_)) < 0){
        cout << "[ERROR]: [socket_client]: Open socket connect FAILURE" << endl;
        return false;
    }

    FD_ZERO(&fds_);
    FD_SET(socket_client_, &fds_);

    cout << "[INFO]: [socket_client]: Open socket SUCCESS" << endl;
    return true;
}


//==================================================
//切断する関数
//==================================================
bool SocketClient::close_socket(void){
    cout << "[INFO]: [socket_client]: Close socket" << endl;

    if(close(socket_client_) < 0){
        cout << "[ERROR]: [socket_client]: Close socket close FAILURE" << endl;
        return false;
    }

    cout << "[INFO]: [socket_client]: Close socket SUCCESS" << endl;
    return true;
}


//==================================================
//送信する関数
//==================================================
bool SocketClient::snd(float snd_data[], size_t snd_data_size){
    tv_.tv_sec = 10;
    tv_.tv_usec = 0;

    if(select(socket_client_ + 1, NULL, &fds_, NULL, &tv_) == 0){
        cout << "[ERROR]: [socket_client]: Send time-out FAILURE" << endl;
        return false;
    }

    if(send(socket_client_, (char *)snd_data, snd_data_size, 0) < 0){ //MSG_DONTWAIT
        cout << "[ERROR]: [socket_client]: Send FAILURE" << endl;
        return false;
    }

    return true;
}


//==================================================
//受信する関数
//==================================================
bool SocketClient::rcv(float rcv_data[], size_t rcv_data_size){
    int buf_cnt_rcv_data_size = 0;        
    int cnt_rcv_data_size = 0;

    tv_.tv_sec = 10;
    tv_.tv_usec = 0;

    if(select(socket_client_ + 1, &fds_, NULL, NULL, &tv_) == 0){
        cout << "[ERROR]: [socket_client]: Recieve time-out FAILURE" << endl;
        return false;
    }

    int cnt_rcv = 0;

    while(rcv_data_size - cnt_rcv_data_size > 0){    
        buf_cnt_rcv_data_size = recv(socket_client_, (char *)rcv_data + cnt_rcv_data_size, rcv_data_size - cnt_rcv_data_size, 0); //MSG_DONTWAIT
            
        if(buf_cnt_rcv_data_size < 0){
            cout << "[ERROR]: [socket_client]: Recieve FAILURE" << endl;
            return false;
        }

        cnt_rcv_data_size += buf_cnt_rcv_data_size;

        if(cnt_rcv >= 10000){
            cout << "[ERROR]: [socket_client]: Recieve FAILURE" << endl;                
            return false;
        }
        else{
            cnt_rcv++;
        }
    }

    return true;
}
