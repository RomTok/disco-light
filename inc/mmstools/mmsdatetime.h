#ifndef MMSDATETIME_H_
#define MMSDATETIME_H_

#include <sys/time.h>
#include "mmstools/base.h"

class MMSDateTime {
	private:
		string daystr;
		string monthstr;
		string yearstr;
		string dbdate;
		int    yearnum;
		int    daynum;
		int    monthnum;
		int    dayofweek;
		int    hour;
		int    minute;
		int    seconds;
		
		time_t timest;
	
	public:
		MMSDateTime();
		MMSDateTime(string timestr, string format = "YYYY-MM-DD hh:mm:ss");
		MMSDateTime(time_t stamp);
		~MMSDateTime();
		
		string &getDay();
		string &getMonth();
		string &getYear();
		string &getDbDate();
		int    getDayNum();
		int    getMonthNum();
		int    getYearNum();
		
	
};

#endif /*MMSDATETIME_H_*/

