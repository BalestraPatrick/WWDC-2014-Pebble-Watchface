WWDC 2014 Pebble Watchface
==========================

A countdown/watchface for WWDC 2014 on your Pebble with a secret feature :)
This watchface uses Javascript to retrieve the user's timezone from the phone so the countdown shouldn't be wrong.

Download from the Pebble App Store: http://pblweb.com/appstore/534ff65f3d49c5991f000204/
For iOS users the watchfaceis not yet available in the Pebble App Store, please manually install with the instructions below.

<img src="screenshot.png">

<a href="http://twitter.com/BalestraPatrick">@BalestraPatrick</a>

# Installation

If you don't want to install directly from the Pebble App Store, you can compile the .pbw yourself. For this purpose, you need to install the Pebble SDK.
Follow this link to install the SDK on your Mac: https://developer.getpebble.com/2/getting-started/

After that, you can install the .pbw on your watch by following these instructions:

1) Download the project's folder

2) Open your terminal and change the directory (cd command) into the folder your just downloaded

3) Type: `pebble build`. If it successfully builds, you should see something like build finished successfully

4) Open the Settings.app on your iPhone, and under the Pebble official app, turn on the developer mode

5) Open the Pebble app and turn on the Developer Connection. Take note of your phone's IP Address

6) Go back to your terminal and type: `export PEBBLE_PHONE=` and add your phone's IP Address

7) Now: `pebble install`. If everything goes well, after a few seconds the watchface should be running on your Pebble.

Let me know if you encounter any problem. 
