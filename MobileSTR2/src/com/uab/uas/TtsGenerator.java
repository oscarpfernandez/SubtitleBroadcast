package com.uab.uas;

import java.util.Locale;

import android.annotation.SuppressLint;
import android.app.Activity;
import android.speech.tts.TextToSpeech;
import android.util.Log;



public class TtsGenerator implements TextToSpeech.OnInitListener{

	private Activity _context;
	private TextToSpeech _tts;
	
	private boolean _isTtsAvailable = false;
	
	
	public TtsGenerator(Activity context){
		_context = context;
		_tts = new TextToSpeech(_context, this);
		
	}
	
	@Override
	public void onInit(int status) {
		if (status == TextToSpeech.SUCCESS) {
			_isTtsAvailable = true;
		}

	}
	
	public boolean setLanguage(String lang){
		int ret=0;
		if(lang.toLowerCase().compareTo(MainActivity.ENGLISH)==0){
			ret = _tts.setLanguage(Locale.UK);
		}
		else if(lang.toLowerCase().compareTo(MainActivity.SPANISH)==0){
			ret = _tts.setLanguage(new Locale("es", "ES"));
		}
		else if(lang.toLowerCase().compareTo(MainActivity.ITALIAN)==0){
			ret = _tts.setLanguage(new Locale("it", "IT"));
		}
		return ret==TextToSpeech.LANG_AVAILABLE;	
	}
	
	public void generateTTSpeech(String lang, String text){

		setLanguage(lang);
		
		
		if(_isTtsAvailable && text!=null && text.length()>0){
			Log.d("TTS", "Speaking text...");
			_tts.speak(text, TextToSpeech.QUEUE_ADD, null);
		}
	}
	
	public void shutdown(){
		if (_tts != null) {
			_tts.stop();
			_tts.shutdown();
		}
	}
	
}
