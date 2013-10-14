#!/bin/sh

git clone git@github.com:jwerle/sphia.git /tmp/sphia
cd /tmp/sphia
make docs
make install
make clean
cd /tmp
rm -rf /tmp/sphia
