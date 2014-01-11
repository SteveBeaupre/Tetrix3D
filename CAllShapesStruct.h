#ifndef CALLSHAPESSTRUCT
#define CALLSHAPESSTRUCT
#ifdef __cplusplus


#include "Common.h"
#include <fstream.h>
#include <Vector>     //Used for Vector storage	
#include <SafeKill.h>


#define MAX_STR_LEN 1024 // the buffer size


//This tell how many color is define in ColorDef
#define MAX_COLOR  7
#define MAX_COLOR_MINUS_1  (MAX_COLOR-1)
enum ColorDef {
	Col_GRAY = 0,
	Col_RED,
	Col_GREEN,
	Col_BLUE,
	Col_YELLOW,
	Col_PURPLE,
	Col_ORANGE,
	Col_BLACK,
	Col_WHITE,
};
//This tell how many shape is define in ShapeDef
#define MAX_SHAPE  7
enum ShapeDef {
	Shape_Empty = 0,
	Shape_Bar,
	Shape_Cube,
	Shape_L1,
	Shape_L2,
	Shape_Z1,
	Shape_Z2,
	Shape_T
};

#define MAX_PCX_WIDTH    4
#define MAX_PCX_HEIGHT   4
#define MAX_PCX_SIZE     (MAX_PCX_WIDTH*MAX_PCX_HEIGHT)

//A struct to hold 3 float
struct CThreeFltpoint{float X,Y,Z;};
//A struct to hold 3 float
struct CTwoFltpoint{float X,Y;};

//A Struct to hold the current shape data
struct CShapeStruct{
	int ShapeIndx;
	int ShapeColor; //Useless now...
	std::vector<CThreeFltpoint> ShapeVect;
	CThreeFltpoint ShapeCenter;
	std::vector<CThreeFltpoint> NextShapeVect;
	CThreeFltpoint NextShapeCenter;
};

//A Struct to hold all block data
class CAllShapesStruct{
public:
  CAllShapesStruct();
  ~CAllShapesStruct();

	//Old
  //void Initialise(int pZ_RowLength);
  //CShapeStruct ShapesDat[(MAX_SHAPE*2)+1];
  //NEW!!!
  int NumOfBlockLoaded;
  std::vector <CShapeStruct> VectShapeStruct;
  int LinesDecoder(char *pLine, CThreeFltpoint *pThreeFltpoint);
  bool LoadPcx(char *pFileName, int Z_RowLength);
  void SpaceRemover(char *pLine);
};

#endif
#endif //CALLSHAPESSTRUCT