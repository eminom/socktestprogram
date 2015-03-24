



#ifndef _SOCK_SESSION_DEF__
#define _SOCK_SESSION_DEF__


#include "SessionProtocol.h"
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>
#include <boost/date_time/posix_time/posix_time.hpp>
#include <boost/array.hpp>

class SockSession;
typedef boost::shared_ptr<SockSession> SockSessionPtr;
typedef std::function<void(int, const char*, int length)> InComingBufferCallback;

class SockSession:public boost::enable_shared_from_this<SockSession>,
	public SockSessionProtocol,
	private boost::noncopyable
{
private:
	SockSession(boost::asio::io_service &io_service, const std::string &host, const std::string &port);

public:
	~SockSession();
	static SockSession* create(boost::asio::io_service &io_service, const std::string &host, const std::string &port);

protected:
	void start();

public:
	void setDestination(const std::string &host, const std::string &port);
	void connect();
	void close();
	void write(int typeCode, const char *msg, int length);
	void read();
	void readBody();
	void setTimeout(float seconds);	//~ in seconds
	bool isTimeout()const;
	bool isSocketFailed()const;

	void setNetMessagePumper(InComingBufferCallback cb);

private:
	void flushOpTime();

private:
	void handle_connected(const boost::system::error_code &error/*, boost::asio::ip::tcp::resolver::iterator ep_iterator*/);
	void handle_closed();
	void handle_written(const boost::system::error_code &error, std::size_t byteWritten, std::string *buffer);
	void handle_headerRead(const boost::system::error_code &error);
	void handle_bodyRead(const boost::system::error_code &error);

private:
	boost::asio::io_service &io_service_;
	boost::asio::ip::tcp::socket socket_;
	boost::posix_time::ptime last_op_;
	float timeout_;
	bool connecting_timeout_;
	bool socket_failed_;

	boost::array<char, 4> headLength_;
	std::string buffer_;
	std::string host_;
	std::string port_;

	InComingBufferCallback callback_;
};






#endif