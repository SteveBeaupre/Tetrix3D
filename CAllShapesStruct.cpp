#include "CAllShapesStruct.h"

CAllShapesStruct::CAllShapesStruct()
{
	//LoadPcx(BLOCK_FNAME);
}

CAllShapesStruct::~CAllShapesStruct(){}

/*void CAllShapesStruct::Initialise(int Z_RowLength)
{
	for(int Cpt = 0; Cpt < (MAX_SHAPE*2)+1; Cpt++)
		ShapesDat[Cpt].ShapeVect.clear();
	
	CThreeFltpoint XYZ;				   //storage for blocks starting position
	
	//The Empty shape(i don't init the other field cause i dont use them later...)
	ShapesDat[0].ShapeIndx = 0;
	ShapesDat[0].ShapeColor = Col_GRAY;
	//The Bar
	ShapesDat[Shape_Bar].ShapeIndx = 1;
	ShapesDat[Shape_Bar].ShapeColor = Col_GRAY;
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_Bar], 1.0f, 1.0f, Z_RowLength-1.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_Bar], 2.0f, 1.0f, Z_RowLength-1.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_Bar], 3.0f, 1.0f, Z_RowLength-1.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_Bar], 4.0f, 1.0f, Z_RowLength-1.0f);
	XYZ_CENTER_SHAPEMAKER2(ShapesDat[Shape_Bar], 2.0f, 1.0f, Z_RowLength-1.0f);
	//The Cube
	ShapesDat[Shape_Cube].ShapeIndx = 2;
	ShapesDat[Shape_Cube].ShapeColor = Col_GRAY;
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_Cube], 1.0f, 1.0f, Z_RowLength-1.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_Cube], 2.0f, 1.0f, Z_RowLength-1.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_Cube], 1.0f, 2.0f, Z_RowLength-1.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_Cube], 2.0f, 2.0f, Z_RowLength-1.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_Cube], 1.0f, 1.0f, Z_RowLength-2.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_Cube], 2.0f, 1.0f, Z_RowLength-2.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_Cube], 1.0f, 2.0f, Z_RowLength-2.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_Cube], 2.0f, 2.0f, Z_RowLength-2.0f);
	XYZ_CENTER_SHAPEMAKER2(ShapesDat[Shape_Cube], 1.5f, 1.5f, Z_RowLength-1.5f);
	//The L1
	ShapesDat[Shape_L1].ShapeIndx = 3;
	ShapesDat[Shape_L1].ShapeColor = Col_GRAY;
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_L1], 1.0f, 1.0f, Z_RowLength-1.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_L1], 1.0f, 2.0f, Z_RowLength-1.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_L1], 1.0f, 3.0f, Z_RowLength-1.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_L1], 2.0f, 3.0f, Z_RowLength-1.0f);
	XYZ_CENTER_SHAPEMAKER2(ShapesDat[Shape_L1], 1.0f, 2.0f, Z_RowLength-1.0f);
	//The L2
	ShapesDat[Shape_L2].ShapeIndx = 4;
	ShapesDat[Shape_L2].ShapeColor = Col_GRAY;
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_L2], 2.0f, 1.0f, Z_RowLength-1.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_L2], 2.0f, 2.0f, Z_RowLength-1.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_L2], 2.0f, 3.0f, Z_RowLength-1.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_L2], 1.0f, 3.0f, Z_RowLength-1.0f);
	XYZ_CENTER_SHAPEMAKER2(ShapesDat[Shape_L2], 2.0f, 2.0f, Z_RowLength-1.0f);
	//The Z1
	ShapesDat[Shape_Z1].ShapeIndx = 5;
	ShapesDat[Shape_Z1].ShapeColor = Col_GRAY;
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_Z1], 1.0f, 1.0f, Z_RowLength-1.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_Z1], 1.0f, 2.0f, Z_RowLength-1.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_Z1], 2.0f, 2.0f, Z_RowLength-1.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_Z1], 2.0f, 3.0f, Z_RowLength-1.0f);
	XYZ_CENTER_SHAPEMAKER2(ShapesDat[Shape_Z1], 1.0f, 2.0f, Z_RowLength-1.0f);
	//The Z2
	ShapesDat[Shape_Z2].ShapeIndx = 6;
	ShapesDat[Shape_Z2].ShapeColor = Col_GRAY;
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_Z2], 2.0f, 1.0f, Z_RowLength-1.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_Z2], 2.0f, 2.0f, Z_RowLength-1.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_Z2], 1.0f, 2.0f, Z_RowLength-1.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_Z2], 1.0f, 3.0f, Z_RowLength-1.0f);
	XYZ_CENTER_SHAPEMAKER2(ShapesDat[Shape_Z2], 2.0f, 2.0f, Z_RowLength-1.0f);
	//The T
	ShapesDat[Shape_T].ShapeIndx = 7;
	ShapesDat[Shape_T].ShapeColor = Col_GRAY;
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_T], 1.0f, 1.0f, Z_RowLength-1.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_T], 2.0f, 1.0f, Z_RowLength-1.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_T], 3.0f, 1.0f, Z_RowLength-1.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[Shape_T], 2.0f, 2.0f, Z_RowLength-1.0f);
	XYZ_CENTER_SHAPEMAKER2(ShapesDat[Shape_T], 2.0f, 1.0f, Z_RowLength-1.0f);
	/////////////////////////////////////////////////////////////////
	//The Preview Bar
	ShapesDat[MAX_SHAPE+Shape_Bar].ShapeIndx = 1;
	ShapesDat[MAX_SHAPE+Shape_Bar].ShapeColor = Col_GRAY;
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_Bar], 1.5f, 0.0f, 0.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_Bar], 0.5f, 0.0f, 0.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_Bar], -0.5f, 0.0f, 0.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_Bar], -1.5f, 0.0f, 0.0f);
	XYZ_CENTER_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_Bar], 0.0f, 0.0f, 0.0f);
	//The Preview Cube
	ShapesDat[MAX_SHAPE+Shape_Cube].ShapeIndx = 2;
	ShapesDat[MAX_SHAPE+Shape_Cube].ShapeColor = Col_GRAY;
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_Cube], 0.5f, 0.5f, -0.5f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_Cube], -0.5f, 0.5f, -0.5f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_Cube], 0.5f, -0.5f, -0.5f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_Cube], -0.5f, -0.5f, -0.5f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_Cube], 0.5f, 0.5f, 0.5f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_Cube], -0.5f, 0.5f, 0.5f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_Cube], 0.5f, -0.5f, 0.5f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_Cube], -0.5f, -0.5f, 0.5f);
	XYZ_CENTER_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_Cube], 0.0f, 0.0f, 0.0f);
	//The Preview L1
	ShapesDat[MAX_SHAPE+Shape_L1].ShapeIndx = 3;
	ShapesDat[MAX_SHAPE+Shape_L1].ShapeColor = Col_GRAY;
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_L1], 0.5f, 1.0f, 0.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_L1], 0.5f, 0.0f, 0.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_L1], 0.5f, -1.0f, 0.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_L1], -0.5f, -1.0f, 0.0f);
	XYZ_CENTER_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_L1], 0.0f, 0.0f, 0.0f);
	//The Preview L2
	ShapesDat[MAX_SHAPE+Shape_L2].ShapeIndx = 4;
	ShapesDat[MAX_SHAPE+Shape_L2].ShapeColor = Col_GRAY;
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_L2], -0.5f, 1.0f, 0.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_L2], -0.5f, 0.0f, 0.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_L2], -0.5f, -1.0f, 0.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_L2], 0.5f, -1.0f, 0.0f);
	XYZ_CENTER_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_L2], 0.0f, 0.0f, 0.0f);
	//The Preview Z1
	ShapesDat[MAX_SHAPE+Shape_Z1].ShapeIndx = 5;
	ShapesDat[MAX_SHAPE+Shape_Z1].ShapeColor = Col_GRAY;
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_Z1], 0.5f, 1.0f, 0.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_Z1], 0.5f, 0.0f, 0.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_Z1], -0.5f, 0.0f, 0.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_Z1], -0.5f, -1.0f, 0.0f);
	XYZ_CENTER_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_Z1], 0.0f, 0.0f, 0.0f);
	//The Preview Z2
	ShapesDat[MAX_SHAPE+Shape_Z2].ShapeIndx = 6;
	ShapesDat[MAX_SHAPE+Shape_Z2].ShapeColor = Col_GRAY;
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_Z2], -0.5f, 1.0f, 0.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_Z2], -0.5f, 0.0f, 0.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_Z2], 0.5f, 0.0f, 0.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_Z2], 0.5f, -1.0f, 0.0f);
	XYZ_CENTER_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_Z2], 0.0f, 0.0f, 0.0f);
	//The Preview T
	ShapesDat[MAX_SHAPE+Shape_T].ShapeIndx = 7;
	ShapesDat[MAX_SHAPE+Shape_T].ShapeColor = Col_GRAY;
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_T], 1.0f, 0.5f, 0.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_T], 0.0f, 0.5f, 0.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_T], -1.0f, 0.5f, 0.0f);
	XYZ_VECTOR_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_T], 0.0f, -0.5f, 0.0f);
	XYZ_CENTER_SHAPEMAKER2(ShapesDat[MAX_SHAPE+Shape_T], 0.0f, 0.0f, 0.0f);
}*/


