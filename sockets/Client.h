#pragma once

#include "../misc/Pch.h"
#include "../misc/Defines.h"

enum class ClientSocketCreationExceptionErrorType
{
	CREATION_FAILED = 0,
	CONNECTION_FAILED = 1
};

class ClientSocketCreationException : public std::exception
{
private:
	const ClientSocketCreationExceptionErrorType m_errorType;

public:
	ClientSocketCreationException(const ClientSocketCreationExceptionErrorType errorType);

	ClientSocketCreationExceptionErrorType getErrorType() const;
};

class ClientSocketReceiveException : public std::exception { };

class ClientSocketSendException : public std::exception { };

class ClientSocket {
private:
	SOCKET m_socket = INVALID_SOCKET;

public:
	ClientSocket();

	~ClientSocket();

	void Connect(const char* host, const unsigned int port);

	void Send(const char* data, int length = -1);
	
	[[nodiscard]] std::pair<std::array<char, WEISS_CLIENT_SOCKET_RECEIVE_BUFFER_SIZE>, size_t> Receive();

	void Disconnect() noexcept;
};