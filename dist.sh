#!/bin/bash

DATE=`date -I`
NAME=netwm-iconset
svn -q export . $NAME-$DATE
tar cfj $NAME-$DATE.tar.bz2 $NAME-$DATE
rm -fr $NAME-$DATE

echo "$NAME-$DATE.tar.bz2 is ready for distribution"
