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
#include "OgreFileSystem.h"
#include "OgreLogManager.h"
#include "OgreException.h"
#include "OgreStringVector.h"
#include "OgreRoot.h"

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
#endif

namespace Ogre {

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
    class FileMappingDataStream : public DataStream
    {
    public:
        FileMappingDataStream(const String& name, const String& filename);
        ~FileMappingDataStream();

		/// @copydoc DataStream::read
		size_t read(void* buf, size_t count);
		/// @copydoc DataStream::readLine
		size_t readLine(char* buf, size_t maxCount, const String& delim = "\n");
		/// @copydoc DataStream::skipLine
		size_t skipLine(const String& delim = "\n");
		/// @copydoc DataStream::skip
		void skip(long count);
		/// @copydoc DataStream::seek
	    void seek( size_t pos );
		/// @copydoc DataStream::tell
		size_t tell(void) const;
		/// @copydoc DataStream::eof
	    bool eof(void) const;
        /// @copydoc DataStream::close
        void close(void);

    protected:
        /// Pointer to the start of the data area
	    uchar* mData;
        /// Pointer to the current position in the memory
	    uchar* mPos;
        /// Pointer to the end of the memory
	    uchar* mEnd;
        /// The file object handle
        HANDLE mFileHandle;
        /// The file mapping object handle
        HANDLE mFileMappingHandle;
    };
    //-----------------------------------------------------------------------
    FileMappingDataStream::FileMappingDataStream(const String& name, const String& filename)
        : DataStream(name)
        , mData(0)
        , mPos(0)
        , mEnd(0)
        , mFileHandle(INVALID_HANDLE_VALUE)
        , mFileMappingHandle(0)
    {
        mFileHandle = CreateFile(
            filename.c_str(),
            GENERIC_READ,
            FILE_SHARE_READ,
            NULL,
            OPEN_EXISTING,
            FILE_ATTRIBUTE_NORMAL,
            NULL);

        mSize = GetFileSize(mFileHandle, NULL);

        mFileMappingHandle = CreateFileMapping(
            mFileHandle,
            NULL,
            PAGE_READONLY,
            0,
            0,
            NULL);

        mData = (uchar*) MapViewOfFile(
            mFileMappingHandle,
            FILE_MAP_READ,
            0,
            0,
            0);
        mEnd = mData + mSize;
        mPos = mData;

        if (!mData ||
            mFileHandle == INVALID_HANDLE_VALUE ||
            !mFileMappingHandle ||
            mSize == 0xFFFFFFFF)
        {
            close();

            OGRE_EXCEPT(Exception::ERR_FILE_NOT_FOUND,
                "Cannot open file: " + filename,
                "FileMappingDataStream::FileMappingDataStream");
        }
    }
    //-----------------------------------------------------------------------
    FileMappingDataStream::~FileMappingDataStream()
    {
        close();
    }
    //-----------------------------------------------------------------------
    size_t FileMappingDataStream::read(void* buf, size_t count)
    {
        size_t cnt = count;
        // Read over end of memory?
        if (mPos + cnt > mEnd)
            cnt = mEnd - mPos;
        if (cnt == 0)
            return 0;

        memcpy(buf, mPos, cnt);
        mPos += cnt;
        return cnt;
    }
    //-----------------------------------------------------------------------
    size_t FileMappingDataStream::readLine(char* buf, size_t maxCount, 
        const String& delim)
    {
        // Deal with both Unix & Windows LFs
		bool trimCR = false;
		if (delim.find_first_of('\n') != String::npos)
		{
			trimCR = true;
		}

        size_t pos = 0;

        // Make sure pos can never go past the end of the data 
        while (pos < maxCount && mPos < mEnd)
        {
            if (delim.find(*mPos) != String::npos)
            {
                // Trim off trailing CR if this was a CR/LF entry
                if (trimCR && pos && buf[pos-1] == '\r')
                {
                    // terminate 1 character early
                    --pos;
                }

                // Found terminator, skip and break out
                ++mPos;
                break;
            }

            buf[pos++] = *mPos++;
        }

        // terminate
        buf[pos] = '\0';

        return pos;
    }
    //-----------------------------------------------------------------------
    size_t FileMappingDataStream::skipLine(const String& delim)
    {
        size_t pos = 0;

        // Make sure pos can never go past the end of the data 
        while (mPos < mEnd)
        {
            ++pos;
            if (delim.find(*mPos++) != String::npos)
            {
                // Found terminator, break out
                break;
            }
        }

        return pos;

    }
    //-----------------------------------------------------------------------
    void FileMappingDataStream::skip(long count)
    {
        size_t newpos = (size_t)( ( mPos - mData ) + count );
        assert( mData + newpos <= mEnd );        

        mPos = mData + newpos;
    }
    //-----------------------------------------------------------------------
    void FileMappingDataStream::seek( size_t pos )
    {
        assert( mData + pos <= mEnd );
        mPos = mData + pos;
    }
    //-----------------------------------------------------------------------
    size_t FileMappingDataStream::tell(void) const
	{
		//mData is start, mPos is current location
		return mPos - mData;
	}
	//-----------------------------------------------------------------------
    bool FileMappingDataStream::eof(void) const
    {
        return mPos >= mEnd;
    }
    //-----------------------------------------------------------------------
    void FileMappingDataStream::close(void)
    {
        if (mData)
        {
            UnmapViewOfFile(mData);
            mData = 0;
        }
        mPos = 0;
        mEnd = 0;

        if (mFileMappingHandle)
        {
            CloseHandle(mFileMappingHandle);
            mFileMappingHandle = 0;
        }

        if (mFileHandle != INVALID_HANDLE_VALUE)
        {
            CloseHandle(mFileHandle);
            mFileHandle = INVALID_HANDLE_VALUE;
        }
    }
    //-----------------------------------------------------------------------
#endif // OGRE_PLATFORM == OGRE_PLATFORM_WIN32


