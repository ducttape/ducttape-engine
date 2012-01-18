#ifndef FPS_H
#define FPS_H
#pragma once

#include <Scene/State.hpp>

class Main : public dt::State {
    Q_OBJECT
public:
    void OnInitialize();
    void UpdateStateFrame(double simulation_frame_time);
};

#endif