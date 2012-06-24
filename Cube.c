// James Donner	CPSC 5125 
// 12-5-10
// Rubik's cube 
#include <stdlib.h>
#include <math.h>
#include <GL/glut.h>
// #include "SOIL.h"

#ifndef M_PI
#define M_PI 3.14159265
#endif
#define V1 1  //viewports 1-4
#define V2 2
#define V3 3
#define V4 4  

void initCubeVerts();  
void buildCube();
void drawCube(int);
void animateYCube();
void animateXCube();
void animateZCube();
void drawMiniCubeZ(int);
void drawMiniCubeY(int);
void drawMiniCubeX(int);
void drawFace(int,int,int,int);
void drawZSlice(int);
void drawYSlice(int);
void drawXSlice(int);
void animateZSlice(int);
void animateYSlice(int);
void animateXSlice(int);
void updateZSlice(int);
void updateYSlice(int);
void updateXSlice(int);
int cubeSize = 1; 
int space = 2;
int rot = 1;
int rotMult = 1;  // + rotation multiplier
int waitTime = 33;
int cubeRot = 0;
int view=1;  //view=1 then V2=Bottom V3=Left V4=Back
			 //view=2 then V2=Top V3=Right V4=Front
int perspView=1;   //1 orig 2 opp view 

                         //   [0] Black        [1] Blue              //[2] Green            [3] Orange
GLfloat colors[7][4] = {{0.0, 0.0, 0.0, 1.0},{0.0, 0.0, 1.0, 1.0}, {0.0, 1.0, 0.0, 1.0}, {1, 0.5, 0.0, 1.0}, 
{1.0, 0.0, 0.0, 1.0},{1.0, 1.0, 0.0, 1.0},{1.0, 1.0, 1.0, 1.0}};
// [4] Red            [5]  Yellow          [6] White  

GLfloat cubeVerts[8][3];
				          // Front[0] Right[1]   Down[2]   Up[3]    Back[4]    Left[5]  
GLfloat faceVertsZ[6][4] = {{0,3,2,1},{2,3,7,6},{3,0,4,7},{1,2,6,5},{4,5,6,7},{5,4,0,1}};
				          // Front[0]   Right[1]  Down[2]   Up[3]    Back[4]   Left[5]  
GLfloat faceVertsY[6][4] = {{1,2,6,5},{2,3,7,6},{0,3,2,1},{4,5,6,7},{3,0,4,7},{5,4,0,1}};
						  // Front[0]   Right[1]  Down[2]   Up[3]    Back[4]   Left[5]  
GLfloat faceVertsX[6][4] = {{5,4,0,1},{0,3,2,1},{3,0,4,7},{1,2,6,5},{2,3,7,6},{4,5,6,7}};

struct cubeStruct {
	 // Front[0] Right[1] Down[2]  Up[3]    Back[4]  Left[5]               
	GLint faceColors[6]; 
};

struct cubeStruct cubes[27];

static GLint ww = 500;
static GLint wh = 500;
static GLint savedww, savedwh;
static GLint lightingEnabled = 0;
static GLint wireframe = 1;
static GLenum mode = GL_SMOOTH;
static GLenum polygonMode = GL_FILL;
static GLint fullscreen = 0;
static GLdouble left, right, bottom, top;
static GLdouble near = -2.0, far = 2.0;
static GLdouble zoom = 10; 
static GLint lastX = 0, lastY = 0;
static int mouseButton, currentModifiers;
static GLdouble theta = 45.0, phi = 45.0;
static GLfloat eyeX=0, eyeY=0, eyeZ=20;
static int radius = 20;
static int animateX =0;
static int animateY =0;
static int animateZ =0;
static int animateSlice1 = 0;
static int animateSlice2 = 0;
static int animateSlice3 = 0;
static int animateSlice4 = 0;
static int animateSlice5 = 0;
static int animateSlice6 = 0;
static int animateSlice7 = 0;
static int animateSlice8 = 0;
static int animateSlice9 = 0;
static GLuint lensAngle = 60;  // for persp view
static int texture = 0; //texture off
GLuint tex_2d = 0;

void animateYCube()	// Animate whole cube on Y
{
	if(animateY)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPushMatrix();
		drawCube(V1);
		glPopMatrix();
		glRotatef(1, 0.0,1.0,0.0);
		glutSwapBuffers();
		glutTimerFunc(33, animateYCube, -1);
	}
}

void animateXCube()	// Animate whole cube on X
{	
	if(animateX)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPushMatrix();
		drawCube(V1);
		glPopMatrix();
		glRotatef(1, 1.0,0.0,0.0);
		glutSwapBuffers();
		glutTimerFunc(33, animateXCube, -1);
	}
}

void animateZCube()	// Animate whole cube on Z
{	
	if(animateZ)
	{
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glPushMatrix();
		drawCube(V1);
		glPopMatrix();
		glRotatef(1, 0.0,0.0,1.0);
		glutSwapBuffers();
		glutTimerFunc(33, animateZCube, -1);
	}
}

// animate slice 1,2 or 3 on Z axis
// 1 is back slice, 2 is middle slice, 3 is front slice
void animateZSlice(int sliceNum)	
{
	glPushMatrix();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(sliceNum == 1) //z slice 1 back
	{
		glTranslatef(0,0,0);
		drawZSlice(2);
		glTranslatef(0,0,+(cubeSize+space));
		drawZSlice(3);
	    glTranslatef(0,0,+ -2*(cubeSize+space));  
		glRotatef(rot*rotMult, 0.0,0.0,1.0);    
		drawZSlice(1);
		if(rot == 90)      //update cube rot complete
			updateZSlice(1); 
	}
	else if(sliceNum == 2) // z slice 2 in middle 
		{
		    glTranslatef(0,0,-(cubeSize+space));  
			drawZSlice(1);
			glTranslatef(0,0, (2*(cubeSize+space)));
			drawZSlice(3);
			glTranslatef(0,0, -(cubeSize+space));  //back to center
			glRotatef(rot*rotMult, 0.0,0.0,1.0);
			drawZSlice(2);
			if(rot == 90)   // update cube
				updateZSlice(2);
	    }  
	else if(sliceNum == 3) // 3rd z slice in front
		{
			glTranslatef(0,0,-(cubeSize+space));  
			drawZSlice(1);
			glTranslatef(0,0,+(cubeSize+space));
			drawZSlice(2);
			glTranslatef(0,0,+(cubeSize+space));
			glRotatef(rot*rotMult, 0.0,0.0,1.0);
			drawZSlice(3);
			if(rot == 90)   // update cube
				updateZSlice(3);
		}
	glPopMatrix();
	glutSwapBuffers();
}

// animate slice 4,5 or 6 on Y axis
// 4 is top slice, 5 is middle slice, 6 is bottom slice
// OR 1st stack, 2nd stack, 3rd stack
void animateYSlice(int sliceNum)
{
	glPushMatrix();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(sliceNum == 4) //y slice 4(or stack) on top
	{	
		glRotatef(90,1.0,0.0,0.0);
		glTranslatef(0,0,0);
		drawYSlice(5);
		glTranslatef(0,0,+(cubeSize+space));
		drawYSlice(6);
	    glTranslatef(0,0,-2 * (cubeSize+space)); 
		glRotatef(-rot*rotMult, 0.0,0.0,1.0);
		drawYSlice(4);
		if(rot == 90)  // update cube
			updateYSlice(4);
	}
	else if(sliceNum == 5)	// y slice 5 (or 2nd stack) middle
	{
		glRotatef(90,1.0,0.0,0.0);
		glTranslatef(0,0,+cubeSize+space);
		drawYSlice(6);
	    glTranslatef(0,0,-2 *(cubeSize+space));
		drawYSlice(4);
		glTranslatef(0,0,+(cubeSize+space));
		glRotatef(-rot*rotMult, 0.0,0.0,1.0);
		drawYSlice(5);
		if(rot == 90)  // update cube
			updateYSlice(5);
	}
	else if(sliceNum == 6)  // y slice 6 (or 3rd stack) bottom
	{
		glRotatef(90,1.0,0.0,0.0);
		glTranslatef(0,0,-(cubeSize+space));
		drawYSlice(4);
	    glTranslatef(0,0,+(cubeSize+space));
		drawYSlice(5);
		glTranslatef(0,0,+(cubeSize+space));
		glRotatef(-rot*rotMult, 0.0,0.0,1.0);
		drawYSlice(6);
		if(rot == 90)  // update cube
			updateYSlice(6);
	}
		glPopMatrix();
		glutSwapBuffers();
}

// animate slice 7,8 or 9 on X axis
// 7 is left slice, 8 is middle slice, 9 is right slice
void animateXSlice(int sliceNum)
{
	glPushMatrix();
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	if(sliceNum == 7) //x slice 7
	{	glRotatef(90,0.0,1.0,0.0);
		glTranslatef(0,0,0);
		drawXSlice(8);
		glTranslatef(0,0,+(cubeSize+space));
		drawXSlice(9);
	    glTranslatef(0,0,-2* (cubeSize+space));  
		glRotatef(rot*rotMult, 0.0,0.0,1.0);
		drawXSlice(7);
		if(rot == 90)  // update cube
			updateXSlice(7);
	}
	else if(sliceNum == 8)	// x slice 8
	{
		glRotatef(90,0.0,1.0,0.0);
		glTranslatef(0,0,+cubeSize+space);
		drawXSlice(9);
	    glTranslatef(0,0,-2 *(cubeSize+space));
		drawXSlice(7);
		glTranslatef(0,0,+(cubeSize+space));
		glRotatef(rot*rotMult, 0.0,0.0,1.0);
		drawXSlice(8);
		if(rot == 90)  // update cube
			updateXSlice(8);
	}
	else if(sliceNum == 9)  // x slice 9
	{
		glRotatef(90,0.0,1.0,0.0);
		glTranslatef(0,0,-(cubeSize+space));
		drawXSlice(7);
	    glTranslatef(0,0,+(cubeSize+space));
		drawXSlice(8);
		glTranslatef(0,0,+(cubeSize+space));
		glRotatef(rot*rotMult, 0.0,0.0,1.0);
		drawXSlice(9);
		if(rot == 90)  // update cube
			updateXSlice(9);
	}
		glPopMatrix();
		glutSwapBuffers();
}

