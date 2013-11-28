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
    result = system_->createStream("veela - let it be.mp3", FMOD_SOFTWARE, 0, &song_);
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

    float max = 0;
    int loopno = 0;
    float avg = 0.0f;
    for(int i = 0; i < SampleSize; i++)
    {
      spec_[i] = (specLeft_[i] + specRight_[i]) / 2;

      if(i >= 8)
        break;

      float curr = 10*spec_[i];
      for(int j = 2; j < c_.getDim2()-2; j++)
      {
        c_[i][j]->setMaterial(curr > 0.2f ? MatHigh_ : MatMed_);
        c_[i][j]->translate(Vector<float, 3>(0.0f, curr-last[i], 0.0f));
      }
      last[i] = curr;

      qDebug() << curr;
//      avg += spec_[i];
//      if((i+1) % 8 == 0 && i > 0)
//      {
//        const int CurrIndex = (i / 8);
//        float curr = 10*(avg/8.0f);
//        c_[CurrIndex][4]->translate(Vector<float, 3>(0.0f, curr-last[CurrIndex], 0.0f));
//        last[CurrIndex] = curr;
//        avg = 0;
//      }

//      max = std::max(spec_[i], max);
//      ++loopno;
    }
//    qDebug() << "max: " << max;
//    qDebug() << "loop_ " << loopno;
//    max *= 10;

//    c_[3][3]->translate(Vector<float, 3>(0.0f, max-last, 0.0f));
//    c_[3][4]->translate(Vector<float, 3>(0.0f, max-last, 0.0f));
//    c_[4][3]->translate(Vector<float, 3>(0.0f, max-last, 0.0f));
//    c_[4][4]->translate(Vector<float, 3>(0.0f, max-last, 0.0f));

    //last = max;


    int a = 0;





  }

  virtual void end() {}

private:
  float last[8];
  FMOD::System* system_;
  FMOD::Sound* song_;
  FMOD::Channel* channel_;
  float* specLeft_;
  float* specRight_;
  float* spec_;
  const int SampleSize = 64;

  GMlib::Material MatLow_;
  GMlib::Material MatMed_;
  GMlib::Material MatHigh_;
};


#endif // PLANEEQUALIZEREFFECT_H
