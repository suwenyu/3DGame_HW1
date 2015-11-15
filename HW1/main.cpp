//3D game programming 2013
//lab2: 2D


#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <windows.h>
//#include <GL/glew.h>
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <time.h>
#include <string.h>

#include "RGBpixmap.h"

// Global variables for measuring time (in milli-seconds)
int startTime;
int prevTime;

//  http://devmaster.net/forums/topic/7934-aabb-collision/
bool AABBtest(float ax1, float ay1, float ax2, float ay2, float bx1, float by1, float bx2, float by2)
{
    return
        ax1 > bx2 || ax2 < bx1 ||
        ay1 > by2 || ay2 < by1;
}

static void CheckError(int line)
{
   GLenum err = glGetError();
   if (err) {
      printf("GL Error %s (0x%x) at line %d\n",
             gluErrorString(err), (int) err, line);
   }
}

//Set windows
int screenWidth = 1024 , screenHeight = 768;

int i=0;
RGBApixmap pic[12]; // create two (empty) global pixmaps
RGBApixmap bg;
int whichPic = 0; // which pixmap to display
int picX=100, picY=270;

bool AttackHit = false;
bool EnemyIsAttack = false;

int goldfingerX=-500 , goldfingerY=270;
int rectX=300, rectY=270;
int enemyX=900 , enemyY=270;
int MarioAttackX=picX + 30 , MarioAttackY = picY + 25;

int Mariolife = 5 ;
int Enemylife = 5;

int GoldFinger =11;
int WinGame = 10;
int GameOver = 9;
int Enemy_Life = 8;
int Mario_Life = 6;
int background = 7;
int MarioattackPic = 5;
int enemyPic = 3;
int attackPic = 4;

float rotation_test=0;
float scale_test=0.3;


int attackState = 0;
int jumpState=0;
int DirectState=0;  //0:right  1:left
bool returnround = false;
bool firstuse = true;

void init();

//<<<<<<<<<<<<<<<<<<<<<<<<< myMouse >>>>>>>>>>>>>>>>>>>>>>>>
/*void myMouse(int button, int state, int mx, int my)
{ // set raster position with a left click
	if(button == GLUT_LEFT_BUTTON )
	{


		glutPostRedisplay();
	}

}*/
//<<<<<<<<<<<<<<<<<<<<<<<<< mouseMove >>>>>>>>>>>>>>>>>
/*void mouseMove(int x, int y)
{// set raster position with mouse motion
	//rasterPos.x = x; rasterPos.y = screenHeight - y;
	//glRasterPos2i(rasterPos.x, rasterPos.y);
	glutPostRedisplay();
}*/

//myReshape
void myReshape(int w, int h)
{
    /* Save the new width and height */
    screenWidth  = w;
    screenHeight = h;

    /* Reset the viewport... */
    glViewport(0, 0, screenWidth, screenHeight);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glOrtho(0.0, (GLfloat)screenWidth, 0.0, (GLfloat)screenHeight, -1.0, 1.0);
    glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
}

void enemydeadend()
{
    pic[GameOver].blendTex(0, 0);
    glutSwapBuffers();
}

void End()
{
    glutDisplayFunc(enemydeadend);
}

void MarioWin()
{
    cout << "great\n";
    pic[WinGame].blendTex(0, 0);
    glutSwapBuffers();
}

void Win()
{
    glutDisplayFunc(MarioWin);
}
//myDisplay
void First()
{
    cout << "HaHa!!\n";
    
}

void FirstUse()
{
    glutDisplayFunc(First);
}

