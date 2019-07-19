#include "wave.h"

Wave::Wave(int wavewidth,int waveamplitude,int wavespeed, int threadscount)
{
    this->Width=wavewidth;
    this->Amplitude=waveamplitude;
    this->Speed=wavespeed;
    this->ThreadsCount=threadscount;
    this->DstMask=nullptr;
}
//Destructor
Wave::~Wave(){
        delete [] DstMask;
}

//Returns the cosine of the angle of wave propagation
float Wave::GetCosAlpha(){
    return std::cos(PI*this->Alpha/180.f);
}
 //Returns the sine of the angle of wave propagation.
float Wave::GetSinAlpha(){
    return std::sin(PI*this->Alpha/180.f);
}
 //the function sets the wave parameters for the image and the propagation angle
void Wave::SetWave(cv::Mat& frame,int alpha){
    int Direct =1 - (alpha / 180) * 2;      //calculate direction of wave propogation
    this->Alpha =alpha%180;
    SetZeroPosition(frame,Direct);
    this->MakeDistortionMask();
}
//Calculation of the location of the crest of the wave at the initial moment
void Wave::SetZeroPosition(cv::Mat& frame,int Direct){
        float k=PI/180.f;
            if (Alpha <= 45) {
                this->ZeroPosition = (1 - Direct) / 2 * (frame.rows + frame.cols*std::tan(Alpha*k)) - Direct * Width / (2 * std::cos(Alpha*k));
                this->SpeedProjection = Direct*Speed* std::cos(Alpha*k);
                Direct*=-1;
                this->FinishWavePosition = (1 - Direct) / 2 * (frame.rows + frame.cols*std::tan(Alpha*k)) - Direct * Width / (2 * std::cos(Alpha*k));
            }else if(Alpha <= 90){
                this->ZeroPosition = (1 - Direct) / 2 * (frame.rows + frame.cols*std::cos(Alpha*k) / std::sin(Alpha*k)) - Direct * Width / (2 * std::sin(Alpha*k));
                this->SpeedProjection= Direct*Speed*std::sin (Alpha*k);
                Direct*=-1;
                this->FinishWavePosition = (1 - Direct) / 2 * (frame.rows + frame.cols*std::cos(Alpha*k) / std::sin(Alpha*k)) - Direct * Width / (2 * std::sin(Alpha*k));
            }else if (Alpha <= 135) {
                this->ZeroPosition = (1 + Direct) / 2 * (frame.cols* std::cos(Alpha*k) / std::sin(Alpha*k)) + (1 - Direct) / 2 * frame.rows - Direct * Width / (2 * std::sin(Alpha*k));
                this->SpeedProjection= Direct*Speed* std::sin(Alpha*k);
                Direct*=-1;
                this->FinishWavePosition = (1 + Direct) / 2 * (frame.cols* std::cos(Alpha*k) / std::sin(Alpha*k)) + (1 - Direct) / 2 * frame.rows - Direct * Width / (2 * std::sin(Alpha*k));
            }else {
                this->ZeroPosition = (1 + Direct) / 2 * frame.cols + (1 - Direct) / 2 *  frame.rows*std::tan(Alpha*k) - Direct * Width / (2 * std::cos(Alpha*k));
                this->SpeedProjection= Direct*Speed* std::cos(Alpha*k);
                Direct*=-1;
                this->FinishWavePosition = (1 + Direct) / 2 * frame.cols + (1 - Direct) / 2 *  frame.rows*std::tan(Alpha*k) - Direct * Width / (2 * std::cos(Alpha*k));
            }
}
//Distortion mask calculation
void  Wave::MakeDistortionMask(){
    if(DstMask!=nullptr){ delete [] DstMask;}

        const float rFactor = 1.33f;				 // refraction factor of water
        float cosalpha = abs(GetCosAlpha());
        float sinalpha = GetSinAlpha();
        float projection;                           //projection of mask
        int nLUT;
        if (cosalpha >= sinalpha)
        {
            nLUT = int(Width /(2* cosalpha));
            projection = cosalpha;
        }
        else {
            nLUT =int( Width /(2 * sinalpha));
            projection = sinalpha;
        }

        this->DstMask = new float[nLUT+1];
//filling the distortion mask
        for (int i = 0; i < nLUT+1; i++) {
            float x = i*projection;											//distance from the crest of the wave
            float tangentAngle = std::atan(waveTangentFunction(x));				//tangent angle value of the wave for current x

            float z = (1.0f + waveFunction(x))*Amplitude;				//the height of the wave for current x

            float beta = std::asin(std::sin(tangentAngle) / rFactor);									//ray deflection angle

            this->DstMask[i] = z * std::tan(tangentAngle - beta);								//Calculate the offset for current x
            }
}
//Returns the height of the wave at a distance "x" from the crest of the wave
float Wave::waveFunction(float x)
{
    return (std::cos(2 * PI * x / Width));
}
 //Returns the  tangent value of the wave at a distance of the "x" from the crest of the wave
float Wave::waveTangentFunction(float x)
{
    return (Amplitude*2*PI/Width*std::sin(2 * PI * x / Width));
}
//Updates the current position of the wave and checks whether the wave is within the image.
bool Wave::WaveUpdate(float dtime){
    float prev = ZeroPosition;
   ZeroPosition += SpeedProjection*dtime;
   if(abs(ZeroPosition-prev)>abs(FinishWavePosition-prev)){
       return false;
    }
   return true;
}
