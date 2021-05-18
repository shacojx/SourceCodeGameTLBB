/*
-----------------------------------------------------------------------------
This source	file is	part of	the	OGRE particle editor
For	the	latest info, see http://www.ogre3d.org/

This program is free software; you can redistribute it and/or modify it under
the terms of the GNU Lesser General Public License as published by the Free Software
Foundation; either version 2 of the License, or (at your option) any later
version.

This program is distributed in the hope that it will be useful, but WITHOUT
ANY WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS
FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public License for more details.

You should have received a copy of the GNU Lesser General Public License along with
this program; if not, write to the Free Software Foundation, Inc., 59 Temple
Place - Suite 330, Boston, MA 02111-1307, USA, or go to
http://www.gnu.org/copyleft/lesser.txt.
-----------------------------------------------------------------------------
//!
//! \brief simple key=value config files grouped in sections
//!
//! \author <piet AT gamecat DOT de>
//!
*/

#include <fstream>
#include <sstream>
#include "assert.h"

#include "WXConfig.h"


bool isMemberOf(std::string s, std::vector<std::string> * list)
{
	if (!list)
		return false;

	std::vector<std::string>::iterator it;
	for (it = list->begin(); it != list->end(); it++)
		if (*it == s)
			return true;
	return false;
}


using namespace std;


//----------------------------------------------------------------------------
FairyConfig::FairyConfig()
{
	mIsError = false;
	mKeyRangeIteratorValid = false;
}

//----------------------------------------------------------------------------
FairyConfig::FairyConfig( const string& filename )
  : mIsError(false),
	mKeyRangeIteratorValid(false)
{
	load( filename );
}

//----------------------------------------------------------------------------
FairyConfig::~FairyConfig()
{
	clear();
}

//----------------------------------------------------------------------------
void FairyConfig::clear()
{
	for( StringHashHash::iterator it = mSections.begin(); it != mSections.end(); it++)
	{
		(*it).second.clear();
	}

	mSections.clear();
	mIsError = false;
	mKeyRangeIteratorValid = false;
}


//----------------------------------------------------------------------------
int FairyConfig::save( const std::string& filename )
{
	ofstream outfile( filename.c_str() );

	if ( outfile.good() )
	{
		outfile << toString();
		outfile.close();
		return 0;
	}
	else
	{
		return 1;
	}
}

//----------------------------------------------------------------------------
int FairyConfig::load( const std::string& filename, bool merge )
{
	std::string line;
	char errBuff[1024];
	int lineNum = 0;
	string sectionName = "default";

	if ( !merge )
	{
		clear();
	}

	ifstream infile( filename.c_str() );

	if ( !infile.good() )
	{
		mIsError = true;
		mLastError = "load(): cannot open " + filename;
		return 1;
	}

	while ( infile.good() )
	{
		std::stringstream ss;
		std::getline(infile, line);
		++lineNum;

		// Remove line endings
		if(!line.empty())
		{
			std::string::iterator e(line.end() - 1);
			if(*e == '\r')
				line.erase(e);
		}

		if ( ( line[0] == ';' ) || ( line[0] == '#' ) )
		{
			// skip comments
		}
		else
		{
			// is this line the start of a new section?
			if ( line[0] == '[' )
			{
				// find the closing bracket
				int pos = 1;
				for ( ; (line[pos] != ']') && (pos < (int)line.length()); pos++ ) ;
				// found it?
				if ( pos == (int)line.length() )
				{
					// presumably an error, no matching ] found
					sprintf( errBuff, "FairyConfig::load(%s): syntax error in line %d: broken section definition: %s", filename.c_str(), lineNum, line.c_str() );
					mIsError = true;
					mLastError = errBuff;
					infile.close();
					return 1;
				}
				else
				{
					line[pos] = 0x00;
					sectionName = &line[1];
					StringHash newSection;
					mSections.insert( pair<string,StringHash>(sectionName, newSection) );
				}

			}
			else
			{
				// filter all whitespaces
				string filter_chars("\t");
				string::size_type pos = 0;
				while ( (pos = line.find_first_of(filter_chars, pos)) != string::npos )
				{
					line.erase( pos, 1);
				}

				if ( line.size() > 0 ) // skip empty lines
				{
					string::size_type delim;
					delim = line.find_first_of('=', 0);
					if ( delim != string::npos )
					{
						string key( line, 0, delim );
						string value( line, delim + 1, line.length() - delim - 1 );

						// get rid of spaces...
						key = trim( key );
						value = trim( value );

						if ( merge )
						{
							// don't allow multiple entries with the same key in merge-mode
							if ( exists( sectionName, key ) )
							{
								removeEntry( sectionName, key );
							}
						}

						mSections[sectionName].insert( pair<string,string>(key,value) );
					}
				}
			}
		}
	}

	infile.close();
	mFilename = filename;
	return 0;
} // load()