void myDisplay(void)
{
//    if (firstuse == true) {
//        FirstUse();
//        firstuse = false;
//    }
	// Measure the elapsed time
	int currTime = glutGet(GLUT_ELAPSED_TIME);
	int timeSincePrevFrame = currTime - prevTime;
	int elapsedTime = currTime - startTime;
	prevTime = currTime;

	char fpsmss[30];
	//sprintf(fpsmss, "Fps %.1f", 1000.0/timeSincePrevFrame);


	glClear(GL_COLOR_BUFFER_BIT);

    //draw background
    
    
    glRasterPos2i(50, 50);
    bg.blend();

    
    pic[background].blendTex(0,0);
    
    // pic[MarioattackPic].blendTex(picX, picY);
    
    int reverse = pic[GoldFinger].nCols;
    pic[GoldFinger].blendTex(goldfingerX+reverse, goldfingerY, -1, 1);
    
    pic[enemyPic].blendTex(enemyX,enemyY);
    
    pic[MarioattackPic].blendTex(MarioAttackX , MarioAttackY);
    pic[attackPic].blendTex(rectX , rectY);

	//change direction
	if(DirectState==0) {
		pic[whichPic].blendTex(picX, picY);
		
	}else {
		int offset = pic[whichPic].nCols;
		pic[whichPic].blendTex(picX+offset, picY, -1, 1);
	}

	//rotation test
	//pic[whichPic].blendTexRotate(100, 250, 1, 1, rotation_test);

	//scale test
	//pic[whichPic].blendTexRotate(250, 250, scale_test, scale_test);

//	bouding box outside test
    bool goldfingerHit = !AABBtest(goldfingerX, goldfingerY, goldfingerX+pic[GoldFinger].w(), goldfingerY+pic[GoldFinger].h(), enemyX, enemyY, enemyX+pic[enemyPic].w(), enemyY+pic[enemyPic].h());
    
    
    bool attackHit = !AABBtest(MarioAttackX, MarioAttackY, MarioAttackX+pic[MarioattackPic].w(), MarioAttackY+pic[MarioattackPic].h(), enemyX, enemyY, enemyX+pic[enemyPic].w(), enemyY+pic[enemyPic].h());
    
    bool Hit = !AABBtest(picX, picY, picX + pic[whichPic].w(), picY + pic[whichPic].h(), rectX, rectY, rectX + pic[attackPic].w(), rectY + pic[attackPic].h());
	
    if(Hit && AttackHit == false) {
        Mariolife -=1 ;
//        glutSwapBuffers();
        AttackHit =true;
        
        if (Mariolife <= 0)
        {
            End();
        }
	}
    if (attackHit && EnemyIsAttack == false) {
        MarioAttackX = picX + 30;
        MarioAttackY = picY + 20;
        Enemylife--;
        EnemyIsAttack = true;
        if (Enemylife <= 0) {
            Win();
        }
        
    }
    if (goldfingerHit && EnemyIsAttack == false) {
        Enemylife-=3;
        EnemyIsAttack = true;
        if (Enemylife <= 0) {
            Win();
        }
        
    }
//    // glRectf(rectX, rectY, rectX + 25, rectY + 25);
//
//
//	//Font
    for ( int i = 0 ; i < Mariolife ; i++ )
    {
        pic[Mario_Life].blendTex(100+50*i, 550);
    }
    
    for (int i =0 ; i < Enemylife; i++ ) {
        pic[Enemy_Life].blendTex(1000 + 50*i, 550);
    }
    

//	glColor3f(0.0, 0.5, 0.3);  //set font color
//    glRasterPos2i(screenWidth-100, 550);    //set font start position
//	for(int i=0; i < Enemylife; i++) {
//        cout << Enemylife << endl;
//        //		glutBitmapCharacter(GLUT_BITMAP_TIMES_ROMAN_24, fpsmss[i]);
//	}

	CheckError(__LINE__);

	glutSwapBuffers();
}



void SpecialKeys(int key, int x, int y)
{
	// this string keeps the last good setting for the game mode
    //char gameModeString[40] = "800x600";

	switch(key) {
	/*	case GLUT_KEY_UP:
			picY += 5;
			break;
		case GLUT_KEY_DOWN:
			picY -= 5;
			break;*/
		case GLUT_KEY_LEFT:
			picX -= 10;
            MarioAttackX-=10;
			if (whichPic==0) whichPic=1;
			else whichPic=0;
			DirectState=0;
			break;
		case GLUT_KEY_RIGHT:
			picX += 10;
            MarioAttackX += 10;
			if (whichPic==0) whichPic=1;
			else whichPic=0;
			DirectState=1;
			break;

	    /*
		case GLUT_KEY_F1:
			// define resolution, color depth
			glutGameModeString("640x480:32");
			// enter full screen
			if (glutGameModeGet(GLUT_GAME_MODE_POSSIBLE)) {
				glutEnterGameMode();
				sprintf(gameModeString,"640x480:32");
				// register callbacks again
				// and init OpenGL context
				init();
			}
			else
				glutGameModeString(gameModeString);
			break;

	    case GLUT_KEY_F2:
				glutFullScreen();
			break;
		case GLUT_KEY_F6:
			// return to default window
			if (glutGameModeGet(GLUT_GAME_MODE_ACTIVE) != 0)
				glutLeaveGameMode();
			break;
		*/
    }
	glutPostRedisplay();
}



void jump(int i)
{
    whichPic=2;

	if(i<10) {
		if (i<5) {
			picY+=20;
            MarioAttackY+=20;
		} else {
			picY-=20;
            MarioAttackY-=20;
		}
		i++;
		glutTimerFunc( 100, jump, i);
	}
    else {
		whichPic=0;
		jumpState=0;
	}
        glutPostRedisplay();
}

void enemyJump (int i )
{
    
    if ( i < 10 )
    {
        if (i < 5)
            enemyY += 30;
        else
            enemyY -= 30;
        
        
        if (enemyX >= 500 && returnround == false)
        {
            enemyX -= 20;
        }
        else
        {
            returnround = true;
            enemyX += 20;
            
            if (enemyX >= 880) {
                returnround = false;
            }
        }
        
    i++;
    glutTimerFunc(100, enemyJump, i);
    }
    
    glutPostRedisplay();
}

//back and forth
void bf(int i)
{

    /*
        Your Implementation
	*/

	glutPostRedisplay();
}


void fly(int i)
{

	/*
        Your Implementation
	*/

	glutPostRedisplay();
}


