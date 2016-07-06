#ifndef pager_header
#define pager_header

#include "frame.h"

#ifndef nullptr
#define nullptr __null
#endif

class Pager {
	virtual Frame * allocate_frame(); 
};

#endif