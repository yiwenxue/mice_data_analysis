#########################################################################
# FileName: run.sh
# Author: Yiwen Xue
# mail: 15225434259xue@gmail.com
# Created Time: Sun 16 Jun 2019 08:17:48 AM IDT
#########################################################################
#!/bin/bash

echo "type the path to the folder"
read path

for file in $(ls $path | grep .txt)
do 
    echo "./main -f $path/$file -T MICE_DATA  -m"
    echo "./main -f $path/$file -T MICE_DATA  -s"
    echo "./main -f $path/$file -T MICE_DATA  -d 1"
    echo "./main -f $path/$file -T MICE_DATA  -d 2"
done
