/******************************************************************************
 * The purpose of this class is store the values obtained from the parsing
 * of the XML message extrated from the datagram received from the STR2
 * Broadcast Server.
 * 
 * Developed by Oscar Lopes (C)2011 [Universidad Autònoma de Barcelona] 
 ******************************************************************************/
package com.uab.mobilestr2;

public class SubtitleLine {

	private String actorName = "";
	private String subtitle = ""; 
	private String audioDesc = "";
	private String subtitleSound = "";
	private String audioDescSound = "";
	
	public String getActorName() {
		return actorName;
	}
	public String getSubtitle() {
		return subtitle;
	}
	public String getAudioDesc() {
		return audioDesc;
	}
	public String getSubtitleSound() {
		return subtitleSound;
	}
	public String getAudioDescSound() {
		return audioDescSound;
	}
	public void setActorName(String actorName) {
		this.actorName = actorName;
	}
	public void setSubtitle(String subtitle) {
		this.subtitle = subtitle;
	}
	public void setAudioDesc(String audioDesc) {
		this.audioDesc = audioDesc;
	}
	public void setSubtitleSound(String subtitleSound) {
		this.subtitleSound = subtitleSound;
	}
	public void setAudioDescSound(String audioDescSound) {
		this.audioDescSound = audioDescSound;
	}
}
