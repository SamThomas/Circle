#include <ComputerVision_jni.h>

#include <iostream>
#include <string>
#include <sstream>
#include <vector>
#include <cstdlib>

#include <ctime>

JNIEXPORT int JNICALL Java_com_example_circles_MainActivity_process
(JNIEnv *jenv, jobject obj, jlong addrGray, jlong addrRgba)
{
	if(DEBUG)
		LOGD("Java_com_example_circles_MainActivity_process");

	Mat& mRgb = *(Mat*)addrRgba;
	vector<Vec3f> circles;

	//Timer
	clock_t begin, end;
	double time_spent;
	begin = clock();

	//Traitement de l'image
	find_circles(addrGray,addrRgba,&circles);
	end = clock();

	//Centrer le cercle vert à l'écran
	computeOrders(addrRgba,&circles);

	//Dessiner le cercle
	drawCircles(addrRgba,&circles);


	time_spent = (double)(end - begin) / 1000.0;
	ostringstream os;
	os << time_spent;
	string text = os.str();
	Point center(50,50);
	//putText(mRgb, text, center,FONT_HERSHEY_PLAIN,3,Scalar(255,0,0),4,8,false);

	return circles.size();
}

void computeOrders(jlong addImage, vector<Vec3f>* circles)
{
	//Paramètre
	int delta = 40;		//tolerance avec le point central (pixels)
	int delta2 = 20;	//tolerance avec la distance entre le cercle et le device (pixels)
	int dist = 100; 	//Rayon cible (pixels)

	Mat& mRgb = *(Mat*)addImage;

	//Si l'on détecte un cercle
	if(circles->size() != 0)
	{
		size_t i = 0;

		//Information sur les cercles (0 = le premier cercle detecté)
		//Rayon, centre x and centre y
		int rr= ((int)(*circles)[i][2]);
		int xx= ((int)(*circles)[i][0]);
		int yy= ((int)(*circles)[i][1]);

		//Image postion central
		int cxx = (mRgb.cols)/2;
		int cyy = (mRgb.rows)/2;

		//Print cercle (vert)
		Point cent(cxx,cyy);
		circle( mRgb, cent, dist, Scalar(0,255,0), 3, 8, 0 );

		//Texte a afficher lorsque l'on ai bien centré
		string text = "Aller";

		//Traitement...
		if(xx-cxx > delta)
		{
			text.append(" droite");
		}
		else if(xx-cxx < -delta)
		{
			text.append(" gauche");
		}
		if(yy-cyy > delta)
		{
			text.append(" reculer");
		}
		else if(yy-cyy < -delta)
		{
			text.append(" en avant");
		}
		if(rr-dist > delta2)
		{
			text.append(" loin");
		}
		else if(rr-dist < -delta2)
		{
			text.append(" proche");
		}

		//Print les ordres à l'écran (rouge)
		Point center(50,50);
		int fontFace = FONT_HERSHEY_PLAIN;
		double fontScale = 3;
		Scalar col = Scalar(255,0,0);
		putText(mRgb, text, center,fontFace, fontScale,col,4,8,false);
	}
}

void find_circles(jlong addrGray, jlong addrRgba, vector<Vec3f>* circles)
{
	Mat& mGr  = *(Mat*)addrGray;
	Mat& mRgb = *(Mat*)addrRgba;

	//Flouter l'image pour détecter le meilleur cercle
	medianBlur(mGr,mGr,5);

	//Transformée de Hough
	HoughCircles(mGr, 			//Noir/Blanc de l'image
			*circles, 			//Vecteur sortie
			CV_HOUGH_GRADIENT,	//Méthode de detection a utiliser
			4, 					//Inverser le ratio
			mGr.rows/8, 		//Distance minimum entre le centre des cercles détecté
			220, 				//Filtre de Canny
			200, 				//Seuil accumulateur 100
			20, 				//Rayon minimm
			mGr.cols/4			//Rayon maximum
	);
}

void drawCircles(jlong addrImage, vector<Vec3f>* circles)
{
	Mat& mRgb = *(Mat*)addrImage;

	//Dessiner chaque cercle détecté
	for( size_t i = 0; i < circles->size(); i++ )
	{
		Point center(cvRound(((double)(*circles)[i][0])), cvRound(((double)(*circles)[i][1])));
		int radius = cvRound(((double)(*circles)[i][2]));
		// Dessiner le centre du cercle
		circle( mRgb, center, 3, Scalar(255,0,255), -1, 8, 0 );	// pourpre
		// Dessiner le contour du cercle
		circle( mRgb, center, radius, Scalar(255,0,0), 3, 8, 0 );	//rouge
	}
}
