/***************************************************************************
 *   Copyright (C) 2005-2008 by                                            *
 *                                                                         *
 *      Stefan Schwarzer <sxs@morphine.tv>                                 *
 *      Guido Madaus     <bere@morphine.tv>                                *
 *      Jens Schneider   <pupeider@morphine.tv>                            *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License as published by  *
 *   the Free Software Foundation; either version 2 of the License.        *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program; if not, write to the                         *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include "mmstools/tools.h"
#include "mmstools/mmsmutex.h"
#include "mmsconfig/mmsconfigdata.h"
#include <wordexp.h>
#include <strings.h>
#include <errno.h>
#include <sys/stat.h>

/* Once-only initialisation of the key */
static pthread_once_t buffer_key_once = PTHREAD_ONCE_INIT;
/* contains the key to the thread specific memory */
static pthread_key_t  key_iam;
static pthread_key_t  key_logfile;
static MMSConfigData  config;
static FILE			  *fp=NULL;
static MMSMutex       debugMsgMutex;

string substituteEnvVars(string input) {
    wordexp_t p;
    char **w;
    string output = "";
    if (input != "") {
        wordexp(input.c_str(), &p, 0);
        w = p.we_wordv;
        for (unsigned i=0; i<p.we_wordc; i++)
            if (i==0) {
                output = w[i];
                break;
            }
        wordfree(&p);
    }
    return output;
}

string maskChars(string str) {
    string  ret;
    for (unsigned i=0; i<str.size(); i++) {
        if (str.at(i) == '\'')
            ret = ret + "''";
        else
            ret = ret + str.at(i);
    }
    return ret;
}

string *strToUpr(string *src) {
    char *dest;
    unsigned int i, len;
    dest=strdup(src->c_str());

    len = strlen(dest);
    for(i=0; i<=len; i++) {

    	if((dest[i] >= 'a') && (dest[i] <= 'z'))
            dest[i]-=32;
    }
    *src = dest;
    free(dest);

    return src;
}

string strToUpr(string src) {
    string s;
    s=src;
    strToUpr(&s);
    return s;
}

int hexToInt(const char *in) {
    int ret=0;

    /* working with first char */
    if (*in>='0' && *in<='9')
        ret+=((int)*in-'0')*16;
    else
    if (*in>='A' && *in<='F')
        ret+=((int)*in-'A'+10)*16;
    else
    if (*in>='a' && *in<='f')
        ret+=((int)*in-'a'+10)*16;

    /* working with second char */
    in++;
    if (*in>='0' && *in<='9')
        ret+=(int)*in-'0';
    else
    if (*in>='A' && *in<='F')
        ret+=(int)*in-'A'+10;
    else
    if (*in>='a' && *in<='f')
        ret+=(int)*in-'a'+10;

    return ret;
}

string getSimpleTimeString() {
    string timstr;

    getCurrentTimeString(NULL,NULL,&timstr,NULL);

    return timstr;
}

bool getCurrentTimeBuffer(char *dtbuf, char *datebuf, char *timebuf, time_t *clock) {
    struct  tm newtime;
    time_t  aclock;

    /* get current date and time */
    time(&aclock);
    if (clock) {
        if (*clock)
            aclock=*clock;
        else
            *clock=aclock;
    }
    localtime_r(&aclock, &newtime);

    if (dtbuf)
        sprintf(dtbuf,"%04d-%02d-%02d %02d:%02d:%02d",
                        newtime.tm_year+1900,
                        newtime.tm_mon+1,
                        newtime.tm_mday,
                        newtime.tm_hour,
                        newtime.tm_min,
                        newtime.tm_sec
                        );

    if (datebuf)
        sprintf(datebuf,"%04d-%02d-%02d",
                        newtime.tm_year+1900,
                        newtime.tm_mon+1,
                        newtime.tm_mday
                        );

    if (timebuf)
        sprintf(timebuf,"%02d:%02d:%02d",
                        newtime.tm_hour,
                        newtime.tm_min,
                        newtime.tm_sec
                        );

    return true;
}

bool getCurrentTimeString(string *dtstr, string *datestr, string *timestr, time_t *clock) {
    char    dtbuf[20];
    char    datebuf[11];
    char    timebuf[9];

    if (getCurrentTimeBuffer((dtstr)?dtbuf:NULL,
                             (datestr)?datebuf:NULL,
                             (timestr)?timebuf:NULL,
                             clock)) {
        if (dtstr) *dtstr=dtbuf;
        if (datestr) *datestr=datebuf;
        if (timestr) *timestr=timebuf;
        return true;
    }
    else
        return false;
}

string getDayOfWeek(time_t *clock) {
    struct  tm newtime;
    time_t  aclock;

    /* get current date and time */
    time(&aclock);
    if (clock) {
        if (*clock)
            aclock=*clock;
        else
            *clock=aclock;
    }
    localtime_r(&aclock, &newtime);

    switch (newtime.tm_wday) {
        case 0:
            return "Sunday";
        case 1:
            return "Monday";
        case 2:
            return "Tuesday";
        case 3:
            return "Wednesday";
        case 4:
            return "Thursday";
        case 5:
            return "Friday";
        case 6:
            return "Saturday";
    }

    return "";
}

