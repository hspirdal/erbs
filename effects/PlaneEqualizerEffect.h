#ifndef PLANEEQUALIZEREFFECT_H
#define PLANEEQUALIZEREFFECT_H

#include "effect.h"
#include <gmParametricsModule>
#include <gmCoreModule>
#include <fmodex/fmod.hpp>
#include <iostream>

using namespace GMlib;

// http://katyscode.wordpress.com/2013/01/16/cutting-your-teeth-on-fmod-part-4-frequency-analysis-graphic-equalizer-beat-detection-and-bpm-estimation/

class PlaneEqualizerEffect : public Effect
{
public:
//  virtual ~PlaneEqualizerEffect()
//  {
//    if(song_ != NULL)
//      delete song_;
//    if(system_ != NULL)
//      delete system_;
//  }

  virtual void init()
  {
    //c_[0][0]->translate(Vector<float, 3>(0.0f, 2.0f, 0.0f));

    FMOD_RESULT result = FMOD::System_Create(&system_);
    system_->setOutput(FMOD_OUTPUTTYPE_DSOUND);
    system_->init(32, FMOD_INIT_NORMAL, 0);
    result = system_->createStream("skrillex.mp3", FMOD_SOFTWARE, 0, &song_);
    //result = system_->createSound("The_Abyss-4T.ogg", FMOD_LOOP_NORMAL, NULL, &song_);

    result = system_->playSound(FMOD_CHANNEL_FREE, song_, false, &channel_);

    specLeft_ = new float[SampleSize];
    specRight_ = new float[SampleSize];
    spec_ = new float[SampleSize];

    //system_->update();
    if(!result == FMOD_OK)
    {
      qDebug() << "fmod not ok";
    }

    MatHigh_ = GMlib::GMmaterial::Emerald;
    MatMed_ = GMlib::GMmaterial::Gold;
    MatLow_ = GMlib::GMmaterial::Snow;
  }

  virtual void update(double dt)
  {
    system_->update();

    channel_->getSpectrum(specLeft_, SampleSize, 0, FMOD_DSP_FFT_WINDOW_RECT);
    channel_->getSpectrum(specRight_, SampleSize, 1, FMOD_DSP_FFT_WINDOW_RECT);

    displayCounterLag++;
    for(int i = 0; i < c_.getDim1(); i++)
    {
      spec_[i] = (specLeft_[i] + specRight_[i]) / 2;

      float curr = dt*1000*spec_[i];
//      if(displayCounterLag < 8)
//      {

//        specAvg_[i] = specAvg_[i] + curr;
//        qDebug() << specAvg_[i];
//        continue;
//      }

      for(int j = 2; j < c_.getDim2()-2; j++)
      {
//        specAvg_[i] = specAvg_[i] + curr;
//        specAvg_[i] /= 8.0f;
        //c_[i][j]->setMaterial(curr > 0.2f ? MatHigh_ : MatMed_);

        float last = c_[j][i]->getPos()[1];
        float linpol = last + (curr - last)/8.0f;

        c_[j][i]->translate(Vector<float, 3>(0.0f, linpol - last, 0.0f));
        specAvg_[i] = 0.0f;
      }

//    if(displayCounterLag > 10)
//      displayCounterLag = 0;

    }





  }

  virtual void end() {}

private:
  FMOD::System* system_;
  FMOD::Sound* song_;
  FMOD::Channel* channel_;
  float* specLeft_;
  float* specRight_;
  float* spec_;
  const int SampleSize = 64;
  int displayCounterLag = 0;
  float specAvg_[8];

  GMlib::Material MatLow_;
  GMlib::Material MatMed_;
  GMlib::Material MatHigh_;
};


#endif // PLANEEQUALIZEREFFECT_H
