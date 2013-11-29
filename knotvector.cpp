#include "knotvector.h"
#include <QDebug>

KnotVector::KnotVector()
  : size_(0), deltaSize_(0), isClosed_(false) {}

KnotVector::KnotVector(int size, float deltaSize, float start, bool isClosed)
  : size_(size), deltaSize_(deltaSize), start_(start), isClosed_(isClosed)
{
  create(size_, deltaSize_, start);
  pad(isClosed_);
}



void KnotVector::create(int size, float deltaSize, float start)
{
  kv_.setDim(size+2);

  for(int i = 1; i <= size; i++)
  {
    kv_[i] = start + (i - 1) * deltaSize / (size-1);
  }
  qDebug() << "knotcreate:";
  for(int i = 0; i < kv_.getDim(); i++)
    qDebug() << kv_[i];
}

void KnotVector::pad(bool isClosed)
{
  if(isClosed)
  {
    kv_[0] = kv_[1] - (kv_[kv_.getDim()-2] - kv_[kv_.getDim()-3]);
    kv_[kv_.getDim()-1] = kv_[kv_.getDim()-2] + kv_[2] - kv_[1];

  }
  else
  {
    kv_[0] = kv_[1];
    kv_[kv_.getDim()-1] = kv_[kv_.getDim()-2];
  }
  qDebug() << "knotpad:";
  for(int i = 0; i < kv_.getDim(); i++)
    qDebug() << kv_[i];
}
