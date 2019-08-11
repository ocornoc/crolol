#!/bin/bash
wget -q https://dl.bintray.com/boostorg/release/1.70.0/source/${BOOSTNAME}.zip
unzip -q ${BOOSTNAME}.zip
cd ${BOOSTNAME}
sudo ./bootstrap.sh
sudo ./b2 stage -j 2 -d0 --with-math
cd ..
