# virgin tivo cec

Use your Raspberry Pi to enable CEC Control of your Virgin Tivo box (i.e. control of Virgin box via TV remote). (c) Copyright 2015 Amos Storkey, licenced under GPL2.

For more background see https://github.com/mkulke/cecanyway

# Requires (hardware):

0) Virgin Box and an always-on Raspberry Pi or Pi2 running OSMC (other OS may well work but are untested).

1) https://www.pulse-eight.com/

Pulse Eight USB-CEC Adapter at 30 quid. About the same as you'd save on a cheapo programmable remote.

2) Network connection to tivo box and Pi.

3) TV. I have set this up for a Samsung Telly. CEC is not as standard as it ought to be so you might have to hack some codes for oher TVs. Note the (or my) Samsung remote does not have both a fast forward and a skip. I find the skip more useful, so have set the FF/RW buttons to do skip forward/back.

# Use Cases
Case 1) You use an osmc pi but have a virgin tivo box. But virgin is not CEC compatible and so your osmc is constantly grabbing attention from it when you turn on the TV, which is a real pain. This adds a CEC capability to the TIVO so this no longer happens.

Case 2) You want to control the virgin box using the TV remote. But you can't, because virgin were cheapskates (just joking VM) and didn't include any CEC hardware on their tivo box. So you're stuffed...

except...

If you have a Pi kicking around and are willing to spend out on the Pulse Eight box, we can do the following:

TV receives remote signal. Pumps signal to Pulse Eight box, which sends it via USB to PI which decodes that input to port control signals for the tivo and sends it to the tivo over the network, which then responds to the keypress.

#Caveats
This code is the worst piece of hacky code ever. But it does achieve this goal (though no promises are made), and it works without any significant problems. I just don't have time to understand all the ins and outs of libCEC, or write proper general purpose code. After all controlling my tivo box via CEC is only worth so much, and I do actually have a life, so I am afraid hacking this together was as good as it was going to get. But I suspect it will still be useful to one or two ther people out there, so up it goes.

#To use
This assumes you are using osmc stable release (currently a version later than November 2015), which contains libcec separation. Other versions may require edits for other versions of libcec.

#Known issues. 
Sometimes if you reboot the pi, the virign box no longer provides a telly signal. It provides the menu items just no picture. Putting the virgin box into standby and out returns life to normality. I assume this is a problem wrt to some hdcp (yuck) handshake issue. Note the assumption is that you will be leaving the pi always-on. If you are worried about the minsicule half a watt the pi consumes destroying the planet, then you wouldn't be running a virgin tivo... There may be ways around this, but its not on my task list.

Again, on the reboot of the Pi, I have noticed it turn the receiver off when it shouldn't after the first selection of Kodi. Turning the receiver on manually fixes this permanently until the next pi reboot. Again I cant see why this is (it is dependent on libCEC version). I hope it all fixes itself if I can end up using a different libCEC in the future. In the meantime, a once in a moon button press is probably not beyond even the most lethargic couch potato.

#Installation:
Ensure the pi/pi2, and virgin box are networked and the virgin box is on a fixed or reserved IP address, and you know it (the IP address that is).

Add the Pulse eight box into the hdmi line from the virgin box to wherever it goes to (TV, receiver whatever)

Plug the USB from the Pulse Eight box into a USB port on your paspberry pi or pi 2.

Download virgintivocec. Also install libcec using

    sudo apt-get update
    sudo apt-get install libcec2

We also need the headers for the libcec 2.1.4. This is best downloaded into the virgintivocec directory from

    wget --no-check-certificate https://github.com/Pulse-Eight/libcec/archive/libcec-2.1.4.tar.gz

and upacked using

    tar -xvf libcec-2.1.4.tar.gz

Hard edit the IP address for the virgin box in main.cpp (default is 192.168.0.16  - told you it was hacky).

Hard edit what you want your virgin box to be labelled as in main.cpp (default is "Virgin Tivo")

Hard edit the CEC address for the Virgin Box (default HDMI port one on the receiver connected to the hdmi port one on the TV. I.e. Physical address 1100. See https://www.mythtv.org/wiki/HDMI-CEC for a bit more description on this.).

Do you use kodi? Probably. If so go in to kodi, settings, input devices, peripherals and you will now see two cec controllers. Disable the one corresponding to the Pulse Eight box (how? I used trial and error).

Now link libcec

    cd virgintivocec/virgintivocec-master
    ln -s ../libcec-libcec-2.1.4/include libcec

Then compile and install virgintivocec   

    sudo apt-get update
    sudo apt-get dist-upgrade
    sudo apt-get install make g++
    make
    sudo make install
    sudo update-rc.d virgintivocec defaults
    sudo service virgintivocec start

All being well that should kick off the virgintivocec service. A reboot of the pi/pi2 and you should be away.

# Thanks
The code is a rehack from cecanyway, licenced under GPL2. (C) 2013 Magnus Kulke. 

I've built this for OSMC on a Pi2. Your milage may vary for other platforms.

Works on OSMC November 2015 build.  
Works on OSMC December 2015 build.  
Works on OSMC January 2016 build.  
