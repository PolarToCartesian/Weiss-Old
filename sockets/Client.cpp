#include "Client.h"

ClientSocketCreationException::ClientSocketCreationException(const ClientSocketCreationExceptionErrorType errorType)
	: m_errorType(errorType) { }

ClientSocketCreationExceptionErrorType ClientSocketCreationException::getErrorType() const { return this->m_errorType; }

ClientSocket::ClientSocket()
{
	WSADATA wsaData;
	WSAStartup(MAKEWORD(2, 0), &wsaData);
}

ClientSocket::~ClientSocket() { this->Disconnect(); }

void ClientSocket::Connect(const char* host, const unsigned int port)
{
	SOCKADDR_IN sockAddr;
	sockAddr.sin_addr.s_addr = inet_addr(host);
	sockAddr.sin_family = AF_INET;
	sockAddr.sin_port = htons(port);

	this->m_socket = socket(AF_INET, SOCK_STREAM, 0);

	if (this->m_socket == INVALID_SOCKET)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("[CLIENT SOCKET] Socket Creation Failed");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw ClientSocketCreationException(ClientSocketCreationExceptionErrorType::CREATION_FAILED);
	}

	if (connect(this->m_socket, (SOCKADDR*)&sockAddr, sizeof(sockAddr)) == SOCKET_ERROR)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("[CLIENT SOCKET] Unable To Connect To Server Socket");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw ClientSocketCreationException(ClientSocketCreationExceptionErrorType::CONNECTION_FAILED);
	}
}

void ClientSocket::Send(const char* data, int length)
{
	if (length < 0)
		length = static_cast<int>(strlen(data) + 1u);

	if (send(this->m_socket, data, length, 0) == SOCKET_ERROR)
	{
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("[CLIENT SOCKET] Error While Sending Data From Client Socket");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw ClientSocketSendException();
	}
}

std::pair<std::array<char, WEISS_CLIENT_SOCKET_RECEIVE_BUFFER_SIZE>, size_t> ClientSocket::Receive()
{
	std::array<char, WEISS_CLIENT_SOCKET_RECEIVE_BUFFER_SIZE> buffer;

	const int iResult = recv(this->m_socket, buffer.data(), WEISS_CLIENT_SOCKET_RECEIVE_BUFFER_SIZE, 0);

	if (iResult == 0 || iResult == SOCKET_ERROR) {
#ifdef __WEISS_SHOW_DEBUG_ERRORS
		MESSAGE_BOX_ERROR("[CLIENT SOCKET] Error While Receiving Data From Server");
#endif // __WEISS_SHOW_DEBUG_ERRORS

		throw ClientSocketReceiveException();
	}

	return { buffer, (size_t)iResult };
}

void ClientSocket::Disconnect() noexcept
{
	this->m_socket = INVALID_SOCKET;

	closesocket(this->m_socket);
	WSACleanup();
}