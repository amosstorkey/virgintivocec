# virgin tivo cec

Use your Raspberry Pi to enable CEC Control of you Virgin Tivo box. (c) 2013 Amos Storkey, licenced under GPL2.

For more background see https://github.com/mkulke/cecanyway

Requires (hardware):

0) Virgin Box and Pi or Pi2 (obviously).

1) https://www.pulse-eight.com/

Pulse Eight USB-CEC Adapter at 30 quid. About the same as you'd save on a cheapo programmable remote.

2) Network connection to tivo box and Pi.


Case 1) You use an osmc pi but have a virgin tivo box. But virgin is not CEC compatible and so your osmc is constantly grabbing attention from it. This adds a CEC capability to the TIVO so this no longer happens.

Case 2) You want to control the virgin box using the TV remote. But you can't, because virgin were cheapskates and didn't include and CEC hardware on their tivo box. So you're stuffed...

except...

If you have a Pi kicking around and are willing to spend out on the Pulse Eight box, we can do the following:

TV receives remote signal. Pumps signal to Pulse Eight box, which sends it via USB to PI which decodes that input to port control signals for the tivo and sends it to the tivo over the network, which then responds to the keypress.

This code is the worst piece of hacky code ever. But it does achieve this goal. sorry its so awful. I just don't have time to understand all the ins and outs of libCEC, or write proper general purpose code. After all controlling my tivo box via CEC is only worth so much, and I do actually have a life.

However it does work (at the moment).

The code is a rehack from cecanyway licence under GPL2. (C) 2013 Magnus Kulke. It also includes the current version of libCEC appropriate for OSMC.

I've built this for OSMC. Your milage may vary for other platforms. Older code may use earlier libCEC packages.




