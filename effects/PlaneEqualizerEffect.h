#ifndef PLANEEQUALIZEREFFECT_H
#define PLANEEQUALIZEREFFECT_H

#include "effect.h"
#include <gmParametricsModule>
#include <gmCoreModule>
#include <fmodex/fmod.hpp>
#include <iostream>

using namespace GMlib;

// I followed a guide to using FMOD from the following URL:
// http://katyscode.wordpress.com/2013/01/16/cutting-your-teeth-on-fmod-part-4-frequency-analysis-graphic-equalizer-beat-detection-and-bpm-estimation/

class PlaneEqualizerEffect : public Effect
{
public:
  virtual ~PlaneEqualizerEffect()
  {
    if(song_ != NULL)
      delete song_;
    if(system_ != NULL)
      delete system_;
  }

  virtual void init()
  {

    FMOD_RESULT result = FMOD::System_Create(&system_);
    system_->setOutput(FMOD_OUTPUTTYPE_DSOUND);
    system_->init(32, FMOD_INIT_NORMAL, 0);
    result = system_->createStream("skrillex.mp3", FMOD_SOFTWARE, 0, &song_);

    result = system_->playSound(FMOD_CHANNEL_FREE, song_, false, &channel_);

    specLeft_ = new float[SampleSize];
    specRight_ = new float[SampleSize];
    spec_ = new float[SampleSize];

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

    // Basically make it work on half of it and mirror the rest.
    const int half = c_.getDim1() / 2;
    for(int i = 0; i < half; i++)
    {
      spec_[i] = (specLeft_[i] + specRight_[i]) / 2;

      float curr = dt*1000*spec_[i];

      // We start in the middle and mirror the effect as if it was 2 planes set against each other.
      const int idx1 = i+half;
      const int idx2 = half-1-i;

      // Leaving the edge untouched
      for(int j = 1; j < c_.getDim2()-1; j++)
      {
        // The distance from last to curr pos tend to vary alot per frame, and it looks bad unless dampening it.
        float last = c_[j][idx1]->getPos()[1];
        float linpol = last + (curr - last)/8.0f;

        c_[j][idx1]->translate(Vector<float, 3>(0.0f, linpol - last, 0.0f));
        c_[j][idx2]->translate(Vector<float, 3>(0.0f, linpol - last, 0.0f));
        specAvg_[i] = 0.0f;
      }
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
  const int SampleSize = 128;
  int displayCounterLag = 0;
  float specAvg_[8];

  GMlib::Material MatLow_;
  GMlib::Material MatMed_;
  GMlib::Material MatHigh_;
};


#endif // PLANEEQUALIZEREFFECT_H
