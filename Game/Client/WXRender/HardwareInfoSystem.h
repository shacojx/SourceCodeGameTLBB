/********************************************************************
filename:   HardwareInfoSystem.h

purpose:    用于检测系统的硬件信息，读取硬件信息表，得出当前系统的最
            低配置和推荐配置。
            用于这个文件会被两个不同的程序使用(Render.dll和tconfig.exe)，
            在代码中有一些不同的地方，在tconfig的工程中会定义TCONFIG.
*********************************************************************/

#ifndef _HARDWAREINFOSYSTEM_H_
#define _HARDWAREINFOSYSTEM_H_

#include <map>
#include <string>

#include <D3D9.h>

#ifndef TCONFIG
    class tDataBase;
#else
    namespace DBC
    {
        class DBCFile;
    }
#endif

/// 保存显卡信息表中保存的信息
struct VideoCardSetupInfo
{
    VideoCardSetupInfo() :
    mId(0),
    mDeviceId(NULL),
    mDesc(NULL),
    mTerrainLightmap(0),
    mAntiAlias(0),
    mRealtimeShadow(0),
    mTextureQuality(0),
    mTextureFilter(0),
    mPostFilter(0),
    mVSync(0)
    {
    }

    int mId;
    const char* mDeviceId;
    const char* mDesc;
    int mTerrainLightmap;
    int mAntiAlias;
    int mRealtimeShadow;
    int mTextureQuality;
    int mTextureFilter;
    int mPostFilter;
    int mVSync;    
};

/// 保存物理内存信息表中保存的信息
struct PhyMemorySetupInfo
{
    PhyMemorySetupInfo() :
    mId(0),
    mPhyMemoryRange(NULL),
    mTerrainLightMap(0)
    {
    }

    int mId;
    const char* mPhyMemoryRange;
    int mTerrainLightMap;
};

/// 保存CPU信息表中保存的信息
struct  CpuSetupInfo
{
    CpuSetupInfo() :
    mId(0),
    mCpuRange(NULL),
    mRealtimeShadow(0),
    mWeather(0),
    mWalkEffect(0)
    {
    }

    int mId;
    const char* mCpuRange;
    int mRealtimeShadow;
    int mWeather;
    int mWalkEffect;
};

/// 当前系统的硬件环境信息
struct HardwareInfo
{
    HardwareInfo() :
    mCpuSpeed(0),
    mPhyMemory(0.0f),
    mFreePhyMemory(0.0f),
    mVideoCardDeviceId(0xFFFF),
    mLocalVideoMemory(0.0f),
    mFreeLocalVideoMemory(0.0f),
    mNonLocalVideoMemory(0.0f),
    mFreeNonLocalVideoMemory(0.0f),
    mDXVersion(NULL),
    mVideoCardVendorId(0xFFFF),
    mVideoDriverVersion(0)
    {        
    }

    ~HardwareInfo()
    {
        if (mDXVersion)
        {
            delete [] mDXVersion;
            mDXVersion = NULL;
        }
    }

    int mCpuSpeed;                                  /// cpu频率，单位是MHz;
    float mPhyMemory;                               /// 物理内存，单位是Mb
    float mFreePhyMemory;                           /// 物理内存，单位是Mb
    DWORD mVideoCardDeviceId;                       /// 显卡id，用于识别各种型号的显卡
    float mLocalVideoMemory;                        /// 显卡显存大小，单位是Mb
    float mFreeLocalVideoMemory;                    /// 空余显卡显存大小，单位是Mb
    float mNonLocalVideoMemory;                     /// AGP显存大小，单位是Mb
    float mFreeNonLocalVideoMemory;                 /// 空余AGP显存大小，单位是Mb
    char* mDXVersion;                               /// dx版本号
    DWORD mVideoCardVendorId;                       /// 显卡厂商Id
    DWORD mVideoDriverVersion;                      /// 驱动程序版本号
};

/// 游戏中显示配置中的项
struct VideoSetupInfo
{
    void init(void)
    {
        mTerrainLightMap = 0;
        mAntiAlias = 0;
        mRealtimeShadow = 0;
        mTextureQuality = 0;
        mTextureFilter = 0;
        mPostFilter = 0;
        mVSync = 0;
        mWeather = 0;
        mWalkEffect = 0;
    }

    VideoSetupInfo()
    {
        init();
    }

    int mTerrainLightMap;
    int mAntiAlias;
    int mRealtimeShadow;
    int mTextureQuality;
    int mTextureFilter;
    int mPostFilter;
    int mVSync;
    int mWeather;
    int mWalkEffect;
};

