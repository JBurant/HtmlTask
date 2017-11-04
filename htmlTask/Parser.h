/**
 * @file Parser
 *
 * @author Jiri Burant
 * @date 14.9.2017
 * 
 * Scans the supplied url for hypertext links using libcurl and adds them to unordered map.
 */

#ifndef PARSER_H
#define PARSER_H

#include <curl/multi.h>
#include <unordered_map>
#include <string>

class Parser
{
  public:
    Parser();
    ~Parser();

    /**
     * Prepares the libcurl environment and performs the download with scan.
     *
     * @param inputUrl url to be scanned
     *
     * @return number of found urls
     */
    unsigned int parseUrl(std::string inputUrl);

    /**
     * Returns the collection of the urls.
     *
     * @return urls
     */
    std::unordered_map<unsigned int, std::string*> getUrls() const;

  private:
    const static size_t estMaxUrlSize = 500;  ///<At the beginning the string is initialized with this value to prevent reallocations
    std::string originalUrl; ///<Url which to be scanned
    char lastChar; ///<Lastly read character from buffer
    bool isPartOfUrl; ///<True if the current character belongs to url, false otherwise
    unsigned int currentUrlIndex; ///<Identificator of the url being extracted from the originalUrl
    std::unordered_map<unsigned int, std::string*> urls; ///<Collection of the extracted urls

    /**
     * Wrapper for the callback function. Has to be static, it calls the member function on the supplied object.
     *
     * @param *buffPtr Pointer to buffer of read characters
     * @param elementSize Size of the element in buffer
     * @param elementCount Number of elements
     * @param *userPtr Pointer to the parser object
     * 
     * @return elementSize*elementCount
     */
    static size_t callbackWrapper(void* buffPtr, size_t elementSize, size_t elementCount, void *userPtr);

    /**
     * Primitive method for fast scan of the url byte by byte. Finds references to files through 'href' and 'src' html tags.
     *
     * @param *buffPtr Pointer to buffer of read characters
     * @param elementSize Size of the element in buffer
     * @param elementCount Number of elements
     * @param *userPtr Not used here
     * 
     * @return elementSize*elementCount
     */
    size_t callbackParser(void* buffPtr, size_t elementSize, size_t elementCount, void *userPtr);

    /**
     * Compares the input character with the bufferElement. If equal, the lastChar variable is set to the desiredChar value.
     *
     * @param desireChar Character, that it should be
     * @param bufferElement Actual element from the buffer
     */
    void compareAndSetLastChar(char desiredChar, char bufferElement);

    /**
     * Prepares and allocates new url string.
     */
    void prepareNewUrl();
};

#endif  //PARSER_H
