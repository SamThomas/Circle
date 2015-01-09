package com.example.circles;
import org.opencv.android.BaseLoaderCallback;
import org.opencv.android.CameraBridgeViewBase;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewFrame;
import org.opencv.android.CameraBridgeViewBase.CvCameraViewListener2;
import org.opencv.android.LoaderCallbackInterface;
import org.opencv.android.OpenCVLoader;
import org.opencv.core.Mat;

import com.codeminders.ardrone.ARDrone;
import com.example.circles.R;
import android.os.Bundle;
import android.app.Activity;
import android.util.Log;
import android.view.WindowManager;

public class MainActivity extends Activity implements CvCameraViewListener2{
	
	private static String TAG = "Circles";

	private CameraBridgeViewBase mOpenCvCameraView;
	
	//Debug
	private static boolean DEBUG = true;
	
	//Drone
	private static final long CONNECT_TIMEOUT = 3000;
	public static ARDrone drone;
		
	@Override
	public void onCreate(Bundle savedInstanceState) {
		
		if(DEBUG)
			Log.i(TAG, "Appel de onCreate");
		
		super.onCreate(savedInstanceState);

		//Empêcher l'écran de s'éteindre
		getWindow().addFlags(WindowManager.LayoutParams.FLAG_KEEP_SCREEN_ON);

		//spécifier le "main" 
		setContentView(R.layout.activity_main);

		//Afficher à l'écran le flux video
		mOpenCvCameraView = (CameraBridgeViewBase) findViewById(R.id.MyCameraView);
		//Camera Front mOpenCvCameraView.setCameraIndex(1);
		//mOpenCvCameraView.setCameraIndex(1);
		mOpenCvCameraView.setMaxFrameSize(800, 600);
		mOpenCvCameraView.setCvCameraViewListener(this);
		
        try
        {
            // Create ARDrone object,
            // connect to drone and initialize it.
            drone = new ARDrone();
            drone.connect();
            drone.clearEmergencySignal();

            // Wait until drone is ready
            drone.waitForReady(CONNECT_TIMEOUT);

            // do TRIM operation
            drone.trim();

        } catch(Throwable e)
        {
            e.printStackTrace();
            System.err.println("Non trouve");
        }

	}

	
	   //Methode appeler quand une nouvel frame arrive
	public Mat onCameraFrame(CvCameraViewFrame inputFrame) {
		Mat mRgba = inputFrame.rgba();
		
		//On convertie l'image en noir/blanc
		Mat mGray = inputFrame.gray();

			//Utilisation du code cpp
			int nbcircles = process(mGray.getNativeObjAddr(), mRgba.getNativeObjAddr());	
			
			if(DEBUG)
				Log.e(TAG , nbcircles+" cercle trouve");
			
			 try
		        {
		            // Take off
		            System.err.println("Taking off");
		            drone.takeOff();

		            // Fly a little :)
		            Thread.sleep(5000);

		            // Land
		            System.err.println("Landing");
		            drone.land();

		            // Give it some time to land
		            Thread.sleep(2000);
		           
		            // Disconnect from the done
		            drone.disconnect();

		        } catch(Throwable e)
		        {
		            e.printStackTrace();
		            System.err.println("Non trouve");
		        }
			
		//Afficher l'image à l'écran
		return mRgba;
	}

	//Fonction cpp pour le traitement d'image
	private static native int process(long matAddrGr, long matAddrRgba);


	//OpenCVManager
	@Override
	public void onPause()
	{
		super.onPause();
		if (mOpenCvCameraView != null)
			mOpenCvCameraView.disableView();
	}

	public void onDestroy() {
		super.onDestroy();
		if (mOpenCvCameraView != null)
			mOpenCvCameraView.disableView();
	}

	//Debug
	public void onCameraViewStarted(int width, int height) 
	{
		if(DEBUG)
		Log.i(TAG , "Taille de l'image est "+width+"x"+height);
	}

	public void onCameraViewStopped() 
	{
		if(DEBUG)
		Log.i(TAG , "CameraViewStopped");
	}

	//Initialisation OpenCV
	private BaseLoaderCallback mLoaderCallback = new BaseLoaderCallback(this) {
		@Override
		public void onManagerConnected(int status) {
			switch (status) {
			case LoaderCallbackInterface.SUCCESS:
			{
				if(DEBUG)
				Log.i(TAG , "OpenCV execute");
				System.loadLibrary("jni_part");
				
				if(DEBUG)
				Log.i(TAG , "NDK execute");
				
				mOpenCvCameraView.enableView();
				
			} break;
			default:
			{
				super.onManagerConnected(status);
			} break;
			}
		}
	};

	@Override
	public void onResume()
	{
		super.onResume();
		OpenCVLoader.initAsync(OpenCVLoader.OPENCV_VERSION_2_4_6, this, mLoaderCallback);
	}
}
