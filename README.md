Outastracoin integration/staging tree
================================

https://www.outastra.com

Copyright (c) 2009-2017 Bitcoin Developers
Copyright (c) 2011-2017 Litecoin Developers
Copyright (c) 2016-2017 Outastracoin Developers

What is Outastracoin?
----------------

Outastracoin is a alternatvie version of Bitcoin using scrypt just like Litecoin as a proof-of-work algorithm.
 - 5 minute block targets
 - ~640 million total coins
 - 500 coins per block
 - 576 blocks to retarget difficulty

For more information, as well as an immediately useable, binary version of
the Outastracoin client sofware, see https://www.outastra.com.

License
-------

Outastracoin is released under the terms of the MIT license. See `COPYING` for more
information or see http://opensource.org/licenses/MIT.

Development process
-------------------

Developers work in their own trees, then submit pull requests when they think
their feature or bug fix is ready.

Compiling Linux Wallet on Ubuntu/Debian
----------------------

Step 1. Install the depencies. 

```sudo apt-get install software-properties-common```

```sudo add-apt-repository ppa:bitcoin/bitcoin```

```sudo apt-get update```

```sudo apt-get install libdb4.8-dev libdb4.8++-dev build-essential libtool autotools-dev automake pkg-config libssl-dev libevent-dev bsdmainutils git libboost-all-dev libminiupnpc-dev libqt5gui5 libqt5core5a libqt5webkit5-dev libqt5dbus5 qttools5-dev qttools5-dev-tools libprotobuf-dev protobuf-compiler libqrencode-dev```

**Note**: If you are on debian, you will also need to `sudo apt-get install libcanberra-gtk-module`.

Step 2. Clone the git repository and compile the daemon and gui wallet:

```git clone https://github.com/outastra/outastracoin && cd outastracoin/src/leveldb && chmod +x build_detect_platform && TARGET_OS=Linux make libleveldb.a libmemenv.a && cd .. && make -f makefile.unix USE_UPNP=-```

```git clone https://github.com/outastra/outastracoin && cd outastracoin && qmake "USE_QRCODE=1" "USE_UPNP=1" "USE_IPV6=1" bitcoin-qt.pro
 && make -f Makefile.Release```


Using the wallet:
----
The gui wallet is in ./outastracoin/src/qt and the daemon in ./outastracoin/src directories.

**Note**: If you see something like 'Killed (program cc1plus)' run ```dmesg``` to see the error(s)/problems(s). This is most likely caused by running out of resources. You may need to add some RAM or add some swap space.

**Optional**:
If you want to copy the binaries for use by all users, run the following commands:

```sudo cp src/outastracoind /usr/bin/```

```sudo cp src/qt/outastracoin-qt /usr/bin/```

Step 3. Creating a configuration file. Type ```cd ~``` to get back to the home folder and type:

```outastracoind.exe```  (or ```./outastracoind``` if on mac or linux)

the output from this command will tell you that you need to make a outastracoin.conf and will suggest some good starting values.

For Linux users, type:
 
```nano ~/.OutAstra/outastracoin.conf```
(For Windows users, see below. For mac users, the command is ```nano ~/Library/Application\ Support\Outastracoin\outastracoin.conf```)
    
Paste the output from the `outastracoind` command into the outastracoin.conf like this: (It is recommended to change the password to something unique.)

    rpcuser=outastracoinrpcuser
    rpcpassword=85CpSuCNvDcYsdQU8w621mkQqJAimSQwCSJL5dPT9wQX
    
    
**Optional**: Add `rpcport=33318` or `port=33317`.

Add `daemon=1`. 

Your config may look something like this:

    rpcuser=outastracoinrpcuser
    rpcpassword=85CpSuCNvDcYsdQU8w621mkQqJAimSQwCSJL5dPT9wQX
    rpcport=33318
    port=33317
    daemon=1

Exit the outastracoin.conf by pressing `ctrl + x` on your keyboard then pressing `y` and hitting enter. This should have created a outastracoin.conf file with what you just added. 

Type ```outastracoind.exe``` (or ```./outastracoind``` if on mac or linux) and your outastracoin daemon should start.

To check the status of how much of the blockchain has been downloaded (aka synced) type `outastracoind.exe getinfo` (or `./outastracoind getinfo` if on mac or linux).


