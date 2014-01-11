#include "Grid.h"


int CGrid::Round(float X)
{
	return (int)X + ((X-(int)X) >= 0.5);
}

void CGrid::GridZColorInit(int *Z_RowLength)
{
	pGridZColor = new CRGB_Byte [*Z_RowLength+1];
	for(int Cpt = 0; Cpt < *Z_RowLength+1; Cpt++){
		if(Cpt > ((float)(*Z_RowLength+1)/2)-1)
			pGridZColor[Cpt].R = 255;
		else
			pGridZColor[Cpt].R = Round((256/((float)((*Z_RowLength+1)-1) / 2)) * Cpt);
		if(Cpt < ((float)(*Z_RowLength+1)/2))
			pGridZColor[Cpt].G = 255;
		else
			pGridZColor[Cpt].G = Round(256-(256/((float)((*Z_RowLength+1)-1)/2))*(Cpt-((float)((*Z_RowLength+1)-1)/2)));
		pGridZColor[Cpt].B = 0;
	}
}


void CGrid::BlockZColorInit(int *Z_RowLength)
{
	pBlockZColor = new CRGB_Byte [*Z_RowLength];
	for(int Cpt = 0; Cpt < *Z_RowLength; Cpt++){
		switch(Cpt % 6)
		{
		case 0:	pBlockZColor[Cpt].R = 255; pBlockZColor[Cpt].G = 0;   pBlockZColor[Cpt].B = 0;   break;
		case 1:	pBlockZColor[Cpt].R = 0;   pBlockZColor[Cpt].G = 255; pBlockZColor[Cpt].B = 0;   break;
		case 2:	pBlockZColor[Cpt].R = 0;   pBlockZColor[Cpt].G = 0;   pBlockZColor[Cpt].B = 255; break;
		case 3:	pBlockZColor[Cpt].R = 255; pBlockZColor[Cpt].G = 255; pBlockZColor[Cpt].B = 0;   break;
		case 4:	pBlockZColor[Cpt].R = 255; pBlockZColor[Cpt].G = 0;   pBlockZColor[Cpt].B = 255; break;
		case 5:	pBlockZColor[Cpt].R = 255; pBlockZColor[Cpt].G = 128; pBlockZColor[Cpt].B = 0;   break;
		}
	}
}

