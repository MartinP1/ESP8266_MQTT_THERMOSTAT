#!/bin/bash

stty_orig=$(stty -g)
stty -echo
read -p"Password auf Zielsystem: " var
stty $stty_orig
echo $var

