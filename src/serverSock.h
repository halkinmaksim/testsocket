/*
 * serverSock.h
 *
 *  Created on: 11 апр. 2019 г.
 *      Author: Maksim
 */

#ifndef SRC_SERVERSOCK_H_
#define SRC_SERVERSOCK_H_

#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <iostream>
#include <vector>

#include <pthread.h>
#include <unistd.h>  // problem solved! it compiles!
#include <termios.h> // POSIX terminal control definitionss

#include "clientSock.h"



using namespace std;
using namespace std::placeholders; // for `_1`

class serverSock {
public:
	serverSock(int port);
	virtual ~serverSock();
	bool Init();

	void Run();
	void Stop();

	void ThreadSock();

	bool action_f(string &arguments);
	bool isInit;
	bool stopSock;

protected:
	int listeningPort;
	int sockfd;			// sock descriptor


	struct sockaddr_in addr;

	pthread_t p_thread_sock;
	static void* _thread_sock(void*);

	static void* _thread_read_sock(void*);

	std::vector<clientSock*>	Clients;



};

#endif /* SRC_SERVERSOCK_H_ */
