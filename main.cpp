#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "dftrTcpClient.h"
#include "generative/dftrHabitat.h"

class Drifter : public ci::app::App
{
public:
    void setup() override;
    void mouseDown( ci::app::MouseEvent event ) override;
    void update() override;
    void draw() override;

    void ConnectTCPSocket(); //just for testing at this time
private:
    drifter::network::TCPClient         _tcpClient;
    std::unique_ptr<drifter::generative::Habitat>        _habitat;
};

void Drifter::ConnectTCPSocket()
{
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
    std::cout << "connecting to port 8888..." << std::endl;
    _tcpClient.Connect( "localhost", (uint16_t)8888 );
}

void Drifter::setup()
{
    using namespace ci;
    using namespace ci::app;
}

void Drifter::mouseDown( ci::app::MouseEvent event )
{}

void Drifter::update()
{}

void Drifter::draw()
{
    using namespace ci;
    using namespace ci::gl;
    using namespace drifter::generative;

    clear();
}

CINDER_APP( Drifter, ci::app::RendererGl(), [&]( ci::app::App::Settings * settings ) {
    settings->setWindowSize( 800, 800 );
})