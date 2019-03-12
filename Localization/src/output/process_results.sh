mkdir full
mkdir proc
mkdir text
for i in $(ls *.avi.txt);do grep X00X $i|head -n 300|cut -f 3,6,8-19 -d ' ' >text/$i;done
for i in $(ls *.avi.txt);do grep X00X $i|head -n 300|cut -f 6,9,11,13,15,17,19 -d ' ' >proc/$i;done
mv *.txt full
zip results.zip -r full proc text
