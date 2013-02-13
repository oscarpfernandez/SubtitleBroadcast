package com.uab.mobilestr2;

import android.app.Activity;
import android.app.ProgressDialog;
import android.content.Context;
import android.content.Intent;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiManager.WifiLock;
import android.os.Bundle;
import android.os.PowerManager;
import android.view.Menu;
import android.view.MenuInflater;
import android.view.MenuItem;
import android.view.View;
import android.view.View.OnClickListener;
import android.widget.AdapterView;
import android.widget.ArrayAdapter;
import android.widget.Button;
import android.widget.Spinner;
import android.widget.ToggleButton;
import android.widget.AdapterView.OnItemSelectedListener;

public class MainActivity extends Activity {

	private UDPLanguageHandler udpLangHandler;
	private ArrayAdapter<String> adapter;
	private Spinner spinner;
	private String language;
	
	private ToggleButton subsToggleButton;
	private ToggleButton adToggleButton;
	
	private boolean isSubsAudioOn = false;
	private boolean isADAudioOn = false;
	
	private Activity instance;

	private static final int LANGS_PORT = 27505;
	
	public static final String ENGLISH = "english";
	public static final String CATALAN = "catalan";
	public static final String ITALIAN = "italian";
	public static final String SPANISH = "spanish";
	
	private static int ENGLISH_PORT = 27500;
	private static int SPANISH_PORT = 27501;
	private static int CATALAN_PORT = 27502;
	private static int ITALIAN_PORT = 27503;
	
	// Wifi lock that we hold when streaming files from the internet, in order to prevent the
    // device from shutting off the Wifi radio
    WifiLock mWifiLock;
	
    PowerManager pm;
    PowerManager.WakeLock wl; 

	@Override
	public void onCreate(Bundle savedInstanceState) {
		super.onCreate(savedInstanceState);
		setContentView(R.layout.main);
		
		instance = this;

		ProgressDialog dialog = ProgressDialog.show(MainActivity.this, "", getString(R.string.toast_message), true);
		dialog.setProgressStyle(ProgressDialog.STYLE_SPINNER);
		dialog.show();

		adapter = new ArrayAdapter<String>(this,android.R.layout.simple_spinner_item);

		spinner = (Spinner) findViewById(R.id.Spinner01);
		spinner.setAdapter(adapter);
		spinner.setOnItemSelectedListener(new OnItemSelectedListener() {
			public void onItemSelected(AdapterView<?> parent,
					View view, int pos, long id) {
				language = parent.getItemAtPosition(pos).toString();
			}

			public void onNothingSelected(AdapterView parent) {
				// Do nothing.
			}
		});
		
		
		//Toggle Buttons
		subsToggleButton = (ToggleButton)findViewById(R.id.subs_togglebutton);
		subsToggleButton.setHapticFeedbackEnabled(true);
		subsToggleButton.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View arg0) {
				if(adToggleButton.isChecked()){
					adToggleButton.setChecked(false);
				}
				
				if(subsToggleButton.isChecked()){
					subsToggleButton.setChecked(true);
				}
				else{
					subsToggleButton.setChecked(false);
				}
			}
		});
		
		adToggleButton = (ToggleButton) findViewById(R.id.ad_togglebutton);
		adToggleButton.setHapticFeedbackEnabled(true);
		adToggleButton.setOnClickListener(new OnClickListener() {
			
			@Override
			public void onClick(View v) {
				if(subsToggleButton.isChecked()){
					subsToggleButton.setChecked(false);
				}
				
				if(adToggleButton.isChecked()){
					adToggleButton.setChecked(true);
				}
				else{
					adToggleButton.setChecked(false);
				}
				
			}
		});
		
		//Build Intent
		final Intent intentMobileStr2 = new Intent().setClass(this, MobileSTR2Activity.class);
		

		Button buttonOk = (Button)findViewById(R.id.button);
		buttonOk.setHapticFeedbackEnabled(true);
		buttonOk.setOnClickListener(new OnClickListener() {

			@Override
			public void onClick(View v) {
				if(language!=null){
					int port=0;
					if(language.equalsIgnoreCase(ITALIAN)){
						port = ITALIAN_PORT;
					}
					else if(language.equalsIgnoreCase(CATALAN)){
						port = CATALAN_PORT;
					}
					else if(language.equalsIgnoreCase(SPANISH)){
						port = SPANISH_PORT;
					}
					else if(language.equalsIgnoreCase(ENGLISH)){
						port = ENGLISH_PORT;
					}
					
					/*AudioManager am = (AudioManager)getSystemService(Context.AUDIO_SERVICE);
					boolean isHeadphonePlugged = am.isWiredHeadsetOn();
					if(!isHeadphonePlugged){
						Toast t = Toast.makeText(instance, "Connect the headphone", Toast.LENGTH_LONG);
				        t.show();
					}*/
					
					if(port!=0 ){
						intentMobileStr2.putExtra(MobileSTR2Activity.LANGUAGE_UDP_PORT, port);
						intentMobileStr2.putExtra(MobileSTR2Activity.LANGUAGE_NAME, language);
						intentMobileStr2.putExtra(MobileSTR2Activity.SUBS_SOUND, subsToggleButton.isChecked());
						intentMobileStr2.putExtra(MobileSTR2Activity.AD_SOUND, adToggleButton.isChecked());
						startActivity(intentMobileStr2);
					}
				}
			}
		});
		

		// Create the Wifi lock (this does not acquire the lock, this just creates it)
        mWifiLock = ((WifiManager) getSystemService(Context.WIFI_SERVICE))
                        .createWifiLock(WifiManager.WIFI_MODE_FULL, "mylock");
        
        //Power manager lock so the screen does not turns off...
        pm = (PowerManager) getSystemService(Context.POWER_SERVICE);
        
        //acquire the locks
        wl = pm.newWakeLock(PowerManager.SCREEN_DIM_WAKE_LOCK, "My Tag");
        mWifiLock.acquire();//wifi always on
        wl.acquire(); //screen always on
		
		udpLangHandler = new UDPLanguageHandler(LANGS_PORT, adapter, dialog);
		udpLangHandler.startReceivingUDPMessages();
		
	}

	@Override
	protected void onDestroy() {
		// TODO Auto-generated method stub
		super.onDestroy();
	}

	@Override
	protected void onPause() {
		if (mWifiLock.isHeld()){
			mWifiLock.release();
		}
		if(wl.isHeld()){
			wl.release();
		}
		super.onPause();
	}

	@Override
	protected void onRestart() {
		// TODO Auto-generated method stub
		super.onRestart();
		mWifiLock.acquire();
		wl.acquire();
	}

	@Override
	protected void onResume() {
		// TODO Auto-generated method stub
		super.onResume();
	}

	@Override
	protected void onStart() {
		// TODO Auto-generated method stub
		super.onStart();
	}

	@Override
	protected void onStop() {
		if (mWifiLock.isHeld()){
			mWifiLock.release();
		}
		if(wl.isHeld()){
			wl.release();
		}
		super.onStop();
	}
	
	@Override
	public boolean onCreateOptionsMenu(Menu menu) {
		MenuInflater inflater = getMenuInflater();
		inflater.inflate(R.menu.menu, menu);
		return true;
	}

	@Override
	public boolean onOptionsItemSelected(MenuItem item) {
		switch (item.getItemId()) {
		case R.id.menu_exit_item:
			finish();
			break;

		}
		return true;
	}
}
