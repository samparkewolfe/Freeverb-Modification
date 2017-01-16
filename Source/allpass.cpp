// Allpass filter implementation
//
// Written by Jezar at Dreampoint, June 2000
// http://www.dreampoint.co.uk
// This code is public domain

#include "allpass.hpp"

//All these functions are written by Jezar except set buffer size.
allpass::allpass()
{
	bufidx = 0;
}

//Allocating what buffer to edit.
void allpass::setbuffer(float *buf, int size)
{
	buffer = buf; 
	bufsize = size;
}

//Making all values in the buffer 0.
void allpass::mute()
{
	for (int i=0; i<bufsize; i++)
		buffer[i]=0;
}

void allpass::setfeedback(float val) 
{
	feedback = val;
}

float allpass::getfeedback() 
{
	return feedback;
}

void allpass::setbufsize(const float& val)
{
    bufsize = val;
}

//ends
