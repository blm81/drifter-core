#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "dftrTcpClient.h"

class Drifter : public ci::app::App
{
public:
    void setup() override;
    void mouseDown( ci::app::MouseEvent event ) override;
    void update() override;
    void draw() override;
private:
    drifter::network::TCPClient         _tcpClient;
};

void Drifter::setup()
{
    using namespace ci;
    using namespace ci::app;

    //connect message parser to message handler
    std::function<void( const Json::Value & )> msgCB = [&]( const Json::Value & jsonVal ) {
        std::cout << "got msg: " << jsonVal << std::endl;
    };
    drifter::network::MsgHandlerRef msgHandlerRef = std::make_shared<drifter::network::MsgHandler>();
    msgHandlerRef->SetInputCB( [&]( const Json::Value & jsonVal ) {
        std::cout << "got message in main class: " << jsonVal.toStyledString() << std::endl;
    });
    msgHandlerRef->SetSendFunc( [&]( Json::Value & jsonVal ) {
       _tcpClient.Send( jsonVal );
    });
    _tcpClient.SetMsgHandler( msgHandlerRef );
    std::cout << "connecting..." << std::endl;
    _tcpClient.Connect( "localhost", (uint16_t)8888 );
}

void Drifter::mouseDown( ci::app::MouseEvent event )
{}

void Drifter::update()
{
    _tcpClient.Update();
}

void Drifter::draw()
{
    using namespace ci;
}

CINDER_APP( Drifter, ci::app::RendererGl(), [&]( ci::app::App::Settings * settings ) {
    settings->setWindowSize( 800, 800 );
})