//----------------------------------------------------------------------------
const std::string& FairyConfig::getString( const std::string& section, const std::string& key, const std::string& defaultValue )
{
	if ( exists(section) )
	{
		StringHash::iterator it = mSections[section].find( key );
		if ( it != mSections[section].end() )
		{
			return (*it).second;
		}
	}
	return defaultValue;
}

//----------------------------------------------------------------------------
const float FairyConfig::getFloat( const std::string& section, const std::string& key, const float defaultValue )
{
	if ( exists(section) )
	{
		StringHash::iterator it = mSections[section].find( key );
		if ( it != mSections[section].end() )
		{
			float retVal;
			stringstream ss( (*it).second );
			ss >> retVal;
			return retVal;
		}
	}
	return 0.0f;
}

//----------------------------------------------------------------------------
int FairyConfig::getInt( const std::string& section, const std::string& key, const int defaultValue )
{
	if ( exists(section) )
	{
		StringHash::iterator it = mSections[section].find( key );
		if ( it != mSections[section].end() )
		{
			int retVal;
			stringstream ss( (*it).second );
			ss >> retVal;
			return retVal;
		}
	}
	return defaultValue;
}


//----------------------------------------------------------------------------
bool FairyConfig::getBool( const std::string& section, const std::string& key, const bool defaultValue )
{
	if ( exists(section) )
	{
		StringHash::iterator it = mSections[section].find( key );
		if ( it != mSections[section].end() )
		{
			if ( (*it).second.compare("true") == 0 )
				return true;
			if ( (*it).second.compare("false") == 0 )
				return false;
		}
	}
	return defaultValue;
}

//----------------------------------------------------------------------------
std::vector<std::string> FairyConfig::getList( const std::string& section, const std::string& key, const std::string& delimiter )
{
	vector<std::string> list;

	if ( exists(section) )
	{
		StringHash::iterator it = mSections[section].find( key );
		if ( it != mSections[section].end() )
		{
			std::string value = (*it).second;

			// split
			unsigned int last = value.find_first_not_of( delimiter, 0 );
			unsigned int pos = value.find_first_of( delimiter, last );

			while (last < pos)
			{
				list.push_back( trim( value.substr( last, pos - last ) ) );
				last = value.find_first_not_of( delimiter, pos );
				pos = value.find_first_of( delimiter, last );
			}
		}
	}

	return list;
}

//----------------------------------------------------------------------------
const bool FairyConfig::exists( const std::string& section )
{
	StringHashHash::iterator it = mSections.find( section );
	return it != mSections.end();
}

//----------------------------------------------------------------------------
const bool FairyConfig::exists( const std::string& section, const std::string& key )
{
	StringHashHash::iterator it = mSections.find( section );
	if( it == mSections.end() )
		return false;

	StringHash sh = (*it).second;
	StringHash::iterator it2 = sh.find( key );
	return it2 != sh.end();
}

//----------------------------------------------------------------------------
void FairyConfig::removeEntry( const std::string& section, const std::string& key )
{
	StringHashHash::iterator it = mSections.find( section );
	if( it == mSections.end() )
		return;

	StringHash sh = (*it).second;
	StringHash::iterator it2 = sh.find( key );
	if ( it2 == sh.end() )
		return;

	mSections[section].erase( key );
}

