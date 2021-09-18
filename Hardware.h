
#ifndef PZI_HARDWARE_H
#define PZI_HARDWARE_H


using namespace cv;

class Hardware {
public:
    Mat currentIMG;
    Hardware();

    void openCamara();

    void turnIRLedOnd();

    void sendSerialSignal();
};


#endif //PZI_HARDWARE_H
