/*
 * Util.hpp
 *
 *  Created on: Jun 2, 2017
 *      Author: jcassidy
 */

#ifndef STORAGE_TEXTFILE_UTIL_HPP_
#define STORAGE_TEXTFILE_UTIL_HPP_

#include <utility>
#include <string>

std::pair<char*,char*> slurp(std::string fn);
char* compressWhitespace(const std::pair<char*,char*> p);
char* stripEndOfLineComments(const std::pair<char*,char*> p,char delim);


#endif /* STORAGE_TEXTFILE_UTIL_HPP_ */
