#ifndef CGRID
#define CGRID
#ifdef __cplusplus

#include <Windows.h>	  // GL stuff
#include <gl\gl.h>	  // GL stuff
#include <SafeKill.h>	

struct CRGB_Byte {
	UCHAR R, G, B;
};

class CGrid
{
private:
  int NumGridLine;
  int Round(float X);
  void GridZColorInit(int *Z_RowLength);
  void BlockZColorInit(int *Z_RowLength);
  CRGB_Byte *pGridZColor, *pBlockZColor;	
public:
  int ListIndex;
  bool Initialise(int *GridListIndex, int *X, int *Y, int *Z, float *Zdiv2, float *MinXGridVal, float *MaxXGridVal, float *MinYGridVal, float *MaxYGridVal, float *MinZGridVal, float *MaxZGridVal, bool IncPtr);
};

#endif
#endif //CGRID