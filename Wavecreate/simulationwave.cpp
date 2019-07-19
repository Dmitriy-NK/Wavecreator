#include "simulationwave.h"
//
std::mutex mtx;

SimulationWave::SimulationWave(class Wave* wave)
{
    this->Wave=wave;
}

//The function starts a wave distortion simulation. Upon completion, puts in the "frametime" variable the average value of the frame duration in milliseconds
bool SimulationWave::StartSimulation(std::string filepath,float& avframetime,float& maxframe){
        int c;          //Variable to store the value of the pressed button

        cv::Mat frame =  cv::imread(filepath, cv::IMREAD_COLOR);   // Original image

        if (!frame.data) {
          //  std::cout << "Error: the image wasn't correctly loaded." << std::endl;
            return 1;
        }
        cv::Mat frameDst;  // Distorted image
        cv::namedWindow("Distorted Image", cv::WINDOW_NORMAL);
        //cv::imshow("Distorted Image", frame);
        auto b_time=std::chrono::high_resolution_clock::now();          //time of the start calculations and image output for current iteration
        auto e_time=std::chrono::high_resolution_clock::now();          //time of the finish calculations and image output for current iteration
        std::chrono::duration<float> dtime=b_time-e_time;               //duration of the calculations and image output for current iteration
        double fulltime=0;                                              //Variable to store the total duration of the calculations and image output
        int counter = 0;                                                //counter of iterations
        int alpha = 45;                                                 //Set the direction of propagation of the initial wave

        while (true)
        {

            Wave->SetWave(frame,alpha);          //Set wave parameters for current alpha

            while (true) {
                b_time=std::chrono::high_resolution_clock::now();//start frame time

                this->blendWaveAndImage(frame, frameDst);		//overlay distortion mask on the "frame"
                // e_time=std::chrono::high_resolution_clock::now();
                //dtime=e_time-b_time;
                cv::imshow("Distorted Image", frameDst);		//Drawing distorted image to window


                std::string str= std::to_string(int(dtime.count()*1000));
                str="Press ESC to exit                   "+str+"ms     angle = "+std::to_string(alpha);
                cv::displayOverlay("Distorted Image", str, 0);          //drawing display overlay with aditional information

                c = cv::waitKey(10) & 255;      //button press checked
                if (c == 27 ||c==32)
                    break;

                e_time=std::chrono::high_resolution_clock::now();       //end frame time

                dtime=e_time-b_time;                 //frame duration
                fulltime+=dtime.count();
                counter++;
                maxframe = maxframe>dtime.count()?maxframe:dtime.count();

               if(!Wave->WaveUpdate(dtime.count()))  //wave moving
                     break;

            }

             if (c == 27)   //ESC button press checked
                break;
            alpha = std::rand() % 360;               //set the direction of wave propagation
        }
        maxframe *=1000;                             //return of the maximum frame duration
        avframetime=(fulltime*1000)/counter;           //return of the average frame duration

         cv::destroyWindow("Distorted Image");

        return 0;
}
//The function of overlay distortion mask on the "sourceImage". The result is transferred to the "targetImage".
void SimulationWave::blendWaveAndImage(cv::Mat& sourceImage, cv::Mat& targetImage){
            targetImage = sourceImage.clone();
            blendWaveImage(sourceImage, targetImage,Wave);
}
//Is called when the wave propagates closer to the horizontal direction.The function is responsible for overlay distortion mask on a part of the "sourceImage", "Nthread" determines on which part of the image the mask will be applied.
void BlendByRow(int &Nthread,cv::Mat& sourceImage, cv::Mat& targetImage,class Wave* Wave){
    int beginRow, endRow ;
mtx.lock();
     beginRow = Nthread*sourceImage.rows/Wave->ThreadsCount;
     endRow = (++Nthread)*sourceImage.rows/Wave->ThreadsCount;
mtx.unlock();
    float waveboundlow, waveboundup;            //wave bound for current row


        float sinalpha = Wave->GetSinAlpha();
        float cosalpha = Wave->GetCosAlpha();
        for (int i = beginRow; i < endRow; i++) {
            float WaveRowPosition = Wave->ZeroPosition - i * sinalpha / cosalpha;
            waveboundlow = WaveRowPosition - abs(Wave->Width / (2*cosalpha));				//Границы волны для данной строки
            waveboundup =  WaveRowPosition + abs(Wave->Width / (2*cosalpha));


            for (int j = 0; j < sourceImage.cols ; j++) {

                if ( j < waveboundup-1 && j > waveboundlow+1)       //Check whether current pixel belongs to the wave
                {
                    float radius= WaveRowPosition - j;				//distance to the wave crest in the current row
                    int Irad = abs(int(radius));					//Round distance to the wave crest in the current
                    float delta = radius/abs(radius) * (  Wave->DstMask[Irad] * (1 - abs(radius) + Irad)   +   Wave->DstMask[Irad+1] * (abs(radius) - Irad ));	//Extrapolation offset
                    if (cosalpha < 0) {
                        delta *= -1.0f;
                    }

                    float deltaRow = delta*sinalpha;                 //Offset in rows
                    float deltaCol = delta*cosalpha;                 //Offset in columns
                    char RowSign = deltaRow / abs(deltaRow);           //Signs of offsets
                    char ColSign = deltaCol / abs(deltaCol);

                    float Rch=0.f, Gch=0.f, Bch=0.f;				//Channels of the current pixel Red, Green, Blue
                    int IdeltaRow = abs(int(deltaRow)), IdeltaCol = abs(int(deltaCol));			//Round absolutly offset in rows and in columns
            // Calculation of the intensity values of the channels Red, Green, Blue of the current pixel
                    for (int k = 0; k < 4; k++) {
                        // Check whether displacement fits the image size
                        if ((i + RowSign*( IdeltaRow + 1  - k % 2)) < sourceImage.rows && (j + ColSign*(IdeltaCol + 1 - k / 2)) < sourceImage.cols && (i + RowSign*(IdeltaRow + 1 - k % 2)) >= 0 && (j + ColSign*(IdeltaCol + 1 - k / 2)) >= 0)
                        {
                            Bch += sourceImage.ptr(i + RowSign*(IdeltaRow + 1 - k % 2), j + ColSign*(IdeltaCol + 1 - k / 2))[0] * abs(abs(deltaRow - RowSign*IdeltaRow ) - k % 2)  *   abs(abs(deltaCol - ColSign*IdeltaCol ) - k / 2);
                            Gch += sourceImage.ptr(i + RowSign*(IdeltaRow + 1 - k % 2), j + ColSign*(IdeltaCol + 1 - k / 2))[1] * abs(abs(deltaRow - RowSign*IdeltaRow ) - k % 2)  *   abs(abs(deltaCol - ColSign*IdeltaCol ) - k / 2);
                            Rch += sourceImage.ptr(i + RowSign*(IdeltaRow + 1 - k % 2), j + ColSign*(IdeltaCol + 1 - k / 2))[2] * abs(abs(deltaRow - RowSign*IdeltaRow ) - k % 2)  *   abs(abs(deltaCol - ColSign*IdeltaCol ) - k / 2);
                        }
                    }
                    cv::Vec3b Intensity;
                    Intensity[0] = uchar(Bch);
                    Intensity[1] = uchar(Gch);
                    Intensity[2] = uchar(Rch);
                    //Write new values of channels to the pixel
                    targetImage.at<cv::Vec3b>(i, j) = Intensity;
                }


            }
        }
}
//Is called when the wave propagates closer to the vertical direction.The function is responsible for overlay distortion mask on a part of the  "sourceImage", "Nthread" determines on which part of the image the mask will be applied.
void BlendByCol(int &Nthread,cv::Mat& sourceImage, cv::Mat& targetImage,class Wave* Wave){
    int beginCol,endCol;
    mtx.lock();
            beginCol = Nthread* sourceImage.cols/Wave->ThreadsCount;
            endCol = (++Nthread)* sourceImage.cols/Wave->ThreadsCount;
     mtx.unlock();
    float waveboundlow, waveboundup;        //wave bound for current column

       float sinalpha = Wave->GetSinAlpha();
       float cosalpha = Wave->GetCosAlpha();
       //applying the distortion mask for pixel with (i,j)
        for (int j = beginCol; j < endCol; j++) {
            float WaveColPosition = Wave->ZeroPosition - j * cosalpha / sinalpha;
            waveboundlow = WaveColPosition - abs(Wave->Width / (2 * sinalpha));
            waveboundup = WaveColPosition + abs(Wave->Width / (2 * sinalpha));

            for (int i = 0; i < sourceImage.rows; i++) {

                if (i < waveboundup-1 && i > waveboundlow+1)        //Check whether current pixel belongs to the wave
                {
                    float radius = WaveColPosition - i;             //distance to the wave crest in the current column
                    int Irad = abs(int(radius));					//Round distance to the wave crest in the current column
                    float delta = radius / abs(radius) * (Wave->DstMask[Irad] * (1 - abs(radius) + Irad) + Wave->DstMask[Irad + 1] * (abs(radius) - Irad));	//Extrapolation offset
                    float deltaRow = delta*sinalpha;				//Offset in rows
                    float deltaCol = delta*cosalpha;				//Offset in columns
                    char RowSign = deltaRow / abs(deltaRow);
                    char ColSign = deltaCol / abs(deltaCol);
                    float Rch = 0.f, Gch = 0.f, Bch = 0.f;					//Channels of the current pixel Red, Green, Blue
                    int IdeltaRow = abs(int(deltaRow)), IdeltaCol = abs(int(deltaCol));			//Round absolutly offset in rows and in columns
            // Calculation of the intensity values of the channels Red, Green, Blue of the current pixel
                    for (int k = 0; k < 4; k++) {
                        // Check whether displacement fits the image size
                        if ((i + RowSign*(IdeltaRow + 1 - k % 2)) < sourceImage.rows && (j + ColSign*(IdeltaCol + 1 - k / 2)) < sourceImage.cols && (i + RowSign*(IdeltaRow + 1 - k % 2)) >= 0 && (j + ColSign*(IdeltaCol + 1 - k / 2)) >= 0)
                        {
                            Bch += sourceImage.ptr(i + RowSign*(IdeltaRow + 1 - k % 2), j + ColSign*(IdeltaCol + 1 - k / 2))[0] * abs(abs(deltaRow - RowSign*IdeltaRow) - k % 2)  *   abs(abs(deltaCol - ColSign*IdeltaCol) - k / 2);
                            Gch += sourceImage.ptr(i + RowSign*(IdeltaRow + 1 - k % 2), j + ColSign*(IdeltaCol + 1 - k / 2))[1] * abs(abs(deltaRow - RowSign*IdeltaRow) - k % 2)  *   abs(abs(deltaCol - ColSign*IdeltaCol) - k / 2);
                            Rch += sourceImage.ptr(i + RowSign*(IdeltaRow + 1 - k % 2), j + ColSign*(IdeltaCol + 1 - k / 2))[2] * abs(abs(deltaRow - RowSign*IdeltaRow) - k % 2)  *   abs(abs(deltaCol - ColSign*IdeltaCol) - k / 2);
                        }
                    }
                    cv::Vec3b Intensity;
                    Intensity[0] = uchar(Bch);
                    Intensity[1] = uchar(Gch);
                    Intensity[2] = uchar(Rch);
                    //Write new values of channels to the pixel
                    targetImage.at<cv::Vec3b>(i, j) = Intensity;
                }

            }
        }
}
//Using distortion mask from "Wave" variable and overlay it on the "sourceImage". The result is transferred to the "targetImage".
void  blendWaveImage(cv::Mat& sourceImage, cv::Mat& targetImage,class Wave* Wave){

    if (abs(Wave->GetCosAlpha()) >= Wave->GetSinAlpha())
    {
        int Ntr=1;
        //creating and running an array of threads
        std::thread* Th= new std::thread[Wave->ThreadsCount];

        for(int i = 1; i < Wave->ThreadsCount;){
            Th[i]=std::thread([&](){BlendByRow(Ntr,sourceImage,targetImage,Wave);});
            i++;
        }
        int ntr=0;
        Th[0]=std::thread([&](){BlendByRow(ntr,sourceImage,targetImage,Wave);});


        for(int i = 0;i < Wave->ThreadsCount;){
            if(Th[i].joinable())
                Th[i].join();
            i++;
        }
    } else{
        int Ntr=1;
        //creating and running an array of threads
        std::thread* Th= new std::thread[Wave->ThreadsCount];

        for(int i = 1; i < Wave->ThreadsCount;){
            Th[i]=std::thread([&](){BlendByCol(Ntr,sourceImage,targetImage,Wave);});
             i++;
        }
         int ntr=0;
             Th[0]=std::thread([&](){BlendByCol(ntr,sourceImage,targetImage,Wave);});


          for(int i=0;i<Wave->ThreadsCount;){
              if(Th[i].joinable())
                    Th[i].join();
            i++;
          }
    }

}