//----------------------------------------------------------------------------
const std::string FairyConfig::toString() const
{
	ostringstream   outputStream;

	for( StringHashHash::const_iterator it = mSections.begin(); it != mSections.end(); it++)
	{
		outputStream << "[" << (*it).first << "]" << endl;
		StringHash sh = (*it).second;
		for( StringHash::iterator it2 = sh.begin(); it2 != sh.end(); it2++)
		{
			outputStream << (*it2).first << " = " << (*it2).second << endl;
		}
		outputStream << endl;
	}

	return outputStream.str();
}

//----------------------------------------------------------------------------
int FairyConfig::startGetMultiString( const std::string & section, const std::string & key )
{
	StringHashHash::iterator it = mSections.find( section );

	if ( it != mSections.end() )
	{
		// ok, section exists
		int count = mSections[section].count( key );
		if ( count > 0 )
		{
			// at least on element with the given key exists
			mKeyRange = mSections[section].equal_range( key );
			mKeyRangeIterator = mKeyRange.first;
			mKeyRangeIteratorValid = true;
			mKeyRangeSection = section;
			return count;
		}
		mKeyRangeIterator = mSections[section].end();
	}
	// section or key doesn't exist
	mKeyRangeIteratorValid = false;
	return 0;
}

//----------------------------------------------------------------------------
const std::string& FairyConfig::getNextMultiString()
{
	assert ( mKeyRangeIteratorValid && "getNextMultiString(): tried to access multi without call to startGetMultiString()" );

	if ( mKeyRangeIterator != mSections[mKeyRangeSection].end() )
	{
		std::string& retVal = (*mKeyRangeIterator).second;
		if ( mKeyRangeIterator != mKeyRange.second )
		{
			// make the iterator point to the next element
			++mKeyRangeIterator;
		}
		else
		{
			// we have reached the end
			mKeyRangeIterator = mSections[mKeyRangeSection].end();
			mKeyRangeIteratorValid = false;
		}
		return retVal;
	}
	return EMPTY_STRING;
}

//----------------------------------------------------------------------------
const int FairyConfig::startGetKeys( const std::string & section )
{
	if ( exists(section) )
	{
		mKeySection = section;
		mKeyIterator = mSections[section].begin();
		return keys(section);
	}
	return 0;
}

//----------------------------------------------------------------------------
const string& FairyConfig::getNextKey()
{
	if ( mKeyIterator != mSections[mKeySection].end() )
	{
		const string& ret = (*mKeyIterator).first;
		++mKeyIterator;
		return ret;
	}
	return EMPTY_STRING;
}

//----------------------------------------------------------------------------
const int FairyConfig::startGetSections( )
{
	mSectionIterator = mSections.begin();
	mSectionRangeEnd = mSections.end();
	return mSections.size();
}

//----------------------------------------------------------------------------
const string& FairyConfig::getNextSection( )
{
	if ( mSectionIterator != mSectionRangeEnd )
	{
		const std::string& ret = (*mSectionIterator).first;
		++mSectionIterator;
		return ret;
	}
	return EMPTY_STRING;
}

//----------------------------------------------------------------------------
const int FairyConfig::startGetSections( const string& beginning )
{
	int size = 0;
	bool foundFirst = false;
	StringHashHash::iterator it = mSections.begin();
	while ( it != mSections.end() )
	{
		if ( 0 == (*it).first.compare( 0, beginning.size(), beginning )  )
		{
			// we have an element with a matching key, is it the first?
			if ( !foundFirst )
			{
				//  set the iterator to the first matching element
				mSectionIterator = it;
				foundFirst = true;
			}
			mSectionRangeEnd = it;
			++size;
		}
		else
		{
			// the beginning of the key isn't like beginning
			if ( foundFirst )
			{
				// this is the first element following the element(s) that we were
				// looking for. We can stop here.
				break;
			}
		}
		++it;
	}
	if ( foundFirst )
	{
		++mSectionRangeEnd;
	}
	return size;
}

// THE END