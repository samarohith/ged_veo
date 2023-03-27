s=("bzr" "aids" "muta" "tox" "mcf" "molt")
b=("p53")
c=(4)
for e in ${c[@]}; do 
    for f in ${b[@]};
        do ./gsim -m0 -o1 -t2 $f/$f".txt" 3 $e; cp result.txt $e".txt";mv $e".txt" $f; rm result.txt;
    done;
done;

