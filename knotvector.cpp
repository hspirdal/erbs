//#include "knotvector.h"
//#include <QDebug>

//KnotVector::KnotVector(int numElements, float deltaSize, bool isClosed)
//{

//}

//void KnotVector::create(int numElements, float deltaSize)
//{


//  kv_.setDim(numElements+2);

//  for(int i = 1; i <= numElements; i++)
//  {
//    kv_[i] = (i - 1) * deltaSize / (numElements-1);
//  }

//  pad();
//}

//void KnotVector::pad()
//{
//  if(isClosed_)
//  {
//    kv_[0] = kv_[1] - (kv_[kv_.getDim()-2] - kv_[kv_.getDim()-3]);
//    kv_[kv_.getDim()-1] = kv_[kv_.getDim()-2] + kv_[2] - kv_[1];

//  }
//  else
//  {
//    kv_[0] = kv_[1];
//    kv_[kv_.getDim()-1] = kv_[kv_.getDim()-2];
//  }

//  qDebug() << "knotpad:";
//  for(int i = 0; i < kv_.getDim(); i++)
//    qDebug() << kv_[i];
//}

