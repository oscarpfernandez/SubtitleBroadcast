/******************************************************************************
 * The purpose of this class is parse the XML message extrated from the 
 * datagram received from the STR2 Broadcast Server.
 * 
 * Developed by Oscar Lopes (C)2011 [Universidad Autònoma de Barcelona] 
 ******************************************************************************/
package com.uab.mobilestr2;

import java.io.IOException;
import java.io.StringReader;

import org.xmlpull.v1.XmlPullParser;
import org.xmlpull.v1.XmlPullParserException;

import android.util.Xml;

public class XMLContentParser {

	private XmlPullParser xmlParser;

	//XML Tags expected in the message from server...
	private static final String SUBTITLE_LINE 	= "subtitleline"; 
	private static final String SUBTITLE 		= "subtitle";
	private static final String ACTOR 			= "actor";
	private static final String AUDIO_DESC 		= "ad";
	private static final String SUB_SOUND 		= "subsound";
	private static final String AD_SOUND 		= "adsound";

	public XMLContentParser(){
		xmlParser = Xml.newPullParser();
	}

	public SubtitleLine getDataFromXML(String xmlLine) throws XmlPullParserException, IOException{
		SubtitleLine subLine = new SubtitleLine();

		if(xmlLine==null){
			return subLine;
		}

		xmlParser.setInput(new StringReader(xmlLine));
		int eventType = xmlParser.getEventType();
		boolean done = false;
		String name = null;

		while (eventType != XmlPullParser.END_DOCUMENT && !done){

			switch (eventType){
			case XmlPullParser.START_DOCUMENT:
				break;
			case XmlPullParser.START_TAG:
				name = xmlParser.getName();
				if (name.equalsIgnoreCase(SUBTITLE_LINE)){
					int numAtrrs = xmlParser.getAttributeCount();
					for(int i=0; i<numAtrrs; i++){
						if(xmlParser.getAttributeName(i).equalsIgnoreCase(SUBTITLE)){
							subLine.setSubtitle(xmlParser.getAttributeValue(i));
						}
						else if(xmlParser.getAttributeName(i).equalsIgnoreCase(ACTOR)){
							subLine.setActorName(xmlParser.getAttributeValue(i));
						}
						else if(xmlParser.getAttributeName(i).equalsIgnoreCase(AUDIO_DESC)){
							subLine.setAudioDesc(xmlParser.getAttributeValue(i));
						}
						else if(xmlParser.getAttributeName(i).equalsIgnoreCase(AD_SOUND)){
							subLine.setAudioDescSound(xmlParser.getAttributeValue(i));
						}
						else if(xmlParser.getAttributeName(i).equalsIgnoreCase(SUB_SOUND)){
							subLine.setSubtitleSound(xmlParser.getAttributeValue(i));
						}
					}
				} 
				break;
			case XmlPullParser.END_TAG:
				//End of subtitle line... no point in reading more
				done = true;
				break;
			}
			eventType = xmlParser.next();
		}

		return subLine;
	}
}
