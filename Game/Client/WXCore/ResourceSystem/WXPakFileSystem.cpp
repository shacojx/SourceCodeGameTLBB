/*
-----------------------------------------------------------------------------
This source file is part of OGRE
(Object-oriented Graphics Rendering Engine)
For the latest info, see http://www.ogre3d.org/

Copyright (c) 2000-2005 The OGRE Team
Also see acknowledgements in Readme.html

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
*/
#include "OgreStableHeaders.h"
#include "OgreLogManager.h"
#include "OgreException.h"
#include "OgreStringVector.h"
#include "OgreRoot.h"
#include "WXPakFileSystem.h"

#include <sys/types.h>
#include <sys/stat.h>

#if OGRE_PLATFORM == OGRE_PLATFORM_LINUX || OGRE_PLATFORM == OGRE_PLATFORM_APPLE
#   include "OgreSearchOps.h"
#   include <sys/param.h>
#   define MAX_PATH MAXPATHLEN
#endif

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
#   include <windows.h>
#   include <direct.h>
#   include <io.h>
#   include <mbstring.h>
#endif

namespace Ogre {

    //-----------------------------------------------------------------------
    PakFileSystemArchive::PakFileSystemArchive(const String& name, const String& archType )
        : Archive(name, archType)
    {
		mdwHeadLength = 0;
		mdwFileLength = 0;
	}
    //-----------------------------------------------------------------------
    bool PakFileSystemArchive::isCaseSensitive(void) const
    {
        #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            return false;
        #else
            return true;
        #endif

    }
    //-----------------------------------------------------------------------
    void PakFileSystemArchive::findFiles(const String& pattern, bool recursive, 
        StringVector* simpleList, FileInfoList* detailList, 
        const String& currentDir)
    {
		// parsing requires locking due to saved returns
		OGRE_LOCK_AUTO_MUTEX

		map<String, FILE_DESC >::iterator it;
		String pat = pattern;
		if( pat.empty() ) pat = "*";
		for( it = mPackedFileMap.begin(); it != mPackedFileMap.end(); ++ it )
		{
			if( StringUtil::match( String(it->second.szFileName), pat ) )
			{
				if( simpleList )
				{
					simpleList->push_back(currentDir + it->first);
				}
				else if (detailList)
				{
					FileInfo fi;
					fi.archive = this;
					fi.filename = currentDir + it->first;
					fi.basename = it->first;
					fi.path = currentDir;
					fi.compressedSize = it->second.dwSize;
					fi.uncompressedSize = it->second.dwSize;
					detailList->push_back(fi);
				}
			}
		}
    }
	//-----------------------------------------------------------------------
	char* PakFileSystemArchive::ConvertNumber( unsigned long num )
	{
		static char result[3] = {'x','0', 0};
		if( num >= 100 ) return "";

		DWORD b1 = num / 10;
		DWORD b2 = num % 10;

		switch( b1 )
		{
		case 0:
			result[0] = 'x';
			break;
		case 1:
			result[0] = 'y';
			break;
		case 2:
			result[0] = 'z';
			break;
		case 3:
			result[0] = 'h';
			break;
		case 4:
			result[0] = 'j';
			break;
		case 5:
			result[0] = 'l';
			break;
		case 6:
			result[0] = 'n';
			break;
		case 7:
			result[0] = 'p';
			break;
		case 8:
			result[0] = 'r';
			break;
		case 9:
			result[0] = 't';
			break;
		}

		switch( b2 )
		{
		case 0:
			result[1] = '0';
			break;
		case 1:
			result[1] = '1';
			break;
		case 2:
			result[1] = '2';
			break;
		case 3:
			result[1] = '3';
			break;
		case 4:
			result[1] = '4';
			break;
		case 5:
			result[1] = '5';
			break;
		case 6:
			result[1] = '6';
			break;
		case 7:
			result[1] = '7';
			break;
		case 8:
			result[1] = '8';
			break;
		case 9:
			result[1] = '9';
			break;
		}

		return result;
	}
	//-----------------------------------------------------------------------
    PakFileSystemArchive::~PakFileSystemArchive()
    {
        unload();
    }
    //-----------------------------------------------------------------------
    void PakFileSystemArchive::load()
    {
		mBasePath = mName;
		String strFileName = mBasePath + ".wx0";
		FILE* fp;
		fp = fopen( strFileName.c_str(), "rb" );
		if( !fp ) {
			OGRE_EXCEPT(Exception::ERR_FILE_NOT_FOUND, "File: " + strFileName + " not found.", "PakFileSystemArchive::load");
			return;
		}
		
		// check version
		char szVersion[5] = {0};
		fread( szVersion, 1, 4, fp );
		if( WXFileVersion != szVersion ) return;

		// read files...
		fread( &mdwHeadLength, 1, 4, fp );
		char* pHead = new char[mdwHeadLength];
		fread( pHead, 1, mdwHeadLength, fp );
		FILE_DESC fd;
		uchar Length;
		char* lpHead = pHead;
		char szFileName[256] = {0};
		unsigned long dwOffset = 0;
		unsigned long dwSize = 0;
		while( (ulong)( lpHead - pHead ) < mdwHeadLength )
		{
			Length = *lpHead ++;
			memset( &fd, 0, sizeof(FILE_DESC) );
			memcpy( szFileName, lpHead, Length ); lpHead += Length;
			DecryptName( szFileName, Length );
			memcpy( &dwOffset, lpHead, 4 ); lpHead += 4;
			DecryptOffset( dwOffset );
			memcpy( &dwSize, lpHead, 4 ); lpHead += 4;
			DecryptSize( dwSize );
			//memcpy( fd.szFileName, lpHead, Length ); lpHead += Length;
			//memcpy( &fd.dwOffset, lpHead, 4 ); lpHead += 4;
			//memcpy( &fd.dwSize, lpHead, 4 ); lpHead += 4;
			strcpy( fd.szFileName, szFileName );
			fd.dwOffset = dwOffset;
			fd.dwSize = dwSize;
			memcpy( &fd.dwCRC, lpHead, 4 ); lpHead += 4;
			mPackedFileMap.insert( mPackedFilePair( String(fd.szFileName), fd ) );
		}
		delete [] pHead;
		fclose( fp );

		int n;
		for( n = 1; n < 100; n ++ )
		{
			//String strPackName;
			//strPackName = mBasePath + ".p" + StringConverter::toString( n, 2, '0' );
			//FILE* fp = fopen( strPackName.c_str(), "rb" );
			//if( !fp ) return;
			//PACKAGE_DESC pd;
			//pd.lpFile = fp;
			//memset( pd.szPackName, 0, _MAX_PATH );
			//memcpy( pd.szPackName, strPackName.c_str(), strPackName.length() );
			//fread( &pd.dwAvailSize, 1, 4, fp );
			//fseek( fp, 0, SEEK_END );
			//pd.dwFileSize = (ulong)ftell( fp );
			//mPackedFiles.push_back( pd );
			String strPackName;
			strPackName = mBasePath + ".w" + ConvertNumber(n);

			PACKAGE_DESC pd;
			FILE* fp = fopen( strPackName.c_str(), "rb" );
			if( !fp ) return;
			fread( &pd.dwAvailSize, 1, 4, fp );
			fseek( fp, 0, SEEK_END );
			pd.dwFileSize = (ulong)ftell( fp );
			fclose( fp );

			HANDLE hFile = CreateFileA( strPackName.c_str(), GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, FILE_FLAG_SEQUENTIAL_SCAN, NULL );
			if( hFile == INVALID_HANDLE_VALUE ) return;
			pd.lpFileMap = CreateFileMapping( hFile, NULL, PAGE_READONLY, 0, pd.dwAvailSize, NULL );
			if( pd.lpFileMap == NULL ) return;
			CloseHandle( hFile );
			pd.lpFileAddr = (LPBYTE)MapViewOfFile( pd.lpFileMap, FILE_MAP_READ, 0, 0, pd.dwAvailSize );
			mPackedFiles.push_back( pd );
		}
	}
    //-----------------------------------------------------------------------
    void PakFileSystemArchive::unload()
    {
		vector<PACKAGE_DESC >::iterator it;
		for( it = mPackedFiles.begin(); it != mPackedFiles.end(); ++ it ) {
			//fclose( (*it).lpFile );
			UnmapViewOfFile( (*it).lpFileMap );
			CloseHandle( (*it).lpFileAddr );
		}
		mPackedFiles.resize(0);
    }
    //-----------------------------------------------------------------------
    DataStreamPtr PakFileSystemArchive::open(const String& filename) const
    {
		// directory change requires locking due to saved returns
		OGRE_LOCK_AUTO_MUTEX

		const char* cspl = "/";
		char* data = new char[filename.length()+1];
		memcpy( data, filename.c_str(), filename.length() );
		data[filename.length()]=0;
		_mbslwr( (uchar*)data );
		String search = data;
		delete [] data;
		while( (int)search.find( "\\" ) != -1 ) { search.replace( search.find( "\\" ), 1, cspl ); }
		map<String, FILE_DESC >::const_iterator fd;
		fd = mPackedFileMap.find( search );

		MemoryDataStream* stream = NULL;
		if( fd != mPackedFileMap.end() )
		{
			ulong dwFarOffset = 0;
			vector<PACKAGE_DESC >::const_iterator it;
			for( it = mPackedFiles.begin(); it != mPackedFiles.end(); ++ it ) {
				if( fd->second.dwOffset < dwFarOffset + (*it).dwAvailSize ) {
					//fseek( (*it).lpFile, fd->second.dwOffset - dwFarOffset + 4, SEEK_SET );
					//char* pData = new char[fd->second.dwSize];
					//ulong result = (ulong)fread( pData, 1, fd->second.dwSize, (*it).lpFile );
					//if( result == fd->second.dwSize )
					//{
					//	stream = new MemoryDataStream( filename, pData, fd->second.dwSize, true );
					//}
					//else
					//{
					//	delete [] pData;
					//}
					//return DataStreamPtr(stream);
					unsigned char* pData = (*it).lpFileAddr + fd->second.dwOffset - dwFarOffset + 4;
					stream = new MemoryDataStream( filename, pData, fd->second.dwSize );
					return DataStreamPtr(stream);
				}
				dwFarOffset += (*it).dwAvailSize;
			}
		}
		else
		{
			stream = NULL;
		}
        return DataStreamPtr(stream);
    }
    //-----------------------------------------------------------------------
    StringVectorPtr PakFileSystemArchive::list(bool recursive)
    {
		// directory change requires locking due to saved returns
		OGRE_LOCK_AUTO_MUTEX

        StringVectorPtr ret(new StringVector());

        findFiles("", recursive, ret.getPointer(), 0);

        return ret;
    }
    //-----------------------------------------------------------------------
    FileInfoListPtr PakFileSystemArchive::listFileInfo(bool recursive)
    {
		// directory change requires locking due to saved returns
		OGRE_LOCK_AUTO_MUTEX

        FileInfoListPtr ret(new FileInfoList());

        findFiles("", recursive, 0, ret.getPointer());

        return ret;
    }
    //-----------------------------------------------------------------------
    StringVectorPtr PakFileSystemArchive::find(const String& pattern, bool recursive)
    {
		// directory change requires locking due to saved returns
		OGRE_LOCK_AUTO_MUTEX

        StringVectorPtr ret(new StringVector());

		const char* cspl = "/";
		char* data = new char[pattern.length()+1];
		memcpy( data, pattern.c_str(), pattern.length() );
		data[pattern.length()]=0;
		_mbslwr( (uchar*)data );
		String search = data;
		delete [] data;
		while( (int)search.find( "\\" ) != -1 ) { search.replace( search.find( "\\" ), 1, cspl ); }
		findFiles(search, recursive, ret.getPointer(), 0);

        return ret;

    }
    //-----------------------------------------------------------------------
    FileInfoListPtr PakFileSystemArchive::findFileInfo(const String& pattern, 
        bool recursive)
    {
		// directory change requires locking due to saved returns
		OGRE_LOCK_AUTO_MUTEX

        FileInfoListPtr ret(new FileInfoList());

		const char* cspl = "/";
		char* data = new char[pattern.length()+1];
		memcpy( data, pattern.c_str(), pattern.length() );
		data[pattern.length()]=0;
		_mbslwr( (uchar*)data );
		String search = data;
		delete [] data;
		while( (int)search.find( "\\" ) != -1 ) { search.replace( search.find( "\\" ), 1, cspl ); }
		findFiles(search, recursive, 0, ret.getPointer());

        return ret;
    }
    //-----------------------------------------------------------------------
	bool PakFileSystemArchive::exists(const String& filename)
	{
		// directory change requires locking due to saved returns
		OGRE_LOCK_AUTO_MUTEX

		const char* cspl = "/";
		char* data = new char[filename.length()+1];
		memcpy( data, filename.c_str(), filename.length() );
		data[filename.length()]=0;
		_mbslwr( (uchar*)data );
		String search = data;
		delete [] data;
		while( (int)search.find( "\\" ) != -1 ) { search.replace( search.find( "\\" ), 1, cspl ); }
		map<String, FILE_DESC >::const_iterator fd;
		fd = mPackedFileMap.find( search );

		if( fd != mPackedFileMap.end() )
			return true;

		return false;
	}
	//-----------------------------------------------------------------------
	char* PakFileSystemArchive::DecryptName(char* name, int len)
	{
		int lo = len * 2;
		if( lo > 28 ) lo = 28;
		for( int i = 0; i < len; i ++ )
		{
			if( name[i] %2 ) //ÆæÊý
			{
				name[i] = name[i] + 22;
			}
			else //Å¼Êý
			{
				name[i] = name[i] - lo;
			}
		}
		name[len] = 0;
		return name;
	}
	//-----------------------------------------------------------------------
	void  PakFileSystemArchive::DecryptOffset(unsigned long& dwOffset)
	{
		unsigned short wh;
		unsigned short wl;

		wh = (unsigned short)( ( dwOffset & 0xFFFF0000 ) >> 16 );
		wl = (unsigned short)( dwOffset & 0x0000FFFF );

		wh -= 20;
		wl += 7;
		unsigned long dw = wl << 16;
		dwOffset = dw + wh;
	}
	//-----------------------------------------------------------------------
	void  PakFileSystemArchive::DecryptSize(unsigned long& dwSize)
	{
		unsigned char flag = (unsigned char)(dwSize & 0x0000000F);
		if( flag > 7 )
		{
			dwSize -= 0x00012340;
		}
		else
		{
			dwSize -= 0x00567800;
		}
	}
    //-----------------------------------------------------------------------
    const String& PakFileSystemArchiveFactory::getType(void) const
    {
        static String name = "WXFileSystem";
        return name;
    }

}
