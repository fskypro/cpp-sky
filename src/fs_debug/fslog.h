/*
** this module implement debug log printer
** author: hyw
** date: 2012.09.20
*/

#ifndef __FS_LOG_H__
#define __FS_LOG_H__

#include <stdarg.h>
#include <ctime>
#include <string>
#include "fsmacros.h"

using namespace std;

NS_FS_BEGIN
namespace debug {

// -----------------------------------------------------------------------
// implement FSLog class
// -----------------------------------------------------------------------
class FSLog
{
public:
	typedef clock_t TIME_TEST_INFO;

protected:
	FSLog();
	FSLog(const char*);
	void setPath(const char*);

	// -------------------------------------------------------------------
	// ��ʽ������������ڡ������ļ��������̵߳� log��
	// ������1����ʾǰ׺��2�������ļ���3�������кţ�4������ı���5����ʽ�������б�
	// -------------------------------------------------------------------
	bool printOut(const char*, const char*, unsigned int, const char*, va_list);
	string formatOut(const char*, const char*, unsigned int, const char*, va_list);

	// -------------------------------------------------------------------
	// ��������ı���
	// ������1������ı���2����ʽ��������3���Ƿ�����д�����
	// -------------------------------------------------------------------
	void messageOut(const char*, va_list, bool=true);

	// -------------------------------------------------------------------
	// ��ʱ���ԣ�
	// ������1����ʾǰ׺��2�������ļ���3�������кţ�4������ı���5����ʽ�������б�
	// �� timeTestBegin �ķ���ֵ��Ϊ�������� timeTestEnd ���һ�����Ե�Ԫ
	// -------------------------------------------------------------------
	TIME_TEST_INFO timeTestBegin(const char*, const char*, unsigned int, const char*, va_list);
	void timeTestEnd(TIME_TEST_INFO);

private:
	bool print(string&, bool=true);

private:
	string m_strPth;
	FILE *m_pFile;
};


}
NS_FS_END
#endif
