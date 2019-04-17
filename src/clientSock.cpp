/*
 * clientSock.cpp
 *
 *  Created on: 16 апр. 2019 г.
 *      Author: Maksim
 */

#include "clientSock.h"



clientSock::clientSock(int socket){
	p_static_thread_sock_read = NULL;
	action_f = NULL;
	thread_sock_read_stop = false;
	this->psocket = socket;
	printf("clientSock(%d) \n", socket);
	Start_static_thread_sock_read();

}


clientSock::clientSock(int socket,std::function<bool(string &arguments)> action_f){
	this->action_f = action_f;
	p_static_thread_sock_read = NULL;
	thread_sock_read_stop = false;
	this->psocket = socket;
	printf("clientSock(%d) \n", socket);
	Start_static_thread_sock_read();
}

clientSock::~clientSock(){
	cout<<"~clientSock()"<<endl;
	Stop_static_thread_sock_read();
	cout<<"_static_thread_sock_read stop"<<endl;

	close(psocket);

}



void clientSock::Start_static_thread_sock_read(){
	pthread_create(&p_static_thread_sock_read, NULL, &_static_thread_sock_read, (void*)this);
	thread_sock_read_stop = false;
}
void clientSock::Stop_static_thread_sock_read(){
	thread_sock_read_stop = true;
	if(p_static_thread_sock_read != NULL){
		pthread_join(p_static_thread_sock_read,NULL);
	}
}


void* clientSock::_static_thread_sock_read(void* arg){
	clientSock* c = (clientSock*) arg;
	c->_thread_sock_read();
	return NULL;
}


void clientSock::_thread_sock_read(){
    std::string key, message;
    int code1, code2;

    int code_recv;
    uint32_t length;
    char msg[1024];

    string linecmd;

    while (!thread_sock_read_stop) {

    	code_recv =recv(psocket, &msg, 1024, 0);
    	if(code_recv > 0){
  //  		cout<<"msg = "<<msg << endl;
    		linecmd = string(msg);
    		if(action_f != NULL){
    			if(action_f(linecmd)){
    				if(linecmd != "exit"){
    					send(psocket, linecmd.c_str(), linecmd.size(), 0);
    				}else{
    					close(psocket);

    				}
    			}else{
    				cout<<"cmd not found = "<<linecmd << endl;
    				return;
    			}

    		}else{
    			cout<<"linecmd = "<<linecmd << endl;
    		}

    	}else{
    		printf("client %d close socket \n", psocket);
    		return;
    	}


        /*code1 = receive(key);
        code2 = receive(message);
        if(code1==0 || code2==0){
            disconnect();
            if(mDisconnectListener!=NULL){
                (*mDisconnectListener)(this);
            }
        }
        else if(code1!=-1 && code2!=-1){
            if(mMessageListenerMap[key]!=NULL){
                (*mMessageListenerMap[key])(this, stringToVector(message));
            }
        }*/
    }
}


int clientSock::recvStringLn(){

}


/*
int SocketClient::receive(std::string &message){
    uint32_t length;
    int code;

    code = ::recv(mSocket, &length, sizeof(uint32_t), 0);
    if(code!=-1 && code!=0){
        length = ntohl(length);
        char server_reply[length];
        message = "";

        int q = length/mPacketSize;
        int r = length%mPacketSize;

        for(int i=0 ; i<q ; i++){
            code = ::recv(mSocket, server_reply, mPacketSize, 0);
            if(code!=-1 && code!=0){
                message += std::string(server_reply, mPacketSize);
            }
            else{
                return code;
            }
        }
        if(r!=0){
            char server_reply_rest[r];
            code = ::recv(mSocket, server_reply_rest, r, 0);
            if(code!=-1 && code!=0){
                message += std::string(server_reply_rest, r);
            }
        }
    }
    return code;
}



 * */
