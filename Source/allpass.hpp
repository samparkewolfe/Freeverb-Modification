// Allpass filter declaration
//
// Written by Jezar at Dreampoint, June 2000
// http://www.dreampoint.co.uk
// This code is public domain

#ifndef _allpass_
#define _allpass_
#include "denormals.h"

//Very similar to the comb filter the only difference is the process function
//This class was largely written by Jezar but has been modified by me to make the tuning of the allpass filter variable.
class allpass
{
public:
					allpass();
			void	setbuffer(float *buf, int size);
	inline  float	process(float inp);
			void	mute();
			void	setfeedback(float val);
			float	getfeedback();
    
    //This was my addition to the class:
    void setbufsize(const float& val);

// private:
	float	feedback;
	float	*buffer;
	int		bufsize;
	int		bufidx;
};

//Jezar's comment:
// Big to inline - but crucial for speed
inline float allpass::process(float input)
{
    //We check this at the begining of the loop incase a change in buffersize made bufidx go out of bounds.
    //Originally bufsize was a constant value that did not change.
    if(bufidx>=bufsize) bufidx = 0;
    
	float output;
	float bufout;
	
    //Locally store the current value in our filter at this point.
    bufout = buffer[bufidx];
    
    //Do not know what this does (Jezar's functionality)
    undenormalise(bufout);
	
    //Output is the new input inverted and combined with the current value in our filter.
	output = -input + bufout;
    
    //Update the current value in our filter by combining the input with the current value in our filter multiplied by a feedback variable.
	buffer[bufidx] = input + (bufout*feedback);

    //Incrememnt the buffer index
    bufidx++;
    
    //Output the pre-modified value in our comb filter.
    return output;
}

#endif//_allpass

//ends
