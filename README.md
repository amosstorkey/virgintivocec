# virgin tivo cec

Use your Raspberry Pi to enable CEC Control of you Virgin Tivo box. (c) 2013 Amos Storkey, licenced under GPL2.

For more background see https://github.com/mkulke/cecanyway

# Requires (hardware):

0) Virgin Box and an always-on Raspberry Pi or Pi2.

1) https://www.pulse-eight.com/

Pulse Eight USB-CEC Adapter at 30 quid. About the same as you'd save on a cheapo programmable remote.

2) Network connection to tivo box and Pi.

# Use Cases
Case 1) You use an osmc pi but have a virgin tivo box. But virgin is not CEC compatible and so your osmc is constantly grabbing attention from it. This adds a CEC capability to the TIVO so this no longer happens.

Case 2) You want to control the virgin box using the TV remote. But you can't, because virgin were cheapskates (just joking VM) and didn't include any CEC hardware on their tivo box. So you're stuffed...

except...

If you have a Pi kicking around and are willing to spend out on the Pulse Eight box, we can do the following:

TV receives remote signal. Pumps signal to Pulse Eight box, which sends it via USB to PI which decodes that input to port control signals for the tivo and sends it to the tivo over the network, which then responds to the keypress.

#Caveats
This code is the worst piece of hacky code ever. But it does achieve this goal (though no promises are made), and it works without any significant problems. I just don't have time to understand all the ins and outs of libCEC, or write proper general purpose code. After all controlling my tivo box via CEC is only worth so much, and I do actually have a life, so I am afraid hacking this together was as good as it was going to get. But I suspect it will still be useful to one or two ther people out there, so up it goes.

#To use
This assumes you are using osmc stable release (i.e. July to October++ 2015), which contains libcec3. Other versions may require edits for other versions of libcec. I've included libcec3.0.0 with the code, but you really only need the headers.

#Known issue. 
Sometimes if you reboot the pi, the virign box no longer provides a telly signal. It provides the menu items just no picture. Putting the telly into standby and out returns life to normality. I assume this is a problem wrt to some hdcp (yuck) handshake issue. Note the assumption is that you will be leaving the pi always-on. If you are worried about the minsicule half a watt the pi consumes destroying the planet, then you wouldn't be running a virgin tivo... There may be ways around this, but its not on my task list.

#Installation:
Ensure the pi/pi2, and virgin box are networked and the virgin box is on a fixed or reserved IP address, and you know it (the IP address that is).

Add the Pulse eight box into the hdmi line from the virgin box to wherever it goes to (TV, receiver whatever)

Plug the USB from the Pulse Eight box into a sub port on your paspberry pi or pi 2.

Download virgintivocec and the copy of libcec that comes with it. Note the libcec has had a version.h added (instead of a version.h.in) to enable its use with commonorgarden make.

Hard edit the IP address for the virgin box in main.cpp (default is 192.168.0.16  - told you it was hacky).

Hard edit what you want your virgin box to be labelled as in main.cpp (default is "Virgin Tivo")

Hard edit the CEC address for the Virgin Box (default HDMI port one on the receiver connected to the hdmi port one on the TV. I.e. Physical address 1100. See https://www.mythtv.org/wiki/HDMI-CEC for a bit more description on this.).


Do you use kodi? Probably. If so go in to kodi, settings, input devices, peripherals and you will now see two cec controllers. Disable the one corresponding to the Pulse Eight box (how? I used trial and error).

Now link libcec

    cd virgintivocec-master
    ln -s ../libcec-libcec-3.0.0/include libcec

Then compile and install it    

    sudo apt-get upgrade
    sudo apt-get update
    sudo apt-get install make g++
    make
    sudo make install
    sudo update-rc.d virgintivocec defaults
    sudo service virgintivocec start

All being well that should kick off the virgintivocec service. A reboot of the pi/pi2 and you should be away.

# Thanks
The code is a rehack from cecanyway, licenced under GPL2. (C) 2013 Magnus Kulke. It also includes the version of libCEC appropriate for current OSMC (again under GPL2).

I've built this for OSMC on a Pi2. Your milage may vary for other platforms. Older code may use earlier libCEC packages.