void updateZSlice(int snum) {	// Finished animating a slice so update the cube 
	int temp1_3,temp1_4, temp0_3, temp0_4, temp0_5, temp3_4, temp3_5, temp5_1, temp5_4;
	int temp, temp1, temp2, temp3, temp4, temp5, temp6, temp7, temp8;
	// Front[0] Right[1] Down[2]  Up[3]    Back[4]  Left[5]   
	// slice1 0-8,   slice2 9-17,  slice3  18-26,  rotMult + CW, rotMult - CCW  
	// cube 1
	if(snum == 1 && rotMult > 0) {   // rotate Slice1 CCW
		temp0_3 = cubes[0].faceColors[3];
		temp0_4 = cubes[0].faceColors[4];
		temp0_5 = cubes[0].faceColors[5];
		cubes[0].faceColors[3] = cubes[2].faceColors[1];  //U 
		cubes[0].faceColors[5] = cubes[2].faceColors[3];  //L 
		cubes[0].faceColors[4] = cubes[2].faceColors[4];  //B 
		//cube 2
		temp1_3 = cubes[1].faceColors[3];
		temp1_4 = cubes[1].faceColors[4];
		cubes[1].faceColors[3] = cubes[5].faceColors[1];  //U 
		cubes[1].faceColors[4] = cubes[5].faceColors[4];  //B
		//cube 3  
		cubes[2].faceColors[1] = cubes[8].faceColors[2];  //R 
		cubes[2].faceColors[3] = cubes[8].faceColors[1];  //U 
		cubes[2].faceColors[4] = cubes[8].faceColors[4];  //B
		//cube 4  
		temp3_5 = cubes[3].faceColors[5];
		temp3_4 = cubes[3].faceColors[4];
		cubes[3].faceColors[5] = temp1_3;  //L 
		cubes[3].faceColors[4] = temp1_4;  //B
		//cube 5 no change
		//cube 6
		cubes[5].faceColors[1] = cubes[7].faceColors[2];  //R
		cubes[5].faceColors[4] = cubes[7].faceColors[4];  //B
		//cube 9  
		cubes[8].faceColors[1] = cubes[6].faceColors[2];  //R
		cubes[8].faceColors[2] = cubes[6].faceColors[5];  //D 
		cubes[8].faceColors[4] = cubes[6].faceColors[4];  //B
		//cube 7  
		cubes[6].faceColors[2] = temp0_5;  //D is orange
		cubes[6].faceColors[5] = temp0_3;  //L is blue
		cubes[6].faceColors[4] = temp0_4;  //B
		// cube 8  
		cubes[7].faceColors[2] = temp3_5;  //D
		cubes[7].faceColors[4] = temp3_4;  //B
	}
	// Front[0] Right[1] Down[2]  Up[3]    Back[4]  Left[5]   
	else if(snum == 1 && rotMult < 0) {   // rotate Slice1 CW
		temp0_3 = cubes[0].faceColors[3];
		temp0_4 = cubes[0].faceColors[4];
		temp0_5 = cubes[0].faceColors[5];
		cubes[0].faceColors[3] = cubes[6].faceColors[5];  //U
		cubes[0].faceColors[5] = cubes[6].faceColors[2];  //L 
		cubes[0].faceColors[4] = cubes[6].faceColors[4];  //B
		temp1_3 = cubes[1].faceColors[3];
		temp1_4 = cubes[1].faceColors[4];
		//cube 2
		cubes[1].faceColors[3] = cubes[3].faceColors[5];  //U 
		cubes[1].faceColors[4] = cubes[3].faceColors[4];  //B
		//cube 4  
		cubes[3].faceColors[5] = cubes[7].faceColors[2];  //L
		cubes[3].faceColors[4] = cubes[7].faceColors[4];  //B
		//cube 5 no change
		//cube 6
		temp5_1 = cubes[5].faceColors[1];
		temp5_4 = cubes[5].faceColors[4];
		cubes[5].faceColors[1] = temp1_3;  //R 
		cubes[5].faceColors[4] = temp1_4;  //B 
		//cube 7  
		cubes[6].faceColors[2] = cubes[8].faceColors[1];  //D
		cubes[6].faceColors[5] = cubes[8].faceColors[2];  //L
		cubes[6].faceColors[4] = cubes[8].faceColors[4];  //B
		//cube 8  
		cubes[7].faceColors[2] = temp5_1;  //D 
		cubes[7].faceColors[4] = temp5_4;  //B
		//cube 9  
		cubes[8].faceColors[1] = cubes[2].faceColors[3];  //R 
		cubes[8].faceColors[2] = cubes[2].faceColors[1];  //D
		cubes[8].faceColors[4] = cubes[2].faceColors[4];  //B
		//cube 3  
		cubes[2].faceColors[1] = temp0_3;  //R
		cubes[2].faceColors[3] = temp0_5;  //U
		cubes[2].faceColors[4] = temp0_4;  //B
	}
	if(snum == 2 && rotMult > 0) {   // rotate Slice2 CCW
		temp2 = cubes[9].faceColors[3];   //no front or back
		temp3 = cubes[9].faceColors[5];
		//cube 10
		cubes[9].faceColors[3] = cubes[11].faceColors[1];  //U 
		cubes[9].faceColors[5] = cubes[11].faceColors[3];  //L 
		//cube 11
		temp = cubes[10].faceColors[3];
		cubes[10].faceColors[3] = cubes[14].faceColors[1];  //U 
		//cube 12 
		cubes[11].faceColors[1] = cubes[17].faceColors[2];  //R 
		cubes[11].faceColors[3] = cubes[17].faceColors[1];  //U
		//cube 13  
		temp4  = cubes[12].faceColors[5];
		cubes[12].faceColors[5] = temp;  //L 
		//cube 14 no change
		//cube 15  
		cubes[14].faceColors[1] = cubes[16].faceColors[2];  //R
		//cube 18  
		cubes[17].faceColors[1] = cubes[15].faceColors[2];  //R 
		cubes[17].faceColors[2] = cubes[15].faceColors[5];  //D 
		//cube 16  
		cubes[15].faceColors[2] = temp3;  //D 
		cubes[15].faceColors[5] = temp2;  //L 
		//cube 17  
		cubes[16].faceColors[2] = temp4;  //D
	}
	// Front[0] Right[1] Down[2]  Up[3]    Back[4]  Left[5]   
	else if(snum == 2 && rotMult < 0) {   // rotate Slice2 CW
		temp2 = cubes[9].faceColors[3];
		temp3 = cubes[9].faceColors[5];
		cubes[9].faceColors[3] = cubes[15].faceColors[5];  //U is red
		cubes[9].faceColors[5] = cubes[15].faceColors[2];  //L is blue
		temp = cubes[10].faceColors[3];
		//cube 11
		cubes[10].faceColors[3] = cubes[12].faceColors[5];  //U is red
		//cube 13  
		cubes[12].faceColors[5] = cubes[16].faceColors[2];  //L is blue
		//cube 14 no change
		//cube 15
		temp4 = cubes[14].faceColors[1];
		cubes[14].faceColors[1] = temp;  //R is green
		//cube 16  
		cubes[15].faceColors[2] = cubes[17].faceColors[1];  //D is orange
		cubes[15].faceColors[5] = cubes[17].faceColors[2];  //L is blue
		//cube 17  
		cubes[16].faceColors[2] = temp4;  //D is orange
		//cube 18  
		cubes[17].faceColors[1] = cubes[11].faceColors[3];  //R is green
		cubes[17].faceColors[2] = cubes[11].faceColors[1];  //D is orange
		//cube 12  
		cubes[11].faceColors[1] = temp2;  //R is green
		cubes[11].faceColors[3] = temp3;  //U is red
	   }

		if(snum == 3 && rotMult > 0) {   // rotate Slice3 CCW
		temp2 = cubes[18].faceColors[3];
		temp3 = cubes[18].faceColors[5];
		temp4 = cubes[18].faceColors[0];
		//cube 19
		cubes[18].faceColors[3] = cubes[20].faceColors[1];  //U 
		cubes[18].faceColors[5] = cubes[20].faceColors[3];  //L 
		cubes[18].faceColors[0] = cubes[20].faceColors[0];  //F
		//cube 20
		temp7 = cubes[19].faceColors[3];
		temp8 = cubes[19].faceColors[0];
		cubes[19].faceColors[3] = cubes[23].faceColors[1];  //U 
		cubes[19].faceColors[0] = cubes[23].faceColors[0];  //F
		//cube 21  
		cubes[20].faceColors[1] = cubes[26].faceColors[2];  //R 
		cubes[20].faceColors[3] = cubes[26].faceColors[1];  //U 
		cubes[20].faceColors[0] = cubes[26].faceColors[0];  //F
		//cube 22  
		temp5 = cubes[21].faceColors[5];
		temp6 = cubes[21].faceColors[0];
		cubes[21].faceColors[5] = temp7; //L 
		cubes[21].faceColors[0] = temp8; //F
		//cube 23 no change
		//cube 24
		cubes[23].faceColors[1] = cubes[25].faceColors[2];  //R
		cubes[23].faceColors[0] = cubes[25].faceColors[0];  //F
		//cube 27  
		cubes[26].faceColors[1] = cubes[24].faceColors[2];  //R 
		cubes[26].faceColors[2] = cubes[24].faceColors[5];  //D
		cubes[26].faceColors[0] = cubes[24].faceColors[0];  //F
		//cube 25  
		cubes[24].faceColors[2] = temp3;  //D 
		cubes[24].faceColors[5] = temp2;  //L 
		cubes[24].faceColors[0] = temp4;  //F
		//cube 26  
		cubes[25].faceColors[2] = temp5;  //D 
		cubes[25].faceColors[0] = temp6;  //F
	   }
		// Front[0] Right[1] Down[2]  Up[3]    Back[4]  Left[5]   
	else if(snum == 3 && rotMult < 0) {   // rotate Slice2 CW
		temp2 = cubes[18].faceColors[3];
		temp3 = cubes[18].faceColors[5];
		temp4 = cubes[18].faceColors[0];
		cubes[18].faceColors[3] = cubes[24].faceColors[5]; //U 
		cubes[18].faceColors[5] = cubes[24].faceColors[2]; //L 
		cubes[18].faceColors[0] = cubes[24].faceColors[0]; //F
		temp7 = cubes[19].faceColors[3];
		temp8 = cubes[19].faceColors[0];
		//cube 20
		cubes[19].faceColors[3] = cubes[21].faceColors[5];  //U 
		cubes[19].faceColors[0] = cubes[21].faceColors[0];  //F
		//cube 22  
		cubes[21].faceColors[5] = cubes[25].faceColors[2];  //L 
		cubes[21].faceColors[0] = cubes[25].faceColors[0];  //F
		//cube 23 no change
		//cube 24
		temp5 = cubes[23].faceColors[1];
		temp6 = cubes[23].faceColors[0];
		cubes[23].faceColors[1] = temp7;  //R 
		cubes[23].faceColors[0] = temp8;  //F
		//cube 25  
		cubes[24].faceColors[2] = cubes[26].faceColors[1];  //D 
		cubes[24].faceColors[5] = cubes[26].faceColors[2];  //L 
		cubes[24].faceColors[0] = cubes[26].faceColors[0];  //F
		//cube 26  
		cubes[25].faceColors[2] = temp5; //D
		cubes[25].faceColors[0] = temp6; //F
		//cube 27  
		cubes[26].faceColors[1] = cubes[20].faceColors[3];  //R
		cubes[26].faceColors[2] = cubes[20].faceColors[1];  //D
		cubes[26].faceColors[0] = cubes[20].faceColors[0];  //F
		//cube 21  
		cubes[20].faceColors[1] = temp2;  //R 
		cubes[20].faceColors[3] = temp3;  //U 
		cubes[20].faceColors[0] = temp4;  //F
	}
}

