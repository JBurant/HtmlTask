/*
 * author: Jiri Burant
 * date created: 14.9.2017
*/

#include "TaskFacade.h"
#include "Parser.h"
#include "Downloader.h"
#include <limits.h>
#include <iostream>
#include <iomanip>

TaskFacade::TaskFacade()
{
}

TaskFacade::~TaskFacade()
{
}

returnCode_t TaskFacade::runTask(std::string url)
{
  Downloader* downloader = new Downloader();
  Parser* parser = new Parser();

  unsigned int numberOfFoundFiles = parser->parseUrl(url);
  if(numberOfFoundFiles < 1)
  {
    return RET_ERR_SCAN;
  }

  unsigned int numberOfDownloadedFiles = downloader->downloadFiles(parser->getUrls());
  if(numberOfDownloadedFiles != numberOfFoundFiles)
  {
    return RET_ERR_DOWNLOAD;
  }

  fileInfo_t *downloadedFilesInfo = downloader->getFilesInfo();

  int indexBiggest = getIndexOfLargest(downloadedFilesInfo, numberOfDownloadedFiles);
  int indexSmallest = getIndexOfSmallest(downloadedFilesInfo,  numberOfDownloadedFiles);

  printResults(downloadedFilesInfo, numberOfDownloadedFiles, indexBiggest, indexSmallest);

  delete parser;

  return RET_OK;
}

int TaskFacade::getIndexOfLargest(fileInfo_t downloadedFilesInfo[], unsigned int size)
{
  unsigned int largestSize = 0;
  int indexOfLargest = -1;

  for(int i = 0; i < size; i++)
  {
    if(downloadedFilesInfo[i].size > largestSize)
    {
      largestSize = downloadedFilesInfo[i].size;
      indexOfLargest = i;
    }
  }
  return indexOfLargest;
}

int TaskFacade::getIndexOfSmallest(fileInfo_t downloadedFilesInfo[], unsigned int size)
{
  unsigned int smallestSize = UINT_MAX;
  int indexOfSmallest = -1;

  for(int i = 0; i < size; i++)
  {
    if(downloadedFilesInfo[i].size < smallestSize)
    {
      smallestSize = downloadedFilesInfo[i].size;
      indexOfSmallest = i;
    }
  }
  return indexOfSmallest;
}

void TaskFacade::printResults(fileInfo_t downloadedFilesInfo[], unsigned int size, unsigned int indexOfLargest, unsigned int indexOfSmallest)
{
  std::string color;

  for(int i=0; i<size; i++)
  {
    if(indexOfLargest == i)
    {
      color = ANSI_COLOR_RED;
    }
    else if(indexOfSmallest == i)
    {
      color = ANSI_COLOR_BLUE;
    }
    else
    {
      color = ANSI_COLOR_RESET;
    }

    std::cout << color << std::left << std::setw(OUTPUT_TABLE_COL_WIDTH) << downloadedFilesInfo[i].filename << std::setw(OUTPUT_TABLE_COL_WIDTH) << std::to_string(downloadedFilesInfo[i].hash) << ANSI_COLOR_RESET << std::endl;
  }
}
