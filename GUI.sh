#!/bin/bash

source SmartMessage/devel/setup.bash
{
gnome-terminal -t "runSub" -x bash -c "rosrun smart_topic anesthesia_subscriber"
}&
echo "Monitor Run!"

