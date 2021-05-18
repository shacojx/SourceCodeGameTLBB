#include <boost/random/linear_congruential.hpp>
#include <boost/random/uniform_real.hpp>
#include <boost/random/variate_generator.hpp>
#include <time.h>
#include <stdlib.h>
#include <malloc.h>

#include "FractalTerrain.h"

namespace WX{

// Use the diamond-square algorithm to tessalate a
// grid of real values into a fractal height map.
// Reference: http://www.gameprogrammer.com/fractal.html


// avgSquareVals - Given the i,j location as the center of a square,
// average the data values at the four corners of the square and return
// it. "Stride" represents half the length of one side of the square.
static
Real avgSquareVals(size_t i, size_t j, size_t stride, size_t size, Real *data)
{
    /* In this diagram, our input stride is 1, the i,j location is
       indicated by "*", and the four value we want to average are
       "X"s:
           X   .   X

           .   *   .

           X   .   X
    */
    return (data[((i-stride)*size) + j-stride] +
            data[((i-stride)*size) + j+stride] +
            data[((i+stride)*size) + j-stride] +
            data[((i+stride)*size) + j+stride]) / 4;
}

// avgDiamondVals - Given the i,j location as the center of a diamond,
// average the data values at the four corners of the diamond and
// return it. "Stride" represents the distance from the diamond center
// to a diamond corner.
static Real
avgDiamondVals(size_t i, size_t j, size_t stride, size_t size, Real *data)
{
    /* In this diagram, our input stride is 1, the i,j location is
       indicated by "X", and the four value we want to average are
       "*"s:
           .   *   .

           *   X   *

           .   *   .

       In order to support tiled surfaces which meet seamless at the
       edges (that is, they "wrap"), We need to be careful how we
       calculate averages when the i,j diamond center lies on an edge
       of the array.
    */
    return (data[(((i == 0 ? size-1 : i)-stride)*size) + j] +
            data[(((i == size-1 ? 0 : i)+stride)*size) + j] +
            data[(i*size) + ((j == 0 ? size-1 : j)-stride)] +
            data[(i*size) + ((j == size-1 ? 0 : j)+stride)]) / 4;
}

void
generateFractalTerrain(FuncParam* param)                      
{
    if(param->xsize != param->zsize || param->seed == 0 || 
        (param->xsize & (param->xsize-1)) != 0 )
    {
        for(int i=0;i<param->zsize;i++)
        for(int j=0;j<param->xsize;j++)
        {
            param->heightMap[j+(param->xsize+1)*i] = 0;
        }

        return ;
    }

    Real *data = param->heightMap;
    size_t size = param->xsize;
    int seed = param->seed;
    Real heightScale = param->scale;
    Real h = Real(0.85);
    

   boost::minstd_rand* engine = new boost::minstd_rand(seed);
   
    // we can't tesselate the array if it is not a power of 2. */
    assert((size & (size-1)) == 0);
    assert(size > 1);

    // subSize is the dimension of the array in terms of connected line
    // segments, while size is the dimension in terms of number of vertices.
    size_t subSize = size;
    ++size;

    // initialize a uniform distribution on the real range [-0.5 .. +0.5]
    boost::variate_generator<boost::minstd_rand&, boost::uniform_real<Real> >
        generator(*engine, boost::uniform_real<Real>((Real)-0.5, (Real)+0.5));

    // Set up our roughness constants.
    // Random numbers are always generated in the range [-0.5 .. +0.5].
    // 'scale' is multiplied by the randum number.
    // 'ratio' is multiplied by 'scale' after each iteration
    // to effectively reduce the randum number range.
    Real ratio = (Real ) pow((Real)2, -h);
    Real scale = heightScale * ratio;

    /* Seed the first four values. For example, in a 4x4 array, we
    would initialize the data points indicated by '*':

    *   .   .   .   *

    .   .   .   .   .

    .   .   .   .   .

    .   .   .   .   .

    *   .   .   .   *

    In terms of the "diamond-square" algorithm, this gives us
    "squares".

    We want the four corners of the array to have the same
    point. This will allow us to tile the arrays next to each other
    such that they join seemlessly.
    */

    data[(0*subSize*size)+0*subSize] =
        data[(0*subSize*size)+1*subSize] = 
        data[(1*subSize*size)+0*subSize] =
        data[(1*subSize*size)+1*subSize] = 0;

    // Now we add ever-increasing detail based on the "diamond" seeded
    // values. We loop over stride, which gets cut in half at the
    // bottom of the loop. Since it's an int, eventually division by 2
    // will produce a zero result, terminating the loop.
    for (size_t stride = subSize >> 1; stride; stride >>= 1)
    {
        size_t i, j;

        /* Take the existing "square" data and produce "diamond"
        data. On the first pass through with a 4x4 matrix, the
        existing data is shown as "X"s, and we need to generate the
        "*" now:

        X   .   .   .   X

        .   .   .   .   .

        .   .   *   .   .

        .   .   .   .   .

        X   .   .   .   X

        It doesn't look like diamonds. What it actually is, for the
        first pass, is the corners of four diamonds meeting at the
        center of the array.
        */
        for (i = stride; i < subSize; i += 2*stride)
        {
            for (j = stride; j < subSize; j += 2*stride)
            {
                data[(i * size) + j] =
                    scale * generator() +
                    avgSquareVals(i, j, stride, size, data);
            }
        }

        /* Take the existing "diamond" data and make it into
        "squares". Back to our 4X4 example: The first time we
        encounter this code, the existing values are represented by
        "X"s, and the values we want to generate here are "*"s:

        X   .   *   .   X

        .   .   .   .   .

        *   .   X   .   *

        .   .   .   .   .

        X   .   *   .   X

        i and j represent our (x,y) position in the array. The
        first value we want to generate is at (i=0,j=2), and we use
        "adjust" to adjust j to the desired value.
        */
        size_t adjust = stride;
        for (i = 0; i < subSize; i += stride, adjust = stride - adjust)
        {
            for (j = adjust; j < subSize; j += 2*stride)
            {
                // i and j are setup. Call avgDiamondVals with the
                // current position. It will return the average of the
                // surrounding diamond data points.
                data[(i * size) + j] =
                    scale * generator() +
                    avgDiamondVals(i, j, stride, size, data);

                // To wrap edges seamlessly, copy edge values around
                // to other side of array
                if (i == 0)
                    data[(subSize*size) + j] = data[(i * size) + j];
                if (j == 0)
                    data[(i*size) + subSize] = data[(i * size) + j];
            }
        }

        // reduce random number range.
        scale *= ratio;
    }
    
    delete engine;
}

void BoxFilterHeightMap(FuncParam* param)
{
    float* heightMap = param->heightMap;
    unsigned long width = param->xsize;
    unsigned long height = param->zsize;
    int seed = param->seed;

    assert(seed != 0);
    if(!seed)
        seed = 1;

    Real heightScale = param->scale * 0.02f;

    width++;
    height++;

    srand(seed);

    for(unsigned int i=0;i<height;i++)
    for(unsigned int j=0;j<width;j++)
        heightMap[j+ width * i] = (float)(rand()%seed) * heightScale;

    bool smoothEdges = true;

    //    width: Width of the height map in bytes
    //    height: Height of the height map in bytes
    // heightMap: Pointer to your height map data

    // Temporary values for traversing single dimensional arrays
    long x = 0;
    long z = 0;

    long  widthClamp = (smoothEdges) ?  width : width  - 1;
    long heightClamp = (smoothEdges) ? height : height - 1;

    // [Optimization] Calculate bounds ahead of time
    unsigned int bounds = width * height;

    // Validate requirements
    if (!heightMap)
        return;

    // Allocate the result
    float* result = heightMap;//new float[bounds];

    // Make sure memory was allocated
    if (!result)
        return;

    for (z = (smoothEdges) ? 0 : 1; z < heightClamp; ++z)
    {
        for (x = (smoothEdges) ? 0 : 1; x < widthClamp; ++x)
        {
            // Sample a 3x3 filtering grid based on surrounding neighbors

            float value = 0.0f;
            float cellAverage = 1.0f;

            // Sample top row

            if (((x - 1) + (z - 1) * width) >= 0 &&
                ((x - 1) + (z - 1) * width) < bounds)
            {
                value += heightMap[(x - 1) + (z - 1) * width];
                ++cellAverage;
            }

            if (((x - 0) + (z - 1) * width) >= 0 &&
                ((x - 0) + (z - 1) * width) < bounds)
            {
                value += heightMap[(x    ) + (z - 1) * width];
                ++cellAverage;
            }

            if (((x + 1) + (z - 1) * width) >= 0 &&
                ((x + 1) + (z - 1) * width) < bounds)
            {
                value += heightMap[(x + 1) + (z - 1) * width];
                ++cellAverage;
            }

            // Sample middle row

            if (((x - 1) + (z - 0) * width) >= 0 &&
                ((x - 1) + (z - 0) * width) < bounds)
            {
                value += heightMap[(x - 1) + (z    ) * width];
                ++cellAverage;
            }

            // Sample center point (will always be in bounds)
            value += heightMap[x + z * width];

            if (((x + 1) + (z - 0) * width) >= 0 &&
                ((x + 1) + (z - 0) * width) < bounds)
            {
                value += heightMap[(x + 1) + (z    ) * width];
                ++cellAverage;
            }

            // Sample bottom row

            if (((x - 1) + (z + 1) * width) >= 0 &&
                ((x - 1) + (z + 1) * width) < bounds)
            {
                value += heightMap[(x - 1) + (z + 1) * width];
                ++cellAverage;
            }

            if (((x - 0) + (z + 1) * width) >= 0 &&
                ((x - 0) + (z + 1) * width) < bounds)
            {
                value += heightMap[(x    ) + (z + 1) * width];
                ++cellAverage;
            }

            if (((x + 1) + (z + 1) * width) >= 0 &&
                ((x + 1) + (z + 1) * width) < bounds)
            {
                value += heightMap[(x + 1) + (z + 1) * width];
                ++cellAverage;
            }

            // Store the result
            result[x + z * width] = value / cellAverage ;
        }
    }
}



//=================================================
//
// Terrain Generation Using Fluid Simulation
// by Francis "DeathWish" Woodhouse
// deathwish@valve-erc.com / http://www.dwish.net
//
// This is a sample application that implements the algorithm
// described in the article.
//
//=================================================


void fluidGenerate(FuncParam* param)               
{
    float* g_pSurface[2] = {NULL,NULL}; // surface at time k and k-1

    // basic parameters
    int g_iSurfW=0, g_iSurfH=0, g_iNumIts=0;
    float g_fMinInitH=-500, g_fMaxInitH=500;

    // parameters to the fluid simulation equation
    float d = 10.0f;
    float t = 0.033f;
    float mu = 100.0f;
    float c = 100.0f;

    // pre-computed coefficients in the fluid equation
    float coefA, coefB, coefC;


    // check that they've got the right number of arguments
    float* heightMap = param->heightMap;
    g_iSurfW = param->xsize;
    g_iSurfH = param->zsize;
    Real heightScale = param->scale * 5;
    int seed = param->seed;
    g_iNumIts = 2;

    if(seed == 0)
        seed = 1;

    srand(seed);

    // allocate memory for the surface at two timesteps
    g_pSurface[0] = new float[g_iSurfW*g_iSurfH];
    g_pSurface[1] = new float[g_iSurfW*g_iSurfH];

    // pre-compute the coefficients in the fluid equation
    coefA = (4 - (8*c*c*t*t) / (d*d)) / (mu*t + 2);
    coefB = (mu*t - 2) / (mu*t + 2);
    coefC = ((2*c*c*t*t) / (d*d)) / (mu*t + 2);

    // initialise the surfaces to random values, except the edges which are set to 0
    for (int y = 0; y < g_iSurfH; y++) {
        for (int x = 0; x < g_iSurfW; x++) {
            if (x == 0 || x == (g_iSurfW-1) || y == 0 || y == (g_iSurfH-1))
                g_pSurface[0][x+y*g_iSurfW] = g_pSurface[1][x+y*g_iSurfW] = 0;
            else
                g_pSurface[0][x+y*g_iSurfW] = g_pSurface[1][x+y*g_iSurfW] = heightScale * ( ((float)(rand()%seed)) * (g_fMaxInitH - g_fMinInitH) + g_fMinInitH);
        }
    }

    int iCurBuf = 0;
    int iInd;

    float* pOld, * pNew;

    // Iterate over the heightmap, applying the fluid simulation equation.
    // Although it requires knowledge of the two previous timesteps, it only
    // accesses one pixel of the k-1 timestep, so using a simple trick we only
    // need to store the heightmap twice, not three times, and we can avoid
    // a memcpy() every iteration.
    for (int i = 0; i < g_iNumIts; i++) {
        pOld = g_pSurface[1-iCurBuf];
        pNew = g_pSurface[iCurBuf];

        for (int y = 1; y < g_iSurfH-1; y++) {
            for (int x = 1; x < g_iSurfW-1; x++) {
                iInd = x+y*g_iSurfW;

                pOld[iInd] = coefA*pNew[iInd] + coefB*pOld[iInd] +
                    coefC*(pNew[iInd+1] + pNew[iInd-1] + pNew[iInd+g_iSurfW] + pNew[iInd-g_iSurfW]);
            }
        }

        iCurBuf = 1-iCurBuf;
    }

    float fMinH=g_pSurface[iCurBuf][0], fMaxH=g_pSurface[iCurBuf][0];

    // find the minimum and maximum heights
    for (int y = 0; y < g_iSurfH; y++) {
        for (int x = 0; x < g_iSurfW; x++) {
            if (g_pSurface[iCurBuf][x+y*g_iSurfW] > fMaxH)
                fMaxH = g_pSurface[iCurBuf][x+y*g_iSurfW];
            else if (g_pSurface[iCurBuf][x+y*g_iSurfW] < fMinH)
                fMinH = g_pSurface[iCurBuf][x+y*g_iSurfW];
        }
    }

    // normalize the surface
    for (int y = 0; y < g_iSurfH; y++) {
        for (int x = 0; x < g_iSurfW; x++)
            g_pSurface[iCurBuf][x+y*g_iSurfW] = (g_pSurface[iCurBuf][x+y*g_iSurfW] - fMinH) / (fMaxH - fMinH);
    }

    // normalize the surface
    for (int y = 0; y < g_iSurfH; y++) {
        for (int x = 0; x < g_iSurfW; x++)
            heightMap[x+y*g_iSurfW] = g_pSurface[0][x+y*g_iSurfW] ;
    }

    delete[] g_pSurface[0];
    delete[] g_pSurface[1];

    return ;
}


unsigned short	m_uZSize;
unsigned short	m_uXSize;
float			m_fHillMin;
float			m_fHillMax;
unsigned short	m_uNumHills;
unsigned short	m_uFlattening;
bool			m_bIsland;
unsigned int	m_uSeed;
Real            m_uHeightScale; 

float	*m_pMap;	// buffer of cells



//	GetCell
//	----------------------------------------------------------------------------
static float
GetCell( int x, int y )
{
    // make sure we have a terrain
    assert( m_pMap != NULL );

    // check the parameters
    assert( ( x >= 0 ) && ( x < m_uXSize ) );
    assert( ( y >= 0 ) && ( y < m_uZSize ) );

    return( m_pMap[ x + ( y * m_uXSize ) ] );
}


//	SetCell
//	----------------------------------------------------------------------------
static void
SetCell( int x, int y, float value )
{
    // make sure we have a terrain
    assert( m_pMap != NULL );

    // check the parameters
    assert( ( x >= 0 ) && ( x < m_uXSize ) );
    assert( ( y >= 0 ) && ( y < m_uZSize ) );

    // set the cell
    m_pMap[ x + ( y * m_uXSize ) ] = value  * m_uHeightScale /2;
}

//	OffsetCell
//	----------------------------------------------------------------------------
static void
OffsetCell( int x, int y, float value )
{
    // make sure we have a terrain
    assert( m_pMap != NULL );

    // check the parameters
    assert( ( x >= 0 ) && ( x < m_uXSize ) );
    assert( ( y >= 0 ) && ( y < m_uZSize ) );

    // offset the cell
    m_pMap[ x + ( y * m_uXSize ) ] += value * m_uHeightScale /2 ;
}

//	Clear
//	----------------------------------------------------------------------------
static void
Clear( void )
{
    // make sure there is a terrain
    assert( m_pMap != NULL );

    for( int x = 0; x < m_uXSize; ++x )
    {
        for( int y = 0; y < m_uZSize; ++y )
        {
            SetCell( x, y, 0 );
        }
    }
}

//	RandomRange
//	----------------------------------------------------------------------------
static float
RandomRange( float min, float max )
{
    return( ( rand() * ( max - min ) / RAND_MAX ) + min );
}


//	AddHill
//	----------------------------------------------------------------------------
static void
AddHill( void )
{
    // make sure there is a terrain
    assert( m_pMap != NULL );

    // pick a size for the hill
    float fRadius = RandomRange( m_fHillMin, m_fHillMax );

    // pick a centerpoint for the hill
    float x, y;
    if( m_bIsland )
    {
        // island code:

        float fTheta = RandomRange( 0.0f, 6.28f );
        // this determines in which direction from the center of the map the
        // hill will be placed.

#define min(a, b)  (((a) < (b)) ? (a) : (b)) 
        float fDistance = RandomRange( fRadius/2, min(m_uZSize,m_uXSize)/2 - fRadius );
        // this is how far from the center of the map the hill be placed. note
        // that the radius of the hill is subtracted from the range to prevent
        // any part of a hill from reaching the very edge of the map.

        x = (float)(m_uXSize/2.0 + cos( fTheta ) * fDistance);
        y = (float)(m_uZSize/2.0 + sin( fTheta ) * fDistance);
        // converts theta and a distance into x and y coordinates.
    }
    else
    {
        // non-island code:

        x = RandomRange( -fRadius, m_uXSize + fRadius );
        y = RandomRange( -fRadius, m_uZSize + fRadius );
        // note that the range of the hill is used to determine the
        // centerpoint. this allows hills to have their centerpoint off the
        // edge of the terrain as long as part of the hill is in bounds. this
        // makes the terrains appear continuous all the way to the edge of the
        // map.
    }

    // square the hill radius so we don't have to square root the distance 
    float fRadiusSq = fRadius * fRadius;
    float fDistSq;
    float fHeight;

    // find the range of cells affected by this hill
    int xMin = (int)(x - fRadius - 1);
    int xMax = (int)(x + fRadius + 1);
    // don't affect cell outside of bounds
    if( xMin < 0 ) xMin = 0;
    if( xMax >= m_uXSize ) xMax = m_uXSize - 1;

    int yMin = (int)(y - fRadius - 1);
    int yMax = (int)(y + fRadius + 1);
    // don't affect cell outside of bounds
    if( yMin < 0 ) yMin = 0;
    if( yMax >= m_uZSize ) yMax = m_uZSize - 1;

    // for each affected cell, determine the height of the hill at that point
    // and add it to that cell
    for( int h = xMin; h <= xMax; ++h )
    {
        for( int v = yMin; v <= yMax; ++v )
        {
            // determine how far from the center of the hill this point is
            fDistSq = ( x - h ) * ( x - h ) + ( y - v ) * ( y - v );
            // determine the height of the hill at this point
            fHeight = fRadiusSq - fDistSq ;

            // don't add negative hill values (i.e. outside the hill's radius)
            if( fHeight > 0 )
            {
                // add the height of this hill to the cell
                OffsetCell( h, v, fHeight );
            }	
        }
    }
}


//	Normalize
//	----------------------------------------------------------------------------
static void
Normalize( void )
{
    // make sure there is a terrain
    assert( m_pMap != NULL );

    float fMin = GetCell( 0, 0 );
    float fMax = GetCell( 0, 0 );

    // find the min and max
    for( int x = 0; x < m_uXSize; ++x )
    {
        for( int y = 0; y < m_uZSize; ++y )
        {
            float z = GetCell( x, y );
            if( z < fMin ) fMin = z;
            if( z > fMax ) fMax = z;
        }
    }

    // avoiding divide by zero (unlikely with floats, but just in case)
    if( fMax != fMin )
    {
        // divide every height by the maximum to normalize to ( 0.0, 1.0 )
        for( int x = 0; x < m_uXSize; ++x )
        {
            for( int y = 0; y < m_uZSize; ++y )
            {
                SetCell( x, y, ( GetCell( x, y ) - fMin ) / ( fMax - fMin ) );
            }
        }
    }
    else
    {
        // if the min and max are the same, then the terrain has no height, so just clear it
        // to 0.0.
        Clear();
    }
}	


//	Flatten
//	----------------------------------------------------------------------------
static void
Flatten( void )
{
    // make sure there is a terrain
    assert( m_pMap != NULL );

    // if flattening is one, then nothing would be changed, so just skip the
    // process altogether.
    if( m_uFlattening > 1 )
    {
        for( int x = 0; x < m_uXSize; ++x )
        {
            for( int y = 0; y < m_uZSize; ++y )
            {
                float fFlat 	= 1.0;
                float fOriginal	= GetCell( x, y );

                // flatten as many times as desired
                for( int i = 0; i < m_uFlattening; ++i )
                {
                    fFlat *= fOriginal;
                }

                // put it back into the cell
                SetCell( x, y, fFlat );
            }
        }
    }
}


//	Generate the Hills
//	----------------------------------------------------------------------------
void
generateHillTerrain(FuncParam* param)
{
    m_pMap = param->heightMap;
    m_uXSize = param->xsize ;
    m_uZSize = param->zsize ;
    m_uSeed = param->seed;
    m_uHeightScale  = param->scale;

    // check the parameters
    assert( m_uXSize > 0 );
    assert( m_uZSize > 0 );
    assert( m_uSeed != 0 );
    assert(m_pMap != NULL);

    if(!m_uSeed)
        m_uSeed=1;

    m_fHillMin = 2.0f ;
    m_fHillMax = 40.0f ;
    m_uNumHills = 200;
    m_uFlattening = 1 ;
    m_bIsland = false;

    m_uZSize++ ;
    m_uXSize++;

    Clear();

    // set the seed
    srand( m_uSeed );

    // add as many hills as needed
    for( int i = 0; i < m_uNumHills; ++i )
    {
        AddHill();
    }

    // now clean it up
    Normalize();
    Flatten();
}


// basis matrix for spline interpolation
#define CR00	-0.5
#define CR01	1.5
#define CR02	-1.5
#define CR03	0.5
#define CR10	1.0
#define CR11	-2.5
#define CR12	2.0
#define CR13	-0.5
#define CR20	-0.5
#define CR21	0.0
#define CR22	0.5
#define CR23	0.0
#define CR30	0.0
#define CR31	1.0
#define CR32	0.0
#define CR33	0.0

#define RANDMASK RAND_MAX
#define MRANDOM (float)(rand() & (RANDMASK))/(RANDMASK)
#define SRANDOM ((MRANDOM) * 2) - 1

// draw lists
#define TERRAIN_LIST 1

#define WATERBOXSIZE 11
#define SAMPLESIZE 10
#define WATERSAMPLE 20

#define DEGTORAD				0.01745329

/*
* spline(...)
*
*	interpolate a 1d spline
*	taken from _Texturing & Modeling: A Procedural Approach_
*		chapter 2 - by Darwyn Peachey
*/
static float spline( float x, int nknots, float *knot )
{
    int span;
    int nspans = nknots - 3;
    float c0, c1, c2, c3;	// coefficients of the cubic

    if( nspans < 1)
    {
        // illegal
        return 0;
    }

    // find the appropriate 4-point span of the spline
    x = (x < 0 ? 0 : (x > 1 ? 1 : x)) * nspans;
    span = (int) x;
    if( span >= nknots -3 )
        span = nknots - 3;
    x -= span;
    knot += span;

    // evaluate the span cubic at x using horner's rule
    c3 = float(CR00*knot[0] + CR01*knot[1]
    + CR02*knot[2] + CR03*knot[3]);
    c2 = float(CR10*knot[0] + CR11*knot[1]
    + CR12*knot[2] + CR13*knot[3]);
    c1 = float(CR20*knot[0] + CR21*knot[1]
    + CR22*knot[2] + CR23*knot[3]);
    c0 = float(CR30*knot[0] + CR31*knot[1]
    + CR32*knot[2] + CR33*knot[3]);

    return ((c3*x + c2)*x + c1)*x + c0;
}

static void fracSynthPass2(FuncParam* param ,float freq, float zscale)                 
{
    float *hbuf;
    int xres;
    int zres ;

    hbuf = param->heightMap;
    xres = (int)(param->xsize);
    zres = (int)(param->zsize);

    int i;
    int x, z;
    float *val;
    int max;
    float dfx, dfz;
    float *zknots, *knots;
    float xk, zk;
    float *hlist;
    float *buf;

   

    // how many to generate (need 4 extra for smooth 2d spline interpolation)
    max = (int)(freq + 2);
    // delta x and z - pixels per spline segment
    dfx = xres / (freq-1);
    dfz = zres / (freq-1);

    // the generated values - to be equally spread across buf
    val = (float*)calloc( sizeof(float)*max*max, 1 );
    // intermediately calculated spline knots (for 2d)
    zknots = (float*)calloc( sizeof(float)*max, 1 );

    // horizontal lines through knots
    hlist = (float*)calloc( sizeof(float)*max*xres, 1 );
    // local buffer - to be added to hbuf
    buf = (float*)calloc( sizeof(float)*xres*zres, 1 );

    // start at -dfx, -dfz - generate knots
    for( z=0; z < max; z++ )
    {
        for( x=0;x < max;x++ )
        {
            val[z*max+x] = SRANDOM;
        }
    }

    // interpolate horizontal lines through knots
    for( i=0;i < max;i++ )
    {
        knots = &val[i*max];
        xk = 0;
        for( x=0;x < xres;x++ )
        {
            hlist[i*xres+x] = spline( xk/dfx, 4, knots );
            xk += 1;
            if( xk >= dfx )
            {
                xk -= dfx;
                knots++;
            }
        }
    }

    // interpolate all vertical lines
    for( x=0;x < xres;x++ )
    {
        zk = 0;
        knots = zknots;
        // build knot list
        for( i=0;i < max;i++ )
        {
            knots[i] = hlist[i*xres+x];
        }
        for( z=0;z < zres;z++ )
        {
            buf[z*xres+x] = spline( zk/dfz, 4, knots ) * zscale;
            zk += 1;
            if( zk >= dfz )
            {
                zk -= dfz;
                knots++;
            }
        }
    }

    // update hbuf
    for( z=0;z < zres;z++ )
        for( x=0;x < xres;x++ )
            hbuf[z*xres+x] += buf[z*xres+x];

    free( val );
    free( buf );
    free( hlist );
    free( zknots );
}


void fracSynthPass(FuncParam* param)                 
{
    int seed = param->seed;
    float fyscale = 0.4f;
    float freqscale = 6.0f;

    srand( seed );
    float freq = 4.0f;
    float yscale = param->scale * 0.08f;//Ä¬ÈÏÎª8

    for(int  pass=0;pass<4;pass++ )
    {
        fracSynthPass2( param,freq,yscale );
        freq += freqscale;
        yscale *= fyscale;
    }
}



};//end namespace