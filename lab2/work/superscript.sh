for l in 0 2 3; do
    for i in `seq 0 2` ; do
        for j in 2; do
            res=""$l"_res_"$i"_"$j".txt"
            rm $res
            for k in `seq 128 128 8448`; do
                echo "$res $k"
                taskset -c 3 ./"$l"_lab2.o $k 0 $i $j >> $res
            done
        done
    done
done
