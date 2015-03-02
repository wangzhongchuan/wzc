package com.example.flight;

import android.graphics.Point;
import android.graphics.SurfaceTexture;
import android.graphics.drawable.TransitionDrawable;
import android.hardware.Camera;
import android.os.Bundle;
import android.transition.Transition;
import android.view.View;
import android.view.ViewGroup.LayoutParams;
import android.widget.ImageView;
import android.widget.Toast;
import android.app.Activity;
import android.content.pm.PackageManager;
public class flashlight extends BaseActivity
{
	 @Override
	   protected void onCreate(Bundle savedInstanceState) 
	    {
	        super.onCreate(savedInstanceState);
	        mImageViewFlashlight.setTag(false);
	        Point point=new Point();
	        getWindowManager().getDefaultDisplay().getSize(point);
	        LayoutParams laParams=(LayoutParams)mImageViewFlashlightController.getLayoutParams();
	        laParams.height=point.y*3/4;
	        laParams.width=point.x*1/3;
	        mImageViewFlashlightController.setLayoutParams(laParams);
        }
public void onClick_Flashlight(View view)
	 {
		if(!getPackageManager().hasSystemFeature(PackageManager.FEATURE_CAMERA_FLASH)) 
		{
			Toast.makeText(this, "当前设备没有闪光灯", Toast.LENGTH_LONG).show();
			return;
			
		}
		if(((Boolean)mImageViewFlashlight.getTag())==false){
			
			openFlashlight();
		}
		else{
			closeFlashlight();
		}
	 }
  //打开闪光灯
protected void openFlashlight()
{
  TransitionDrawable drawable=(TransitionDrawable)mImageViewFlashlight.getDrawable();
  drawable.startTransition(50);
  mImageViewFlashlight.setTag(true);
  
  try {
	  
		mCamera=Camera.open();
		int textureId=0;
		mCamera.setPreviewTexture(new SurfaceTexture(textureId));
		mCamera.startPreview();
		mParameters=mCamera.getParameters();
		mParameters.setFlashMode(mParameters.FLASH_MODE_TORCH);
		mCamera.setParameters(mParameters);
	  } catch(Exception e){
		  // TODO:handle exception
	  }
}

//关闭闪光灯
protected void closeFlashlight()
{
	TransitionDrawable drawable=(TransitionDrawable)mImageViewFlashlight.getDrawable();
	if(((Boolean)mImageViewFlashlight.getTag()));
	{
		
		drawable.reverseTransition(50);
		mImageViewFlashlight.setTag(false);
		if(mCamera!=null)
		{
			mParameters=mCamera.getParameters();
			mParameters.setFlashMode(mParameters.FLASH_MODE_OFF);
			mCamera.setParameters(mParameters);
			mCamera.stopPreview();
			mCamera.release();
			mCamera=null;
			
			
			
		}
	}
}
}