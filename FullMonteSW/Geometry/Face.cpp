#include <cassert>

#include "Face.hpp"

//// Intersects a ray with the face plane
//// NOTE: does not guarantee that the point is actually inside the face points
//pair<bool,Point<3,double> > Face::rayIntersectPoint(const Ray<3,double>& r,bool invert) const
//{
//	double costheta=-dot(r.getDirection(),normal);
//	double h=pointHeight(r.getOrigin());
//	if (signum(costheta)!=signum(h))
//		return make_pair(false,r.getOrigin());
//	return make_pair(true,r(h/costheta*(invert ? -1 : 1)));
//}
//
//// checks if a ray intersects the face plane within t units of travel
//// NOTE: does not guarantee that the point is actually inside the face points
//pair<bool,double> Face::rayIntersect(const Ray<3,double>& r,double t,bool invert) const
//{
//	double costheta=-dot(r.getDirection(),normal);
//	double h=pointHeight(r.getOrigin());
//	if(signum(costheta)!=signum(h) || (invert ^ (h >= t*costheta)))
//		return make_pair(false,t);
//	else
//		return make_pair(true,h/costheta);
//}
