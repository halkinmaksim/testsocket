#include <iostream>

#include "serverSock.h"


using namespace std;


int main() {
	string in_arg;
	cout << "Server" << endl;

	serverSock srv(5678);

	if(!srv.Init()){
		cout << " Error init socket"<<endl;
		return -1;
	}
	//Start thread for keyboard input
	srv.StartKeyThread();
	//Start main thread for listen socket
	srv.ThreadSock();


	srv.StopKeyThread();
	//srv.Stop();
	return 0;
}
