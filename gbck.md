# Green Bean Connect Kit Setup

## Physical Connections:
[gbck_top](images/gbck_top.jpg)
* 2 USB Hubs 
* 1 Ethernet Port (On the top of the refrigerator) ![no_plug](images/refrigerator_top.jpg) ![plug](images/refrigerator_top_with_plug.jpg)
* 1 Power Supply 

## Network Connection
### Overview
The steps to setup the GBCK seem pretty straight forward, but there are multiple ways to accomplish them, and some ways may apply only to certain revisions of the firmware.

1. Connect to the GBCK as a hotspot using the wifi name and password printed on the enclosure
2. Configure the GBCK to use the Quelab Wireless Network
3. Use the GBCK

### Connect to the GBCK as a hotspot
Copied from [FirstBuild Instructions](https://firstbuild.com/mylescaley/green-bean-connect-kit/activity/)

```
To use ssh to connect to your GBCK, you will need to do a few things.

First, you need to install the green-bean-connect-utils from github:

git clone https://github.com/FirstBuild/green-bean-connect-utils
cd green-bean-connect-utils/configuration
npm install

Next, you need to get the IP address of your GBCK. Look on the bottom of your GBCK. You need the Firebase User name and the Firebase Pass. For example, your Firebase User might be connected-firehouse@firebase.com and your Firebase Pass might be jobbylu. With these two pieces of information, run the following command, substituting your own Firebase User and Pass:

node sandbox-data.js connected-firehouse@firebase.com jobbylu

Something like the following should print, assuming the LED on your GBCK is solid:
```

```javascript
{
  "devices": {
    "chillhubs": {
      "0ec150f0-d259-11e4-9a59-afb430376951": { "ip_address": "10.202.9.131" }
    }
  }
}
```

```
You should now have the IP address of your GBCK. Now you can use SSH to connected to your GBCK by typing the following, replacing the IP address below with the IP address of your GBCK:

ssh root@10.202.9.131

When prompted, use 'root' as your password without the quotation marks.
```

### Method 2

The flashing red light ...
nmap -sP 10.1.10.1/24

## Cloud Connection


