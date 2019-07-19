#ifndef SIMULATIONWAVE_H
#define SIMULATIONWAVE_H
#pragma once
#include "wave.h"
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include <mutex>
#include <thread>
#include <string>


class SimulationWave
{
private:
    //Variable to store wave parameters
    class Wave* Wave;
    //The function of overlay distortion mask on the "sourceImage". The result is transferred to the "targetImage".
    void blendWaveAndImage(cv::Mat& sourceImage, cv::Mat& targetImage);
public:
    //Сonstructor. Accepts a pointer to the wave object as an argument.
    SimulationWave(class Wave* wave);
    //The function starts a wave distortion simulation. Upon completion, puts in the "frametime" variable the average value of the frame duration in milliseconds
    bool StartSimulation(std::string filepath,float& frametime,float& maxframe);
};

//Is called when the wave propagates closer to the horizontal direction.The function is responsible for overlay distortion mask on a part of the image, "Nthread" determines on which part the mask will be applied.
void BlendByRow(int &Nthread,cv::Mat& sourceImage, cv::Mat& targetImage,class Wave* Wave);
//Is called when the wave propagates closer to the vertical direction.The function is responsible for overlay distortion mask on a part of the image, "Nthread" determines on which part the mask will be applied.
void BlendByCol(int &Nthread,cv::Mat& sourceImage, cv::Mat& targetImage,class Wave* Wave);
//Using distortion mask from "Wave" variable overlay it on the "sourceImage". The result is transferred to the "targetImage".
void  blendWaveImage(cv::Mat& sourceImage, cv::Mat& targetImage,class Wave* Wave);
#endif // SIMULATIONWAVE_H
