#ifndef WAVE_H
#define WAVE_H
#pragma once
#define  PI 3.1415926f
#include <cmath>
#include "opencv2/core/core.hpp"
#include "opencv2/highgui/highgui.hpp"
#include "opencv2/imgproc/imgproc.hpp"



class Wave
{
    //wave amlitude
    int Amplitude;
    //Wave propogation speed
    int Speed;
    //The projection of the speed of propagation of the wave
    float SpeedProjection;
    //boundary value of wave position
    float FinishWavePosition;
    //Distortion mask calculation
    void MakeDistortionMask();
    //Calculation of the location of the crest of the wave at the initial moment
    void SetZeroPosition(cv::Mat& frame,int Direct);
    //Returns the height of the wave at a distance "x" from the crest of the wave
    float waveFunction(float x);
    //Returns the  tangent value of the wave at a distance of the "x" from the crest of the wave
    float waveTangentFunction(float x);
public:
    //Wave width
   int Width; 
   //number of threads for calculations
   int ThreadsCount;
   //Pointer to the distortion mask
   float* DstMask;
   //Location of the crest of the wave
   float ZeroPosition;
   //The angle between the horizon and the direction of wave propagation
   int Alpha;
   //constructor
   Wave(int wavewidth,int waveamplitude,int wavespeed, int threadscount);
   //Destructor
   ~Wave();
   //Returns the cosine of the angle of wave propagation
   float GetCosAlpha();
   //Returns the sine of the angle of wave propagation.
   float GetSinAlpha();
   //the function sets the wave parameters for the image and the propagation angle
   void SetWave(cv::Mat& frame,int alpha);
   //Updates the current position of the wave and checks whether the wave is within the image.
   bool WaveUpdate(float dtime);
};

#endif // WAVE_H
