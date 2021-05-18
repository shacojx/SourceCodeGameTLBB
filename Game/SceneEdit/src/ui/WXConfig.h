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

#ifndef PE_CONFIG_H
#define PE_CONFIG_H

#include <string>
#include <map>
#include <vector>

const std::string EMPTY_STRING("");

//! Is \c str a member of \c list?
bool isMemberOf(std::string str, std::vector<std::string> * list);

inline std::string trim_right( const std::string& source, const std::string& t = " " )
{
	std::string str = source;
	return str.erase( str.find_last_not_of( t ) + 1 );
}

inline std::string trim_left( const std::string& source, const std::string& t = " " )
{
	std::string str = source;
	return str.erase( 0, source.find_first_not_of( t ) );
}

inline std::string trim( const std::string& source, const std::string& t = " " )
{
	std::string str = source;
	return trim_left( trim_right( str , t ), t );
}


//! \brief read configuration files
//!
//! Format:
//! [section]
//! key = value
//!
//! Names of sections and keys are not allowed to contain whitespaces.
//! Whitespaces are allowed between key, '=' and value.
//!
//! An assert tetst, if key exists. So in debug mode the program will exit on a not existing key,
//! while a release program will just return 0, 0.0f or "" (empty string).
//!
//! Usage:
//! \code
//! IniConfig* conf = new IniConfig( Filename );
//! int value1 = conf->getInt("section1", "key1");
//! string value2 = conf->getString("section2", "key2");
//! \endcode
class FairyConfig
{
public:
	FairyConfig();
	FairyConfig( const std::string& filename );
	~FairyConfig();

	//! Set to true if an error occurred.
	bool isError() const { return mIsError; }

	//! Returns the last error.
	const std::string& lastError() const { return mLastError; }

	//! Deletes all stored sections and all their keys and values. Note that this invalidates
	//! all references to strings previously returned by all flavours of the
	//! getString() functions.
	void clear();

	//! Initialises the IniConfig object with data from the given file.
	//!
	//! setting merge to true allows to mix data from repeated load operations.
	//! Note that multiple entries with the same key are not possible in merge-mode!
	int load( const std::string& filename, bool merge = false );

	//! Writes the contents of the IniConfig object to the given file.
	int save( const std::string& filename );

	//! Overwrites the file from which the config has been read. Note that all comments are lost.
	int save() { return save( mFilename ); }

	//! Returns a string representation for the configuration.
	const std::string toString() const;

	const std::string& getString( const std::string& section, const std::string& key, const std::string& defaultValue = "" );
	const float getFloat( const std::string& section, const std::string& key, const float defaultValue = 0.0f );
	int getInt( const std::string& section, const std::string& key, const int defaultValue = 0 );
	bool getBool( const std::string& section, const std::string& key, const bool defaultValue = false );

	std::vector<std::string> getList( const std::string& section, const std::string& key, const std::string& delimiter = "," );

	//! Returns true if section exists otherwise false.
	const bool exists( const std::string& section );

	//! Returns true if key exists in section otherwise false.
	const bool exists( const std::string& section, const std::string& key);

	//! Removes one matching entry from the config
	void removeEntry( const std::string& section, const std::string& key );

	//! Prepares to get all values from the given section and key.
	//! \return The number of elements in the given section with the key.
	int startGetMultiString( const std::string& section, const std::string& key );

	//! Fetches a single string after
	const std::string& getNextMultiString();

	//! Returns the number of keys in the given section.
	int keys( const std::string& section ) { return mSections[section].size(); }

	const int startGetKeys( const std::string& section );
	const std::string& getNextKey();

	const int startGetSections( const std::string& start );

	const int startGetSections();
	const std::string& getNextSection();

protected:

	bool mIsError;
	std::string mLastError;
	std::string mFilename;

	typedef std::multimap<std::string,std::string> StringHash;
	typedef std::map<std::string,StringHash> StringHashHash;
	StringHashHash mSections;

	std::pair<StringHash::iterator,StringHash::iterator> mKeyRange;
	StringHash::iterator mKeyRangeIterator;
	bool mKeyRangeIteratorValid;
	std::string mKeyRangeSection;

	StringHashHash::iterator mSectionIterator;
	StringHashHash::iterator mSectionRangeEnd;

	StringHash::iterator mKeyIterator;
	std::string mKeySection;
};

#endif