void CAllShapesStruct::SpaceRemover(char *pLine)
{
	int sLen = strlen(pLine);
	int CptNew = 0;
	for(int CptOld = 0; CptOld <= sLen; CptOld++){
		if(pLine[CptOld] != ' ' && pLine[CptOld] != '	')
			pLine[CptNew++] = pLine[CptOld];
	}
	pLine[CptNew] = NULL;
}

int CAllShapesStruct::LinesDecoder(char *pLine, CThreeFltpoint *pThreeFltpoint)
{

	if((pLine[0] == '/') && (pLine[1] == '/'))
		return 0;

	SpaceRemover(pLine);

	int CommandNum;
	if(strstr(pLine, "MakeBlock(") != NULL){
		CommandNum = 1;
	} else if(strstr(pLine, "MakeCenterBlock(") != NULL){
		CommandNum = 2;
	} else if(strstr(pLine, "MakeNextBlock(") != NULL){
		CommandNum = 3;
	} else if(strstr(pLine, "MakeNextCenterBlock(") != NULL){
		CommandNum = 4;
	} else
		CommandNum = 0;



	if(CommandNum > 0){
		char Accum[MAX_STR_LEN];
		int LineCpt;
		switch(CommandNum)
		{
		case 1: LineCpt = strlen("MakeBlock(");           break;
		case 2: LineCpt = strlen("MakeCenterBlock(");     break;
		case 3: LineCpt = strlen("MakeNextBlock(");       break;
		case 4: LineCpt = strlen("MakeNextCenterBlock("); break;
		}
		int AccumCpt;
		int XYZ_Switch = 0;
		Jmp:
		AccumCpt = 0;
		while(pLine[LineCpt] != NULL){

			// Skip the current char. if it is ',' or ')' 
			if(pLine[LineCpt] == ',' || pLine[LineCpt] == ')'){
				LineCpt++;
				continue;
			}

			// Add the current char to the Accum buffer
			Accum[AccumCpt++] = pLine[LineCpt];
			// Check if the next char is ',' or ')'
			if(pLine[LineCpt+1] == ',' || pLine[LineCpt+1] == ')'){
				Accum[AccumCpt] = NULL;
				LineCpt++;
				break;
			}

			// Int the char. Index counter
			LineCpt++;
		}

		//Add the readen chunck in the correct var 
		switch(XYZ_Switch)
		{
		case 0: pThreeFltpoint->X = atof(Accum); XYZ_Switch++; goto Jmp; break;
		case 1: pThreeFltpoint->Y = atof(Accum); XYZ_Switch++; goto Jmp; break;
		case 2: pThreeFltpoint->Z = atof(Accum); break;
		}

		// Return the correct code
		return CommandNum;
	}

	return 0;
}