void updateYSlice(int snum) {  // Finished animating a slice so update the cube 
	int temp, temp2, temp3, temp4, temp5, temp6, temp7, temp8;
	// stack1/snum4  = 0,1,2,9,10,11,18,19,20      rotMult + CW, rotMult - CCW  
	// stack2/snum5  = 3,4,5,12,13,14,21,22,23  
	// stack3/snum6  = 6,7,8,15,16,17,24,25,26 
	// cube 1
	// Front[0] Right[1] Down[2]  Up[3]    Back[4]  Left[5]   
	if(snum == 4 && rotMult > 0) {   // rotate Slice4/Stack1 CCW
		//cube 1
		temp=cubes[0].faceColors[4];
		temp2=cubes[0].faceColors[5];
		temp3=cubes[0].faceColors[3];
		cubes[0].faceColors[4] = cubes[2].faceColors[1];  //B 
		cubes[0].faceColors[5] = cubes[2].faceColors[4];  //L 
		cubes[0].faceColors[3] = cubes[2].faceColors[3];  //U
		//cube 2
		temp6=cubes[1].faceColors[4];
		temp7=cubes[1].faceColors[3];
		cubes[1].faceColors[4] = cubes[11].faceColors[1];  //B 
		cubes[1].faceColors[3] = cubes[11].faceColors[3];  //U
		//cube 3  
		cubes[2].faceColors[1] = cubes[20].faceColors[0];  //R 
		cubes[2].faceColors[4] = cubes[20].faceColors[1];  //B
		cubes[2].faceColors[3] = cubes[20].faceColors[3];  //U
		//cube 10
		temp4 = cubes[9].faceColors[5];
		temp5 = cubes[9].faceColors[3];
		cubes[9].faceColors[5] = temp6;  //L 
		cubes[9].faceColors[3] = temp7;  //U
		//cube 11
		//no change
		//cube 12 
		cubes[11].faceColors[1] = cubes[19].faceColors[0];  //R 
		cubes[11].faceColors[3] = cubes[19].faceColors[3];  //U
		//cube 21
		cubes[20].faceColors[0] = cubes[18].faceColors[5];  //F 
		cubes[20].faceColors[1] = cubes[18].faceColors[0];  //R
		cubes[20].faceColors[3] = cubes[18].faceColors[3];  //U
		//cube 19
		cubes[18].faceColors[0] = temp2; //F   
		cubes[18].faceColors[5] = temp;  //L
		cubes[18].faceColors[3] = temp3; //U
		//cube 20
		cubes[19].faceColors[0] = temp4; //F  
		cubes[19].faceColors[3] = temp5; //U
	}
	// Front[0] Right[1] Down[2]  Up[3]    Back[4]  Left[5]   
	else if(snum == 4 && rotMult < 0) {   // rotate Slice4 CW
		//cube 1
		temp=cubes[0].faceColors[4];
		temp2=cubes[0].faceColors[5];
		temp3=cubes[0].faceColors[3];
		cubes[0].faceColors[4] = cubes[18].faceColors[5];  //B 
		cubes[0].faceColors[5] = cubes[18].faceColors[0];  //L 
		cubes[0].faceColors[3] = cubes[18].faceColors[3];  //U
		//cube 2
		temp4=cubes[1].faceColors[4];
		temp5=cubes[1].faceColors[3];
		cubes[1].faceColors[4] = cubes[9].faceColors[5];  //B 
		cubes[1].faceColors[3] = cubes[9].faceColors[3];  //U
		//cube 10
		cubes[9].faceColors[5] = cubes[19].faceColors[0];  //L 
		cubes[9].faceColors[3] = cubes[19].faceColors[3];  //U
		//cube 11
		//no change
		//cube 19
		cubes[18].faceColors[0] = cubes[20].faceColors[1];  //F   
		cubes[18].faceColors[5] = cubes[20].faceColors[0];  //L 
		cubes[18].faceColors[3] = cubes[20].faceColors[3];  //U 
		//cube 20
		cubes[19].faceColors[0] = cubes[11].faceColors[1];  //F 
		cubes[19].faceColors[3] = cubes[11].faceColors[3];  //U
		//cube 21
		cubes[20].faceColors[0] = cubes[2].faceColors[1];  //F   
		cubes[20].faceColors[1] = cubes[2].faceColors[4];  //R 
		cubes[20].faceColors[3] = cubes[2].faceColors[3];  //U
		//cube 3  
		cubes[2].faceColors[1] = temp;   //R 
		cubes[2].faceColors[4] = temp2;  //B
		cubes[2].faceColors[3] = temp3;  //U
		//cube 12 
		cubes[11].faceColors[1] = temp4;  //R 
		cubes[11].faceColors[3] = temp5;  //U
	}
	
	if(snum == 5 && rotMult > 0) {  // rotate Slice5/Stack2 CCW
		// stack1/snum4  = 0,1,2,9, 10,11,18,19,20      rotMult + CW, rotMult - CCW  
		// stack2/snum5  = 3,4,5,12,13,14,21,22,23
		//cube 4
		temp=cubes[3].faceColors[4];
		temp2=cubes[3].faceColors[5];
		cubes[3].faceColors[4] = cubes[5].faceColors[1];  //B 
		cubes[3].faceColors[5] = cubes[5].faceColors[4];  //L 
		//cube 5
		temp6=cubes[4].faceColors[4];
		cubes[4].faceColors[4] = cubes[14].faceColors[1];  //B 
		//cube 6  
		cubes[5].faceColors[1] = cubes[23].faceColors[0];  //R 
		cubes[5].faceColors[4] = cubes[23].faceColors[1];  //B
		//cube 13
		temp4 = cubes[12].faceColors[5];
		cubes[12].faceColors[5] = temp6;  //L 
		//cube 14
		//no change
		//cube 15 
		cubes[14].faceColors[1] = cubes[22].faceColors[0];  //R 
		//cube 24
		cubes[23].faceColors[0] = cubes[21].faceColors[5];  //F 
		cubes[23].faceColors[1] = cubes[21].faceColors[0];  //R
		//cube 22
		cubes[21].faceColors[0] = temp2; //F   
		cubes[21].faceColors[5] = temp;  //L
		//cube 23
		cubes[22].faceColors[0] = temp4; //F  
	}
	// Front[0] Right[1] Down[2]  Up[3]    Back[4]  Left[5]  
	// stack1/snum4  = 0,1,2,9, 10,11,18,19,20    
	// stack2/snum5  = 3,4,5,12,13,14,21,22,23
	else if(snum == 5 && rotMult < 0) {  // rotate Slice5/Stack2 CW
		//cube 1
		temp=cubes[3].faceColors[4];
		temp2=cubes[3].faceColors[5];
		cubes[3].faceColors[4] = cubes[21].faceColors[5];  //B 
		cubes[3].faceColors[5] = cubes[21].faceColors[0];  //L 
		//cube 2
		temp4=cubes[4].faceColors[4];
		cubes[4].faceColors[4] = cubes[12].faceColors[5];  //B 
		//cube 10
		cubes[12].faceColors[5] = cubes[22].faceColors[0];  //L 
		//cube 11
		//no change
		//cube 19
		cubes[21].faceColors[0] = cubes[23].faceColors[1];  //F   
		cubes[21].faceColors[5] = cubes[23].faceColors[0];  //L 
		//cube 20
		cubes[22].faceColors[0] = cubes[14].faceColors[1];  //F 
		//cube 21
		cubes[23].faceColors[0] = cubes[5].faceColors[1];  //F   
		cubes[23].faceColors[1] = cubes[5].faceColors[4];  //R 
		//cube 3  
		cubes[5].faceColors[1] = temp;   //R 
		cubes[5].faceColors[4] = temp2;  //B
		//cube 12 
		cubes[14].faceColors[1] = temp4;  //R
		}
		// stack1/snum4  = 0,1,2,9, 10,11,18,19,20        
		// stack3/snum6  = 6,7,8,15,16,17,24,25,26 
		// Front[0] Right[1] Down[2]  Up[3]    Back[4]  Left[5]   
		 if(snum == 6 && rotMult > 0) {   // rotate Slice6/Stack3 CCW
		//cube 7
		temp=cubes[6].faceColors[4];
		temp2=cubes[6].faceColors[5];
		temp3=cubes[6].faceColors[2];
		cubes[6].faceColors[4] = cubes[8].faceColors[1];  //B 
		cubes[6].faceColors[5] = cubes[8].faceColors[4];  //L 
		cubes[6].faceColors[2] = cubes[8].faceColors[2];  //D
		//cube 8
		temp4=cubes[7].faceColors[4];
		temp5=cubes[7].faceColors[2];
		cubes[7].faceColors[4] = cubes[17].faceColors[1];  //B
		cubes[7].faceColors[2] = cubes[17].faceColors[2];  //D
		//cube 9  
		cubes[8].faceColors[1] = cubes[26].faceColors[0];  //R 
		cubes[8].faceColors[4] = cubes[26].faceColors[1];  //B 
		cubes[8].faceColors[2] = cubes[26].faceColors[2];  //D 
		//cube 16
		temp6 = cubes[15].faceColors[5];
		temp7 = cubes[15].faceColors[2];
		cubes[15].faceColors[5] = temp4;  //L
		cubes[15].faceColors[2] = temp5; //D
		//cube 17
		//no change
		//cube 18 
		cubes[17].faceColors[1] = cubes[25].faceColors[0];  //R 
		cubes[17].faceColors[2] = cubes[25].faceColors[2];  //D
		//cube 27
		cubes[26].faceColors[0] = cubes[24].faceColors[5];  //F   
		cubes[26].faceColors[1] = cubes[24].faceColors[0];  //R 
		cubes[26].faceColors[2] = cubes[24].faceColors[2];  //D
		//cube 25
		cubes[24].faceColors[0] = temp2; //F 
		cubes[24].faceColors[5] = temp;  //L 
		cubes[24].faceColors[2] = temp3; //D
		//cube 26
		cubes[25].faceColors[0] = temp6;  //F
		cubes[25].faceColors[2] = temp7;  //D
	   }
		// Front[0] Right[1] Down[2]  Up[3]    Back[4]  Left[5]  
		// stack1/snum4  = 0,1,2,9, 10,11,18,19,20    
		// stack3/snum6  = 6,7,8,15,16,17,24,25,26 
		else if(snum == 6 && rotMult < 0) {   // rotate Slice6/Stack3 CW
		//cube 1
		temp=cubes[6].faceColors[4];
		temp2=cubes[6].faceColors[5];
		temp3=cubes[6].faceColors[2];
		cubes[6].faceColors[4] = cubes[24].faceColors[5];  //B 
		cubes[6].faceColors[5] = cubes[24].faceColors[0];  //L
		cubes[6].faceColors[2] = cubes[24].faceColors[2];  //D
		//cube 2
		temp4=cubes[7].faceColors[4];
		temp5=cubes[7].faceColors[2];  //D
		cubes[7].faceColors[4] = cubes[15].faceColors[5];  //B 
		cubes[7].faceColors[2] = cubes[15].faceColors[2];  //D 
		//cube 10
		cubes[15].faceColors[5] = cubes[25].faceColors[0];  //L 
		cubes[15].faceColors[2] = cubes[25].faceColors[2];  //D 
		//cube 11
		//no change
		//cube 19
		cubes[24].faceColors[0] = cubes[26].faceColors[1];  //F   
		cubes[24].faceColors[5] = cubes[26].faceColors[0];  //L 
		cubes[24].faceColors[2] = cubes[26].faceColors[2];  //D 
		//cube 20
		cubes[25].faceColors[0] = cubes[17].faceColors[1];  //F 
		cubes[25].faceColors[2] = cubes[17].faceColors[2];  //D 
		//cube 21
		cubes[26].faceColors[0] = cubes[8].faceColors[1];  //F   
		cubes[26].faceColors[1] = cubes[8].faceColors[4];  //R 
		cubes[26].faceColors[2] = cubes[8].faceColors[2];  //D
		//cube 3  
		cubes[8].faceColors[1] = temp;   //R
		cubes[8].faceColors[4] = temp2;  //B 
		cubes[8].faceColors[2] = temp3;  //D
		//cube 12 
		cubes[17].faceColors[1] = temp4;  //R 
		cubes[17].faceColors[2] = temp5;  //D
	}
}

