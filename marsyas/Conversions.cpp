/*
** Copyright (C) 1998-2005 George Tzanetakis <gtzan@cs.uvic.ca>
**  
** This program is free software; you can redistribute it and/or modify
** it under the terms of the GNU General Public License as published by
** the Free Software Foundation; either version 2 of the License, or
** (at your option) any later version.
** 
** This program is distributed in the hope that it will be useful,
** but WITHOUT ANY WARRANTY; without even the implied warranty of
** MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
** GNU General Public License for more details.
** 
** You should have received a copy of the GNU General Public License
** along with this program; if not, write to the Free Software 
** Foundation, Inc., 59 Temple Place - Suite 330, Boston, MA 02111-1307, USA.
*/

/** 
    \class Conversions
	\ingroup Notmar
    \brief Various conversion functions
*/

#include "Conversions.h"
#include "realvec.h"

using namespace std;
using namespace Marsyas;

mrs_real
Marsyas::pitch2hertz(mrs_real pitch) {
  return mrs_real(440.0 * pow(2.0, ((pitch - 69.0) / 12.0)));
}

mrs_real
Marsyas::hertz2pitch(mrs_real hz) {
  return (hz == 0.0) ? (mrs_real)0.0 : (mrs_real)(69.0 + 12.0 * (log(hz/440.0)/log(2.0)));
}

mrs_real
Marsyas::samples2hertz(mrs_natural samples, mrs_real srate) {
    return (samples == 0.0)  ? (mrs_real)0.0 : (mrs_real) (srate * 1.0) / (samples);
}

mrs_real
Marsyas::samples2hertz(mrs_real samples, mrs_real srate) {
    return (samples == 0.0)  ? (mrs_real)0.0 : (mrs_real) (srate * 1.0) / (samples);
}


mrs_natural
Marsyas::hertz2samples(mrs_real hz, mrs_real srate) {
  return (hz == 0.0) ? (mrs_natural)0 : (mrs_natural) (srate / hz);
}

/* convert a string representing time to number of samples base on the 
   given sample rate. Format "123.456#" where # is the time division.
   Valid time divisions: { h, m, s, ms, us }.
   On a format error, 
   Errors: -1 is returned. ie more than 1 decimal point, invalid time
     division.
*/
mrs_natural
Marsyas::time2samples(string time, mrs_real srate) {
//example times: { "10us", "10ms", "10s", "10m", "10h" }
    if (time=="") { return 0; }
    // calculate time value
    mrs_real samples=0;
    int i=0;
    int len=(int)time.length();
    bool decimal_point=false;
    mrs_real divisor = 10.0;
    for (i=0;i<len && (time[i]=='.' || (time[i]>='0' && time[i]<='9'));i++) {
        if (decimal_point) {
            if (time[i]=='.') { return -1; }
            samples = samples + ((mrs_real)(time[i]-'0'))/divisor;
            divisor = divisor * 10.0;
        } else if (time[i]=='.') {
            decimal_point=true;
        } else {
            samples = samples * 10.0 + (time[i]-'0');
        }
    }
    //
    if (i<len) {
        char a=time[i++];
        if (i>=len) {
            if (a=='h') { // hours
                samples= 120.0*samples*srate;
            } else if (a=='m') { // minutes
                samples=  60.0*samples*srate;
            } else if (a=='s') { // seconds
                samples=       samples*srate;
            } else {
                return -1;
            }
        } else {
            char b=time[i];
            if ((i+1)>=len) {
                if (a=='u' && b=='s') { // micro-seconds
                    samples= samples/1000000.0*srate;
                } else if (a=='m' && b=='s') { // milli-seconds
                    samples= samples/1000.0*srate;
                } else {
                    return -1;
                }
            }
        }
    }
    return (mrs_natural)samples;
}
mrs_natural
Marsyas::time2usecs(string time) {
//example times: { "10us", "10ms", "10s", "10m", "10h" }
    if (time=="") { return 0; }
    // calculate time value
    mrs_real usecs=0;
    int i=0;
    int len=(int)time.length();
    bool decimal_point=false;
    mrs_real divisor = 10.0;
    for (i=0;i<len && (time[i]=='.' || (time[i]>='0' && time[i]<='9'));i++) {
        if (decimal_point) {
            if (time[i]=='.') { return -1; }
            usecs = usecs + ((mrs_real)(time[i]-'0'))/divisor;
            divisor = divisor * 10.0;
        } else if (time[i]=='.') {
            decimal_point=true;
        } else {
            usecs = usecs * 10.0 + (time[i]-'0');
        }
    }
    //
    if (i<len) {
        char a=time[i++];
        if (i>=len) {
            if (a=='h') { // hours
                usecs *= 1000.0 * 1000.0 * 60.0 * 60.0;
            } else if (a=='m') { // minutes
                usecs *= 1000.0 * 1000.0 * 60.0;
            } else if (a=='s') { // seconds
                usecs *= 1000.0 * 1000.0;
            } else {
                return -1;
            }
        } else {
            char b=time[i];
            if ((i+1)>=len) {
                if (a=='u' && b=='s') { // micro-seconds
                    ;
                } else if (a=='m' && b=='s') { // milli-seconds
                    usecs *= 1000.0;
                } else {
                    return -1;
                }
            }
        }
    }
    return (mrs_natural)usecs;
}

mrs_real Marsyas::amplitude2dB(mrs_real a)
{
return 20*log10(a);
}

mrs_real Marsyas::dB2amplitude(mrs_real a)
{
return pow(10.0, a/20);
}


mrs_real Marsyas::hertz2bark(mrs_real f)
{
return  6 * log(f/600 + sqrt(1+ (pow(f/600,2)))); // 6*asinh(f/600);
}

mrs_real Marsyas::bark2hertz(mrs_real f)
{
return 600*sinh(f/6);
}

mrs_natural Marsyas::powerOfTwo(mrs_real v)
{
	mrs_natural n=1, res=0;
	while(res < v)
	{
		res = (mrs_natural) pow(2.0, n+.0);
		n++;
	}
	return res;
}

void
Marsyas::string2parameters(string s, realvec &v, char d)
{
	mrs_natural i =0, pos=0, newPos=0;
	string tmp;
	while(newPos != -1 )
	{
		newPos = (mrs_natural) s.find_first_of(&d, pos, 1);
		tmp = s.substr(pos, newPos);
		v(i++) = atof(tmp.c_str());
		pos = newPos+1;
	}
}
