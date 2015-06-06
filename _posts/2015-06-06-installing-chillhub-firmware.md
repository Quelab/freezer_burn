---
layout: post
title:  Replicating the Green Bean Connect Kit
categories: chillhub raspberrypi
---
## Replicating the Green Bean Connect Kit
While we have the Green Bean Connect Kit (GBCK) hooked up to the refrigerator at Quelab I wanted to be able to replicate the system to experiment with at home. To do this I bought the following parts:

* [CanaKit RaspberryPi 2 Complete Starter Kit](http://www.canakit.com/raspberry-pi-starter-kit.html)
* [Green Bean Maker Module](http://market.firstbuild.com/products/greenbean)

I had to make several modifications to the default First Build installation scripts to make things work well.
Those changes can be found on the [Quelab GitHub page](https://github.com/Quelab):
Complete instructions for recreating what I've done are available in the [Quelab green been connect utils fork](https://github.com/Quelab/green-bean-connect-utils/blob/master/chillhub-host/QuelabInstall.md)

## Screen Casts
Some screen casts that walk you through the steps detailed above

1. Installing Archlinux on a SD card for the raspberry pi 2 [![asciicast](https://asciinema.org/a/21125.png)](https://asciinema.org/a/21125)

2. Installing the Chillhub firmware. [![asciicast](https://asciinema.org/a/21114.png)](https://asciinema.org/a/21114)

## Note
In the future it looks like FirstBuild will be moving to a different platform, so these instructions are most likely for historic purposes and replicating the environment of the Chillhub Connect as sent to Maker spaces.