void updateXSlice(int snum) {  // Finished animating a slice so update the cube 
	int temp, temp2, temp3, temp4, temp5, temp6, temp7, temp8;
	//x slice7  0 ,3 ,6 ,9 ,12 ,15, 18 ,21 ,24   rotMult + CW, rotMult - CCW  
	//x slice8  1 ,4 ,7 ,10 ,13 ,16 ,19 ,22 ,25
	//x slice9  2 ,5 ,8 ,11 ,14 ,17 ,20, 23 ,26	
	// cube 1
	// Front[0] Right[1] Down[2]  Up[3]    Back[4]  Left[5]   
	if(snum == 7 && rotMult > 0) {   // rotate Slice7 CCW
		//cube 1
		temp=cubes[0].faceColors[4];
		temp2=cubes[0].faceColors[5];
		temp3=cubes[0].faceColors[3];
		cubes[0].faceColors[4] = cubes[6].faceColors[2];  //B 
		cubes[0].faceColors[5] = cubes[6].faceColors[5];  //L 
		cubes[0].faceColors[3] = cubes[6].faceColors[4];  //U
		//cube 4
		temp6=cubes[3].faceColors[4];
		temp7=cubes[3].faceColors[5];
		cubes[3].faceColors[4] = cubes[15].faceColors[2];  //B 
		cubes[3].faceColors[5] = cubes[15].faceColors[5];  //L
		//cube 7  
		cubes[6].faceColors[5] = cubes[24].faceColors[5];  //L 
		cubes[6].faceColors[4] = cubes[24].faceColors[2];  //B
		cubes[6].faceColors[2] = cubes[24].faceColors[0];  //D
		//cube 10
		temp4 = cubes[9].faceColors[5];
		temp5 = cubes[9].faceColors[3];
		cubes[9].faceColors[5] = temp7;  //L 
		cubes[9].faceColors[3] = temp6;  //U
		//cube 13
		//no change
		//cube 16 
		cubes[15].faceColors[5] = cubes[21].faceColors[5];  //L
		cubes[15].faceColors[2] = cubes[21].faceColors[0];  //D
		//cube25
		cubes[24].faceColors[0] = cubes[18].faceColors[3]; //F  
		cubes[24].faceColors[5] = cubes[18].faceColors[5]; //L
		cubes[24].faceColors[2] = cubes[18].faceColors[0]; //D
		//cube 19
		cubes[18].faceColors[0] = temp3;  //F 
		cubes[18].faceColors[5] = temp2;  //L
		cubes[18].faceColors[3] = temp;   //U
		//cube 22
		cubes[21].faceColors[0] = temp5; //F   
		cubes[21].faceColors[5] = temp4;  //L
	}
	// Front[0] Right[1] Down[2]  Up[3]    Back[4]  Left[5]   
	else if(snum == 7 && rotMult < 0) {   // rotate Slice7 CW
		//cube 1
		temp=cubes[0].faceColors[4];
		temp2=cubes[0].faceColors[5];
		temp3=cubes[0].faceColors[3];
		cubes[0].faceColors[4] = cubes[18].faceColors[3];  //B 
		cubes[0].faceColors[5] = cubes[18].faceColors[5];  //L 
		cubes[0].faceColors[3] = cubes[18].faceColors[0];  //U
		//cube 4
		temp4 = cubes[3].faceColors[4];
		temp5 = cubes[3].faceColors[5];
		cubes[3].faceColors[4] = cubes[9].faceColors[3];  //B 
		cubes[3].faceColors[5] = cubes[9].faceColors[5];  //L
		//cube 10
		cubes[9].faceColors[5] = cubes[21].faceColors[5];  //L 
		cubes[9].faceColors[3] = cubes[21].faceColors[0];  //U
		//cube 13
		//no change
		//cube 19
		cubes[18].faceColors[0] = cubes[24].faceColors[2];  //F 
		cubes[18].faceColors[5] = cubes[24].faceColors[5];  //L
		cubes[18].faceColors[3] = cubes[24].faceColors[0];  //U
		//cube 22
		cubes[21].faceColors[0] = cubes[15].faceColors[2]; //F   
		cubes[21].faceColors[5] = cubes[15].faceColors[5]; //L
		//cube25
		cubes[24].faceColors[0] = cubes[6].faceColors[2]; //F  
		cubes[24].faceColors[5] = cubes[6].faceColors[5]; //L
		cubes[24].faceColors[2] = cubes[6].faceColors[4]; //D
		//cube 7  
		cubes[6].faceColors[5] = temp2;  //L 
		cubes[6].faceColors[4] = temp3;  //B
		cubes[6].faceColors[2] = temp;  //D
		//cube 16 
		cubes[15].faceColors[5] = temp5;  //L
		cubes[15].faceColors[2] = temp4;  //D	
	}
	// Front[0] Right[1] Down[2]  Up[3]    Back[4]  Left[5]  
	if(snum == 8 && rotMult > 0) {  // rotate Slice8 CCW
	 //x slice1  0 ,3 ,6 ,9 , 12 ,15, 18 ,21 ,24     
	 //x slice2  1 ,4 ,7 ,10 ,13 ,16 ,19 ,22 ,25
		//cube 2
		temp=cubes[1].faceColors[4];
		temp3=cubes[1].faceColors[3];
		cubes[1].faceColors[4] = cubes[7].faceColors[2];  //B 
		cubes[1].faceColors[3] = cubes[7].faceColors[4];  //U
		//cube 5
		temp6=cubes[4].faceColors[4];
		cubes[4].faceColors[4] = cubes[16].faceColors[2];  //B 
		//cube 8  
		cubes[7].faceColors[4] = cubes[25].faceColors[2];  //B
		cubes[7].faceColors[2] = cubes[25].faceColors[0];  //D
		//cube 11
		temp5 = cubes[10].faceColors[3];
		cubes[10].faceColors[3] = temp6;  //U
		//cube 14
		//no change
		//cube 17 
		cubes[16].faceColors[2] = cubes[22].faceColors[0];  //D
		//cube26
		cubes[25].faceColors[0] = cubes[19].faceColors[3]; //F  
		cubes[25].faceColors[2] = cubes[19].faceColors[0]; //D
		//cube 20
		cubes[19].faceColors[0] = temp3;  //F 
		cubes[19].faceColors[3] = temp;   //U
		//cube 23
		cubes[22].faceColors[0] = temp5; //F   
	} 
	else if(snum == 8 && rotMult < 0) {  // rotate Slice8 CW
		//x slice1  0 ,3 ,6 ,9 , 12 ,15, 18 ,21 ,24     
		//x slice2  1 ,4 ,7 ,10 ,13 ,16 ,19 ,22 ,25
		//Front[0] Right[1] Down[2]  Up[3]    Back[4]  Left[5]  
		//cube 2
		temp=cubes[1].faceColors[4];
		temp3=cubes[1].faceColors[3];
		cubes[1].faceColors[4] = cubes[19].faceColors[3];  //B  
		cubes[1].faceColors[3] = cubes[19].faceColors[0];  //U
		//cube 5
		temp4 = cubes[4].faceColors[4];
		cubes[4].faceColors[4] = cubes[10].faceColors[3];  //B 
		//cube 11 
		cubes[10].faceColors[3] = cubes[22].faceColors[0];  //U
		//cube 14
		//no change
		//cube 20
		cubes[19].faceColors[0] = cubes[25].faceColors[2];  //F 
		cubes[19].faceColors[3] = cubes[25].faceColors[0];  //U
		//cube 23
		cubes[22].faceColors[0] = cubes[16].faceColors[2]; //F   
		//cube26
		cubes[25].faceColors[0] = cubes[7].faceColors[2]; //F  
		cubes[25].faceColors[2] = cubes[7].faceColors[4]; //D
		//cube 8  
		cubes[7].faceColors[4] = temp3;  //B
		cubes[7].faceColors[2] = temp;  //D
		//cube 17 
		cubes[16].faceColors[2] = temp4;  //D			
	}
	// Front[0] Right[1] Down[2]  Up[3]    Back[4]  Left[5]   
	if(snum == 9 && rotMult > 0) {   // rotate Slice9 CCW
		//x slice1  0 ,3 ,6 ,9 ,12 ,15, 18 ,21 ,24   rotMult + CW, rotMult - CCW  
		//x slice3  2 ,5 ,8 ,11 ,14 ,17 ,20, 23 ,26	
		//cube 3
		temp=cubes[2].faceColors[4];
		temp2=cubes[2].faceColors[1];
		temp3=cubes[2].faceColors[3];
		cubes[2].faceColors[4] = cubes[8].faceColors[2];  //B 
		cubes[2].faceColors[1] = cubes[8].faceColors[1];  //R 
		cubes[2].faceColors[3] = cubes[8].faceColors[4];  //U
		//cube 6
		temp6=cubes[5].faceColors[4];
		temp7=cubes[5].faceColors[1];
		cubes[5].faceColors[4] = cubes[17].faceColors[2];  //B 
		cubes[5].faceColors[1] = cubes[17].faceColors[1];  //R
	
		//cube 9  
		cubes[8].faceColors[1] = cubes[26].faceColors[1];  //R 
		cubes[8].faceColors[4] = cubes[26].faceColors[2];  //B
		cubes[8].faceColors[2] = cubes[26].faceColors[0];  //D
		//cube 12
		temp4 = cubes[11].faceColors[1];
		temp5 = cubes[11].faceColors[3];
		cubes[11].faceColors[1] = temp7;  //R 
		cubes[11].faceColors[3] = temp6;  //U
		//cube 15
		//no change
		//cube 27
		cubes[26].faceColors[1] = cubes[20].faceColors[1]; //R
		cubes[26].faceColors[2] = cubes[20].faceColors[0]; //D
		cubes[26].faceColors[0] = cubes[20].faceColors[3]; //F
		//cube 21
		cubes[20].faceColors[0] = temp3;  //F 
		cubes[20].faceColors[1] = temp2;  //R
		cubes[20].faceColors[3] = temp;   //U
		//cube18
		cubes[17].faceColors[1] = cubes[23].faceColors[1]; //R
		cubes[17].faceColors[2] = cubes[23].faceColors[0]; //D
		//cube 24
		cubes[23].faceColors[0] = temp5; //F   
		cubes[23].faceColors[1] = temp4; //R	
	}
	// Front[0] Right[1] Down[2]  Up[3]    Back[4]  Left[5]   
	else if(snum == 9 && rotMult < 0) {   // rotate Slice9 CW
		//x slice1  0 ,3 ,6 ,9  ,12 ,15, 18 ,21 ,24  
		//x slice3  2 ,5 ,8 ,11 ,14 ,17 ,20, 23 ,26	
		//cube 3
		temp=cubes[2].faceColors[4];
		temp2=cubes[2].faceColors[1];
		temp3=cubes[2].faceColors[3];
		cubes[2].faceColors[4] = cubes[20].faceColors[3];  //B 
		cubes[2].faceColors[1] = cubes[20].faceColors[1];  //R 
		cubes[2].faceColors[3] = cubes[20].faceColors[0];  //U
		//cube 6
		temp4 = cubes[5].faceColors[4];
		temp5 = cubes[5].faceColors[1];
		cubes[5].faceColors[4] = cubes[11].faceColors[3];  //B 
		cubes[5].faceColors[1] = cubes[11].faceColors[1];  //R
		//cube 12
		cubes[11].faceColors[1] = cubes[23].faceColors[1];  //R 
		cubes[11].faceColors[3] = cubes[23].faceColors[0];  //U
		//cube 15
		//no change
		//cube 21
		cubes[20].faceColors[0] = cubes[26].faceColors[2];  //F 
		cubes[20].faceColors[1] = cubes[26].faceColors[1];  //R
		cubes[20].faceColors[3] = cubes[26].faceColors[0];  //U
		//cube 24
		cubes[23].faceColors[0] = cubes[17].faceColors[2]; //F   
		cubes[23].faceColors[1] = cubes[17].faceColors[1]; //R
		//cube27
		cubes[26].faceColors[0] = cubes[8].faceColors[2]; //F  
		cubes[26].faceColors[1] = cubes[8].faceColors[1]; //R
		cubes[26].faceColors[2] = cubes[8].faceColors[4]; //D
		//cube 9  
		cubes[8].faceColors[1] = temp2;  //R 
		cubes[8].faceColors[4] = temp3;  //B
		cubes[8].faceColors[2] = temp;  //D
		//cube 18 
		cubes[17].faceColors[1] = temp5;  //R
		cubes[17].faceColors[2] = temp4;  //D
	}
}

