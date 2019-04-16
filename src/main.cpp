#include <iostream>

#include "serverSock.h"


using namespace std;


int main() {
	string in_arg;
	cout << "Server" << endl;

	serverSock srv(5678);

	//srv.Run();
	//sleep(10);

	srv.ThreadSock();
	cout<<"Enter 'quit' to exit ..."<<endl;
	while(in_arg != "quit"){
		getline(cin, in_arg);

	}
	//srv.Stop();
	return 0;
}