/// 保存显示配置中单项的值
struct SingleOption
{
    int mMinValue;  /// 推荐值
    int mMaxValue;  /// 系统允许的最大值
};

/// 包中文件信息
struct FileInfo
{
	std::string strName;
	DWORD dwOffset;
	DWORD dwSize;
	DWORD dwCRC;
};

typedef std::map<std::string, SingleOption> VideoOptionMap;

class HardwareInfoSystem
{
private:

    static HardwareInfoSystem* sHardwareInfoSystem;

    VideoSetupInfo mVideoSetupInfo;

#ifndef TCONFIG
    tDataBase* mVideoCardConfigDataBase;
    tDataBase* mCpuConfigDataBase;
    tDataBase* mPhyMemoryDataBase;
#else
    DBC::DBCFile* mVideoCardConfigDataBase;
    DBC::DBCFile* mCpuConfigDataBase;
    DBC::DBCFile* mPhyMemoryDataBase;
#endif

    HardwareInfo mHardwareInfo;

    VideoOptionMap mVideoOptionMap;

    HINSTANCE mD3DLibrary;
    IDirect3D9* mIDirect3D9;
    D3DADAPTER_IDENTIFIER9 mD3DIdentifier9;
    D3DCAPS9 mD3DCAPS9;

    unsigned short mMaxVSVersionMajor;  /// 硬件支持的vs主版本号
    unsigned short mMaxVSVersionMinor;  /// 硬件支持的vs副版本号
    unsigned short mMaxPSVersionMajor;  /// 硬件支持的ps主版本号
    unsigned short mMaxPSVersionMinor;  /// 硬件支持的ps副版本号

    std::string mHardwareInfoString;    /// 硬件信息字符串

    std::string mErrorInfoString;       /// 保存错误的信息

    std::string mHardwareInfoValidateString; /// 硬件信息验证字符串

	std::map<std::string, FileInfo> mConfigFileInfo;
public:

    HardwareInfoSystem();
    ~HardwareInfoSystem();

    static HardwareInfoSystem* getMe()
    {
        return sHardwareInfoSystem;
    }

    /** 初始化硬件信息系统
    @remarks 其中会读取信息表，并根据当前计算机的配置，计算出每个选项的推荐值和最大允许值
    */
    bool initSystem(void);

    const VideoSetupInfo& getVideoSetupInfo(void) const
    {
        return mVideoSetupInfo;
    }

    const HardwareInfo& getHardwareInfo(void) const
    {
        return mHardwareInfo;
    }

    const char* getHardwareInfoAsString(void);

    const char* getErrorInfoString(void);

    const VideoOptionMap& getVideoOptionMap(void) const
    {
        return mVideoOptionMap;
    }

    const char* getDXVersion(void);

    const std::string& validateVideoDriver(void);

private:

    bool _ifCpuId();
    void _cpuVendor(char *szVendor);
    void _cpuTFMS(int *pnType, int *pnFamily, int *pnModel, int *pnStepping);
    bool _cpuCache(int *pnL2, int *pnL3);
    int _cpuBrand();
    int _cpuSpeed();

    void _InterCpu(char *szName, int nType, int nFamily, int nModel, int nStepping, int nL2, int nL3, int nBrand);
    void _AmdCpu(char *szName, int nType, int nFamily, int nModel, int nStepping);

    void _getOSInfo(char* osInfo);
    void _getCPUInfo(char* cpuInfo);
    void _getMemoryInfo(char* memoryInfo);
    void _getVideoCardInfo(char *szDevice, char *szDriver, char *extraInfo);


    bool _openVideoConfigFile(void);
    bool _openCpuConfigFile(void);
    bool _openPhyMemoryConfigFile(void);

    bool _isInRange(const char* rangeStr, float number);

    void _initOptionMap(void);

    int _getMaxFSAA(void);
    int _getMaxShadowTechnique(void);
    int _getMaxTextureFilter(void);
    int _getPostFilterEnable(void);

    void _setSingleOption(SingleOption& option, int minValue, int maxValue);

    bool _initD3D(void);
    void _deinitD3D(void);

    bool _openConfigFiles(void);
    void _closeConfigFiles(void);

    void _buildHardwareInfoString(void);

    void _pushErrorInfo(const std::string& errorInfo);

#ifdef TCONFIG

    /// 打开数据表（用于tconfig，可能是在包里打开，也可能是直接从文件打开）
    DBC::DBCFile* _openDBCText(const std::string& namFileName, const std::string& fileName);

#endif

	int _getFileFromPackage(const std::string& strPack, const std::string& strFileName, char*& pFile );
};

#endif // _HardwareInfoSystem_H_