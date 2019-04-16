/*
 * serverSock.cpp
 *
 *  Created on: 11 апр. 2019 г.
 *      Author: Maksim
 */

#include "serverSock.h"

serverSock::serverSock(int port) {
	// TODO Auto-generated constructor stub
	listeningPort = port;
	sockfd = -1;
	isInit = false;
	stopSock = false;
	p_thread_sock = NULL;
	if(!Init()){
		cout<<"Error init socket"<<endl;
	}
}

serverSock::~serverSock() {
	// TODO Auto-generated destructor stub
	//close();
}

bool serverSock::Init(){
	sockfd = socket(AF_INET, SOCK_STREAM, 0);
	if(sockfd < 0){
		isInit = false;
		return false;
	}
	addr.sin_family = AF_INET;
	addr.sin_port = htons(listeningPort);
	addr.sin_addr.s_addr = htonl(INADDR_ANY);

	if(bind(sockfd, (struct sockaddr *)&addr, sizeof(addr)) < 0)
	{
		perror("bind");
		return false;
	}
	listen(sockfd, 1);
	return true;
}


void serverSock::Run(){
	stopSock = false;
	pthread_create(&p_thread_sock, NULL, &_thread_sock, (void*)this);
}
void serverSock::Stop(){
	stopSock = true;
	pthread_join(p_thread_sock,NULL);

}


void serverSock::ThreadSock(){

	serverSock* c;
	socklen_t clilen;
	int cnt = 0;

	struct sockaddr cliaddr;
	sockaddr_in cs_addr;
	clilen = sizeof(cs_addr);

	printf("sizeof(cliaddr) %d \n",clilen);

	char buf[1024];
	int bytes_read;

	int sock;
		//if(arg != NULL){
	while(!stopSock){
		//cout << "thread test"<<endl;
		//sock = accept(c->sockfd,  &cliaddr, &clilen);
		sock = accept(sockfd, (struct sockaddr* )&cs_addr, &clilen);
		if(sock < 0){
			cout << "Error accept"<<endl;
			stopSock = true;
			continue;
		}
		printf("sizeof(cliaddr) %d \n",clilen);
		printf("Connected %d \n",cs_addr.sin_addr.s_addr);
		//cout<<""
		//printf("Connected %s \n",cliaddr.sa_family);

		while(true){

			//bytes_read = read(sock, buf, 1024);
			bytes_read = recv(sock, buf, 1024, 0);
			//sleep(1);
			if(bytes_read <= 0){
				cout<<"err"<<endl;
			if(bytes_read <= 0) break;
				send(sock, buf, bytes_read, 0);
			}else{
				printf("bytes_read %d \n", bytes_read);
			}

		}
		close(sock);
		cnt++;
		if(cnt > 1){
			stopSock = true;
		}

	}
}


void* serverSock::_thread_sock(void* arg){
	serverSock* c;
	socklen_t clilen;

	struct sockaddr cliaddr;
	char buf[1024];
	int bytes_read;

	int sock;
		if(arg != NULL){
			c = (serverSock*)arg;
			while(!(c->stopSock)){
				//c->modem->ReadPort();
				sleep(1);
				cout << "thread test"<<endl;
				//sock = accept(c->sockfd,  &cliaddr, &clilen);
				sock = accept(c->sockfd, NULL, NULL);
				if(sock < 0){
					cout << "Error accept"<<endl;
					continue;
				}
				printf("Connected %s \n", cliaddr.sa_data);

				while(true){

						//bytes_read = read(sock, buf, 1024);
					bytes_read = recv(sock, buf, 1024, 0);
					//sleep(1);
					if(bytes_read <= 0){
						cout<<"err"<<endl;

						if(bytes_read <= 0) break;
						send(sock, buf, bytes_read, 0);
					}else{
						printf("bytes_read %d \n", bytes_read);
					}

				}
				close(sock);

			}
		}
		return NULL;
}
