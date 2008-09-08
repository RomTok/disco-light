#include "mmstools/mmsdatetime.h"
#include "mmstools/tools.h"

MMSDateTime::MMSDateTime() {
	MMSDateTime(time(NULL));
}

MMSDateTime::MMSDateTime(string timestr, string format) {
	struct tm mytime;
	if(format == "YYYY-MM-DD hh:mm:ss") {
		this->yearstr = timestr.substr(0,4);
		this->yearnum = strToInt(this->yearstr);
		this->monthnum = atoi(timestr.substr(5,2).c_str()) -1;
		this->daynum = atoi(timestr.substr(8,2).c_str());
		this->hour = atoi(timestr.substr(11,2).c_str());
		this->minute = atoi(timestr.substr(14,2).c_str());
		this->seconds = atoi(timestr.substr(17,2).c_str());
	}
	
	mytime.tm_mday = this->daynum;
	mytime.tm_mon = this->monthnum;
	mytime.tm_year = this->yearnum -1900;
	mytime.tm_hour = this->hour;
	mytime.tm_min = this->minute;
	mytime.tm_sec = this->seconds;
	
	this->timest = mktime(&mytime);
}

MMSDateTime::MMSDateTime(time_t stamp) {
	this->timest = stamp;
	struct tm mytime;
	
	
	localtime_r(&(this->timest),&mytime);
	this->daynum = mytime.tm_mday;
	this->monthnum = mytime.tm_mon;
	this->yearnum = 1900 + mytime.tm_year;
	this->dayofweek = mytime.tm_wday;
	this->hour = mytime.tm_hour;
	this->minute = mytime.tm_min;
	this->seconds = mytime.tm_sec;
	
}

MMSDateTime::~MMSDateTime() {
	
}

string &MMSDateTime::getDay() {
	return this->daystr;
}

string &MMSDateTime::getMonth() {
	return this->monthstr;
}

string &MMSDateTime::getYear() {
	return this->yearstr;
}

string &MMSDateTime::getDbDate() {
	this->dbdate = iToStr(this->yearnum) + "-" 
			+ ((this->monthnum < 10) ? "0" : "") + iToStr(this->monthnum + 1) + "-" 
			+ ((this->daynum < 10) ? "0" : "") + iToStr(this->daynum) + " " 
			+ ((this->hour < 10) ? "0" : "") + iToStr(this->hour) + ":" 
			+ ((this->minute < 10) ? "0" : "") + iToStr(this->minute) + ":" 
			+ ((this->seconds < 10) ? "0" : "") + iToStr(this->seconds);

	return this->dbdate;
}

int	MMSDateTime::getDayNum() {
	return this->daynum;
}

int MMSDateTime::getMonthNum() {
	return this->monthnum;
}

int MMSDateTime::getYearNum() {
	return this->yearnum;
}
