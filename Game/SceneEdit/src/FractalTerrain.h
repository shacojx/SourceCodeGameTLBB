#ifndef __FractalTerrain_H__
#define __FractalTerrain_H__


namespace WX{
            
    
    struct FuncParam
    {
        float* heightMap;
        int xsize;
        int zsize;

        int seed;
        float scale;
    };

    typedef float Real;

    void generateFractalTerrain(FuncParam* param);
    void BoxFilterHeightMap(FuncParam* param);
    void fluidGenerate(FuncParam* param);
    void generateHillTerrain(FuncParam* param);
    void fracSynthPass(FuncParam* param);

}

#endif