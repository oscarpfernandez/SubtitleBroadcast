/*   
 * Copyright (C) 2011 The Android Open Source Project
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.uab.uas;

import java.io.IOException;

import android.app.Notification;
import android.app.NotificationManager;
import android.app.PendingIntent;
import android.app.Service;
import android.content.Context;
import android.content.Intent;
import android.media.AudioManager;
import android.media.MediaPlayer;
import android.media.MediaPlayer.OnCompletionListener;
import android.media.MediaPlayer.OnErrorListener;
import android.media.MediaPlayer.OnPreparedListener;
import android.net.wifi.WifiManager;
import android.net.wifi.WifiManager.WifiLock;
import android.os.IBinder;
import android.os.PowerManager;
import android.util.Log;
import android.widget.Toast;

/**
 * Service that handles media playback. This is the Service through which we perform all the media
 * handling in our application. Upon initialization, it starts a {@link MediaRetriever} to scan
 * the user's media. Then, it waits for Intents (which come from our main activity,
 * {@link MainActivity}, which signal the service to perform specific operations: Play, Pause,
 * Rewind, Skip, etc.
 */
public class MusicService extends Service 
implements OnCompletionListener, OnPreparedListener,OnErrorListener, MusicFocusable {

    NotificationManager mNotificationManager;

    // our media player
    MediaPlayer mPlayer = null;

    // our AudioFocusHelper object, if it's available (it's available on SDK level >= 8)
    // If not available, this will be null. Always check for null before using!
    AudioFocusHelper mAudioFocusHelper = null;

    // indicates the state our service:
    enum State {
        Retrieving, // the MediaRetriever is retrieving music
        Stopped,    // media player is stopped and not prepared to play
        Preparing,  // media player is preparing...
        Playing,    // playback active (media player ready!). (but the media player may actually be
                    // paused in this state if we don't have audio focus. But we stay in this state
                    // so that we know we have to resume playback once we get focus back)
        Paused      // playback paused (media player ready!)
    };

    State mState = State.Retrieving;

    enum PauseReason {
        UserRequest,  // paused by user request
        FocusLoss,    // paused because of audio focus loss
    };

    // why did we pause? (only relevant if mState == State.Paused)
    PauseReason mPauseReason = PauseReason.UserRequest;

    // do we have audio focus?
    enum AudioFocus {
        NoFocusNoDuck,    // we don't have audio focus, and can't duck
        NoFocusCanDuck,   // we don't have focus, but can play at a low volume ("ducking")
        Focused           // we have full audio focus
    }
    AudioFocus mAudioFocus = AudioFocus.NoFocusNoDuck;

    // title of the song we are currently playing
    String mSongTitle = "";

    // whether the song we are playing is streaming from the network
    boolean mIsStreaming = false;

    // Wifi lock that we hold when streaming files from the internet, in order to prevent the
    // device from shutting off the Wifi radio
    WifiLock mWifiLock;

    // The tag we put on debug messages
    final static String TAG = "RandomMusicPlayer";

    // These are the Intent actions that we are prepared to handle. Notice that the fact these
    // constants exist in our class is a mere convenience: what really defines the actions our
    // service can handle are the <action> tags in the <intent-filters> tag for our service in
    // AndroidManifest.xml.
    public static final String ACTION_PLAY 		= "com.uab.mobilestr2.musicplayer.action.PLAY";
    public static final String ACTION_PAUSE 	= "com.uab.mobilestr2.musicplayer.action.PAUSE";
    public static final String ACTION_STOP 		= "com.uab.mobilestr2.musicplayer.action.STOP";
//    public static final String ACTION_SKIP 		= "com.uab.mobilestr2.musicplayer.action.SKIP";
//    public static final String ACTION_REWIND	= "com.uab.mobilestr2.musicplayer.action.REWIND";
    public static final String ACTION_URL 		= "com.uab.mobilestr2.musicplayer.action.URL";

    // The volume we set the media player to when we lose audio focus, but are allowed to reduce
    // the volume instead of stopping playback.
    public final float DUCK_VOLUME = 0.1f;

    // The ID we use for the notification (the onscreen alert that appears at the notification
    // area at the top of the screen as an icon -- and as text as well if the user expands the
    // notification area).
    final int NOTIFICATION_ID = 1;

    // Our instance of our MusicRetriever, which handles scanning for media and
    // providing titles and URIs as we need.
//    MusicRetriever mRetriever;

    Notification mNotification = null;
    
    boolean lockLoadingAudio = false;

    /**
     * Makes sure the media player exists and has been reset. This will create the media player
     * if needed, or reset the existing media player if one already exists.
     */
    void createMediaPlayerIfNeeded() {
    	Log.i("MusicService","ENTRY createMediaPlayerIfNeeded()");
    	
        if (mPlayer == null) {
            mPlayer = new MediaPlayer();
            Log.i("MusicService","createMediaPlayerIfNeeded: creating new MediaPlayer");
            
            // Make sure the media player will acquire a wake-lock while playing. If we don't do
            // that, the CPU might go to sleep while the song is playing, causing playback to stop.
            //
            // Remember that to use this, we have to declare the android.permission.WAKE_LOCK
            // permission in AndroidManifest.xml.
            mPlayer.setWakeMode(getApplicationContext(), PowerManager.PARTIAL_WAKE_LOCK);

            // we want the media player to notify us when it's ready preparing, and when it's done
            // playing:
            mPlayer.setOnPreparedListener(this);
            mPlayer.setOnCompletionListener(this);
            mPlayer.setOnErrorListener(this);
        }
        else{
        	Log.i("MusicService","createMediaPlayerIfNeeded: reset()");
        	mPlayer.reset();
        }
        Log.i("MusicService","EXIT createMediaPlayerIfNeeded()");
    }

    @Override
    public void onCreate() {
    	Log.i("MusicService","ENTRY onCreate()");

        // Create the Wifi lock (this does not acquire the lock, this just creates it)
        mWifiLock = ((WifiManager) getSystemService(Context.WIFI_SERVICE))
                        .createWifiLock(WifiManager.WIFI_MODE_FULL, "mylock");

        mNotificationManager = (NotificationManager) getSystemService(NOTIFICATION_SERVICE);

        // create the Audio Focus Helper, if the Audio Focus feature is available (SDK 8 or above)
        if (android.os.Build.VERSION.SDK_INT >= 8){
        	mAudioFocusHelper = new AudioFocusHelper(getApplicationContext(), this);
        }
        else{
        	mAudioFocus = AudioFocus.Focused; // no focus feature, so we always "have" audio focus
        }
        
        Log.i("MusicService","EXIT onCreate()");
    }

    /**
     * Called when we receive an Intent. When we receive an intent sent to us via startService(),
     * this is the method that gets called. So here we react appropriately depending on the
     * Intent's action, which specifies what is being requested of us.
     */
    @Override
    public int onStartCommand(Intent intent, int flags, int startId) {
    	Log.i("MusicService","ENTRY onStartCommand(): intentAction = "+intent.getAction());
    	
        String action = intent.getAction();
        if (action.equals(ACTION_PLAY)) {
        	processPlayRequest();
        }
        else if (action.equals(ACTION_PAUSE)) {
        	processPauseRequest();
        }
        else if (action.equals(ACTION_STOP)){
        	processStopRequest();
        }
        else if (action.equals(ACTION_URL)){
        	processAddRequest(intent);
        }

        Log.i("MusicService","EXIT onStartCommand()");
        
        return START_NOT_STICKY; // Means we started the service, but don't want it to
                                 // restart in case it's killed.
    }

    void processPlayRequest() {
    	Log.i("MusicService","ENTRY processPlayRequest()");
    	
//        if (mState == State.Retrieving) {
//        	Log.i("MusicService","processPlayRequest(): State.Retrieving");
//            mStartPlayingAfterRetrieve = true;
//            return;
//        }

        tryToGetAudioFocus();

        if (mState == State.Stopped) {
        	Log.i("MusicService","processPlayRequest(): State.Stopped");
            // If we're stopped, just go ahead to the next song and start playing
            playNextSong(null);
        }
        else if (mState == State.Paused) {
        	Log.i("MusicService","processPlayRequest(): State.Paused");
            // If we're paused, just continue playback and restore the 'foreground service' state.
            mState = State.Playing;
            setUpAsForeground(mSongTitle + " (playing)");
            configAndStartMediaPlayer();
        }
        
    	Log.i("MusicService","EXIT processPlayRequest()");
    }

    void processPauseRequest() {
    	Log.i("MusicService","ENTRY processPauseRequest()");

        if (mState == State.Playing) {
        	Log.i("MusicService","ENTRY processPauseRequest(): State.Playing");
            // Pause media player and cancel the 'foreground service' state.
            mState = State.Paused;
            mPlayer.pause();
            relaxResources(false); // while paused, we always retain the MediaPlayer
            giveUpAudioFocus();
        }
        Log.i("MusicService","EXIT processPauseRequest()");
    }

    void processStopRequest() {
    	Log.i("MusicService","ENTRY processStopRequest()");
        if (mState == State.Playing || mState == State.Paused) {
        	Log.i("MusicService","ENTRY processStopRequest(): if case...");
            mState = State.Stopped;

            // let go of all resources...
            relaxResources(true);
            giveUpAudioFocus();

            // service is no longer necessary. Will be started again if needed.
            stopSelf();
        }
        Log.i("MusicService","EXIT processStopRequest()");
    }

    /**
     * Releases resources used by the service for playback. This includes the "foreground service"
     * status and notification, the wake locks and possibly the MediaPlayer.
     *
     * @param releaseMediaPlayer Indicates whether the Media Player should also be released or not
     */
    void relaxResources(boolean releaseMediaPlayer) {
    	Log.i("MusicService","ENTRY relaxResources("+releaseMediaPlayer+")");
    	
        // stop being a foreground service
        stopForeground(true);

        // stop and release the Media Player, if it's available
        if (releaseMediaPlayer && mPlayer != null) {
        	Log.i("MusicService","ENTRY relaxResources(): release and reset player");
            mPlayer.reset();
            mPlayer.release();
            mPlayer = null;
        }

        // we can also release the Wifi lock, if we're holding it
        if (mWifiLock.isHeld()){
        	Log.i("MusicService","ENTRY relaxResources(): release the wifi lock");
        	mWifiLock.release();
        }
        Log.i("MusicService","EXIT relaxResources()");
    }

    void giveUpAudioFocus() {
    	Log.i("MusicService","ENTRY giveUpAudioFocus()");
    	
        if (mAudioFocus == AudioFocus.Focused 
        		&& mAudioFocusHelper != null
        		&& mAudioFocusHelper.abandonFocus())
        {
        	Log.i("MusicService","giveUpAudioFocus(): if case...");
        	mAudioFocus = AudioFocus.NoFocusNoDuck;
        }
        
        Log.i("MusicService","EXIT giveUpAudioFocus()");
    }

    /**
     * Reconfigures MediaPlayer according to audio focus settings and starts/restarts it. This
     * method starts/restarts the MediaPlayer respecting the current audio focus state. So if
     * we have focus, it will play normally; if we don't have focus, it will either leave the
     * MediaPlayer paused or set it to a low volume, depending on what is allowed by the
     * current focus settings. This method assumes mPlayer != null, so if you are calling it,
     * you have to do so from a context where you are sure this is the case.
     */
    void configAndStartMediaPlayer() {
    	Log.i("MusicService","ENTRY configAndStartMediaPlayer()");
    	
        if (mAudioFocus == AudioFocus.NoFocusNoDuck) {
        	Log.i("MusicService","ENTRY configAndStartMediaPlayer(): AudioFocus.NoFocusNoDuck");
            // If we don't have audio focus and can't duck, we have to pause, even if mState
            // is State.Playing. But we stay in the Playing state so that we know we have to resume
            // playback once we get the focus back.
            if (mPlayer.isPlaying()) {
            	Log.i("MusicService","ENTRY configAndStartMediaPlayer(): pausing media player");
            	mPlayer.pause();
            }
            Log.i("MusicService","EXIT configAndStartMediaPlayer()");
            return;
        }
        else if (mAudioFocus == AudioFocus.NoFocusCanDuck){
        	Log.i("MusicService","ENTRY configAndStartMediaPlayer(): AudioFocus.NoFocusCanDuck");
            mPlayer.setVolume(DUCK_VOLUME, DUCK_VOLUME);  // we'll be relatively quiet
        }
        else{
        	Log.i("MusicService","ENTRY configAndStartMediaPlayer(): set it loud!");
        	mPlayer.setVolume(1.0f, 1.0f); // we can be loud
        }

        if (!mPlayer.isPlaying()) {
        	Log.i("MusicService","ENTRY configAndStartMediaPlayer(): start playing!!!!");
        	mPlayer.start();
        }
        
        Log.i("MusicService","EXIT configAndStartMediaPlayer()");
    }

    void processAddRequest(Intent intent) {
    	Log.i("MusicService","ENTRY processAddRequest()");
    	
    	tryToGetAudioFocus();
        playNextSong(intent.getData().toString());
    	      
    	Log.i("MusicService","EXIT processAddRequest()");
    }

    /**
     * Shortcut to making and displaying a toast. Seemed cleaner than repeating
     * this code everywhere, at least for this sample.
     */
    void say(String message) {
        Toast.makeText(this, message, Toast.LENGTH_SHORT).show();
    }

    void tryToGetAudioFocus() {
    	Log.i("MusicService","ENTRY tryToGetAudioFocus()");
    	
        if (mAudioFocus != AudioFocus.Focused && mAudioFocusHelper != null
                        && mAudioFocusHelper.requestFocus()){
        	Log.i("MusicService","tryToGetAudioFocus(): if case...");
        	mAudioFocus = AudioFocus.Focused;
        }
        
        Log.i("MusicService","EXIT tryToGetAudioFocus()");
    }

    /**
     * Starts playing the next song. If manualUrl is null, the next song will be randomly selected
     * from our Media Retriever (that is, it will be a random song in the user's device). If
     * manualUrl is non-null, then it specifies the URL or path to the song that will be played
     * next.
     */
    void playNextSong(String manualUrl) {
    	Log.i("MusicService","ENTRY playNextSong()");
    	
    	if(lockLoadingAudio){
    		return;
    	}
    		
        mState = State.Stopped;
        relaxResources(false); // release everything except MediaPlayer

        try {
            if (manualUrl != null) {
            	Log.i("MusicService","playNextSong(): manualURL = " + manualUrl);
                // set the source of the media player to a manual URL or path
                createMediaPlayerIfNeeded();
                mPlayer.setAudioStreamType(AudioManager.STREAM_MUSIC);
                mPlayer.setDataSource(manualUrl);
                mSongTitle = manualUrl;
                mIsStreaming = manualUrl.startsWith("http:") || manualUrl.startsWith("https:");
            }

            mState = State.Preparing;
            setUpAsForeground(mSongTitle + " (loading)");

            // starts preparing the media player in the background. When it's done, it will call
            // our OnPreparedListener (that is, the onPrepared() method on this class, since we set
            // the listener to 'this').
            //
            // Until the media player is prepared, we *cannot* call start() on it!
            mPlayer.prepareAsync();
            /*
             * Set this flag to ensure if the operator starts calling fast a lot of
             * audio file, that the mPlayer does not get into a inconsistent state
             * and setDataSource raising an exception that crashes the whole app!!!
             */
            lockLoadingAudio = true;

            // If we are streaming from the internet, we want to hold a Wifi lock, which prevents
            // the Wifi radio from going to sleep while the song is playing. If, on the other hand,
            // we are *not* streaming, we want to release the lock if we were holding it before.
            if (mIsStreaming){
            	Log.i("MusicService","playNextSong(): acquire Wifi lock");
            	mWifiLock.acquire();
            }
            else if (mWifiLock.isHeld()){
            	Log.i("MusicService","playNextSong(): release Wifi lock");
            	mWifiLock.release();
            }
        }
        catch (IOException ex) {
            Log.e("MusicService", "IOException playing next song: " + ex.getMessage());
            ex.printStackTrace();
        }
        
        Log.i("MusicService","EXIT playNextSong()");
    }

    /** Called when media player is done playing current song. */
    @Override
    public void onCompletion(MediaPlayer player) {
        Log.i("MusicService","ENTRY onCompletion");

        mState = State.Retrieving;
        
        Log.i("MusicService","EXIT onCompletion");
    }

    /** Called when media player is done preparing. */
    @Override
    public void onPrepared(MediaPlayer player) {
    	Log.i("MusicService","ENTRY onPrepared");
    	
    	//the data is now loaded, so the operator can set another source http sound file...
    	lockLoadingAudio = false;
        // The media player is done preparing. That means we can start playing!
        mState = State.Playing;
        updateNotification(mSongTitle + " (playing)");
        configAndStartMediaPlayer();
        
        Log.i("MusicService","EXIT onPrepared");
    }

    /** Updates the notification. */
    void updateNotification(String text) {
        PendingIntent pi = PendingIntent.getActivity(getApplicationContext(), 0,
                new Intent(getApplicationContext(), MainActivity.class),
                PendingIntent.FLAG_UPDATE_CURRENT);
        mNotification.setLatestEventInfo(getApplicationContext(), "RandomMusicPlayer", text, pi);
        mNotificationManager.notify(NOTIFICATION_ID, mNotification);
    }

    /**
     * Configures service as a foreground service. A foreground service is a service that's doing
     * something the user is actively aware of (such as playing music), and must appear to the
     * user as a notification. That's why we create the notification here.
     */
    void setUpAsForeground(String text) {
        PendingIntent pi = PendingIntent.getActivity(getApplicationContext(), 0,
                new Intent(getApplicationContext(), MainActivity.class),
                PendingIntent.FLAG_UPDATE_CURRENT);
        mNotification = new Notification();
        mNotification.tickerText = text;
        mNotification.icon = R.drawable.ic_stat_playing;
        mNotification.flags |= Notification.FLAG_ONGOING_EVENT;
        mNotification.setLatestEventInfo(getApplicationContext(), "MobileSTR2AudioPlayer",
                text, pi);
        startForeground(NOTIFICATION_ID, mNotification);
    }

    /**
     * Called when there's an error playing media. When this happens, the media player goes to
     * the Error state. We warn the user about the error and reset the media player.
     */
    @Override
    public boolean onError(MediaPlayer mp, int what, int extra) {
        Toast.makeText(getApplicationContext(), "Media player error! Resetting.",
            Toast.LENGTH_SHORT).show();
        Log.e(TAG, "Error: what=" + String.valueOf(what) + ", extra=" + String.valueOf(extra));

        mState = State.Stopped;
        relaxResources(true);
        giveUpAudioFocus();
        return true; // true indicates we handled the error
    }

    @Override
    public void onGainedAudioFocus() {
        Toast.makeText(getApplicationContext(), "gained audio focus.", Toast.LENGTH_SHORT).show();
        mAudioFocus = AudioFocus.Focused;

        // restart media player with new focus settings
        if (mState == State.Playing)
            configAndStartMediaPlayer();
    }

    @Override
    public void onLostAudioFocus(boolean canDuck) {
        Toast.makeText(getApplicationContext(), "lost audio focus." + (canDuck ? "can duck" :
            "no duck"), Toast.LENGTH_SHORT).show();
        mAudioFocus = canDuck ? AudioFocus.NoFocusCanDuck : AudioFocus.NoFocusNoDuck;

        // start/restart/pause media player with new focus settings
        if (mPlayer != null && mPlayer.isPlaying())
            configAndStartMediaPlayer();
    }

    @Override
    public void onDestroy() {
        // Service is being killed, so make sure we release our resources
        mState = State.Stopped;
        relaxResources(true);
        giveUpAudioFocus();
    }

    @Override
    public IBinder onBind(Intent arg0) {
        return null;
    }
}
