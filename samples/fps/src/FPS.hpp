#ifndef DUCTTAPE_SAMPLE_FPS_FPS
#define DUCTTAPE_SAMPLE_FPS_FPS

#include <Scene/State.hpp>

class Main : public dt::State {
    Q_OBJECT
public:
    void onInitialize();
    void updateStateFrame(double simulation_frame_time);
};

#endif
