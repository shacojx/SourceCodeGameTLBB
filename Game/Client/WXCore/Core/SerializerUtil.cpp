#include "SerializerUtil.h"

#include <OgreDataStream.h>
#include <OgreVector3.h>
#include <OgreQuaternion.h>
#include <OgreException.h>

namespace WX {

    ///////////////////////////////////////////////////////////////////////

    void flipToLittleEndian(void* pData, size_t size, size_t count)
    {
#	if OGRE_ENDIAN == OGRE_ENDIAN_BIG
        flipEndian(pData, size, count);
#	endif
    }

    void flipFromLittleEndian(void* pData, size_t size, size_t count)
    {
#	if OGRE_ENDIAN == OGRE_ENDIAN_BIG
        flipEndian(pData, size, count);
#	endif
    }

    void flipEndian(void * pData, size_t size, size_t count)
    {
        for(size_t index = 0; index < count; index++)
        {
            flipEndian((void *)((int)pData + (index * size)), size);
        }
    }

    void flipEndian(void * pData, size_t size)
    {
        char swapByte;
        for(size_t byteIndex = 0; byteIndex < size/2; byteIndex++)
        {
            swapByte = *(char *)((int)pData + byteIndex);
            *(char *)((int)pData + byteIndex) = *(char *)((int)pData + size - byteIndex - 1);
            *(char *)((int)pData + size - byteIndex - 1) = swapByte;
        }
    }

    ///////////////////////////////////////////////////////////////////////

    //---------------------------------------------------------------------
    void Writer::writeFloats(const float* pFloat, size_t count)
    {
#	if OGRE_ENDIAN == OGRE_ENDIAN_BIG
            float * pFloatToWrite = (float *)malloc(sizeof(float) * count);
            memcpy(pFloatToWrite, pFloat, sizeof(float) * count);

            flipToLittleEndian(pFloatToWrite, sizeof(float), count);
            _write(pFloatToWrite, sizeof(float), count);

            free(pFloatToWrite);
# 	else
            _write(pFloat, sizeof(float), count);
#	endif
    }
    //---------------------------------------------------------------------
    void Writer::writeFloats(const double* pDouble, size_t count)
    {
		// Convert to float, then write
		float* tmp = new float[count];
		for (size_t i = 0; i < count; ++i)
		{
			tmp[i] = static_cast<float>(pDouble[i]);
		}
#	if OGRE_ENDIAN == OGRE_ENDIAN_BIG
            flipToLittleEndian(tmp, sizeof(float), count);
            _write(tmp, sizeof(float), count);
# 	else
            _write(tmp, sizeof(float), count);
#	endif
		delete [] tmp;
    }
    //---------------------------------------------------------------------
    void Writer::writeShorts(const uint16* pShort, size_t count)
    {
#	if OGRE_ENDIAN == OGRE_ENDIAN_BIG
            uint16 * pShortToWrite = (uint16 *)malloc(sizeof(uint16) * count);
            memcpy(pShortToWrite, pShort, sizeof(uint16) * count);

            flipToLittleEndian(pShortToWrite, sizeof(uint16), count);
            _write(pShortToWrite, sizeof(uint16), count);

            free(pShortToWrite);
# 	else
            _write(pShort, sizeof(uint16), count);
#	endif
    }
    //---------------------------------------------------------------------
    void Writer::writeInts(const uint32* pInt, size_t count)
    {
#	if OGRE_ENDIAN == OGRE_ENDIAN_BIG
            uint32 * pIntToWrite = (uint32 *)malloc(sizeof(uint32) * count);
            memcpy(pIntToWrite, pInt, sizeof(uint32) * count);

            flipToLittleEndian(pIntToWrite, sizeof(uint32), count);
            _write(pIntToWrite, sizeof(uint32), count);

            free(pIntToWrite);
# 	else
            _write(pInt, sizeof(uint32), count);
#	endif
    }
    //---------------------------------------------------------------------
    void Writer::writeBools(const bool* pBool, size_t count)
    {
    //no endian flipping for 1-byte bools
    //XXX Nasty Hack to convert to 1-byte bools
#	if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
        char * pCharToWrite = (char *)malloc(sizeof(char) * count);
        for(int i = 0; i < count; i++)
        {
            *(char *)(pCharToWrite + i) = *(bool *)(pBool + i);
        }

        _write(pCharToWrite, sizeof(char), count);

        free(pCharToWrite);
#	else
        _write(pBool, sizeof(bool), count);
#	endif

    }
    //---------------------------------------------------------------------
    void Writer::writeString(const String& string)
    {
        uint32 numChars = string.length();
        writeInts(&numChars);
        _write(string.c_str(), sizeof(String::value_type), numChars);
    }
    //---------------------------------------------------------------------
    void Writer::writeObject(const Ogre::Vector3& v)
    {
        writeFloats(&v.x);
        writeFloats(&v.y);
        writeFloats(&v.z);
    }
    //---------------------------------------------------------------------
    void Writer::writeObject(const Ogre::Quaternion& q)
    {
        writeFloats(&q.x);
        writeFloats(&q.y);
        writeFloats(&q.z);
        writeFloats(&q.w);
    }
    //---------------------------------------------------------------------
    void Writer::writeData(const void* data, size_t size)
    {
        _write(data, size, 1);
    }

