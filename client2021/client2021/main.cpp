#include <iostream>
#include <string>
#include <WS2tcpip.h>
#pragma comment (lib, "ws2_32.lib")

using namespace std;

void main() {
	string ipAdress = "192.168.1.46"; // ip adress of the server
	int port = 54000;  // listening port # on the server

	// init winsock
	WSAData data;
	WORD ver = MAKEWORD(2, 2);
	int wsResult = WSAStartup(ver, &data);
	if (wsResult != 0) {
		cerr << "can't start winsock. Err #" << wsResult << endl;
		return;
	}

	// create a socket
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) {
		cerr << "can't create a socket. err #" << WSAGetLastError() << endl;
		WSACleanup();
		return;
	}

	// fill in a hint structure
	sockaddr_in hint;
	hint.sin_family = AF_INET;
	hint.sin_port = htons(port);
	inet_pton(AF_INET, ipAdress.c_str(), &hint.sin_addr);

	// connect to server
	int connResult = connect(sock, (sockaddr*)&hint, sizeof(hint));
	if (connResult == SOCKET_ERROR) {
		cerr << "can't connect to the server, err#" << WSAGetLastError() << endl;
		closesocket(sock);
		WSACleanup();
		return;
	}

	// do-while loop to send and receive data
	char buf[4096];
	string userInput;

	do {
		// prompt the user for some text
		cout << "> ";
		getline(cin, userInput);

		if (userInput.size() > 0) // make sure the use typed in something
		{
			// send the text
			int sendResult = send(sock, userInput.c_str(), userInput.size() + 1, 0);
			if (sendResult != SOCKET_ERROR) {
				// wait for response
				ZeroMemory(buf, 4096);
				int bytesReceived = recv(sock, buf, 4096, 0);
				if (bytesReceived > 0) {
					//echo response to console
					cout << "SERVER> " << string(buf, 0, bytesReceived) << endl;
				}
			}
		}
	} while (userInput.size() > 0);

	// close down everything
	closesocket(sock);
	WSACleanup();
}
