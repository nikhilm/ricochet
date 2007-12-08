#!/bin/bash
echo $0
cd ${0%`basename $0`}
./src/ricochet