    ///////////////////////////////////////////////////////////////////////

    //---------------------------------------------------------------------
    void Reader::readFloats(float* pFloat, size_t count)
    {
        _read(pFloat, sizeof(float), count);
        flipFromLittleEndian(pFloat, sizeof(float), count);
    }
    //---------------------------------------------------------------------
    void Reader::readFloats(double* pDouble, size_t count)
    {
		// Read from float, convert to double
		float* tmp = new float[count];
		float* ptmp = tmp;
        _read(tmp, sizeof(float), count);
        flipFromLittleEndian(tmp, sizeof(float), count);
		// Convert to doubles (no cast required)
		while (count--)
		{
			*pDouble++ = *ptmp++;
		}
		delete [] tmp;
    }
    //---------------------------------------------------------------------
    void Reader::readShorts(uint16* pShort, size_t count)
    {
        _read(pShort, sizeof(uint16), count);
        flipFromLittleEndian(pShort, sizeof(uint16), count);
    }
    //---------------------------------------------------------------------
    void Reader::readInts(uint32* pInt, size_t count)
    {
        _read(pInt, sizeof(uint32), count);
        flipFromLittleEndian(pInt, sizeof(uint32), count);
    }
    //---------------------------------------------------------------------
    void Reader::readBools(bool* pBool, size_t count)
    {
        //XXX Nasty Hack to convert 1 byte bools to 4 byte bools
#	if OGRE_PLATFORM == OGRE_PLATFORM_APPLE
        char * pTemp = (char *)malloc(1*count); // to hold 1-byte bools
        _read(pTemp, sizeof(char), count);
        for(int i = 0; i < count; i++)
            *(bool *)(pBool + i) = *(char *)(pTemp + i);

        free (pTemp);
#	else
        _read(pBool, sizeof(bool), count);
#	endif
        //no flipping on 1-byte datatypes
    }
    //---------------------------------------------------------------------
    String Reader::readString(size_t numChars)
    {
        String str(numChars, '\0');
        _read(const_cast<String::value_type*>(str.data()), sizeof(String::value_type), numChars);
        return str;
    }
    //---------------------------------------------------------------------
    String Reader::readString(void)
    {
        uint32 numChars;
        readInts(&numChars);
        return readString(numChars);
    }
    //---------------------------------------------------------------------
    void Reader::readObject(Ogre::Vector3& v)
    {
        readFloats(&v.x);
        readFloats(&v.y);
        readFloats(&v.z);
    }
    //---------------------------------------------------------------------
    void Reader::readObject(Ogre::Quaternion& q)
    {
        readFloats(&q.x);
        readFloats(&q.y);
        readFloats(&q.z);
        readFloats(&q.w);
    }
    //---------------------------------------------------------------------
    void Reader::readData(void* data, size_t size)
    {
        _read(data, size, 1);
    }

    ///////////////////////////////////////////////////////////////////////

    void FileWriter::_write(const void* buf, size_t size, size_t count)
    {
        if (fwrite(buf, size, count, mFile) != count)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_CANNOT_WRITE_TO_FILE,
                "Can't write data to file",
                "FileWriter::_write");
        }
    }

    ///////////////////////////////////////////////////////////////////////

    void DataStreamReader::_read(void* buf, size_t size, size_t count)
    {
        if (mStream->read(buf, size * count) != size * count)
        {
            OGRE_EXCEPT(Ogre::Exception::ERR_INTERNAL_ERROR,
                "Can't read data from stream",
                "DataStreamReader::_read");
        }
    }
}
