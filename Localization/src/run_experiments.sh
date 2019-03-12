r=$1 #num of robots
#a=nogui #makes processing faster, but no gui
d=$(($r+4))
for i in $(ls crop/"$r"s[1,2,3]*.avi);do 
	../bin/swarmcon $i $d $a;
	cd output
	for j in $(ls 00*.bmp);
	do
		u=$(echo $i\_$j|sed s/crop.//)
		mv $j $u 
	done
	cd ..
done;
for i in $(ls crop/"$r"s4*.avi);
do 
	../bin/swarmcon $i $d advection 0.254 $a;
	cd output
	for j in $(ls 00*.bmp);
	do
		u=$(echo $i\_$j|sed s/crop.//)
		mv $j $u 
	done
	cd ..
done;

for i in $(ls crop/"$r"s[5-6]*.avi);
do 
	../bin/swarmcon $i $d advection 0.563 $a;
	cd output
	for j in $(ls 00*.bmp);
	do
		u=$(echo $i\_$j|sed s/crop.//)
		mv $j $u 
	done
	cd ..
done
