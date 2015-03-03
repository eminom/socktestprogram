



#ifndef _SOCK_SESSION_DEF__
#define _SOCK_SESSION_DEF__


#include "SessionProtocol.h"
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/noncopyable.hpp>
#include <boost/asio.hpp>

class SockSession;
typedef boost::shared_ptr<SockSession> SockSessionPtr;

class SockSession:public boost::enable_shared_from_this<SockSession>,
	public SockSessionProtocol,
	private boost::noncopyable
{
private:
	SockSession(boost::asio::io_service &io_service);

public:
	~SockSession();
	static SockSession* create(boost::asio::io_service &io_service);

protected:
	void start();

public:
	void connect();
	void close();
	void write(const std::string &msg);
	void read();

private:
	void handle_connected(const boost::system::error_code &error/*, boost::asio::ip::tcp::resolver::iterator ep_iterator*/);
	void handle_closed();
	void handle_written(const boost::system::error_code &error, std::size_t byteWritten, std::string *buffer);

private:
	boost::asio::io_service &io_service_;
	boost::asio::ip::tcp::socket socket_;

	friend SockSessionPtr;
};






#endif