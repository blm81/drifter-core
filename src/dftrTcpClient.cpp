/**
 * implements Cinder block Cinder-Asio
 * https://github.com/BanTheRewind/Cinder-Asio
 *
 * by Blakely L McConnell  2015->~∞
 */

#include "dftrTcpClient.h"
#include "cinder/app/App.h"

namespace drifter
{
namespace network
{
    /**
     * error event handler
     * @param errorMsg: the error returned
     * @param bytesTransferred: size in byte of error message
     */
    void TCPClient::onError( std::string errorMsg, size_t bytesTransferred ) const
    {
        std::cout << "WARNING: tcp socket error: " << errorMsg << std::endl;
        //TODO attempt reconnection
    }

    /**
     * disconnection event handler
     * @param endpoint: the endpoint that has disconnected
     */
    void TCPClient::onDisconnected( const asio::ip::tcp::endpoint & endpoint ) const
    {
        std::cout << "Disconnected from:" << endpoint.address().to_string() << std::endl;
    }

    /**
     * incoming message event handler
     * @param buffer: raw buffer of incoming message
     */
    void TCPClient::onMessage( ci::BufferRef buffer )
    {
        std::string message = TcpSession::bufferToString( buffer );
        SafeCall<MsgHandlerRef>([&](){ _msgHandlerRef->HandleInput( message ); }, _msgHandlerRef, "HandleInput" );
        _needsReadUpdate = true; //TODO don't set this here, do it in read complete handler below
    }

    /**
     * connection event handler
     * @param tcpSessionRef: a reference to the session instantiated at connection time
     */
    void TCPClient::onConnect( TcpSessionRef tcpSessionRef )
    {
        std::cout << "Connected to server " << std::endl;
        _tcpSessionRef = tcpSessionRef;
        _tcpSessionRef->connectCloseEventHandler( [&]() {
            std::cout << "TCP session was closed" << std::endl;
        });
        _tcpSessionRef->connectErrorEventHandler( &TCPClient::onError, this );
        _tcpSessionRef->connectReadEventHandler( &TCPClient::onMessage, this );
        _tcpSessionRef->connectReadCompleteEventHandler( [ & ]() {
            std::cout << "read completed" << std::endl;
            //TODO debug why this is not firing.  read update flag should be set here not onMessage
        });
        _tcpSessionRef->read();
    }

    /**
     * write to socket
     * @param output: the string to write
     */
    void TCPClient::Write( const std::string & output ) const
    {
        if ( _tcpSessionRef != nullptr && _tcpSessionRef->getSocket()->is_open() ) {
            std::string outStr = output;
            _tcpSessionRef->write( TcpSession::stringToBuffer( outStr ));
        }
        else {
            std::cout << "WARNING: message could not be sent because session is not open" << std::endl;
        }
    }

    TCPClient::TCPClient() : _msgHandlerRef( nullptr ),
        _tcpClientRef( nullptr ),
        _tcpSessionRef( nullptr ),
        _needsReadUpdate( false )
    {
        //connect tcp listeners
        _tcpClientRef = TcpClient::create( ci::app::App::get()->io_service() );
        _tcpClientRef->connectConnectEventHandler( [&]( TcpSessionRef tcpSessionRef ) {
            onConnect( tcpSessionRef );
        });
        _tcpClientRef->connectErrorEventHandler( &TCPClient::onError, this );
    }

    /**
     * send message to server
     * @param message: message to send, NOTE that the type is set automatically here
     */
    void TCPClient::Send( Json::Value & message ) const
    {
        message["type"] = "output"; //mark message as output
        SafeCall<MsgHandlerRef>( [&, message](){ _msgHandlerRef->Parse( message.toStyledString() );}, _msgHandlerRef, "Parse" ); //TODO just send, no need to route through MsgHandler
    }

    /**
     * bind passed message handler object and set neccesary state
     * @param msgHandlerRef: the message handler to bind
     */
    void TCPClient::SetMsgHandler( const MsgHandlerRef & msgHandlerRef )
    {
        _msgHandlerRef = msgHandlerRef;
        _msgHandlerRef->SetName( "drifter" ); //default client name
        _msgHandlerRef->SetSendFunc( [&]( const Json::Value & jsonOut ) {
            Write( jsonOut.toStyledString() );
        });
    }

    /**
     * connect to server
     * @param ip: server address
     * @param port: server port
     */
    void TCPClient::Connect( const std::string & ip, unsigned short port ) const
    {
        SafeCall<TcpClientRef>( [&]() {
            _tcpClientRef->connect( ip, port );
            std::cout << "connecting to " << ip << ":" << port << std::endl;
        }, _tcpClientRef, "connect" );
    }

    void TCPClient::Update()
    {
        if ( _needsReadUpdate ) {
            _tcpSessionRef->read();
            _needsReadUpdate = false;
        }
    }
} //network
} //drifter