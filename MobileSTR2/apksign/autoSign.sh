#!/bin/bash

#******************************************************************************
# APK Auto Signer
# This script allows the automatic signing of a APK package, before releasing
# it to the Android Market (this step is mandatory), by using a previously
# generated private key.
#
# Create by Oscar Lopes @ Universidad Autonoma de Barcelona (November 2011)
#******************************************************************************

echo "*****************************************"
echo "*            APK Autosigner             *"
echo "*****************************************"

if [ $1 == ""] 
then
   echo "You must specify a .apk filename"
   echo "Sintax: apksign.sh <APK filename>"
   exit
fi

NAME=`echo "$1" | cut -d'.' -f1`
EXT=`echo  "$1" | cut -d'.' -f2`

#Check we're dealing with an APK file...
if [ "$EXT" != "apk" ]; 
then
  echo "The file must be a .apk !!!"
  exit
fi

FILENAME=$1

#Update the version if you wish...
VERSION="v1.0"

#######################################
#Private key to sign the APK
#######################################
#Private key was generated with the following command (this was already done... check "key" folder)
#keytool -genkey -v -keystore caiac-release-key.keystore -alias caiac-uas-release-key -keyalg RSA -keysize 2048 -validity 10000

KEY="./key/caiac-release-key.keystore"

# This alias was assigned during the key generation process.
# Update this variable if the key is modified.
ALIAS="caiac-uas-release-key"
echo "* Signing with Private Key = "$KEY" on "$FILENAME
echo "* Using \'Alias\' = $ALIAS"

echo "#######################################"
echo "# Step 1 - Signing the Jar"
echo "#######################################"
jarsigner -verbose -keystore $KEY $FILENAME $ALIAS

echo "#######################################"
echo "# Step 2 - Verifying the Jar"
echo "#######################################"
jarsigner -verify -verbose -certs $FILENAME

echo "#######################################"
echo "# Step 3 - Align the final APK package"
echo "#######################################"
zipalign -v 4 $FILENAME $NAME$VERSION".signed.apk"






