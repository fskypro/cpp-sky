/*
** header for fsky
**   给本库用，或预编译
** author: hyw
** date: 2012.10.23
*/

#ifndef __FS_AFX_H__
#define __FS_AFX_H__

#include <vector>
#include <map>
#include <string>

// -----------------------------------------------------------------------
// replace new
// -----------------------------------------------------------------------
#ifdef __FS_DEBUG
#define DEBUG_NEW new(__FILE__, __LINE__)
#else
#define DEBUG_NEW new
#endif
#define new DEBUG_NEW

#endif
