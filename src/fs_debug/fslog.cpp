/*
** this module implement debug log printer
** author: hyw
** date: 2012.09.20
*/

#include <stdio.h>
#include <cassert>
#include "fsmacros.h"
#include "fs_task/fsThread.h"
#include "fs_str/fs_string.h"
#include "fslog.h"

using namespace std;

NS_FS_BEGIN

namespace debug {

// -----------------------------------------------------------------------
// inners
// -----------------------------------------------------------------------
static void formatTime(string &strBuff)
{
	time_t now;
	time(&now);
	struct tm tmNow;
#if (_MSC_VER) && (_MSC_VER >= 1200)
	localtime_s(&tmNow, &now);
#else
	struct tm *pTmNow = localtime(&now);
	memcpy(&tmNow, pTmNow, sizeof(tmNow));
#endif
	char buff[32];
	strftime(buff, sizeof(buff), "%Y/%m/%d - %H:%M:%S", &tmNow);
	strBuff.append(buff);

	FS_SNPRINTF(buff, sizeof(buff), "::%d ", clock());
	strBuff.append(buff);
}

static void formatThreadID(string &strBuff)
{
	char buff[32];
	FS_SNPRINTF(buff, sizeof(buff), "(thread: %d)\n", sp_thread_self());
	strBuff.append(buff);
}

static void formatSource(string &strBuff, const char *file, unsigned lineNo)
{
	strBuff.append(file);
	char buff[16];
	FS_SNPRINTF(buff, sizeof(buff), "(%d)\n", lineNo);
	strBuff.append(buff);
}

static void formatMessage(string &strBuff, const char *prefix, const char *msg, va_list args)
{
	strBuff.append(prefix);
	size_t msgLen = strlen(msg) + 100;
	char *buff = new char[msgLen];
	vsnprintf(buff, msgLen, msg, args);
	strBuff.append(buff);
	strBuff.append("\n");
	delete[] buff;
}


// -----------------------------------------------------------------------
// private
// -----------------------------------------------------------------------
bool FSLog::print(string &strMsg, bool save)
{
	assert(!m_strPth.empty());
	FILE *file = m_pFile ? m_pFile : fopen(m_strPth.c_str(), "a+");
	assert(file);

	fseek(file, 0, SEEK_END);
	fwrite(strMsg.c_str(), strMsg.length(), 1, file);
	if(save)
	{
		fwrite("\n", 1, 1, file);
		fclose(file);
		m_pFile = NULL;
	}
	else
	{
		m_pFile = file;
	}
	return true;
}


// -----------------------------------------------------------------------
// protected
// -----------------------------------------------------------------------
FSLog::FSLog()
	: m_pFile(NULL)
{
}

FSLog::FSLog(const char *pszPath)
	: m_pFile(NULL)
{
	setPath(pszPath);
}


void FSLog::setPath(const char *pszPath)
{
	if (m_pFile)
	{
		fclose(m_pFile);
		m_pFile = NULL;
	}
	FILE *file = fopen(pszPath, "w");
	if(file)
	{
		fclose(file);
		m_strPth = pszPath;
	}
	else
	{
		assert(false);
	}
}

// ---------------------------------------------------
string FSLog::formatOut(const char *prefix, const char *src,
	unsigned int lineNo, const char *msg, va_list args)
{
	string strMsg;
	formatTime(strMsg);
	formatThreadID(strMsg);
	formatSource(strMsg, src, lineNo);
	formatMessage(strMsg, prefix, msg, args);
	print(strMsg);
	return strMsg;
}


bool FSLog::printOut(const char *prefix, const char *src,
	unsigned int lineNo, const char *msg, va_list args)
{
	string strMsg;
	formatTime(strMsg);
	formatThreadID(strMsg);
	formatSource(strMsg, src, lineNo);
	formatMessage(strMsg, prefix, msg, args);
	return print(strMsg);
}

void FSLog::messageOut(const char *pszMsg, va_list args, bool save)
{
	char *buff = new char[strlen(pszMsg) + 100];
	vsnprintf(buff, sizeof(buff), pszMsg, args);
	string strMsg = buff;
	print(strMsg, save);
	delete[] buff;
}


// ---------------------------------------------------
FSLog::TIME_TEST_INFO FSLog::timeTestBegin(const char *prefix, const char *src,
	unsigned int lineNo, const char *msg, va_list args)
{
	string strMsg("time test begin: ");
	char splitter[59];
	NS_FS::fs_cstr::makeCharLine(splitter, '-', sizeof(splitter));
	strMsg.append(splitter);
	strMsg.append("\n");
	formatTime(strMsg);
	formatThreadID(strMsg);
	formatSource(strMsg, src, lineNo);

	formatMessage(strMsg, prefix, msg, args);

	print(strMsg, false);
	return clock();
}

void FSLog::timeTestEnd(FSLog::TIME_TEST_INFO start)
{
	FSLog::TIME_TEST_INFO end = clock();
	char buff[66];
	FS_SNPRINTF(buff, sizeof(buff), "waste time: %fs\n",
		((double)(clock() - start)) / CLOCKS_PER_SEC);
	string strMsg(buff);

	strMsg.append("test end. ");
	NS_FS::fs_cstr::makeCharLine(buff, '-', sizeof(buff));
	strMsg.append(buff);
	strMsg.append("\n");

	print(strMsg);
}

}
NS_FS_END
