/*
 * serverSock.cpp
 *
 *  Created on: 11 ���. 2019 �.
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
	cout << "close socket "<<sockfd<<endl;
	close(sockfd);
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
	if(listen(sockfd, 3) < 0){
		perror("listen");
		return false;
	}
	return true;
}




// start server
void serverSock::ThreadSock(){

	serverSock* c;
	socklen_t clilen;
	int cnt = 0;

	struct sockaddr cliaddr;
	sockaddr_in cs_addr;
	clilen = sizeof(cs_addr);

	//printf("sizeof(cliaddr) %d \n",clilen);

	char buf[1024];
	int bytes_read;

	struct timeval tv;
	fd_set master;
	fd_set readfds;

	tv.tv_sec = 0;
	tv.tv_usec = 0;

	FD_ZERO(&master);
	//FD_SET(0, &readfds);
	FD_SET(sockfd, &master);
	int fdmax = sockfd;



	int STDIN = 0;
	int sock;
		//if(arg != NULL){
	while(!stopSock){

		readfds = master;
		int ret_sel = select(sockfd+1, &readfds, NULL, NULL, &tv);
		//printf("ret_sel = %d\n",ret_sel);
		//sleep(1);
		if(FD_ISSET(sockfd, &readfds)){
		   printf("sockfd.\n");
		   sock = accept(sockfd, NULL, NULL);
		   if(sock < 0){
			   cout << "Error accept"<<endl;
		   		stopSock = true;
		   		continue;
		   	}
		   	Clients.push_back(new clientSock(sock,std::bind(&serverSock::action_f,this,_1)));

		}else{

		}
	}
}




// Start thread keyboard event
void serverSock::StartKeyThread(){
	pthread_create(&t_thread_key_input, NULL, &_thread_key_input, (void*)this);
}
// Stop thread keyboard event
void serverSock::StopKeyThread(){
	pthread_join(t_thread_key_input,NULL);
}
//	Thread for read data from keyboard
void* serverSock::_thread_key_input(void* arg){
	serverSock* c;
	string nazv_sh;
	cout << "Please enter 'quit' to exit"<<endl;
	if(arg != NULL){
		c = (serverSock*)arg;
		while(nazv_sh != "quit"){
			getline(cin, nazv_sh);
		}
		c->stopSock = true;
	}
	return NULL;
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
		arguments = "Version Socket Server 1.00\n";
		return true;
	}
	arguments = "error\n";
	return false;
}