void update(int i)
{
	double r = double(i)/50.0;
	
	rectX = enemyX - i*20;
    
    if (rectX == (enemyX - 20) )
        enemyJump(0);

	rotation_test += 2;
	scale_test += 0.01;
    
	++i;
//    if (Hit)
//    {
//        
//    }
    if (rectX == 0)
    {
        i = 0;
        AttackHit = false;
        EnemyIsAttack = false;
    }
    glutTimerFunc( 33, update, i);
    
	glutPostRedisplay();
}


void attack (int i)
{
    //glutDisplayFunc(attackdisplay);
    if (i < 10)
    {
        if (i%2 == 0)
            MarioAttackY = picY + 20 - 10;
        else
            MarioAttackY=picY + 20 + 10;
        
        MarioAttackX=picX + 30 + 30*i;
        i++;
        glutTimerFunc(100, attack, i);
    }
    else
    {
        MarioAttackX = picX+30;
        MarioAttackY = picY+20;
        attackState = 0;
    }
    
}
void goldfinger (int i)
{
    if(i < 10)
    {
        goldfingerX = goldfingerX + 50 * i;
        i++;
        glutTimerFunc(200, goldfinger, i);
    }
    else
    {
        goldfingerX = -500 ;
        goldfingerY = 270 ;
        attackState =0;
    }
}

//<<<<<<<<<<<<<<<<<<<<<<<< myKeys >>>>>>>>>>>>>>>>>>>>>>
void myKeys(unsigned char key, int x, int y)
{
	switch(key)
	{
        case 'Q':
        case 'q':
			exit(0);
			break;

		case 'm':
		case ' ':
			if(jumpState==0) {
				jumpState=1;
				jump(0);
			}
			break;

        case 'b':
				bf(0);
			break;

        case 'f':
				fly(0);
			break;
        case 'a':
            if (attackState == 0) {
                attackState = 1;
                attack(0);
            }
            break;
        case 'w':
            if (attackState == 0) {
                for (int i=0; i<5; i++) {
                    goldfinger(0);
                }
                    attackState = 1;
            }
            break;
	} //switch(key)

	glutPostRedisplay();
}


void init()
{
	//GLenum err = glewInit();
	//if (GLEW_OK != err)
	//{
	//  // Problem: glewInit failed, something is seriously wrong.
	//  fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	//}
	
	glutSpecialFunc(SpecialKeys);
	glutKeyboardFunc(myKeys);
//	glutMouseFunc(myMouse);
//	glutMotionFunc(mouseMove);
//    glutDisplayFunc(substitle);
	glutDisplayFunc(myDisplay);
	glutReshapeFunc(myReshape);

	glShadeModel(GL_SMOOTH); 
	//glEnable(GL_DEPTH_TEST);

    glClearColor(1.0f, 1.0f, 0.0f, 0.0f); //background color(1.0, 1.0, 1.0): white color
}


int main(int argc, char **argv)
{
	glutInit(&argc, argv);
	glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
	glutInitWindowSize(screenWidth, screenHeight);
	glutInitWindowPosition(50, 30);
	glutCreateWindow("2D World");

	init();
    

	srand(time(0));  //rand seed

	cout<<"Reading sprite";
    pic[0].readBMPFile("image/Mario_Stand.bmp");  cout<<'.';
	pic[1].readBMPFile("image/Mario_Walk.bmp");  cout<<'.';
    pic[2].readBMPFile("image/Mario_Jump.bmp");  cout<<'.';
    pic[3].readBMPFile("image/Bowser.bmp");  cout<<'.';
    pic[4].readBMPFile("image/Bowserturtle.bmp");cout << '.';
    pic[5].readBMPFile("image/Mario_Fireball.bmp"); cout<<'.';
    pic[6].readBMPFile("image/Mario_Life.bmp"); cout<<'.';
    pic[7].readBMPFile("image/Battle.bmp");  cout<<'.';
    pic[8].readBMPFile("image/BowserLife.bmp");  cout<<'.';
    pic[9].readBMPFile("image/GameOver.bmp");  cout<<'.';
    pic[10].readBMPFile("image/Win.bmp");  cout<<'.';
    pic[11].readBMPFile("image/GoldFinger.bmp");  cout<<'.'<<endl;
    for (int i=0; i<3; i++) pic[i].setChromaKey(0, 0, 0);
    for (int i=3 ; i<7 ; i++ ) pic[i].setChromaKey( 255, 255 , 255);
    for (int i=8; i<9; i++) pic[i].setChromaKey( 255, 255 , 255);
    for (int i=11; i<12; i++) pic[i].setChromaKey(255, 255, 255);
    
	//cout<<"Reading Backgroud........"<<endl;
	//bg.readBMPFile("image/dog.bmp");

	// Initialize the time variables
	startTime = glutGet(GLUT_ELAPSED_TIME);
	prevTime = startTime;
    
    update(0);
    
    
	glutMainLoop();
    
    
	return 0;
}

