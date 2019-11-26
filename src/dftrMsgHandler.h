/**
 * incoming message handler, "business logic" goes here
 * set the callbacks with std::fuction enclosing logic specific to your application
 * and pass instance of this class to client
 *
 * by Blakely L McConnell  2015->~∞
 */

#pragma once

#include "jsoncpp/json.h"
#include <functional>

namespace drifter
{
namespace network
{
    class MsgHandler
    {
    public:
        MsgHandler() : _name( "drifter" ),
                       _inputCB( nullptr ),
                       _sendFunc( nullptr ) {}
        ~MsgHandler(){}

        int8_t Send( const std::string & output ) const;
        int8_t HandleInput( const std::string & input ) const;

        void SetName( const std::string name ) { _name = name; }
        void SetInputCB( const std::function<void( const Json::Value & )> & inputCB ) { _inputCB = inputCB; }
        void SetSendFunc( const std::function<void( Json::Value & )> & sendFunc ) { _sendFunc = sendFunc; }
        Json::Value Parse( const std::string & input ) const;

    private:
        int8_t Eval( const Json::Value & input ) const;
        MsgHandler( const MsgHandler & other )=delete;
        MsgHandler & operator=( const MsgHandler & )=delete;
        MsgHandler( const MsgHandler && other )=delete;
        MsgHandler & operator=( const MsgHandler && )=delete;

        std::string _name;
        std::function<void( const Json::Value & )> _inputCB;
        std::function<void( Json::Value & )> _sendFunc;
    };
}; //network
}; //drifter