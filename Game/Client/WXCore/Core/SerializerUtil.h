#ifndef __SERIALIZERUTIL_H__
#define __SERIALIZERUTIL_H__

#include "WXPrerequisites.h"
#include <OgreString.h>
#include <OgreDataStream.h>

namespace WX {

    // Endian routines

    extern void flipToLittleEndian(void* pData, size_t size, size_t count = 1);
    extern void flipFromLittleEndian(void* pData, size_t size, size_t count = 1);

    extern void flipEndian(void * pData, size_t size, size_t count);
    extern void flipEndian(void * pData, size_t size);

    //////////////////////////////////////////////////////////////////////////

    class Writer {
    public:
        virtual ~Writer() {}

        void writeFloats(const float* pFloat, size_t count = 1);
        void writeFloats(const double* pDouble, size_t count = 1);
        void writeShorts(const uint16* pShort, size_t count = 1);
        void writeInts(const uint32* pInt, size_t count = 1);
        void writeBools(const bool* pBool, size_t count = 1);

        void writeString(const String& string);

        void writeObject(const Ogre::Vector3& v);
        void writeObject(const Ogre::Quaternion& q);

        void writeData(const void* data, size_t size);

    protected:
        virtual void _write(const void* buf, size_t size, size_t count) = 0;
    };

    //////////////////////////////////////////////////////////////////////////

    class Reader {
    public:
        virtual ~Reader() {}

        void readFloats(float* pFloat, size_t count = 1);
        void readFloats(double* pDouble, size_t count = 1);
        void readShorts(uint16* pShort, size_t count = 1);
        void readInts(uint32* pInt, size_t count = 1);
        void readBools(bool* pBool, size_t count = 1);

        String readString(void);
        String readString(size_t numChars);

        void readObject(Ogre::Vector3& v);
        void readObject(Ogre::Quaternion& q);

        void readData(void* data, size_t size);

    protected:
        virtual void _read(void* buf, size_t size, size_t count) = 0;
    };

    //////////////////////////////////////////////////////////////////////////

    class FileWriter : public Writer {
    private:
        FileWriter(const FileWriter&);
        FileWriter& operator= (const FileWriter&);

    public:
        explicit FileWriter(FILE* file, bool adopts = false)
            : mFile(file)
            , mAdopts(adopts)
        {
        }

        explicit FileWriter(const String& filename)
            : mFile(fopen(filename.c_str(), "wb"))
            , mAdopts(true)
        {
        }

        ~FileWriter()
        {
            if (mAdopts && mFile)
                fclose(mFile);
        }

        FILE* getFile(void) const
        {
            return mFile;
        }

    protected:
        FILE* mFile;
        bool mAdopts;

        void _write(const void* buf, size_t size, size_t count);
    };

    //////////////////////////////////////////////////////////////////////////

    class DataStreamReader : public Reader {
    private:
        DataStreamReader(const DataStreamReader&);
        DataStreamReader& operator= (const DataStreamReader&);

    public:
        explicit DataStreamReader(Ogre::DataStreamPtr& stream)
            : mStream(stream)
        {
        }

        Ogre::DataStreamPtr& getStream(void)
        {
            return mStream;
        }

    protected:
        Ogre::DataStreamPtr mStream;

        void _read(void* buf, size_t size, size_t count);
    };

}

#endif
