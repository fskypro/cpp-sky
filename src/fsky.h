/*
** common header of fs for users
** 注：本库内的模块不要包含此文件
** author: hyw
** date: 2012.10.23
*/


#ifndef __FS_H__
#define __FS_H__

#include "fsmacros.h"
//#include "fs_debug/fsdebug.h"

#include "fs_io/fs_path.h"
#include "fs_math/fs_geomethry.h"

#include "fs_str/fs_string.h"
#include "fs_str/fs_convert.h"
#include "fs_str/fs_format.h"

#include "fs_dtime/fs_datetime.h"
#include "fs_dtime/fs_time.h"

#include "fs_tpls/fs_property.h"		// C++11 must be supported
#include "fs_tpls/fs_singleton.h"
#include "fs_tpls/fs_event.h"

#include "fs_type/fs_type.h"			// C++11 must be supported
#include "fs_type/fs_collection.h"

#include "fs_encode/fs_urlencoder.h"
#include "fs_encode/fs_base64.h"

//#include "fs_task/fsThread.h"
//#include "fs_task/fsTaskPool.h"

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
