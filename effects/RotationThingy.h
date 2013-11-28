#ifndef ROTATIONTHINGY_H
#define ROTATIONTHINGY_H

#include "effect.h"
#include <QDebug>
#include "../Constants.h"

class RotationThingy : public Effect
{
public:
  RotationThingy() { }
  //virtual ~RotationThingy() {}

  virtual void update(double dt)
  {
    if(stop) return;

    double INC = BaseInc * dt;
    rad_ +=INC;

    if(rad_ > 2.0)
    {
      stop; return;
    }

    c_[0][0]->rotate(INC, GMlib::Vector<float,3>(0, 1, 0));
    c_[2][0]->rotate(-INC, GMlib::Vector<float,3>(1, 0, 0));
    c_[4][0]->rotate(-INC, GMlib::Vector<float,3>(0, 1, 0));
    c_[6][0]->rotate(INC, GMlib::Vector<float,3>(1, 0, 0));

    qDebug() << rad_;
  }

private:
  double rad_ = 0.0;
  int counter_= 0;
  double BaseInc = 0.7;
  int i = 0;
  int j = 0;
  bool stop = false;


};

#endif // ROTATIONTHINGY_H