static void bufferDestroy(void *buffer) {
    free(buffer);
}

static void bufferKeyAlloc() {
    pthread_key_create(&key_iam, bufferDestroy);
    pthread_key_create(&key_logfile, bufferDestroy);
}

void initLogging(char *Iam, char *logfile) {
    char *dest;
    pthread_once(&buffer_key_once, bufferKeyAlloc);

    dest = (char *)pthread_getspecific(key_iam);
    if (dest == NULL)
        pthread_setspecific(key_iam, malloc(100));

    dest = (char *)pthread_getspecific(key_iam);
    memset(dest,0,100);
    strncpy(dest,Iam,99);

    dest = (char *)pthread_getspecific(key_logfile);
    if (dest == NULL)
        pthread_setspecific(key_logfile, malloc(1000));
    dest = (char *)pthread_getspecific(key_logfile);
    memset(dest,0,1000);
    strncpy(dest,logfile,999);

}


void writeMessage(const char *ctrl,...) {
    char *iam;
    char *logname;
    char currTimebuffer[128];
    FILE *File;
    char line[10000];

    va_list marker;
    va_start(marker, ctrl);

    iam     = (char *)pthread_getspecific(key_iam);
    logname = (char *)pthread_getspecific(key_logfile);

    if(logname == NULL) {
        logname = (char*)"/var/log/disko/logfile";
    }
    if(iam == NULL) {
        iam = (char*)"unkown";
    }

    getCurrentTimeBuffer(currTimebuffer);

    if ((File=fopen(logname,"at"))) {

        fprintf (File,"%s  ",currTimebuffer);
        vsprintf(line,ctrl,marker);
        if(line[0]!='[')
            fprintf(File,"[%s]: ",iam);

        fprintf (File,"%s",line);

        fprintf (File,"\n");

        fflush(File);
        fclose(File);
    }

    va_end(marker);

}

int strToInt(string s) {
	return atoi(s.c_str());
}

string iToStr(int i) {
    char mychar[1024];
    string mystr;

    sprintf(mychar,"%d",i);
    mystr = mychar;
    return mystr;
}

string fToStr(double i) {
    char mychar[1024];
    string mystr;

    sprintf(mychar,"%f",i);
    mystr = mychar;
    return mystr;
}


char *scanForString(char *buf, char *toFind, char **ret,
					int offset, unsigned int length) {
	char 			*ptr;
	char 			*tmp;
	unsigned int	reallen;

	if ((ptr = strstr(buf, toFind))) {
		if (ret) {
			tmp = ptr + strlen(toFind);
			reallen = strlen(tmp);
			if ((int)reallen>=offset) {
				tmp+=offset;
				reallen-=offset;
			}
			else {
				tmp+=reallen;
				reallen=0;
			}
			if (!length)
				length = reallen;
			else
				if (reallen<length) length=reallen;
			*ret = (char *)malloc(length+1);
			memcpy(*ret, tmp, length);
			(*ret)[length]=0;
		}
	}

	return ptr;
}

char *scanForString(char *buf, char *toFind, string *ret,
					int offset, unsigned int length) {
	char	*rbuf;
	char 	*tmpret;

 	if (ret) *ret="";
	if ((rbuf = scanForString(buf, toFind, &tmpret, offset, length))) {
		if (tmpret) {
			if (ret) *ret = tmpret;
			free(tmpret);
		}
	}

	return rbuf;
}

string scanForString(string buf, string toFind, string *ret,
					int offset, unsigned int length) {
	int 			ptr;
	string			tmp;
	unsigned int 	reallen;

	if ((ptr=buf.find(toFind))>=0) {
		if (ret) {
			tmp=buf.substr(ptr + toFind.size());
			reallen = tmp.size();
			if ((int)reallen>=offset) {
				tmp=tmp.substr(offset);
				reallen-=offset;
			}
			else {
				tmp=tmp.substr(reallen);
				reallen=0;
			}
			if (!length)
				length = reallen;
			else
				if (reallen<length) length=reallen;
			*ret = tmp.substr(0, length);
		}

		return buf.substr(ptr);
	}

	return "";
}

void msleep(unsigned long msec) {
	usleep(msec * 1000);
}


bool scanString(string toscan, string frontkey, string backkey,
                unsigned int offset, unsigned int length, string *result, unsigned int *nextpos) {
    int pos;

    if (frontkey != "") {
        pos = (int)toscan.find(frontkey);
        if (pos<0)
            return false;
        toscan = toscan.substr(pos + frontkey.size());
        if (nextpos)
            (*nextpos)+=pos + frontkey.size();
    }
    if (backkey != "") {
        pos = (int)toscan.find(backkey);
        if (pos<0)
            return false;
        else {
            toscan = toscan.substr(0, pos);
            if (nextpos)
                (*nextpos)+=pos + backkey.size();
            if (frontkey != "") {
                while (1) {
                    pos = (int)toscan.find(frontkey);
                    if (pos<0)
                        break;
                    toscan = toscan.substr(pos + frontkey.size());
                }
            }
        }
    }

    if (length) {
        if (toscan.size() >= offset + length)
            *result = toscan.substr(offset, length);
        else
            return false;
    }
    else
        if (offset) {
            if (toscan.size() >= offset)
                *result = toscan.substr(offset);
            else
                return false;
        }
        else
            *result = toscan;

    return true;
}


