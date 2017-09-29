/*
 * Util.cpp
 *
 *  Created on: Jun 2, 2017
 *      Author: jcassidy
 */

#include "Util.hpp"

#include <fstream>
#include <iostream>
#include <stdexcept>
using namespace std;


/** Read the entire contents of a file into memory
 */

pair<char*,char*> slurp(string fn)
{
	ifstream is(fn.c_str());

	if (!is.good())
		throw logic_error("slurp(fn) failed to open file");

	// slurp entire file
	is.seekg(0,ios_base::end);
	size_t Nb = is.tellg();
	is.seekg(0,ios_base::beg);

	char *buf = new char[Nb];
	is.read(buf,Nb);

	is.close();

	return make_pair(buf,buf+Nb);
}


/** Strips end of line comments starting with a single-character delimiter
 *
 */

char* stripEndOfLineComments(const pair<char*,char*> p,char delim)
{
	bool comment=false;

	const char* bufRead;
	char* bufWrite;

	for(bufRead=bufWrite=p.first; bufRead != p.second; ++bufRead)
	{
		if (*bufRead == delim)
			comment=true;
		else if (*bufRead == '\n')
			comment=false;

		if (!comment)
			*(bufWrite++) = *bufRead;
	}
	return bufWrite;

}

/** Smashes multiple space/tab/newlines into a single newline, and multiple space/tabs into a single space
 *
 */

char* compressWhitespace(const pair<char*,char*> p)
{
	unsigned spaces=0;
	unsigned newlines=0;

	const char* bufRead;
	char* bufWrite;

	// tokenize with spaces & newlines
	// squash multiple spaces/tabs/newlines into a single newline, and spaces/tabs into a single space
	for(bufRead=bufWrite=p.first; bufRead != p.second; ++bufRead)
	{
		if (*bufRead == ' ' || *bufRead == '\t')
			spaces++;
		else if (*bufRead == '\n')
			newlines++;
		else
		{
			if (newlines)
				*(bufWrite++) = '\n';
			else if (spaces)
				(*bufWrite++) = ' ';
			*(bufWrite++) = *bufRead;
			spaces=newlines=0;
		}
	}

	return bufWrite;
}
