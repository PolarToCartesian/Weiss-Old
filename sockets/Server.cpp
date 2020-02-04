#include "Server.h"

ServerSocketCreationException::ServerSocketCreationException(const ServerSocketCreationExceptionErrorType errorType)
	: m_errorType(errorType) { }

ServerSocketCreationExceptionErrorType ServerSocketCreationException::getErrorType() const { return this->m_errorType; }

ServerSocket::ServerSocket()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData);
}

ServerSocket::~ServerSocket() { this->Disconnect(); }

void ServerSocket::Bind(const unsigned int port)
{
	this->m_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (this->m_socket == INVALID_SOCKET)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("[SERVER SOCKET] Socket Creation Failed");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw ServerSocketCreationException(ServerSocketCreationExceptionErrorType::CREATION_FAILED);
	}

	SOCKADDR_IN sockAddr;
	sockAddr.sin_addr.s_addr = INADDR_ANY;
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(5555);

	if (bind(this->m_socket, (SOCKADDR*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("[SERVER SOCKET] Could Not Bind Socket");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw ServerSocketCreationException(ServerSocketCreationExceptionErrorType::BINDING_FAILED);
	}

	if (listen(this->m_socket, 0) == SOCKET_ERROR)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("[SERVER SOCKET] Could Make The Socket Listen");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw ServerSocketCreationException(ServerSocketCreationExceptionErrorType::LISTENING_FAILED);
	}
}

int ServerSocket::Accept() noexcept
{
	SOCKADDR_IN clientAddr;
	int clientAddrSize = 0;

	const SOCKET client = accept(this->m_socket, (SOCKADDR*)&clientAddr, &clientAddrSize);

	if (client != INVALID_SOCKET)
	{
		this->m_clients.push_back(client);

		return static_cast<int>(this->m_clients.size() - 1u);
	}

	return -1;
}

void ServerSocket::Send(const int clientID, const char* data, int length)
{
	if (length < 0)
		length = static_cast<int>(strlen(data) + 1);

	if (send(this->m_clients[clientID], data, length, 0) == SOCKET_ERROR)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("[SERVER SOCKET] Could Not Send Data To Client Socket");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw ServerSocketSendException();
	}
}

std::pair<std::array<char, WEISS_CLIENT_SOCKET_RECEIVE_BUFFER_SIZE>, size_t> ServerSocket::Receive(const int clientID)
{
	std::array<char, WEISS_CLIENT_SOCKET_RECEIVE_BUFFER_SIZE> buffer;

	const int iResult = recv(this->m_clients[clientID], buffer.data(), WEISS_CLIENT_SOCKET_RECEIVE_BUFFER_SIZE, 0);

	if (iResult == 0 || iResult == SOCKET_ERROR) {
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("[SERVER SOCKET] Error While Receiving Data From Client");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw ServerSocketReceiveException();
	}

	return { buffer, (size_t)iResult };
}

void ServerSocket::Disconnect() noexcept
{
	this->m_socket = INVALID_SOCKET;

	closesocket(this->m_socket);
	WSACleanup();
}