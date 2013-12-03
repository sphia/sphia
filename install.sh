#!/bin/sh

git clone git@github.com:sphia/sphia.git /tmp/sphia
make -C /tmp/sphia install
rm -rf /tmp/sphia
