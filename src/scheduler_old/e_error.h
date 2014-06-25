
/*
 * Simple stubs to simulate eclipse's error/warning/comment functions
 */

#ifndef _E_ERROR_H_
#define _E_ERROR_H_

#ifdef __cplusplus
extern "C" {
#endif

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
//#include <unistd.h>
#include <stdarg.h>


/*-------------------------------------------------------------------------*/
/**
  @brief	Sends out a warning to the appropriate channels
  @param	fmt formatted input, strictly similar to printf
  @return	void

  This function should be used for any warning output. It might print
  a message on the @c stderr console, send a copy to a log file, to an
  error log system, etc.
 */
/*--------------------------------------------------------------------------*/

void e_warning(char *fmt, ...);


/*-------------------------------------------------------------------------*/
/**
  @brief	Sends out an error to the appropriate channels
  @param	fmt formatted input, strictly similar to printf
  @return	void

  This function should be used for any error output. It might print
  a message on the @c stderr console, send a copy to a log file, to an
  error log system, etc.
 */
/*--------------------------------------------------------------------------*/


void e_error(char *fmt, ...);


/*-------------------------------------------------------------------------*/
/**
  @brief	sends out a comment to the appropriate channels
  @param	level comment level (unused)
  @param	fmt formatted input, strictly similar to printf
  @return	void

  This function should be used for any comment output. It might print
  a message on the @c stderr console, send a copy to a log file, to an
  error log system, etc.
 */
/*--------------------------------------------------------------------------*/




void e_comment(int level, char *fmt, ...);

#ifdef __cplusplus
}
#endif

#endif
