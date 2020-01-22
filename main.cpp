#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "cinder/gl/gl.h"
#include "dftrTcpClient.h"
#include "generative/dftrHabitat.h"

//TODO remove the below, they are here for expedient testing of drifter::algorithm::GetNeighborsByLayer
#include "generative/dftrFauna.h"
#include "algorithm/dftrGraphTraversal.h"

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
    using namespace drifter::generative;
    using namespace drifter::algorithm;

    //TODO move the below into a unit test in the algorithm directory
    _habitat = std::make_unique<Habitat>( 7, 7 );
    std::vector<std::pair<float, float>> positions = {
        { 2, 4 },
        { 1, 3 },
        { 0, 2 },
        { 5, 6 }
    };
    _habitat->Initialize( positions );
    std::vector<std::vector<std::shared_ptr<HabitatTile>>> neighbors =
    GetNeighborsByLayer<std::shared_ptr<HabitatTile>>( _habitat->FaunaLocs(), { 3, 3 }, 3, []( std::shared_ptr<HabitatTile> tile )-> bool {
        if ( tile == nullptr ) return false;
        return !tile->IsEmpty();
    });
    ci::vec2 outPos1 = _habitat->FaunaRefMap().find( neighbors[0][0]->Resident() )->second->Position();
    ci::vec2 outPos2 = _habitat->FaunaRefMap().find( neighbors[1][0]->Resident() )->second->Position();
    ci::vec2 outPos3 = _habitat->FaunaRefMap().find( neighbors[2][0]->Resident() )->second->Position();
    ci::vec2 outPos4 = _habitat->FaunaRefMap().find( neighbors[2][1]->Resident() )->second->Position();
    if ( outPos1.x != positions[0].first || outPos1.y != positions[0].second ) {
        std::cout << "test 1 failed" << std::endl;
    }
    if ( outPos2.x != positions[1].first || outPos2.y != positions[1].second ) {
        std::cout << "test 2 failed" << std::endl;
    }
    if ( outPos3.x != positions[2].first || outPos3.y != positions[2].second ) {
        std::cout << "test 3 failed" << std::endl;
    }
    if ( outPos4.x != positions[3].first || outPos4.y != positions[3].second ) {
        std::cout << "test 4 failed" << std::endl;
    }
    std::cout << "all GetNeighborsByLayer tests passed" << std::endl;
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
    _habitat->Draw();
}

CINDER_APP( Drifter, ci::app::RendererGl(), [&]( ci::app::App::Settings * settings ) {
    settings->setWindowSize( 800, 800 );
})