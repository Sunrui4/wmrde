//utility for converting between rotation representations

#ifndef _WMRDE_ROTATION_H_
#define _WMRDE_ROTATION_H_

#include <wmrde/algebra/linalg3.h>
#include <Eigen/Geometry>

namespace wmrde
{
typedef Eigen::Quaternion<Real> Quaternion;
typedef Eigen::AngleAxis<Real> AngleAxis;

inline Real radToDeg(Real rad) { return rad*180.0/M_PI; }
inline Real degToRad(Real deg) { return deg*M_PI/180.0; }

//functions to compute rotation matrices from Euler angles

inline Mat3 Rotx(const Real angle)
{
  Real ca = cos(angle);
  Real sa = sin(angle);
  Mat3 out;
  out << 1, 0,   0,
         0, ca,-sa,
         0, sa, ca;
  return out;
}

inline Mat3 Roty(const Real angle)
{
  Real ca = cos(angle);
  Real sa = sin(angle);
  Mat3 out;
  out << ca, 0, sa,
         0,  1, 0,
        -sa, 0, ca;
  return out;
}

inline Mat3 Rotz(const Real angle)
{
  Real ca = cos(angle);
  Real sa = sin(angle);
  Mat3 out;
  out << ca, -sa, 0,
         sa,  ca, 0,
         0,   0,  1;
  return out;
}

//R = Rotz(yaw)*Roty(pitch)*Rotx(roll)
inline Mat3 eulerToRot(const Real roll, const Real pitch, const Real yaw)
{
  Real sR=sin(roll);  Real cR=cos(roll);
  Real sP=sin(pitch); Real cP=cos(pitch);
  Real sY=sin(yaw);   Real cY=cos(yaw);
  Mat3 out;
  out << cP*cY, cY*sP*sR - cR*sY, sR*sY + cR*cY*sP,
         cP*sY, cR*cY + sP*sR*sY, cR*sP*sY - cY*sR,
        -sP,    cP*sR,            cP*cR;
  return out;
}

void rotToEuler(const Mat3 R,
    Real& roll, Real& pitch, Real& yaw) //output
{
  yaw = atan2(R(1,0), R(0,0));
  roll = atan2(R(2,1), R(2,2));
  Real sR=sin(roll);
  Real cR=cos(roll);
  Real cP;
  if (fabs(cR) > fabs(sR)) {
    cP = R(2,2)/cR; //singularity at rol = +/- pi/2
  } else {
    cP = R(2,1)/sR; //singularity at rol = 0,+/-pi
  }
  pitch = atan2(-R(2,0), cP); //pit
}

//TODO, check this
//returns a differential quaternion from angular velocity and time step
inline Quaternion diffQuatFromAngularVel(
    const Vec3& angular_vel,
    const Real dt)
{
  Vec3 dr = angular_vel*dt; //differential rotation about each axis
  Real angle = dr.norm();
  return Quaternion(AngleAxis(angle, dr/angle));
}

} //namespace

#endif //_WMRDE_TRANSFORM_H_
