/**
 * incoming message handler, "business logic" goes here
 * set the callbacks with std::fuction enclosing logic specific to your application
 * and pass instance of this class to client
 *
 * by Blakely L McConnell  2015->~∞
 */

#include "dftrMsgHandler.h"
#include <iostream>

namespace drifter
{
namespace network
{
    /**
     * handles input raw json string
     * @param input the raw input json
     * @returns
     */
    int8_t MsgHandler::HandleInput( const std::string & input ) const
    {
        std::cout << "received message: " << input << std::endl;
        Json::Value inputJson = Parse( input );
        if ( !inputJson.isMember( "type" ) || inputJson["type"].isNull() ) {
            std::cout << "WARNING: incoming message could not be parsed: " << input << std::endl;
            return -1;
        }
        return Eval( inputJson );
    }


    /**
     * parse and evaluate incoming json messages
     * @param input stringified json
     * @returns parsed json, or default if parsing fails (type : null)
     */
    Json::Value MsgHandler::Parse( const std::string & input ) const
    {
        Json::Reader reader;
        Json::Value jsonIn;
        bool parsingSuccessful = reader.parse( input, jsonIn );
        if ( !parsingSuccessful )
        {
            std::cout  << "WARNING: Failed to parse configuration\n"
                       << reader.getFormattedErrorMessages() << std::endl;
            jsonIn["type"] = nullptr;
        }
        //cout << jsonIn << endl;
        return jsonIn;
    }

    /**
     * evaluate incoming JSON messages, write response to socket
     * @param input
     */
    int8_t MsgHandler::Eval( const Json::Value & input ) const
    {
        Json::Value jsonOut;
        //set default for json out type to filter invalid input
        jsonOut["type"] = "null";
        Json::StyledWriter writer;
        if ( input["type"].asString() == "output" ) { //message from within cinder
            jsonOut["type"] = "to_" + input["recipient"].asString();
            jsonOut["data"] = input["data"];
            jsonOut["id"] = input["id"];
        }
        //identification message
        else if ( input["type"].asString() == "request_ident" ) {
            jsonOut["type"] = "ident";
            jsonOut["data"] = _name;
        }
        //route message to susbscribers
        else {
            if ( _inputCB != nullptr ) {
                _inputCB( input );
            }
            else {
                std::cout << "WARNING: TCP client message handler is not set.  Incoming message cannot be parsed" << std::endl;
            }
        }
        //TODO what other cases would we need to do this?  Just ident? If so, combine with logic above
        if ( jsonOut["type"].asString() != "null" ) {
            if ( _sendFunc != nullptr ) {
                _sendFunc( jsonOut );
            }
            else {
                std::cout << "WARNING: message could not be send because TCP write function is null" << std::endl;
            }
        }
    }
} //network
} //drifter