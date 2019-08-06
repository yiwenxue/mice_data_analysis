#########################################################################
# FileName: generate.sh
# Author: Yiwen Xue
# mail: 15225434259xue@gmail.com
# Created Time: Mon 22 Jul 2019 09:43:04 PM CST
#########################################################################
#!/bin/bash
for file in $(ls -A1 $1 | grep $2.Temperature.txt)
do 
    echo $file 
    ./test $1/$file 
done

for file in $(ls -A1 $1 | grep $2.Activity.txt)
do 
    echo $file 
    ./test $1/$file 
done

sleep 1 

mv $1/*.pdf $1/$2/
