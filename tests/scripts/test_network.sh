#!/bin/bash

script_path=$(dirname $(readlink -f $0))
$script_path/test_network server &
sleep 1
$script_path/test_network client
if [[ $? != 0 ]]; then
    exit 1
fi