bool CGrid::Initialise(int *GridListIndex, int *X, int *Y, int *Z, float *Zdiv2, float *MinXGridVal, float *MaxXGridVal, float *MinYGridVal, float *MaxYGridVal, float *MinZGridVal, float *MaxZGridVal, bool IncPtr)
{
	//Init. the block and grid color pointers
	GridZColorInit(Z);
	BlockZColorInit(Z);


	//This is our main cpt(calisse, va faloir faire de quoi avec ce float la...)
	float Cpt;
	//Used for pointers 
	//int ptrCpt;
	


	glNewList(*GridListIndex, GL_COMPILE_AND_EXECUTE);
	glDisable(GL_TEXTURE_2D);
	glDisable(GL_LIGHTING);
	glBegin(GL_LINES);
		//Bottom
		glColor3ub(0, 255, 0); 
		//GLGRID_MAKER();
		for (Cpt = *MinYGridVal+1; Cpt < *MaxYGridVal; Cpt++){//Bottom horizontal
			glVertex3f(*MinXGridVal, Cpt, *MinZGridVal); 
			glVertex3f(*MaxXGridVal, Cpt, *MinZGridVal);
		}
		for (Cpt = *MinXGridVal+1; Cpt < *MaxXGridVal; Cpt++){//Bottom vertical
			glVertex3f(Cpt, *MinYGridVal, *MinZGridVal); 
			glVertex3f(Cpt, *MaxYGridVal, *MinZGridVal);
		}
		//Up 
		for (Cpt = *MinZGridVal; Cpt <= *MaxZGridVal; Cpt++){//Up horizontal
			glColor3ub(pGridZColor[(int)Cpt].R, pGridZColor[(int)Cpt].G, pGridZColor[(int)Cpt].B); 
			glVertex3f(*MinXGridVal, *MaxYGridVal, Cpt); 
			glVertex3f(*MaxXGridVal, *MaxYGridVal, Cpt);
		} 
		for (Cpt = *MinXGridVal; Cpt < *MaxXGridVal+1; Cpt++){//Up vertical
			glColor3ub(0, 255, 0); 
			glVertex3f(Cpt, *MaxYGridVal, *MinZGridVal); 
			glColor3ub(255, 255, 0); 
			glVertex3f(Cpt, *MaxYGridVal, *Zdiv2); 
			glVertex3f(Cpt, *MaxYGridVal, *Zdiv2); 
			glColor3ub(255, 0, 0); 
			glVertex3f(Cpt, *MaxYGridVal, *MaxZGridVal);
		}
		//Down
		for (Cpt = *MinZGridVal; Cpt <= *MaxZGridVal; Cpt++){//Up horizontal
			glColor3ub(pGridZColor[(int)Cpt].R, pGridZColor[(int)Cpt].G, pGridZColor[(int)Cpt].B); 
			glVertex3f(*MinXGridVal, *MinYGridVal, Cpt); 
			glVertex3f(*MaxXGridVal, *MinYGridVal, Cpt);
		}
		for (Cpt = *MinXGridVal; Cpt < *MaxXGridVal+1; Cpt++){//Up vertical
			glColor3ub(0, 255, 0); 
			glVertex3f(Cpt, *MinYGridVal, *MinZGridVal); 
			glColor3ub(255, 255, 0); 
			glVertex3f(Cpt, *MinYGridVal, *Zdiv2); 
			glVertex3f(Cpt, *MinYGridVal, *Zdiv2); 
			glColor3ub(255, 0, 0); 
			glVertex3f(Cpt, *MinYGridVal, *MaxZGridVal);
		}
		//Left
		for (Cpt = *MinZGridVal; Cpt <= *MaxZGridVal; Cpt++){//Up horizontal
			glColor3ub(pGridZColor[(int)Cpt].R, pGridZColor[(int)Cpt].G, pGridZColor[(int)Cpt].B); 
			glVertex3f(*MinXGridVal, *MinYGridVal, Cpt); 
			glVertex3f(*MinXGridVal, *MaxYGridVal, Cpt);
		}
		for (Cpt = *MinYGridVal+1; Cpt < *MaxYGridVal; Cpt++){//Up vertical
			glColor3ub(0, 255, 0); 
			glVertex3f(*MinXGridVal, Cpt, *MinZGridVal); 
			glColor3ub(255, 255, 0); 
			glVertex3f(*MinXGridVal, Cpt, *Zdiv2); 
			glVertex3f(*MinXGridVal, Cpt, *Zdiv2); 
			glColor3ub(255, 0, 0); 
			glVertex3f(*MinXGridVal, Cpt, *MaxZGridVal);
		}
		//Right
		for (Cpt = *MinZGridVal; Cpt <= *MaxZGridVal; Cpt++){//Up horizontal
			glColor3ub(pGridZColor[(int)Cpt].R, pGridZColor[(int)Cpt].G, pGridZColor[(int)Cpt].B); 
			glVertex3f(*MaxXGridVal, *MinYGridVal, Cpt); 
			glVertex3f(*MaxXGridVal, *MaxYGridVal, Cpt);
		}
		for (Cpt = *MinYGridVal+1; Cpt < *MaxYGridVal; Cpt++){//Up vertical
			glColor3ub(0, 255, 0); 
			glVertex3f(*MaxXGridVal, Cpt, *MinZGridVal); 
			glColor3ub(255, 255, 0); 
			glVertex3f(*MaxXGridVal, Cpt, *Zdiv2); 
			glVertex3f(*MaxXGridVal, Cpt, *Zdiv2); 
			glColor3ub(255, 0, 0); 
			glVertex3f(*MaxXGridVal, Cpt, *MaxZGridVal);
		}
	glEnd();
	glEndList();

	ListIndex = *GridListIndex;
	if(IncPtr)
		*GridListIndex++;

	return true;
}