void adjustOrtho()
{
   if (ww <= wh)
   {
	   left = -zoom;
	   right = zoom;
	   bottom = (-zoom*(GLfloat)wh/(GLfloat)ww);
	   top = (zoom*(GLfloat)wh/(GLfloat)ww);
	   near = -zoom;
	   far = 2*zoom;
   }
   else
   {
	   left = (-zoom*(GLfloat)ww/(GLfloat)wh);
	   right = (zoom*(GLfloat)ww/(GLfloat)wh);
	   bottom = -zoom;
	   top = zoom;
	   near = -2*zoom;
	   far = 2*zoom;
   }
}

void reshape(int w, int h)
{
	ww = w;
	wh = h;
   glMatrixMode(GL_PROJECTION);
   glLoadIdentity();
   adjustOrtho();
   glMatrixMode(GL_MODELVIEW);
}

void mouse(int button, int state, int x, int y)
{
	GLfloat tempPos[3];
	mouseButton = button;
	//setActiveViewport(x, y);
	currentModifiers = glutGetModifiers();
	switch(button) {
		case GLUT_LEFT_BUTTON:
			if (state == GLUT_DOWN)
			{				
				//glutPostRedisplay();
			}
			break;
		case GLUT_MIDDLE_BUTTON:
			if (state == GLUT_DOWN)
				break;
			break;
		case GLUT_RIGHT_BUTTON:
			if(state == GLUT_DOWN)
				break;
			break;
		default:
			break;
	}
}

void initCubeVerts() { //initialize cube vertices
	// {{-cubeSize,-cubeSize,cubeSize},{-cubeSize,cubeSize,cubeSize},  {cubeSize,cubeSize,cubeSize},
    //  {cubeSize,-cubeSize,cubeSize} ,{-cubeSize,-cubeSize,-cubeSize},{-cubeSize,cubeSize,-cubeSize},
	//  {cubeSize,cubeSize,-cubeSize},{cubeSize,-cubeSize,-cubeSize} };

	cubeVerts[0][0]= -cubeSize; cubeVerts[0][1]= -cubeSize; cubeVerts[0][2]=  cubeSize;
	cubeVerts[1][0]= -cubeSize; cubeVerts[1][1]=  cubeSize; cubeVerts[1][2]=  cubeSize;
	cubeVerts[2][0]=  cubeSize; cubeVerts[2][1]=  cubeSize; cubeVerts[2][2]=  cubeSize;
	cubeVerts[3][0]=  cubeSize; cubeVerts[3][1]= -cubeSize; cubeVerts[3][2]=  cubeSize;
	cubeVerts[4][0]= -cubeSize; cubeVerts[4][1]= -cubeSize; cubeVerts[4][2]= -cubeSize;
	cubeVerts[5][0]= -cubeSize; cubeVerts[5][1]=  cubeSize; cubeVerts[5][2]= -cubeSize;
	cubeVerts[6][0]=  cubeSize; cubeVerts[6][1]=  cubeSize; cubeVerts[6][2]= -cubeSize;
	cubeVerts[7][0]=  cubeSize; cubeVerts[7][1]= -cubeSize; cubeVerts[7][2]= -cubeSize;
}

void buildCube() //color cube at start/completed state 
{	
	//cube 1
	cubes[0].faceColors[0] = 0;  //F is black  
	cubes[0].faceColors[1] = 0;  //R is black
	cubes[0].faceColors[2] = 0;  //D is black
	cubes[0].faceColors[3] = 4;  //U is red
	cubes[0].faceColors[4] = 5;  //B is yellow
	cubes[0].faceColors[5] = 1;  //L is blue
	//cube 2
	cubes[1].faceColors[0] = 0;  //F is black  
	cubes[1].faceColors[1] = 0;  //R is black
	cubes[1].faceColors[2] = 0;  //D is black
	cubes[1].faceColors[3] = 4;  //U is red
	cubes[1].faceColors[4] = 5;  //B is yellow
	cubes[1].faceColors[5] = 0;  //L is black
	//cube 3
	cubes[2].faceColors[0] = 0;  //F is black  
	cubes[2].faceColors[1] = 2;  //R is green
	cubes[2].faceColors[2] = 0;  //D is black
	cubes[2].faceColors[3] = 4;  //U is red
	cubes[2].faceColors[4] = 5;  //B is yellow
	cubes[2].faceColors[5] = 0;  //L is black
	//cube 4
	cubes[3].faceColors[0] = 0;  //F is black  
	cubes[3].faceColors[1] = 0;  //R is black
	cubes[3].faceColors[2] = 0;  //D is black
	cubes[3].faceColors[3] = 0;  //U is black
	cubes[3].faceColors[4] = 5;  //B is yellow
	cubes[3].faceColors[5] = 1;  //L is blue
	//cube 5
	cubes[4].faceColors[0] = 0;  //F is black  
	cubes[4].faceColors[1] = 0;  //R is black
	cubes[4].faceColors[2] = 0;  //D is black
	cubes[4].faceColors[3] = 0;  //U is black
	cubes[4].faceColors[4] = 5;  //B is yellow
	cubes[4].faceColors[5] = 0;  //L is black
	//cube 6
	cubes[5].faceColors[0] = 0;  //F is black  
	cubes[5].faceColors[1] = 2;  //R is green
	cubes[5].faceColors[2] = 0;  //D is black
	cubes[5].faceColors[3] = 0;  //U is black
	cubes[5].faceColors[4] = 5;  //B is yellow
	cubes[5].faceColors[5] = 0;  //L is black
	//cube 7
	cubes[6].faceColors[0] = 0;  //F is black  
	cubes[6].faceColors[1] = 0;  //R is black
	cubes[6].faceColors[2] = 3;  //D is orange
	cubes[6].faceColors[3] = 0;  //U is black
	cubes[6].faceColors[4] = 5;  //B is yellow
	cubes[6].faceColors[5] = 1;  //L is blue
	//cube 8
	cubes[7].faceColors[0] = 0;  //F is black  
	cubes[7].faceColors[1] = 0;  //R is black
	cubes[7].faceColors[2] = 3;  //D is orange
	cubes[7].faceColors[3] = 0;  //U is black
	cubes[7].faceColors[4] = 5;  //B is yellow
	cubes[7].faceColors[5] = 0;  //L is black
	//cube 9
	cubes[8].faceColors[0] = 0;  //F is black  
	cubes[8].faceColors[1] = 2;  //R is green
	cubes[8].faceColors[2] = 3;  //D is orange
	cubes[8].faceColors[3] = 0;  //U is black
	cubes[8].faceColors[4] = 5;  //B is yellow
	cubes[8].faceColors[5] = 0;  //L is black
	//cube 10
	cubes[9].faceColors[0] = 0;  //F is black  
	cubes[9].faceColors[1] = 0;  //R is black
	cubes[9].faceColors[2] = 0;  //D is black
	cubes[9].faceColors[3] = 4;  //U is red
	cubes[9].faceColors[4] = 0;  //B is black
	cubes[9].faceColors[5] = 1;  //L is blue
	//cube 11
	cubes[10].faceColors[0] = 0;  //F is black  
	cubes[10].faceColors[1] = 0;  //R is black
	cubes[10].faceColors[2] = 0;  //D is black
	cubes[10].faceColors[3] = 4;  //U is red
	cubes[10].faceColors[4] = 0;  //B is black
	cubes[10].faceColors[5] = 0;  //L is black
	//cube 12
	cubes[11].faceColors[0] = 0;  //F is black  
	cubes[11].faceColors[1] = 2;  //R is green
	cubes[11].faceColors[2] = 0;  //D is black
	cubes[11].faceColors[3] = 4;  //U is red
	cubes[11].faceColors[4] = 0;  //B is black
	cubes[11].faceColors[5] = 0;  //L is black
	//cube 13
	cubes[12].faceColors[0] = 0;  //F is black  
	cubes[12].faceColors[1] = 0;  //R is black
	cubes[12].faceColors[2] = 0;  //D is black
	cubes[12].faceColors[3] = 0;  //U is black
	cubes[12].faceColors[4] = 0;  //B is black
	cubes[12].faceColors[5] = 1;  //L is blue
	//cube 14
	cubes[13].faceColors[0] = 0;  //F is black  
	cubes[13].faceColors[1] = 0;  //R is black
	cubes[13].faceColors[2] = 0;  //D is black
	cubes[13].faceColors[3] = 0;  //U is black  
	cubes[13].faceColors[4] = 0;  //B is black
	cubes[13].faceColors[5] = 0;  //L is black
	//cube 15
	cubes[14].faceColors[0] = 0;  //F is black  
	cubes[14].faceColors[1] = 2;  //R is green
	cubes[14].faceColors[2] = 0;  //D is black
	cubes[14].faceColors[3] = 0;  //U is black
	cubes[14].faceColors[4] = 0;  //B is black
	cubes[14].faceColors[5] = 0;  //L is black
	//cube 16
	cubes[15].faceColors[0] = 0;  //F is black  
	cubes[15].faceColors[1] = 0;  //R is black
	cubes[15].faceColors[2] = 3;  //D is orange
	cubes[15].faceColors[3] = 0;  //U is black
	cubes[15].faceColors[4] = 0;  //B is black
	cubes[15].faceColors[5] = 1;  //L is blue
	//cube 17
	cubes[16].faceColors[0] = 0;  //F is black  
	cubes[16].faceColors[1] = 0;  //R is black
	cubes[16].faceColors[2] = 3;  //D is orange
	cubes[16].faceColors[3] = 0;  //U is black
	cubes[16].faceColors[4] = 0;  //B is yellow
	cubes[16].faceColors[5] = 0;  //L is black
	//cube 18
	cubes[17].faceColors[0] = 0;  //F is black  
	cubes[17].faceColors[1] = 2;  //R is green
	cubes[17].faceColors[2] = 3;  //D is orange
	cubes[17].faceColors[3] = 0;  //U is black
	cubes[17].faceColors[4] = 0;  //B is yellow
	cubes[17].faceColors[5] = 0;  //L is black
	//cube 19
	cubes[18].faceColors[0] = 6;  //F is white  
	cubes[18].faceColors[1] = 0;  //R is black
	cubes[18].faceColors[2] = 0;  //D is black
	cubes[18].faceColors[3] = 4;  //U is red
	cubes[18].faceColors[4] = 0;  //B is black
	cubes[18].faceColors[5] = 1;  //L is blue
	//cube 20
	cubes[19].faceColors[0] = 6;  //F is white  
	cubes[19].faceColors[1] = 0;  //R is black
	cubes[19].faceColors[2] = 0;  //D is black
	cubes[19].faceColors[3] = 4;  //U is red
	cubes[19].faceColors[4] = 0;  //B is black
	cubes[19].faceColors[5] = 0;  //L is blue
	//cube 21
	cubes[20].faceColors[0] = 6;  //F is white  
	cubes[20].faceColors[1] = 2;  //R is green
	cubes[20].faceColors[2] = 0;  //D is black
	cubes[20].faceColors[3] = 4;  //U is red
	cubes[20].faceColors[4] = 0;  //B is black
	cubes[20].faceColors[5] = 0;  //L is black
	//cube 22
	cubes[21].faceColors[0] = 6;  //F is white  
	cubes[21].faceColors[1] = 0;  //R is black
	cubes[21].faceColors[2] = 0;  //D is black
	cubes[21].faceColors[3] = 0;  //U is black
	cubes[21].faceColors[4] = 0;  //B is black
	cubes[21].faceColors[5] = 1;  //L is blue
	//cube 23
	cubes[22].faceColors[0] = 6;  //F is white  
	cubes[22].faceColors[1] = 0;  //R is black
	cubes[22].faceColors[2] = 0;  //D is black
	cubes[22].faceColors[3] = 0;  //U is black
	cubes[22].faceColors[4] = 0;  //B is black
	cubes[22].faceColors[5] = 0;  //L is black
	//cube 24
	cubes[23].faceColors[0] = 6;  //F is white  
	cubes[23].faceColors[1] = 2;  //R is green
	cubes[23].faceColors[2] = 0;  //D is black
	cubes[23].faceColors[3] = 0;  //U is black  
	cubes[23].faceColors[4] = 0;  //B is black
	cubes[23].faceColors[5] = 0;  //L is black
	//cube 25
	cubes[24].faceColors[0] = 6;  //F is white  
	cubes[24].faceColors[1] = 0;  //R is black
	cubes[24].faceColors[2] = 3;  //D is orange
	cubes[24].faceColors[3] = 0;  //U is black
	cubes[24].faceColors[4] = 0;  //B is black
	cubes[24].faceColors[5] = 1;  //L is blue
	//cube 26
	cubes[25].faceColors[0] = 6;  //F is white  
	cubes[25].faceColors[1] = 0;  //R is black
	cubes[25].faceColors[2] = 3;  //D is orange
	cubes[25].faceColors[3] = 0;  //U is black
	cubes[25].faceColors[4] = 0;  //B is black
	cubes[25].faceColors[5] = 0;  //L is black
	//cube 27
	cubes[26].faceColors[0] = 6;  //F is white  
	cubes[26].faceColors[1] = 2;  //R is green
	cubes[26].faceColors[2] = 3;  //D is orange
	cubes[26].faceColors[3] = 0;  //U is black
	cubes[26].faceColors[4] = 0;  //B is yellow
	cubes[26].faceColors[5] = 0;  //L is black
} // end buildCube()

