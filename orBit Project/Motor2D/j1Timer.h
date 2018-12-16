#ifndef __j1TIMER_H__
#define __j1TIMER_H__

#include "p2Defs.h"

class j1Timer
{
public:

	// Constructor
	j1Timer();

	void Start();
	uint32 Read() const;
	float ReadSec() const;
	 // getters
	uint32 startreturn();

	void LoadXtime(uint32 loaded);
	void ResetX();

private:
	uint32	started_at;
	uint32	loadX =0;

};

#endif //__j1TIMER_H__