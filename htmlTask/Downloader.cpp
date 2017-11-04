/*
 * author: Jiri Burant
 * date created: 14.9.2017
*/

#include "Downloader.h"
#include <zlib.h>
#include <iostream>

Downloader::Downloader() : filesInfoPtr(NULL)
{
}

Downloader::~Downloader()
{
  delete filesInfoPtr;
  filesInfoPtr = NULL;
}

size_t Downloader::downloadCallback(char *bufferPtr, size_t elementSize, size_t elementCount, void *paramsPtr)
{
  size_t realSize = (elementCount * elementSize);
  fileInfo_t  *params = (fileInfo_t*) paramsPtr;

  fwrite (bufferPtr, elementSize, elementCount, params->filePtr);
  params->hash = adler32(params->hash, (const Bytef*)bufferPtr, (elementCount * elementSize));
  params->size += realSize; 
  return realSize;
}

void Downloader::initDownloadHandle(CURLM *cmPtr, std::string *urlPtr, fileInfo_t *fileInfoPtr)
{
  CURL *eh = curl_easy_init();
 
  curl_easy_setopt(eh, CURLOPT_WRITEFUNCTION, downloadCallback);
  curl_easy_setopt(eh, CURLOPT_HEADER, 0L);
  curl_easy_setopt(eh, CURLOPT_URL, urlPtr->c_str());
  curl_easy_setopt(eh, CURLOPT_WRITEDATA, fileInfoPtr);
  curl_easy_setopt(eh, CURLOPT_VERBOSE, 0L);
 
  curl_multi_add_handle(cmPtr, eh);
}

void* Downloader::threadDownloadFiles(void *threadParams)
{
  int still_running = -1;
  CURLM *cm = curl_multi_init();
  threadParameters params = *(threadParameters *) threadParams;

  for(int i=0; i<params.count; i++)
  {
    initDownloadHandle(cm, params.filesInfoPtr[i].url, &params.filesInfoPtr[i]);
  }

  int repeats = 0;

  do {
    CURLMcode mc;
    int numfds;

    mc = curl_multi_wait(cm, NULL, 0, 0, &numfds);
 
    if(mc != CURLM_OK) {
      std::cout << "curl_multi_wait() failed, code" << mc << "\n";
      break;
    }
 
    if(0 != numfds) {
      repeats++;
    }
    else
    {
      repeats = 0;
    } 

    curl_multi_perform(cm, &still_running);
  } while(still_running);

  curl_multi_cleanup(cm);
}

int Downloader::downloadFiles(std::unordered_map<unsigned int, std::string*> urls)
{
  int modUrls = 0;
  int index = 0;
  int urlsPerThread = 0;
  int error;

  filesInfoPtr = new fileInfo_t[urls.size()];
  pthread_t tid[NUM_THREADS];

  curl_global_init(CURL_GLOBAL_ALL);
  curl_global_init(CURL_GLOBAL_SSL);

  threadParameters threadParams[NUM_THREADS];

  for(int i=0; i<urls.size(); i++)
  {
    filesInfoPtr[i].filename = ("files/file" + std::to_string(i));
    filesInfoPtr[i].filePtr = (fopen (filesInfoPtr[i].filename.c_str(), "w"));
    filesInfoPtr[i].hash = adler32(0L, Z_NULL, 0);
    filesInfoPtr[i].size = 0U;
    filesInfoPtr[i].url = urls.at(i+1);
  }

  if(urls.size() >= NUM_THREADS)
  {
    urlsPerThread = urls.size()/NUM_THREADS;
    modUrls = urls.size() % NUM_THREADS;

   for(int i=0; i<NUM_THREADS; i++)
   {
     threadParams[i].count = urlsPerThread;

     if(modUrls > 0)
     {
       urlsPerThread++;
       modUrls--;
     }
     threadParams[i].filesInfoPtr = &(filesInfoPtr[index]);
     index += urlsPerThread;
   }
  }

  for(int i = 0; i< NUM_THREADS; i++) {
    error = pthread_create(&tid[i], NULL, threadDownloadFiles, (void *)&threadParams[i]);

    if(0 != error)
    {
      std::cout << "Couldn't run thread number " << i << "\n";
    }
  }

  for(int i = 0; i< NUM_THREADS; i++) {
    pthread_join(tid[i], NULL);
  }

  for(int i=0; i<urls.size(); i++)
  {
    fclose(filesInfoPtr[i].filePtr);
  }

  curl_global_cleanup();

  return urls.size();
}


fileInfo_t * Downloader::getFilesInfo() const
{
  return filesInfoPtr;
}
