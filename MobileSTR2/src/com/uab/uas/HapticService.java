package com.uab.uas;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;

import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.content.res.Configuration;
import android.os.Handler;
import android.os.IBinder;
import android.os.Vibrator;
import android.util.Log;

public class HapticService extends Service{

	private DatagramSocket udpSocket;
	private Thread t;
	private String udpMessage;

	private Vibrator v;

	private final Handler mHandler = new Handler();

	public static final int HAPTIC_PORT = 27504;
	public static final int BUFFER_SIZE = 100;
	public static boolean allWorkDone = false;

	public static final String ACTION_STOP = "com.uab.hapticservice.action.STOP";
	public static final String ACTION_START = "com.uab.hapticservice.action.START";

	@Override
	public void onCreate() {
		startReceivingUDPMessages();
		super.onCreate();
	}

	final Runnable mUpdateResults = new Runnable() {
		public void run() {
			vibratePhone();
		}
	};

	public synchronized void connect(){
		if(v==null){
			v = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
		}

		if(udpSocket!=null && (udpSocket.isBound() || udpSocket.isConnected())){
			//already connected... nothing to do here...
			return;
		}

		try {
			Log.d("MobileSTR2.UDPConnHandler", "Socket created successfuly to port="+HAPTIC_PORT);
			allWorkDone = false;
			udpSocket = new DatagramSocket(HAPTIC_PORT);

		} catch (SocketException e) {
			Log.d("MobileSTR2.UDPConnHandler", "Socket creation failed to port="+HAPTIC_PORT);
			Log.d("MobileSTR2.UDPConnHandler", e.getMessage());
		}
	}

	public synchronized void startReceivingUDPMessages() {
//		if(t!=null && t.isAlive()){
//			//do nothing
//			return;
//		}
		
		// thread for work we shouldn't do in the UI thread
		t = new Thread("Haptic Receiver Thread") {
			public void run() {
				connect();

				for (;;) {
					if(allWorkDone){
						v = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
						v.cancel();

						//finishes the thread;
						return;
					}

					DatagramPacket dgram; 
					byte[] buffer;

					try {
						buffer = new byte[BUFFER_SIZE];
						dgram = new DatagramPacket(buffer, buffer.length);
						if(udpSocket!=null && udpSocket.isBound()){
							udpSocket.receive(dgram);
							udpMessage = (new String(dgram.getData())).trim();
						}

						Log.i("MobileSTR2:startReceivingUDPMessages", "Received message:" + udpMessage);
					} catch (IOException e) {
						//disp_str = e.toString();
					}
					mHandler.post(mUpdateResults);
				}
			}
		};
		t.start();
		allWorkDone=false;
	}

	@Override
	protected void finalize() throws Throwable {
		// TODO Auto-generated method stub
		super.finalize();
	}

	@Override
	public void onConfigurationChanged(Configuration newConfig) {
		// TODO Auto-generated method stub
		super.onConfigurationChanged(newConfig);
	}

	@Override
	public void onRebind(Intent intent) {
		// TODO Auto-generated method stub
		super.onRebind(intent);
	}

	@Override
	public boolean onUnbind(Intent intent) {
		// TODO Auto-generated method stub
		return super.onUnbind(intent);
	}

	public synchronized void stopReceivingUDPMessages(){
		//stop the thread from listening...
		if(udpSocket!=null){
			udpSocket.disconnect();
			udpSocket.close();
		}
		allWorkDone = true;
		
		v = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);
		v.cancel();
	}

	private void vibratePhone(){
		if(udpMessage==null || udpMessage.length()==0 || !udpMessage.contains("shake")){
			return;
		}

		v = (Vibrator) getSystemService(Context.VIBRATOR_SERVICE);

		// This example will cause the phone to vibrate "SOS" in Morse Code
		// In Morse Code, "s" = "dot-dot-dot", "o" = "dash-dash-dash"
		// There are pauses to separate dots/dashes, letters, and words
		// The following numbers represent millisecond lengths
		int dot = 200;      // Length of a Morse Code "dot" in milliseconds
		int dash = 500;     // Length of a Morse Code "dash" in milliseconds
		int short_gap = 200;    // Length of Gap Between dots/dashes
		int medium_gap = 500;   // Length of Gap Between Letters
		int long_gap = 1000;    // Length of Gap Between Words
		//		long[] pattern = {
		//				0,  // Start immediately
		//				dot, short_gap, dot, short_gap, dot,    // s
		//				medium_gap,
		//				dash, short_gap, dash, short_gap, dash, // o
		//				medium_gap,
		//				dot, short_gap, dot, short_gap, dot,    // s
		//				long_gap
		//		};
		long[] pattern = {
				0,  // Start immediately
				dash,short_gap,dash
		};


		// Only perform this pattern one time (-1 means "do not repeat")
		//		v.vibrate(pattern, -1);
		v.vibrate(500);

		udpMessage = null;

	}

	@Override
	public int onStartCommand(Intent intent, int flags, int startId) {
		String action = intent.getAction();
		if (action.equals(ACTION_START)) {
			processStartRequest();
		}
		else if (action.equals(ACTION_STOP)) {
			processStopRequest();
		}

		return START_STICKY;
	}

	void processStartRequest(){
		startReceivingUDPMessages();	
	}

	void processStopRequest(){
		stopReceivingUDPMessages();
		stopSelf();
	}

	@Override
	public void onDestroy() {
		stopReceivingUDPMessages();
		super.onDestroy();
	}

	@Override
	public void onStart(Intent intent, int startId) {
		startReceivingUDPMessages();
		super.onStart(intent, startId);
	}

	@Override
	public IBinder onBind(Intent arg0) {
		return null;
	}

}
