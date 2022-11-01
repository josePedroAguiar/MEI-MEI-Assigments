search_dir=./inputs/flow_9999/capacaty_9999
files=""
echo > fileName.txt
for entry in "$search_dir"/*.txt
do
    files="$files$entry "
done
echo "$files " >> fileName.txt


for  file in $files
    do
        echo "correr $file"
        ./Dinic 9999 $file >> Dinic_1.txt
        ./EK 9999  $file >> EK_1.txt
        ./MPM 9999 $file  >> MPM_1.txt
done
