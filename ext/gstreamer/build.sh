#!/bin/sh

if [ ! -d gstreamer-0.10.23 ]; then
    wget http://gstreamer.freedesktop.org/src/gstreamer/gstreamer-0.10.23.tar.bz2;
    tar jxf gstreamer-0.10.23.tar.bz2;
    cd gstreamer-0.10.23;
    ./configure && make && make install;
    cd -
fi;

if [ ! -d gst-plugins-base-0.10.23 ]; then
    wget http://gstreamer.freedesktop.org/src/gst-plugins-base/gst-plugins-base-0.10.23.tar.bz2;
    tar jxf gst-plugins-base-0.10.23.tar.bz2;
    cd gst-plugins-base-0.10.23;
    ./configure && make && make install;
    cd -
fi;

if [ ! -d gst-plugins-bad-0.10.12 ]; then
    wget http://gstreamer.freedesktop.org/src/gst-plugins-bad/gst-plugins-bad-0.10.12.tar.bz2;
    tar jxf gst-plugins-bad-0.10.12.tar.bz2;
    cd gst-plugins-bad-0.10.12;
    patch -p1 < gst-plugins-bad-0.10.12_diskovideosink.diff;
    ./configure && make && make install;
    cd -
fi;