    //-----------------------------------------------------------------------
    FileSystemArchive::FileSystemArchive(const String& name, const String& archType )
        : Archive(name, archType)
    {
    }
    //-----------------------------------------------------------------------
    bool FileSystemArchive::isCaseSensitive(void) const
    {
        #if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
            return false;
        #else
            return true;
        #endif

    }
    //-----------------------------------------------------------------------
    static bool is_reserved_dir (const char *fn)
    {
        return (fn [0] == '.' && (fn [1] == 0 || (fn [1] == '.' && fn [2] == 0)));
    }
    //-----------------------------------------------------------------------
    static bool is_absolute_dir(const char* dir)
    {
#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        if (isalpha(uchar(dir[0])) && dir[1] == ':')
            return true;
#endif
        return dir[0] == '/' || dir[0] == '\\';
    }
    //-----------------------------------------------------------------------
    static String concatenate_path(const String& base, const String& name)
    {
        if (base.empty() || is_absolute_dir(name.c_str()))
            return name;
        else
            return base + '/' + name;
    }
    //-----------------------------------------------------------------------
    void FileSystemArchive::findFiles(const String& pattern, bool recursive, 
        bool dirs, StringVector* simpleList, FileInfoList* detailList)
    {
        long lHandle, res;
        struct _finddata_t tagData;

        // pattern can contain a directory name, separate it from mask
        size_t pos1 = pattern.rfind ('/');
        size_t pos2 = pattern.rfind ('\\');
        if (pos1 == pattern.npos || ((pos2 != pattern.npos) && (pos1 < pos2)))
            pos1 = pos2;
        String directory;
        if (pos1 != pattern.npos)
            directory = pattern.substr (0, pos1 + 1);

        String full_pattern = concatenate_path(mName, pattern);

        lHandle = _findfirst(full_pattern.c_str(), &tagData);
        res = 0;
        while (lHandle != -1 && res != -1)
        {
            if ((dirs == ((tagData.attrib & _A_SUBDIR) != 0)) &&
                (!dirs || !is_reserved_dir (tagData.name)))
            {
                if (simpleList)
                {
                    simpleList->push_back(directory + tagData.name);
                }
                else if (detailList)
                {
                    FileInfo fi;
                    fi.archive = this;
                    fi.filename = directory + tagData.name;
                    fi.basename = tagData.name;
                    fi.path = directory;
                    fi.compressedSize = tagData.size;
                    fi.uncompressedSize = tagData.size;
                    detailList->push_back(fi);
                }
            }
            res = _findnext( lHandle, &tagData );
        }
        // Close if we found any files
        if(lHandle != -1)
            _findclose(lHandle);

        // Now find directories
        if (recursive)
        {
            String base_dir = mName;
            if (!directory.empty ())
            {
                base_dir = concatenate_path(mName, directory);
                // Remove the last '/'
                base_dir.erase (base_dir.length () - 1);
            }
            base_dir.append ("/*");

            // Remove directory name from pattern
            String mask ("/");
            if (pos1 != pattern.npos)
                mask.append (pattern.substr (pos1 + 1));
            else
                mask.append (pattern);

            lHandle = _findfirst(base_dir.c_str (), &tagData);
            res = 0;
            while (lHandle != -1 && res != -1)
            {
                if ((tagData.attrib & _A_SUBDIR) &&
                    !is_reserved_dir (tagData.name))
                {
                    // recurse
                    base_dir = directory;
                    base_dir.append (tagData.name).append (mask);
                    findFiles(base_dir, recursive, dirs, simpleList, detailList);
                }
                res = _findnext( lHandle, &tagData );
            }
            // Close if we found any files
            if(lHandle != -1)
                _findclose(lHandle);
        }
    }
    //-----------------------------------------------------------------------
    FileSystemArchive::~FileSystemArchive()
    {
        unload();
    }
    //-----------------------------------------------------------------------
    void FileSystemArchive::load()
    {
        // do nothing here, what has to be said will be said later
    }
    //-----------------------------------------------------------------------
    void FileSystemArchive::unload()
    {
        // nothing to see here, move along
    }
    //-----------------------------------------------------------------------
    DataStreamPtr FileSystemArchive::open(const String& filename) const
    {
        String full_path = concatenate_path(mName, filename);

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        return DataStreamPtr(new FileMappingDataStream(filename, full_path));
#else
        // Use filesystem to determine size 
        // (quicker than streaming to the end and back)
        struct stat tagStat;
	int ret = stat(full_path.c_str(), &tagStat);
        assert(ret == 0 && "Problem getting file size" );

        // Always open in binary mode
        std::ifstream *origStream = new std::ifstream();
        origStream->open(full_path.c_str(), std::ios::in | std::ios::binary);

        // Should check ensure open succeeded, in case fail for some reason.
        if (origStream->fail())
        {
            delete origStream;
            OGRE_EXCEPT(Exception::ERR_FILE_NOT_FOUND,
                "Cannot open file: " + filename,
                "FileSystemArchive::open");
        }

        /// Construct return stream, tell it to delete on destroy
        FileStreamDataStream* stream = new FileStreamDataStream(filename,
            origStream, tagStat.st_size, true);
        return DataStreamPtr(stream);
#endif
    }
    //-----------------------------------------------------------------------
    StringVectorPtr FileSystemArchive::list(bool recursive)
    {
		// directory change requires locking due to saved returns
        StringVectorPtr ret(new StringVector());

        findFiles("*", recursive, false, ret.getPointer(), 0);

        return ret;
    }
    //-----------------------------------------------------------------------
    FileInfoListPtr FileSystemArchive::listFileInfo(bool recursive)
    {
        FileInfoListPtr ret(new FileInfoList());

        findFiles("*", recursive, false, 0, ret.getPointer());

        return ret;
    }
    //-----------------------------------------------------------------------
    StringVectorPtr FileSystemArchive::find(const String& pattern, bool recursive)
    {
        StringVectorPtr ret(new StringVector());

        findFiles(pattern, recursive, false, ret.getPointer(), 0);

        return ret;

    }
    //-----------------------------------------------------------------------
    FileInfoListPtr FileSystemArchive::findFileInfo(const String& pattern, 
        bool recursive)
    {
        FileInfoListPtr ret(new FileInfoList());

        findFiles(pattern, recursive, false, 0, ret.getPointer());

        return ret;
    }
    //-----------------------------------------------------------------------
	bool FileSystemArchive::exists(const String& filename)
	{
        String full_path = concatenate_path(mName, filename);

#if OGRE_PLATFORM == OGRE_PLATFORM_WIN32
        WIN32_FIND_DATA findData;
        HANDLE handle = FindFirstFile(full_path.c_str(), &findData);
        if (handle == INVALID_HANDLE_VALUE)
            return false;
        FindClose(handle);
        return true;
#else
        struct stat tagStat;
        bool ret = (stat(full_path.c_str(), &tagStat) == 0);

		return ret;
#endif
	}
    //-----------------------------------------------------------------------
    const String& FileSystemArchiveFactory::getType(void) const
    {
        static String name = "FileSystem";
        return name;
    }

}
