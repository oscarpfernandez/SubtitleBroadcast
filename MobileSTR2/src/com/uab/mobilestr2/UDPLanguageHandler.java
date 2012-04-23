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

import android.app.ProgressDialog;
import android.os.Handler;
import android.util.Log;
import android.widget.ArrayAdapter;

public class UDPLanguageHandler {
		
	private DatagramSocket udpSocket;
	private String udpMessage;
	private static final int BUFFER_SIZE = 65000;
	private boolean allWorkDone = false;
	
	private ArrayAdapter<String> spinnerAdapter;
	String[] langs = new String[0];
	
	private Thread t;
	// handler for callbacks to the UI thread
    private final Handler mHandler = new Handler();
	
	private int port;
	private ProgressDialog progress;
		
	public UDPLanguageHandler(int portNumber, ArrayAdapter<String> adapter, ProgressDialog progressDiag)
	{
		port = portNumber;
		spinnerAdapter = adapter;
		progress = progressDiag;
	}
	
	public void connect(){
		if(udpSocket!=null && (udpSocket.isBound() || udpSocket.isConnected())){
			//already connected... nothing to do here...
			return;
		}
		
		try {
			Log.d("MobileSTR2.UDPLanguageHandler", "Socket created successfuly to port="+port);
			allWorkDone = false;
			udpSocket = new DatagramSocket(port);
		} catch (SocketException e) {
			Log.d("MobileSTR2.UDPLanguageHandler", "Socket creation failed to port="+port);
			Log.d("MobileSTR2.UDPLanguageHandler", e.getMessage());
		}
	}
	
    // create runnable for posting
    final Runnable mUpdateResults = new Runnable() {
	    public void run() {
	    	updateResultsInUi();
	    }
	};
	
	public void startReceivingUDPMessages() {
		if(t!=null && t.isAlive()){
			return;
		}
		allWorkDone=false;
		
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
	}
	
	public void stopReceivingUDPMessages(){
		//stop the thread from listening...
		if(udpSocket!=null){
			udpSocket.disconnect();
			udpSocket.close();
		}

		allWorkDone = true;
	}
	
	private void updateResultsInUi(){
		if(udpMessage!=null && langs.length == 0){
			//load the languages only once... while langs.length==0
			langs = udpMessage.split(":");
			
			if(spinnerAdapter!=null && langs!=null && langs.length>0){
				//stop receiving the languages config ping...
				stopReceivingUDPMessages();
				
				for(int i=0; i<langs.length;i++){
					//fill the spinner's adapter.
					spinnerAdapter.add(langs[i]);
					//disable the waiting progress dialog...
					progress.dismiss();
				}
			}
		}
		
		
	}
}

