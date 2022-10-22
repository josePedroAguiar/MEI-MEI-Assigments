
files="input01.txt input02.txt"

for  file in $files
    do
        echo "correr $file"
        ./Dinic 9999 $file
        ./EK 9999  $file
        ./MPM 9999 $file
done