/*
 * clientSock.h
 *
 *  Created on: 16 апр. 2019 г.
 *      Author: Maksim
 */

#ifndef SRC_CLIENTSOCK_H_
#define SRC_CLIENTSOCK_H_


#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>

#include <iostream>

#include <pthread.h>
#include <unistd.h>
#include <termios.h>

#include <functional>


using namespace std;





class clientSock {
public:


	clientSock(int socket);
	//	Create client with pointer to action func
	clientSock(int socket,std::function<bool(string &arguments)> action_f);
	~clientSock();

	void Start_static_thread_sock_read();
	void Stop_static_thread_sock_read();

	static void* _static_thread_sock_read(void*);

	void _thread_sock_read();
	//bool (*action_f)(string &arguments);

	std::function<bool(string &arguments)> action_f;


protected:
	int recvStringLn();
	int psocket;
	bool thread_sock_read_stop;
	pthread_t p_static_thread_sock_read;
};

#endif /* SRC_CLIENTSOCK_H_ */
