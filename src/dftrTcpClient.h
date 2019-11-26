/**
 * implements Cinder block Cinder-Asio
 * https://github.com/BanTheRewind/Cinder-Asio
 *
 * by Blakely L McConnell  2015->~∞
 */

#pragma once

#include "TcpClient.h"
#include "jsoncpp/json.h"
#include "dftrMsgHandler.h"
#include "CinderAsio.h"
#include <iostream>

namespace drifter
{
namespace network
{
    typedef std::shared_ptr<MsgHandler> MsgHandlerRef;
    class TCPClient
    {
        template <class T>
        void SafeCall( const std::function<void()> & wrappedMethod, T refToCheck, const std::string & funcName ) const
        {
            if ( refToCheck == nullptr ) {
                std::cout << "WARNING: function " << funcName << " could not be called because function object is null" << std::endl;
            }
            wrappedMethod();
        }
        //event handlers
        void onError( std::string errorMsg, size_t bytesTransferred ) const;
        void onDisconnected( const asio::ip::tcp::endpoint & endpoint ) const;
        void onMessage( ci::BufferRef buffer );
        void onConnect( TcpSessionRef tcpSessionRef );
        void Write( const std::string & output ) const;

        TcpClientRef			_tcpClientRef;
        TcpSessionRef           _tcpSessionRef;
        MsgHandlerRef           _msgHandlerRef;
        bool                    _needsReadUpdate;

    public:
        TCPClient();
        void Send( Json::Value & message ) const;
        void SetMsgHandler( const MsgHandlerRef & msgHandlerRef );
        void Connect( const std::string & ip, unsigned short port ) const;
        void Update();
    }; //TCPClient
} //network
} //drifter
