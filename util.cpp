#include "util.h"
#include <iostream>


Util::Util()
{
}

void Util::writeDebugValues(GMlib::DVector<float> v, const std::string& msg)
{
  std::cout << msg << ": Dim " << v.getDim() << " {";
  for(int i = 0; i < v.getDim(); i++)
    if(i == 0) std::cout << v[i];
    else std::cout << ", " << v[i];

  std::cout << "}" << std::endl;
}