void drawZSlice(int num){  //draw 1 of the Z slices
	int cubeNum=1;

	if(num==1)
		cubeNum=1;
	else if(num==2)  //slice 2 10-18
		cubeNum=10;
	else if(num==3)  //slice 3 19-27
		cubeNum=19;
	glPushMatrix();
	glTranslatef(-(cubeSize+space), cubeSize+space ,0); //row1 col1 cube
	drawMiniCubeZ(cubeNum);           // slice num 1 2 or 3  -- slice num * 
	glTranslatef(cubeSize+space,0,0);  //row1 col2 cube
	drawMiniCubeZ(cubeNum+1);
	glTranslatef(cubeSize+space,0,0);  //row1 col3 cube
	drawMiniCubeZ(cubeNum+2);
	glTranslatef(-2*(cubeSize+space), -(cubeSize+space),0);  //row2 col1 cube
	drawMiniCubeZ(cubeNum+3);
	glTranslatef(cubeSize+space,0,0); //row2 col2 cube  -- center cube
	drawMiniCubeZ(cubeNum+4);
	glTranslatef(cubeSize+space,0,0); //row2 col3 cube
	drawMiniCubeZ(cubeNum+5);
	glTranslatef(-2*(cubeSize+space), -(cubeSize+space),0);  //row3 col1 cube
	drawMiniCubeZ(cubeNum+6);
	glTranslatef(cubeSize+space,0,0); //row3 col2 cube
	drawMiniCubeZ(cubeNum+7);
	glTranslatef(cubeSize+space,0,0); //row3 col3 cube
	drawMiniCubeZ(cubeNum+8);
	glPopMatrix();
}

void drawYSlice(int num){  //draw one of the Y slices (stacks)
	int cubeArray[9];

	if(num==4) {		 //slice 4(or stack 1) 
		cubeArray[0]= 19;
		cubeArray[1]= 20;
		cubeArray[2]= 21;
		cubeArray[3]= 10;
		cubeArray[4]= 11;
		cubeArray[5]= 12;
		cubeArray[6]= 1;
		cubeArray[7]= 2;
		cubeArray[8]= 3;
	}
	else if(num==5) { //slice 5 (or stack 2) 
		cubeArray[0]= 22;
		cubeArray[1]= 23;
		cubeArray[2]= 24;
		cubeArray[3]= 13;
		cubeArray[4]= 14;
		cubeArray[5]= 15; 
		cubeArray[6]= 4; 
		cubeArray[7]= 5;
		cubeArray[8]= 6;
	}
	else if(num==6) { //slice 6 (or stack 3) 
		cubeArray[0]= 25;
		cubeArray[1]= 26;
		cubeArray[2]= 27;
		cubeArray[3]= 16;
		cubeArray[4]= 17;
		cubeArray[5]= 18; 
		cubeArray[6]= 7; 
		cubeArray[7]= 8;
		cubeArray[8]= 9;
	}

	glPushMatrix();
	glTranslatef(-(cubeSize+space), cubeSize+space ,0); //row1 col1 cube
	drawMiniCubeY(cubeArray[0]);           // slice num 4 5 or 6
	glTranslatef(cubeSize+space,0,0);  //row1 col2 cube
	drawMiniCubeY(cubeArray[1]);
	glTranslatef(cubeSize+space,0,0);  //row1 col3 cube
	drawMiniCubeY(cubeArray[2]);
	glTranslatef(-2*(cubeSize+space), -(cubeSize+space),0);  //row2 col1 cube
	drawMiniCubeY(cubeArray[3]);
	glTranslatef(cubeSize+space,0,0); //row2 col2 cube  -- center cube
	drawMiniCubeY(cubeArray[4]);
	glTranslatef(cubeSize+space,0,0); //row2 col3 cube
	drawMiniCubeY(cubeArray[5]);
	glTranslatef(-2*(cubeSize+space), -(cubeSize+space),0);  //row3 col1 cube
	drawMiniCubeY(cubeArray[6]);
	glTranslatef(cubeSize+space,0,0); //row3 col2 cube
	drawMiniCubeY(cubeArray[7]);
	glTranslatef(cubeSize+space,0,0); //row3 col3 cube
	drawMiniCubeY(cubeArray[8]);
	glPopMatrix();
}

void drawXSlice(int num){  // draw one of the X slices 7,8, or 9
	int cubeArray[9];

	if(num==7) {		 //x slice7  1,4,7,10,13,16,19,22,25
		cubeArray[0]= 19;
		cubeArray[1]= 10;
		cubeArray[2]= 1;
		cubeArray[3]= 22;
		cubeArray[4]= 13;
		cubeArray[5]= 4;
		cubeArray[6]= 25;
		cubeArray[7]= 16;
		cubeArray[8]= 7;
	}
	else if(num==8) { //x slice8  2,5,8,11,14,17,20,23,26
		cubeArray[0]= 20;
		cubeArray[1]= 11;
		cubeArray[2]= 2;
		cubeArray[3]= 23;
		cubeArray[4]= 14;
		cubeArray[5]= 5;
		cubeArray[6]= 26;
		cubeArray[7]= 17;
		cubeArray[8]= 8;
	}
	else if(num==9) { //x slice9  3,6,9,12,15,18,21,24,27
		cubeArray[0]= 21;  //21,3,12
		cubeArray[1]= 12;
        cubeArray[2]= 3; 
		cubeArray[3]= 24;  //24,6,15
		cubeArray[4]= 15;
		cubeArray[5]= 6;
		cubeArray[6]= 27;  //27,9,18
		cubeArray[7]= 18;
		cubeArray[8]= 9;
	}
	glPushMatrix();
	glTranslatef(-(cubeSize+space), cubeSize+space ,0); //row1 col1 cube
	drawMiniCubeX(cubeArray[0]);           // slice num 7 8 or 9
	glTranslatef(cubeSize+space,0,0);  //row1 col2 cube
	drawMiniCubeX(cubeArray[1]);
	glTranslatef(cubeSize+space,0,0);  //row1 col3 cube
	drawMiniCubeX(cubeArray[2]);
	glTranslatef(-2*(cubeSize+space), -(cubeSize+space),0);  //row2 col1 cube
	drawMiniCubeX(cubeArray[3]);
	glTranslatef(cubeSize+space,0,0); //row2 col2 cube  -- center cube
	drawMiniCubeX(cubeArray[4]);
	glTranslatef(cubeSize+space,0,0); //row2 col3 cube
	drawMiniCubeX(cubeArray[5]);
	glTranslatef(-2*(cubeSize+space), -(cubeSize+space),0);  //row3 col1 cube
	drawMiniCubeX(cubeArray[6]);
	glTranslatef(cubeSize+space,0,0); //row3 col2 cube
	drawMiniCubeX(cubeArray[7]);
	glTranslatef(cubeSize+space,0,0); //row3 col3 cube
	drawMiniCubeX(cubeArray[8]);
	glPopMatrix();
}