bool CAllShapesStruct::LoadPcx(char *pFileName, int Z_RowLength)
{
	bool ErrorInFile = false;
	const char Header[] = "//This file as been created by Slug and is for Tetrix3D only..."; //This must be at the begenig of the file, oterwise, we return false



	
	//Open the file, if exists...
    ifstream f(pFileName);
    if(!f){
      cout << "Ouverture impossible du fichier.";
      f.close();
	  return false;
    }

	//Create a dynamic char buffer 
	char *Str = new char[MAX_STR_LEN];

	//Check if the first line is equal to the supposed header in the define
    f.getline(Str, MAX_STR_LEN);      // Delimiteur de nouvelle ligne
    if(strcmp(Str, Header)){
      cout << "Bad Header File...";
      f.close();
	  return false;
    }

	VectShapeStruct.clear();

	NumOfBlockLoaded = 0;
	{
		CShapeStruct TmpShapeStruct;
		TmpShapeStruct.ShapeIndx = 0;
		TmpShapeStruct.ShapeColor = Col_GRAY;
		VectShapeStruct.push_back(TmpShapeStruct);	
	}


	//Read line by line all the data of the file
	int CurrentPcxIndex = 1;
    while(f){
		//Read a line...
        f.getline(Str, MAX_STR_LEN);      // Delimiteur de nouvelle ligne
		
		//This will create and fill a new vector of CShapeStruct
		if(strcmp(Str, "BeginPcx") == 0){
			//Create a tmp CThreeFltpoint struct 
			CThreeFltpoint TmpXYZ;
			//Create a tmp CShapeStruct
			CShapeStruct TmpShapeStruct;

			//Reserve enought memory for vector's
			TmpShapeStruct.ShapeVect.reserve(MAX_PCX_SIZE);
			TmpShapeStruct.NextShapeVect.reserve(MAX_PCX_SIZE);

			do{
				//Gather a line of text
				f.getline(Str, MAX_STR_LEN);
				if(!f){
					ErrorInFile = true;
					goto ErrorJmp;
				}

				switch(LinesDecoder(Str, &TmpXYZ))				
				{
				case -1: 
					ErrorInFile = true; 
					goto ErrorJmp;
					break;
				case 1: 
					TmpXYZ.Z = (-TmpXYZ.Z) + Z_RowLength;
					TmpShapeStruct.ShapeVect.push_back(TmpXYZ);   
					break;
				case 2: 
					TmpXYZ.Z = (-TmpXYZ.Z) + Z_RowLength;
					TmpShapeStruct.ShapeCenter = TmpXYZ; 
					break;
				case 3: 
					TmpShapeStruct.NextShapeVect.push_back(TmpXYZ);   
					break;
				case 4: 
					TmpShapeStruct.NextShapeCenter = TmpXYZ; 
					break;
				}
					

			}while(strcmp(Str, "EndPcx") != 0);
			
			ErrorJmp:
			if(ErrorInFile){
				cout << "An error has occured when trying to read data.";
				break;
			}

			//Find the index and the color of the pcx(auto.)
			TmpShapeStruct.ShapeIndx = CurrentPcxIndex++;
			TmpShapeStruct.ShapeColor = Col_GRAY;

			//Fill the true vector with the tmp struct
			VectShapeStruct.push_back(TmpShapeStruct);	

			//This tell how many block we've load.
			NumOfBlockLoaded++;
		}    
	}

	
	//Free the char buffer
	SAFE_DELETE_ARRAY(Str);
	//Close the file
    f.close();

	//Retrun false if no block found
	if(NumOfBlockLoaded < 1)
		return false;

	//return sucess to caller
	return true;
}

