/**
 * @mainpage HTML download task
 *
 * The program scans the URL supplied as console parameter, downloads all the references from this URL and computes the hashes of those files.
 * Finally, it prints the filenames and hashes to the console and highlights the largest (red) and the smallest (blue) file.
 */

/** @file TaskFacade
 *
 * @author Jiri Burant
 * @date 14.9.2017
 * 
 * Wrapper for Downloader and Parser functionality. It calls the parser functions on the supplied url, then it passes the found links to
 * the downloader, and displays the filenames with hashes and highlights the largest(red) and the smallest(blue) files.  
 */

#ifndef TASK_FACADE_H
#define TASK_FACADE_H

#include "TaskTypes.h"
#include <string>

#define ANSI_COLOR_RED          "\x1b[31m"
#define ANSI_COLOR_BLUE         "\x1b[34m"
#define ANSI_COLOR_RESET        "\x1b[0m"
#define OUTPUT_TABLE_COL_WIDTH   15

class TaskFacade
{
  public:
    TaskFacade();
    ~TaskFacade();

    /**
     * Constructs the downloader and parser. The parser scans the file, supplied as argument. The downloader downloads all the url files
     * returned by the parser.
     *
     * @param url Url supplied as argument to the program    
     *
     * @return RET_OK in case of succcess
     */
    returnCode_t runTask(std::string url);

  private:

    /**
     * Returns index of the largest file
     * 
     * @param downloadedFilesInfo array of files info
     * @param size size of the array
     *
     * @return index of the filename
     */
    int getIndexOfLargest(fileInfo_t downloadedFilesInfo[], unsigned int size);

    /**
     * Returns index of the smallest file
     * 
     * @param downloadedFilesInfo array of files info
     * @param size size of the array
     *
     * @return index of the filename
     */
    int getIndexOfSmallest(fileInfo_t downloadedFilesInfo[], unsigned int size);

    /**
     * Prints the filenames and the file hashes and marks the largest (red) and the smallest (blue).
     * 
     * @param downloadedFilesInfo array of the info about the downloaded files
     * @param size size of the array
     * @param indexOfLargest index of the largest file
     * @param indexOfSmallest index of the smallest file
     */
    void printResults(fileInfo_t downloadedFilesInfo[], unsigned int size, unsigned int indexOfLargest, unsigned int indexOfSmallest);
};

#endif //TASK_FACADE_H