void drawMiniCubeZ(int num){   //draw cube looking from Z 
	glColor3fv(colors[cubes[num-1].faceColors[0]]);
	drawFace(faceVertsZ[0][0],faceVertsZ[0][1],faceVertsZ[0][2],faceVertsZ[0][3]);
	glColor3fv(colors[cubes[num-1].faceColors[1]]);
	drawFace(faceVertsZ[1][0],faceVertsZ[1][1],faceVertsZ[1][2],faceVertsZ[1][3]);
	glColor3fv(colors[cubes[num-1].faceColors[2]]);
	drawFace(faceVertsZ[2][0],faceVertsZ[2][1],faceVertsZ[2][2],faceVertsZ[2][3]);
	glColor3fv(colors[cubes[num-1].faceColors[3]]);
	drawFace(faceVertsZ[3][0],faceVertsZ[3][1],faceVertsZ[3][2],faceVertsZ[3][3]);
	glColor3fv(colors[cubes[num-1].faceColors[4]]);
	drawFace(faceVertsZ[4][0],faceVertsZ[4][1],faceVertsZ[4][2],faceVertsZ[4][3]);
	glColor3fv(colors[cubes[num-1].faceColors[5]]);
	drawFace(faceVertsZ[5][0],faceVertsZ[5][1],faceVertsZ[5][2],faceVertsZ[5][3]);
}

void drawMiniCubeY(int num){ //draw cube looking from Y
	glColor3fv(colors[cubes[num-1].faceColors[0]]);
	drawFace(faceVertsY[0][0],faceVertsY[0][1],faceVertsY[0][2],faceVertsY[0][3]);
	glColor3fv(colors[cubes[num-1].faceColors[1]]);
	drawFace(faceVertsY[1][0],faceVertsY[1][1],faceVertsY[1][2],faceVertsY[1][3]);
	glColor3fv(colors[cubes[num-1].faceColors[2]]);
	drawFace(faceVertsY[2][0],faceVertsY[2][1],faceVertsY[2][2],faceVertsY[2][3]);
	glColor3fv(colors[cubes[num-1].faceColors[3]]);
	drawFace(faceVertsY[3][0],faceVertsY[3][1],faceVertsY[3][2],faceVertsY[3][3]);
	glColor3fv(colors[cubes[num-1].faceColors[4]]);
	drawFace(faceVertsY[4][0],faceVertsY[4][1],faceVertsY[4][2],faceVertsY[4][3]);
	glColor3fv(colors[cubes[num-1].faceColors[5]]);
	drawFace(faceVertsY[5][0],faceVertsY[5][1],faceVertsY[5][2],faceVertsY[5][3]);
}

void drawMiniCubeX(int num){  //draw cube looking from X
	glColor3fv(colors[cubes[num-1].faceColors[0]]);
	drawFace(faceVertsX[0][0],faceVertsX[0][1],faceVertsX[0][2],faceVertsX[0][3]);
	glColor3fv(colors[cubes[num-1].faceColors[1]]);
	drawFace(faceVertsX[1][0],faceVertsX[1][1],faceVertsX[1][2],faceVertsX[1][3]);
	glColor3fv(colors[cubes[num-1].faceColors[2]]);
	drawFace(faceVertsX[2][0],faceVertsX[2][1],faceVertsX[2][2],faceVertsX[2][3]);
	glColor3fv(colors[cubes[num-1].faceColors[3]]);
	drawFace(faceVertsX[3][0],faceVertsX[3][1],faceVertsX[3][2],faceVertsX[3][3]);
	glColor3fv(colors[cubes[num-1].faceColors[4]]);
	drawFace(faceVertsX[4][0],faceVertsX[4][1],faceVertsX[4][2],faceVertsX[4][3]);
	glColor3fv(colors[cubes[num-1].faceColors[5]]);
	drawFace(faceVertsX[5][0],faceVertsX[5][1],faceVertsX[5][2],faceVertsX[5][3]);
}

// draw one side of one cube
void drawFace(int vert1, int vert2, int vert3, int vert4) {
	if(texture) glEnable(GL_TEXTURE_2D);

	glBindTexture(GL_TEXTURE_2D, tex_2d);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_REPEAT);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
	glTexParameterf(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST_MIPMAP_LINEAR);

	glBegin(GL_POLYGON);
	glTexCoord2f(0.0, 0.0);
	glVertex3fv(cubeVerts[vert1]);
	glTexCoord2f(1.0, 0.0);
	glVertex3fv(cubeVerts[vert2]);
	glTexCoord2f(1.0, 1.0);
	glVertex3fv(cubeVerts[vert3]);
	glTexCoord2f(0.0, 1.0);
	glVertex3fv(cubeVerts[vert4]);
	glEnd();

	glDisable(GL_TEXTURE_2D);
}

void drawCube(int viewportNum)  //draw entire cube
{
	glPushMatrix();
	if(lightingEnabled == 0)
		glDisable(GL_LIGHTING);
	else
		glEnable(GL_LIGHTING);

	glPolygonMode(GL_FRONT_AND_BACK, polygonMode); 
	glShadeModel(mode);
	glLineWidth(2);  // for line through axis

	if(view == 1) {
		if(viewportNum == V4) {   // V1,V2,V3,V4 CW starting at persp view lower right
			glColor3fv(colors[5]);  //Yellow
			glRasterPos3f(1,5,0);
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'B');
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'A');
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'C');
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'K');
		}
		else 
			if(viewportNum == V3) {   // V1,V2,V3,V4 CW starting at persp view lower right
				glColor3fv(colors[1]);  //Blue
				glRasterPos3f(0,5,-1);
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'L');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'E');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'F');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'T');
			}
		else 
			if(viewportNum == V2) {   // V1,V2,V3,V4 CW starting at persp view lower right
				glColor3fv(colors[3]);  //Orange
				glRasterPos3f(-1,0,5);
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'B');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'O');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'T');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'T');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'O');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'M');
			}
	}  // end if(view==1)

	if(view == 2) {
		if(viewportNum == V4) {   // V1,V2,V3,V4 CW starting at persp view lower right
			glColor3fv(colors[6]);  //White
			glRasterPos3f(1,5,0);
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'F');
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'R');
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'O');
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
			glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'T');
		}
		else 
			if(viewportNum == V3) {   // V1,V2,V3,V4 CW starting at persp view lower right
				glColor3fv(colors[2]);  //Green
				glRasterPos3f(0,5,-1);
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'R');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'I');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'G');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'H');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'T');
			}
		else 
			if(viewportNum == V2) {   // V1,V2,V3,V4 CW starting at persp view lower right
				glColor3fv(colors[4]);  //Red
				glRasterPos3f(-1,0,5);
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'T');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'O');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'P');
			}
	}  // end if(view==2)

	if(viewportNum == V1) {   // V1 persp view lower right
		if(perspView ==1) {
			    glColor3fv(colors[6]);  //White
				glRasterPos3f(0,0,9);
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'F');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'R');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'O');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'N');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'T');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '(');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'Z');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ')');
				glBegin(GL_LINES);
					glVertex3f(0,0,-5*(cubeSize+space));
					glVertex3f(0,0,5*(cubeSize+space));
				glEnd();
				glColor3fv(colors[2]);  //Green
				glRasterPos3f(7,0,0);
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'R');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'I');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'G');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'H');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'T');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '(');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'X');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ')');
				glBegin(GL_LINES);
					glVertex3f(-5*(cubeSize+space),0,0);
					glVertex3f(5*(cubeSize+space),0,0);
				glEnd();
				glColor3fv(colors[4]);  //Red
				glRasterPos3f(0,7,0);
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'T');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'O');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'P');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '(');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'Y');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ')');
				glBegin(GL_LINES);
					glVertex3f(0,-5*(cubeSize+space),0);
					glVertex3f(0,5*(cubeSize+space),0);
				glEnd();
			}
	
		else
			if(perspView ==2) {
				glColor3fv(colors[5]);  //Yellow
				glRasterPos3f(0,0,-9);
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'B');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'A');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'C');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'K');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '(');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'Z');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ')');
				glColor3fv(colors[6]);  //White
				glBegin(GL_LINES);
					glVertex3f(0,0,-5*(cubeSize+space));
					glVertex3f(0,0,5*(cubeSize+space));
				glEnd();
				glColor3fv(colors[1]);  //Blue
				glRasterPos3f(-7,0,0);
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'L');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'E');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'F');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'T');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '(');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'X');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ')');
				glColor3fv(colors[2]);  //Green
				glBegin(GL_LINES);
					glVertex3f(-5*(cubeSize+space),0,0);
					glVertex3f(5*(cubeSize+space),0,0);
				glEnd();
				glColor3fv(colors[3]);  //Orange
				glRasterPos3f(0,-7,0);
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'B');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'O');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'T');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'T');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'O');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'M');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, '(');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, 'Y');
				glutBitmapCharacter(GLUT_BITMAP_9_BY_15, ')');
				glColor3fv(colors[4]);  //Red
				glBegin(GL_LINES);
					glVertex3f(0,-5*(cubeSize+space),0);
					glVertex3f(0,5*(cubeSize+space),0);
				glEnd();
			}
		} 
	glLineWidth(1);  //Reset line width
	//slice one
	glTranslatef(0,0,-(cubeSize+space));
	drawZSlice(1);
	//2nd slice 
	glTranslatef(0,0,+(cubeSize+space));
	drawZSlice(2);
	//3rd slice 
	glTranslatef(0,0,+(cubeSize+space));
	drawZSlice(3);
	glPopMatrix();
}

