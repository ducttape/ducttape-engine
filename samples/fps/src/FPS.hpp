#include <Scene/State.hpp>

class Main: public dt::State {
    Q_OBJECT
public:
    void OnInitialize();
    void UpdateStateFrame(double simulation_frame_time);

private:
};