string cpToStr(char *str) {
    string ret = str;
    return ret;
}


string cToStr(char chr) {
    char my[2];
    my[0]=chr;
    my[1]=0;

    string ret = my;

    return ret;
}

void trim(string& str)
{
  string::size_type pos = str.find_last_not_of(' ');
  if(pos != string::npos) {
    str.erase(pos + 1);
    pos = str.find_first_not_of(' ');
    if(pos != string::npos) str.erase(0, pos);
  }
  else str.erase(str.begin(), str.end());
}

bool strToBool(string s) {
	if(s.empty())
		return false;

	if(strcasecmp(s.c_str(), "true") == 0)
		return true;
	else
		return false;
}

void executeCmd(string cmd) {
	pid_t pid;
	int i,y;
	int argc;
	char *argv[256];
	char buffer[4096];


    for (i=0;i<256;i++)
    	argv[i]=NULL;
    argc=0;
    sprintf(buffer,"%s",cmd.c_str());
    printf("\n%s\n",buffer);
    argv[0]=buffer;

    i=0;
    while ((buffer[i]!=0)&&(argc<256)) {
        while(buffer[i]==' ')
        	i++;

        if(buffer[i]==0)
        	break;

        if(buffer[i]=='\'') {
            i++;
            y=i;
            while ((buffer[i]!='\'')&&(buffer[i]!=0))
            	i++;

            if (buffer[i]=='\'') {
                buffer[i]=0;
                i++;
            }
        } else if (buffer[i]=='"') {
            i++;
            y=i;
            while ((buffer[i]!='"')&&(buffer[i]!=0))
            	i++;

            if (buffer[i]=='"') {
                buffer[i]=0;
                i++;
            }
        } else {
            y=i;
            while ((buffer[i]!=' ')&&(buffer[i]!=0))
            	i++;

            if (buffer[i]==' ') {
                buffer[i]=0;
                i++;
            }
        }
        argv[argc]=&buffer[y];
        argc++;
    }


	pid = fork();
		if(pid!=-1) {
		if(pid>0)
			return;
		if(pid==0) {
			unsetenv("LD_PRELOAD");
			execvp(argv[0],argv);
			printf("\nError while exec: %s",strerror(errno));
			printf("\nargv[0]: %s",argv[0]);
			printf("\nargv[1]: %s",argv[1]);
			exit(1);
		}
	}
}


bool file_exist( string filename ) {
	struct stat buffer ;

	if ( stat( filename.c_str(), &buffer ) == 0)
		return true;

	return false;
}

void writeDebugMessage(const char *identity, const char *filename, const int lineno, const char *msg, ...) {
	va_list arglist;
	struct  timeval tv;
	char    timebuf[12];
	char 	buffer[1024]={0};
	char 	buffer2[1024]={0};
	int		num;

    debugMsgMutex.lock();
	if((fp=fopen(config.getLogfile().c_str(), "a+"))==NULL)
		throw new MMSError(errno, "Can't open logfile [" + string(strerror(errno)) + "]");

	va_start(arglist, (char *)msg);
	vsprintf(buffer, msg, arglist);

	gettimeofday(&tv, NULL);

    getCurrentTimeBuffer(NULL, NULL, timebuf, NULL);

	num = snprintf(buffer2, sizeof(buffer2), "%s:%02ld %s: %s [%s:%d]\n", timebuf,
	                    tv.tv_usec/10000, identity, buffer, filename, lineno);

	fwrite(buffer2, 1, num, fp);

	va_end(arglist);

	fclose(fp);
    debugMsgMutex.unlock();

	return;
}

void writeDebugMessage(const char *identity, const char *filename, const int lineno, const string &msg) {
	struct  timeval tv;
	char    timebuf[12];
	char 	buffer[1024]={0};
	int		num;

    debugMsgMutex.lock();
	if((fp=fopen(config.getLogfile().c_str(), "a+"))==NULL)
		throw new MMSError(errno, "Can't open logfile [" + string(strerror(errno)) + "]");

	gettimeofday(&tv, NULL);
    getCurrentTimeBuffer(NULL, NULL, timebuf, NULL);

	num = snprintf(buffer, sizeof(buffer), "%s:%02ld %s: %s [%s:%d]\n", timebuf,
	               tv.tv_usec/10000, identity.c_str(), msg.c_str(), filename, lineno);

	fwrite(buffer, 1, num, fp);
	fclose(fp);
    debugMsgMutex.unlock();

	return;
}