void keyHandler(unsigned char key, int x, int y)
{
	int i = 0, j = 0;
	switch(key)
	{
	case 'X':
	case 'x':
		if(animateX)
		{
			cubeRot=0;
			animateX = 0;
			glPopMatrix();
		}
		else
		{
			animateX = 1;
			glPushMatrix();
			animateXCube();  
			glutTimerFunc(33, animateXCube, -1);

		}
		break;
	case 'Y':
	case 'y':
		if(animateY)
		{
			cubeRot=0;
			animateY = 0;
			glPopMatrix();
		}
		else
		{
			animateY = 1;
			glPushMatrix();
			animateYCube();  
		    glutTimerFunc(33, animateYCube, -1);
			
		}
		break;
	case 'Z':
	case 'z':
		if(animateZ)
		{
			animateZ = 0;
			cubeRot=0;
			glPopMatrix();
		}
		else
		{
			animateZ = 1;
			glPushMatrix();
			animateZCube();
			glutTimerFunc(33, animateZCube, -1);
		}
		break;
	case '1':
		if(animateSlice1)
		{
			animateSlice1 = 0;
		}
		else
		{
			animateSlice1 = 1;
			for(rot=1; rot<=90; rot++)
			{
				animateZSlice(1);  //  1st z slice CCW use 'r' for CW
				//_sleep(waitTime);
			}
		}
		break;
	case '2':
		if(animateSlice2)
		{
			animateSlice2 = 0;
		}
		else
		{
			animateSlice2 = 1;
			for(rot=1; rot<=90; rot++)
			{
				animateZSlice(2);  //  2nd z slice (middle z slice) CCW use 'r' for CW
				//_sleep(waitTime);
			}
		}
		break;
		case '3':
		if(animateSlice3)
		{
			animateSlice3 = 0;
		}
		else
		{
			animateSlice3 = 1;
			for(rot=1; rot<=90; rot++)
			{
				animateZSlice(3);
				//_sleep(waitTime);
			}
		}
		break;
		case '4':
		if(animateSlice4)
		{
			animateSlice4 = 0;
		}
		else
		{
			animateSlice4 = 1;
			for(rot=1; rot<=90; rot++)
			{
				animateYSlice(4);
				//_sleep(waitTime);
			}
		}
		break;	
	case '5':
		if(animateSlice5)
		{
			animateSlice5 = 0;
		}
		else
		{
			animateSlice5 = 1;
			for(rot=1; rot<=90; rot++)
			{
				animateYSlice(5);
				//_sleep(waitTime);
			}
		}
		break;	
	case '6':
		if(animateSlice6)
		{
			animateSlice6 = 0;
		}
		else
		{
			animateSlice6 = 1;
			for(rot=1; rot<=90; rot++)
			{
				animateYSlice(6);
				//_sleep(waitTime);
			}
		}
		break;	
	case '7':
		if(animateSlice7)
		{
			animateSlice7 = 0;
		}
		else
		{
			animateSlice7 = 1;
			for(rot=1; rot<=90; rot++)
			{
				animateXSlice(7);
				//_sleep(waitTime);
			}
		}
		break;	
	case '8':
		if(animateSlice8)
		{
			animateSlice8 = 0;
		}
		else
		{
			animateSlice8 = 1;
			for(rot=1; rot<=90; rot++)
			{
				animateXSlice(8);
			//	_sleep(waitTime);
			}
		}
		break;	
	case '9':
		if(animateSlice9)
		{
			animateSlice9 = 0;
		}
		else
		{
			animateSlice9 = 1;
			for(rot=1; rot<=90; rot++)
			{
				animateXSlice(9);
			//	_sleep(waitTime);
			}
		}
		break;
	case 'L':
	case 'l':
		if(lightingEnabled == 0)
		{
			lightingEnabled = 1;
		}
		else
		{
			lightingEnabled = 0;
		}
		break;
	case 'W':
	case 'w':
		if(wireframe == 0)
		{
			wireframe = 1;
			polygonMode = GL_FILL;
		}
		else
		{
			wireframe = 0;
			polygonMode = GL_LINE;
		}
		break;
	case 'S':
	case 's':
		if(mode == GL_SMOOTH)
			mode = GL_FLAT;
		else
			mode = GL_SMOOTH;
		break;
	case 'F':
	case 'f':
		if(fullscreen == 0)
		{
			savedww = ww;
			savedwh = wh;
			glutFullScreen();
			fullscreen = 1;
		}
		else
		{
			glutPositionWindow(10, 10);
			glutReshapeWindow(savedww, savedwh);
			fullscreen = 0;
		}
		break;
	case '-':
	case '_':
		eyeX++; eyeY++; eyeZ++;
		break;
	case '+':
	case '=':
		eyeX--; eyeY--; eyeZ--;
		break;
	case 'Q':
	case 'q':
		exit(0);
	    break;
	case 'r':
	case 'R':
		if(rotMult == 1)
			rotMult = -1;
		else
			rotMult = 1;
		break;
	case '<':
	case ',': 
			eyeX =cubeSize-10;  
			eyeY =cubeSize-10;   
			eyeZ =cubeSize-10;
			perspView=2;
		break;
	case '>':
	case '.':
			eyeX =cubeSize+10;  
			eyeY =cubeSize+10;   
			eyeZ =cubeSize+10;
			perspView=1;
		break;
	case 'V':
	case 'v': 
			if(view==1)
				view=2;
			else  //view==2
				view=1;
		break;
	case 'T':
	case 't': 
			if(texture==1)
				texture=0;
			else  //texture = off so turn on
				texture=1;
		break;
	}
	glutPostRedisplay();
}

void display(void)
{
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glPushMatrix();
	//(lower-left)
	glViewport(0,0,ww/2, wh/2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(left, right, bottom, top, near, far);
	
	if(perspView==1) { //forward and above
		if(view == 1)  // toggle bottom view
			gluLookAt(0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);
		else           // toggle top view
			gluLookAt(0.0,1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);
	}
 
	if(perspView==2) { // opposite view
		if(view == 1)  //toggle bottom view
			gluLookAt(0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, -1.0);
		else           // toggle top view
			gluLookAt(0.0,1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0);   
	}
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	drawCube(V2);
	// (upper-right)
	glViewport(ww/2,wh/2,ww/2,wh/2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(left, right, bottom, top, near, far);
	if(view==1)
		gluLookAt(0.0, 0.0, -1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	else
		gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);	
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	drawCube(V4);
	// (upper-left)
	glViewport(0,wh/2,ww/2,wh/2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(left, right, bottom, top, near, far);
	if(view==1)
		gluLookAt(-1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	else
		gluLookAt(1.0, 0.0, 0.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	drawCube(V3);
	// Perspective viewport (lower-right)
	glViewport(ww/2, 0, ww/2,wh/2);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	gluPerspective(lensAngle, (GLdouble)ww/(GLdouble)wh, 0.5, 1000);
	gluLookAt(eyeX, eyeY, eyeZ, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	drawCube(V1);
	glPopMatrix();
	glutSwapBuffers();
}

void myinit(void)
{  
	GLfloat light_position[] = { 1.0, 0.0, 0.0, 0.0 };
	GLfloat light_position1[] = { 0.0, 1.0, 0.0, 0.0 };

	/* Change the color of the light */
	GLfloat white_light[] = { 1.0, 1.0, 1.0, 1.0 };
	GLfloat lmodel_ambient[] = { 0.3, 0.3, 0.3, 1.0 };

	glShadeModel(GL_SMOOTH);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glMatrixMode(GL_MODELVIEW);
	glClearColor(0.0, 0.0, 0.0, 1.0);
	glColor3f(0.0, 0.0, 0.0);
	glLightfv(GL_LIGHT0, GL_POSITION, light_position);
	glLightfv(GL_LIGHT1, GL_POSITION, light_position1);
	/* Your specular light could be different */
	glLightfv(GL_LIGHT0, GL_SPECULAR, white_light);
	glLightfv(GL_LIGHT1, GL_SPECULAR, white_light);
	glLightfv(GL_LIGHT0, GL_AMBIENT, white_light);
	glLightfv(GL_LIGHT1, GL_AMBIENT, white_light);
	glLightfv(GL_LIGHT0, GL_DIFFUSE, white_light);
	glLightfv(GL_LIGHT1, GL_DIFFUSE, white_light);
	glLightModelfv(GL_LIGHT_MODEL_AMBIENT, lmodel_ambient);

	/* Even though you set stuff, you need to enable these features */
	glEnable(GL_LIGHTING);
	glEnable(GL_LIGHT0);
	glEnable(GL_LIGHT1);
	glEnable(GL_AUTO_NORMAL);
	glEnable(GL_DEPTH_TEST);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glutSetCursor(GLUT_CURSOR_CROSSHAIR);
	initCubeVerts();

	/* load an image file directly as a new OpenGL texture */
	/*****
	tex_2d = SOIL_load_OGL_texture
	(
		"stone.jpg",
		SOIL_LOAD_AUTO,
		SOIL_CREATE_NEW_ID,
		SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB | SOIL_FLAG_COMPRESS_TO_DXT
	);
	printf("%i\n", tex_2d);
	// check for an error during the load process 
	if( 0 == tex_2d )
	{
		printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
	}	
	*********************/

	buildCube(); // set up initial cube -- colors...
	lightingEnabled = 0;
}

void myMenu(int id)
{
	switch( id )
	{
	case 1:
		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glutSwapBuffers();
		break;
	case 2:
		exit(0);
		break;
	default:
		break;
	}
	glutPostRedisplay();
}

void perspMenu(int id) {
	switch(id){
		case 15:
			lensAngle = 15;
			break;
		case 30:
			lensAngle = 30;
			break;
		case 45:
			lensAngle = 45;
			break;
		case 60:
			lensAngle = 60;
			break;
		case 75:
			lensAngle = 75;
			break;
		case 90:
			lensAngle = 90;
			break;
		case 120:
			lensAngle = 120;
			break;
		default:
			printf("unknown value\n");
			break;
	}
	glutPostRedisplay();
}

void imageMenu(int id) {
	/*****
	switch(id){
		case 1:
			// load an image file directly as a new OpenGL texture 
			tex_2d = SOIL_load_OGL_texture
			(
				"stone.jpg",
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB |
				SOIL_FLAG_COMPRESS_TO_DXT
			);
			break;
		case 2:
			// load an image file directly as a new OpenGL texture 
			tex_2d = SOIL_load_OGL_texture
			(
				"water_drops.jpg",
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB |
				SOIL_FLAG_COMPRESS_TO_DXT
			);
			break;
		case 3:
			// load an image file directly as a new OpenGL texture 
			tex_2d = SOIL_load_OGL_texture
			(
				"mich_me.jpg",
				SOIL_LOAD_AUTO,
				SOIL_CREATE_NEW_ID,
				SOIL_FLAG_MIPMAPS | SOIL_FLAG_INVERT_Y | SOIL_FLAG_NTSC_SAFE_RGB |
				SOIL_FLAG_COMPRESS_TO_DXT
			);
			break;
		default:
			printf("unknown value\n");
			break;
	}
	printf("%i\n", tex_2d);
	// check for an error during the load process 
	if( 0 == tex_2d )
	{
		printf( "SOIL loading error: '%s'\n", SOIL_last_result() );
	}	
	glutPostRedisplay();
	********/
}

int main(int argc, char **argv)
{
	int menu_id, persp_id, rotate_id, image_id;
	eyeX =cubeSize+10;  
	eyeY =cubeSize+10;   
	eyeZ =cubeSize+10;

	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB | GLUT_DEPTH);
	glutInitWindowSize(500, 500);
	glutCreateWindow("Rubik's Cube");
	glutDisplayFunc(display);
	glutMouseFunc(mouse);
	glutKeyboardFunc(keyHandler);
	glutReshapeFunc(reshape);
	glutTimerFunc(33, animateXCube, -1);
	glutTimerFunc(33, animateYCube, -1);
	glutTimerFunc(33, animateZCube, -1);
	myinit();
	persp_id = glutCreateMenu(perspMenu);
	glutAddMenuEntry("15", 15);
	glutAddMenuEntry("30", 30);
	glutAddMenuEntry("45", 45);
	glutAddMenuEntry("60", 60);
	glutAddMenuEntry("75", 75);
	glutAddMenuEntry("90", 90);
	glutAddMenuEntry("120", 120);

	image_id = glutCreateMenu(imageMenu);
	glutAddMenuEntry("Stone", 1);
	glutAddMenuEntry("Water drops", 2);
	glutAddMenuEntry("People", 3);

	menu_id = glutCreateMenu(myMenu);
	glutAddSubMenu("Lens Angle", persp_id);
	glutAddSubMenu("Texture Image", image_id);
	glutAddMenuEntry("Exit", 2);
	glutAttachMenu(GLUT_RIGHT_BUTTON);
	glutMainLoop();
}