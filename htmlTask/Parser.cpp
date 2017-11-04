/*
 * author: Jiri Burant
 * date created: 14.9.2017
*/


#include "Parser.h"
#include <iostream>

Parser::Parser() : lastChar('0'), isPartOfUrl(false), currentUrlIndex(0)
{
  curl_global_init(CURL_GLOBAL_ALL);
}

Parser::~Parser()
{
  urls.clear();
}

size_t Parser::callbackWrapper(void* buffPtr, size_t elementSize, size_t elementCount, void *userPtr)
{
  Parser *parserPtr = static_cast<Parser*> (userPtr);
  return parserPtr->callbackParser(buffPtr, elementSize, elementCount, userPtr);
}

unsigned int Parser::parseUrl(std::string inputUrl)
{
  CURL *curl;
  originalUrl = inputUrl;

  curl = curl_easy_init();

  curl_easy_setopt(curl, CURLOPT_URL, inputUrl.c_str());
  curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, callbackWrapper);
  curl_easy_setopt(curl, CURLOPT_WRITEDATA, this);
  curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);

  curl_easy_perform(curl);
  curl_easy_cleanup(curl);

  return urls.size();
}

size_t Parser::callbackParser(void* buffPtr, size_t elementSize, size_t elementCount, void *userPtr)
{
  size_t realSize = elementCount * elementSize;
  char *buffer = static_cast<char *> (buffPtr);

  for(int i=0; i < elementCount; i++)
  {  
    //Flag isPartOfUrl is set -> add this character to current url
    if(isPartOfUrl)
    {
      if('"' != buffer[i])
      { 
	(*urls.at(currentUrlIndex)) += buffer[i];
      }
      else
      {
        isPartOfUrl= false;
      }
    }
    else
    {
	  //Ignore whitespace
      if(' ' == buffer[i])
      {
        continue;
      }
      //lastChar was important, continue with parsing
      if('0' != lastChar)
      {      
        switch(lastChar)
	  {
	  case 'h':
            {              
	      compareAndSetLastChar('r', buffer[i]);
	      break;
            }
	  case 's':
            {
	    compareAndSetLastChar('r', buffer[i]);
	    break;
            }
          case 'r':
            {
            if ('e' == buffer[i])
            {
              lastChar = buffer[i];
            }else
            {
              compareAndSetLastChar('c', buffer[i]);
            }
	    break;
            }
          case 'e':
            {
	      compareAndSetLastChar('f', buffer[i]);
	      break;
            }
	  case 'f':
            {
            compareAndSetLastChar('=', buffer[i]);
	    break;
            }
	  case 'c':
            {
            compareAndSetLastChar('=', buffer[i]);
	    break;
            }
          case '=':
            {
            compareAndSetLastChar('"', buffer[i]);
	    break;
            }
			//New url begins, allocate new stringand add it to url collection
          case '"':	
            prepareNewUrl();
            if('/' == buffer[i])
            {
              (*urls.at(currentUrlIndex)) += originalUrl.c_str();              
            }
            (*urls.at(currentUrlIndex)) += buffer[i];
            break;
          default:
            {
	    lastChar = '0';
            }
	}
      }
	  //lastChar is not significant. Check if new interesting tag begins (checks for "href" and "src" tags)
      else if('h' == buffer[i] || 's' == buffer[i])
      {
        lastChar = buffer[i];
      }  
    }
  }

  return realSize;
}


inline void Parser::compareAndSetLastChar(char desiredChar, char bufferElement)
{
  if(desiredChar == bufferElement)
  {
    lastChar = desiredChar;
  }
  else
  {
    lastChar = '0';
  }
}

inline void Parser::prepareNewUrl()
{
  std::string *newUrl;
  lastChar = 0;
  currentUrlIndex++;
  newUrl = new std::string();
  newUrl->reserve(estMaxUrlSize);
  urls[currentUrlIndex] = newUrl;
  isPartOfUrl = true;
}

std::unordered_map<unsigned int, std::string*> Parser::getUrls() const
{
  return this->urls;
}


