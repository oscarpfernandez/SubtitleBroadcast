/******************************************************************************
 * The purpose of this class is to handle the UDP socket that receive the
 * contents from the BroadcastStr2 Server. It processes the datagrams, and the 
 * and update the contents of the TextView GUI elements passed by the 
 * controlling activity.
 * 
 * Developed by Oscar Lopes (C)2011 [Universidad Autònoma de Barcelona] 
 ******************************************************************************/
package com.uab.mobilestr2;

import java.io.IOException;
import java.net.DatagramPacket;
import java.net.DatagramSocket;
import java.net.SocketException;

import org.xmlpull.v1.XmlPullParserException;

import android.app.Activity;
import android.content.Intent;
import android.net.Uri;
import android.os.Handler;
import android.util.Log;
import android.widget.TextView;

public class UDPConnectionHandler {
		
	private DatagramSocket udpSocket;
	private String udpMessage;
	private static final int BUFFER_SIZE = 65000;
	private boolean allWorkDone = false;
	
	private Thread t;
	// handler for callbacks to the UI thread
    private final Handler mHandler = new Handler();
	
	private String language;
	private int port;
	private TextView subtitleTV;
	private TextView adescTV;
	private TextView actorTV;
	private Activity contextActivity;
	
	private boolean isSubsSoundOn;
	private boolean isADSoundOn;
	
	private TtsGenerator ttsGen;
		
	public UDPConnectionHandler(Activity context,
			String languageName, 
			int languagePort,
			TextView subtitles,
			TextView adescription,
			TextView actor,
			boolean subSoundOn,
			boolean adSoundOn)
	{
		port = languagePort;
		language = languageName;
		subtitleTV = subtitles;
		adescTV = adescription;
		actorTV = actor;
		contextActivity = context;
		isSubsSoundOn = subSoundOn;
		isADSoundOn = adSoundOn;
		
		ttsGen = new TtsGenerator(context);
		ttsGen.setLanguage(language);
	}
	
	
	public synchronized void connect(){
		if(udpSocket!=null && (udpSocket.isBound() || udpSocket.isConnected())){
			//already connected... nothing to do here...
			return;
		}
		
		try {
			Log.d("MobileSTR2.UDPConnHandler", "Socket created successfuly to port="+port);
			allWorkDone = false;
			udpSocket = new DatagramSocket(port);
		} catch (SocketException e) {
			Log.d("MobileSTR2.UDPConnHandler", "Socket creation failed to port="+port);
			Log.d("MobileSTR2.UDPConnHandler", e.getMessage());
		}
	}
	
    // create runnable for posting
    final Runnable mUpdateResults = new Runnable() {
	    public void run() {
	    	updateResultsInUi();
	    }
	};
	
	public synchronized void startReceivingUDPMessages() {
		
		// thread for work we shouldn't do in the UI thread
		t = new Thread() {
			public void run() {
				connect();
				
				for (;;) {
					if(allWorkDone){
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
						if(udpMessage==null){
							return;
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
	
	public synchronized void stopReceivingUDPMessages(){
		//stop the thread from listening...
		if(udpSocket!=null){
			udpSocket.disconnect();
			udpSocket.close();
		}
		allWorkDone = true;
		
		//stopMusicService();
		
		//Stop TTS
		ttsGen.shutdown();

	}
	
	private void updateResultsInUi(){
    	if(allWorkDone){
    		/*
    		 * stop the update hanlder...
    		 * without this, the last audio sound will play on screen roation...
    		 */
    		
    		return;
    	}

		SubtitleLine subLine = new SubtitleLine();
		XMLContentParser xmlContentParser = new XMLContentParser();
		try {
			subLine = xmlContentParser.getDataFromXML(udpMessage);
		} catch (XmlPullParserException e) {
			Log.i("MobileSTR2:updateResultsInUi", "Error parsing the datagram message: " + e.getMessage());
		} catch (IOException e) {
			Log.i("MobileSTR2:updateResultsInUi", "Error parsing the datagram message: " + e.getMessage());
		}

		if(actorTV!=null){
			actorTV.setText(subLine.getActorName());
			actorTV.postInvalidate();
		}

		if(subtitleTV!=null){
			subtitleTV.setText(subLine.getSubtitle());
			subtitleTV.postInvalidate();
		}
		if(adescTV!=null){
			adescTV.setText(subLine.getAudioDesc());
			adescTV.postInvalidate();
		}
		
		if(subLine.getSubtitle().length()!=0 && isSubsSoundOn){
			Log.i("MusicService","url="+subLine.getSubtitleSound());
			ttsGen.generateTTSpeech(language, subLine.getSubtitle());
			//setURLAudioAndStartMusicService(subLine.getSubtitleSound());
		}
		
		if( subLine.getAudioDesc().length()!=0 && isADSoundOn){
			Log.i("MusicService","url="+subLine.getSubtitleSound());
			ttsGen.generateTTSpeech(language, subLine.getAudioDesc());
			//setURLAudioAndStartMusicService(subLine.getAudioDescSound());
		}
	}
	
    public void setURLAudioAndStartMusicService(String audioURL){
        Intent i = new Intent(MusicService.ACTION_URL);
        Uri uri = Uri.parse(audioURL);
        i.setData(uri);
        Log.i("MusicService","parsed uri = "+ uri.toString());
        contextActivity.startService(i);
        
        Intent startI = new Intent(MusicService.ACTION_PLAY);
        contextActivity.startService(startI);
    }
    
    public void stopMusicService(){
        Intent i = new Intent(MusicService.ACTION_STOP);
        contextActivity.startService(i);
    }
}
