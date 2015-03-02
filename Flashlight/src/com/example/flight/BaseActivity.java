package com.example.flight;
import android.hardware.Camera;
import android.hardware.Camera.Parameters;
import android.os.Bundle;
import android.app.Activity;
import android.widget.ImageView;
import android.view.Menu;
public class BaseActivity extends Activity 
{
  
  protected  ImageView mImageViewFlashlight;
  protected ImageView mImageViewFlashlightController;
  protected Camera mCamera;
  protected Parameters mParameters;
  
    @Override
   protected void onCreate(Bundle savedInstanceState) 
    {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        mImageViewFlashlight=(ImageView)findViewById(R.id.imageview_flashlight);
        mImageViewFlashlightController=(ImageView)findViewById(R.id.imageview_flashlight_controller);
    }

}
