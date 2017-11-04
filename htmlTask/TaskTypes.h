/**
 * @file TaskTypes
 *
 * @author Jiri Burant
 * @date 14.9.2017
 * 
 * Project specific enumerations and types.
 */


#ifndef TASK_TYPES_H
#define TASK_TYPES_H

#include <stdio.h>
#include <string>

/**
 * Return codes of the TaskFacade::runTask function 
 */
enum returnCode_t
{
  RET_OK,
  RET_ERR_WRONG_NO_ARGUMENTS,
  RET_ERR_SCAN,
  RET_ERR_DOWNLOAD,
  RET_ERR_PRINT,
  RET_ERR_OTHER,
};

/**
 * Contains info about the downloaded file.
 */
typedef struct
{
  FILE *filePtr; ///<Pointer to the opened file
  unsigned long hash; ///<Running hash of the file
  std::string filename; ///<As saved on disc
  std::string *url; ///<Source url
  size_t size; ///<Size of the file
} fileInfo_t;

/**
 * Contains parameters for download thread
 */
typedef struct
{
  unsigned int count; ///<Number of files (urls)
  fileInfo_t *filesInfoPtr; ///<Pointer to first filesInfoin array
} threadParameters;

#endif //TASK_TYPES_H
