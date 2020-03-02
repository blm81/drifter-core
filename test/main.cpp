/**
 * temporary test runner for Drifter
 */

#include "cinder/app/App.h"
#include "cinder/app/RendererGl.h"
#include "algorithm/tests/dftrGraphTraversalTests.h"
#include "generative/tests/dftrHabitatTests.h"

class DrifterTests : public ci::app::App
{
public:
    void setup() override;
    void update() override;
    void draw() override;

private:
    bool _testsRun;
};

void DrifterTests::setup()
{
    using namespace ci;
    using namespace ci::app;

    _testsRun = false;
}

void DrifterTests::update()
{
    using namespace ci;
    using namespace drifter::tests;

    if ( _testsRun ) return;
    if ( !GetNeighborsByLayerTest() ) {
        std::cout << "GetNeighborsByLayer failed" << std::endl;
    }
    if ( !PopulateTest() ) {
        std::cout << "Populate failed" << std::endl;
    }
    if ( !AdvanceHuntTest() ) {
        std::cout << "AdvanceHunt failed" << std::endl;
    }
    _testsRun = true;
}

void DrifterTests::draw()
{}

CINDER_APP( DrifterTests, ci::app::RendererGl(), [&]( ci::app::App::Settings * settings ) {
    settings->setBorderless();
    settings->setWindowSize( 0, 0 );
})