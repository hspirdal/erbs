#ifndef KNOTVECTOR_H
#define KNOTVECTOR_H

#include <gmCoreModule>

class KnotVector
{
public:
  KnotVector();
  KnotVector(int size, float deltaSize, float start, bool isClosed);

  float operator[](int i) { return kv_[i]; }

  void create(int size, float deltaSize, float start);
  void pad(bool isClosed);

  const GMlib::DVector<float>& data() const { return kv_; }
  int getDim() const { return kv_.getDim(); }

private:
  KnotVector(const KnotVector& copy); // nocopy


  GMlib::DVector<float> kv_;
  float deltaSize_;
  int size_;
  bool isClosed_;
  float start_;



};

#endif // KNOTVECTOR_H
