#ifndef UTIL_H
#define UTIL_H

#include <gmCoreModule>
#include <string>

class Util
{
public:
  static void writeDebugValues(GMlib::DVector<float> v, const std::string& msg);


private:
  Util();
};

#endif // UTIL_H
