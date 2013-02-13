/******************************************************************************
 * The purpose of this class is to represent an Activity that displays 
 * subtitles, actor, and audio descriptions.
 * 
 * Developed by Oscar Lopes (C)2011 [Universidad Autònoma de Barcelona] 
 ******************************************************************************/
package com.uab.mobilestr2;

import android.app.Activity;
import android.content.Context;
import android.content.Intent;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiManager.WifiLock;
import android.os.Bundle;
import android.os.PowerManager;
import android.widget.TextView;
import android.widget.Toast;

public class MobileSTR2Activity extends Activity {
	
	public static final String LANGUAGE_UDP_PORT = "language.upd.port";
	public static final String LANGUAGE_NAME = "language.name";
	public static final String SUBS_SOUND = "subs.sound";
	public static final String AD_SOUND = "ad.sound";
	
	UDPConnectionHandler udpConnHandler;
	TextView subtitleTV;
	TextView adescTV;
	TextView actorTV;
	
    // Wifi lock that we hold when streaming files from the internet, in order to prevent the
    // device from shutting off the Wifi radio
    WifiLock mWifiLock;
	
    PowerManager pm;
    PowerManager.WakeLock wl; 
    
    /** Called when the activity is first created. */
    @Override
    public void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.ad_subtitle);
        
        Bundle bundle = this.getIntent().getExtras();
        int languagePort = bundle.getInt(MobileSTR2Activity.LANGUAGE_UDP_PORT);
        String language = bundle.getString(MobileSTR2Activity.LANGUAGE_NAME);
        boolean isSubsSoundOn = bundle.getBoolean(MobileSTR2Activity.SUBS_SOUND);
        boolean isADSoundOn = bundle.getBoolean(MobileSTR2Activity.AD_SOUND);
        
        
        
        // Create the Wifi lock (this does not acquire the lock, this just creates it)
        mWifiLock = ((WifiManager) getSystemService(Context.WIFI_SERVICE))
                        .createWifiLock(WifiManager.WIFI_MODE_FULL, "mylock");
        
        pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        wl = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "My Tag");

        
        subtitleTV = (TextView)findViewById(R.id.value_subtitle);

		//The following tags were dehactivated to use them again
        //uncomment them, and adjust ad_subtitle.xml file ;)
        //adescTV = (TextView)findViewById(R.id.value_ad);
        //actorTV = (TextView)findViewById(R.id.value_actorname);
                
        udpConnHandler = new UDPConnectionHandler(this,language, languagePort, subtitleTV, adescTV, actorTV, 
        		isSubsSoundOn, isADSoundOn);
        
        udpConnHandler.startReceivingUDPMessages();
        
        mWifiLock.acquire();//wifi always on
        wl.acquire(); //screen always on
        
        startService(new Intent(HapticService.ACTION_START));
                
        //Toast notification to warn the user about the language...
        String msg = getString(R.string.connection_msg) + " Language " +language.toUpperCase()+ " on port " + languagePort;
        Toast t = Toast.makeText(this,msg, Toast.LENGTH_LONG);
        t.show();
    }
        
    public void onRestart(){
    	super.onRestart();
    	startService(new Intent(HapticService.ACTION_START));
    	if(udpConnHandler!=null){
    		udpConnHandler.startReceivingUDPMessages();
    		mWifiLock.acquire();
    		wl.acquire();
    	}
    }
    
    public void onResume() {
    	
    	startService(new Intent(HapticService.ACTION_START));
    	if(udpConnHandler!=null){
    		udpConnHandler.startReceivingUDPMessages();
    		mWifiLock.acquire();
    		wl.acquire();
    	}
    	super.onResume();
    }
    
    public void onPause() {
    	startService(new Intent(HapticService.ACTION_STOP));
    	if(udpConnHandler!=null){
    		udpConnHandler.stopReceivingUDPMessages();
    		if (mWifiLock.isHeld()){
    			mWifiLock.release();
    		}
    		if(wl.isHeld()){
    			wl.release();
    		}
    	}
    	super.onPause();
    }

    public void onStop() {
    	stopService(new Intent(HapticService.ACTION_STOP));
    	if(udpConnHandler!=null){
    		udpConnHandler.stopReceivingUDPMessages();
    		if (mWifiLock.isHeld()){
    			mWifiLock.release();
    		}
    	}
    	super.onStop();
    }

    public void onDestroy() {
    	//finish();
    	super.onDestroy();
    }
    
    
} 