#pragma once

#include "../misc/Pch.h"
#include "../misc/Defines.h"

class ServerSocketReceiveException : public std::exception { };
class ServerSocketSendException    : public std::exception { };

enum class ServerSocketCreationExceptionErrorType
{
	CREATION_FAILED = 0,
	BINDING_FAILED = 1,
	LISTENING_FAILED = 2
};

class ServerSocketCreationException : public std::exception
{
private:
	const ServerSocketCreationExceptionErrorType m_errorType;

public:
	ServerSocketCreationException(const ServerSocketCreationExceptionErrorType errorType);

	ServerSocketCreationExceptionErrorType getErrorType() const;
};

class ServerSocket
{
private:
	SOCKET m_socket = INVALID_SOCKET;

	std::vector<SOCKET> m_clients;

public:
	ServerSocket();

	~ServerSocket();

	void Bind(const unsigned int port);

	[[nodiscard]] int Accept() noexcept;

	void Send(const int clientID, const char* data, int length = -1);

	[[nodiscard]] std::pair<std::array<char, WEISS_CLIENT_SOCKET_RECEIVE_BUFFER_SIZE>, size_t> Receive(const int clientID);

	void Disconnect() noexcept;
};