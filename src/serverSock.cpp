/*
 * serverSock.cpp
 *
 *  Created on: 11 апр. 2019 г.
 *      Author: Maksim
 */

#include "serverSock.h"


//Init server socket
serverSock::serverSock(int port) {
	listeningPort = port;
	sockfd = -1;
	isInit = false;
	stopSock = false;
	p_thread_sock = NULL;
}

serverSock::~serverSock() {
	// TODO Auto-generated destructor stub
	//close();
	for(int i = 0; i < Clients.size(); i++){
		delete(Clients[i]);
	}
	Clients.clear();
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
	listen(sockfd, 3);
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


// start server
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
		sock = accept(sockfd, NULL, NULL);
		if(sock < 0){
			cout << "Error accept"<<endl;
			stopSock = true;
			continue;
		}
		printf("sizeof(cliaddr) %d \n",clilen);
		printf("Connected %d \n",cs_addr.sin_addr.s_addr);

		// after connect, start thread read data from socket
		Clients.push_back(new clientSock(sock,std::bind(&serverSock::action_f,this,_1)));

		cnt++;
		if(cnt > 1){
			stopSock = true;
		}

	}
}


bool serverSock::action_f(string &arguments){
	int find_pos;
	find_pos = arguments.find("exit");
	if(find_pos != string::npos){
		arguments = "exit";
		return true;
	}
	find_pos = arguments.find("version");
	if(find_pos != string::npos){
		arguments = "Version Socket Server 1.00";
		return true;
	}
	cout<<"arguments" << arguments << endl;
	arguments = "error";
	return false;
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


void* serverSock::_thread_read_sock(void*){

}
