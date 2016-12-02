/*
 * Vector2Scalar.cpp
 *
 *  Created on: Nov 9, 2016
 *      Author: Bernd Doser, HITS gGmbH <bernd.doser@h-its.org>
 */

#include "Vector2Scalar.h"

namespace fda {

std::ostream& operator << (std::ostream& os, Vector2Scalar r)
{
  switch(r) {
    case Vector2Scalar::NORM:
      return os << "norm";
    case Vector2Scalar::PROJECTION:
      return os << "projection";
    default:
      return os << "invalid";
  }
}

std::istream& operator >> (std::istream& is, Vector2Scalar& r)
{
  std::string s;
  is >> s;
  if (s == "norm")
	r = Vector2Scalar::NORM;
  else if (s == "projection")
	r = Vector2Scalar::PROJECTION;
  else
	r = Vector2Scalar::INVALID;
  return is;
}

} // namespace fda
