#!/bin/sh
cd "$(dirname "$0")"

LD_LIBRARY_PATH=../liboic:../libcoap ./ocf-livingroom-arm-device -d
