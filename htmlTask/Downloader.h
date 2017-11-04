/**
 * @file Downloader
 *
 * @author Jiri Burant
 * @date 14.9.2017
 * 
 * Downloads files from specified urls using libcurl and computes their hash on the fly using adler32 algorithm from zlib.
 */


#ifndef DONWLOADER_H
#define DONWLOADER_H

#include "TaskTypes.h"
#include <curl/multi.h>
#include <unordered_map>
#include <string>

#define NUM_THREADS 4 ///<Configures number of threads for parallel download

class Downloader
{
  public:
    Downloader();
    ~Downloader();

    /**
     * Configures the libcurl download handles and callbacks and controls the download. It is using multithreading.
     *
     * @param urls addresses of the files to be downloaded
     * @return number of downloaded files
     */
    int downloadFiles(std::unordered_map<unsigned int, std::string*> urls);

    /**
     * Returns the pointer to filesInfo
     * 
     * @return *filesInfo
     */ 
    fileInfo_t *getFilesInfo() const;

  private:
    fileInfo_t *filesInfoPtr; ///<Pointer to array of fileInfo_t structures containing basic info about the downloaded files

    /**
     * Called in loop for each file to be downloaded, initializes appropriate libcurl structure.
     *
     * @param *cm libcurl handle
     * @param *urlPtr pointer to url to be downloaded
     * @param *fileInfoPtr pointer to fileInfo structure of the appropriate file
     */
    static void initDownloadHandle(CURLM *cmPtr, std::string *urlPtr, fileInfo_t *fileInfoPtr);

    /**
     * Callback for libcurl functionality. Writes the buffer to file and updates the file hash. 
     * 
     * @param *buffer buffer pointer of read bytes
     * @param elementCount number of read elements
     * @param elementSize size of the elements
     * @param *paramsPtr pointer to parameters of the current donwload session
     *
     * @return elementCount*elementSize
     */
    static size_t downloadCallback(char *bufferPtr, size_t elementSize, size_t elementCount, void *paramsPtr);

    /**
     * Run function for each thread. Creates the handles for the thread and manages the download.
     *
     * @param *threadParams structure specifying input thread parameters (files and urls for the thread)
     */
    static void* threadDownloadFiles(void *threadParams);
};

#endif //DONWLOADER_H
