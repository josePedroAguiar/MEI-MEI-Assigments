#!/usr/bin/bash
#criar lista de nomes dos ficheiros testes
files=""
echo > fileName.txt
for (( j=1; j<=5; j++ ))
do
maxflow=$((9999*$j))

for (( k=1; k<=5; k++ ))
do
maxcapacaty=$((9999*$k))
cd inputs
mkdir -p ./flow_$maxflow/capacaty_$maxcapacaty
cd ..
echo "Gerar inputs$maxcapacaty:"
  for (( c=1; c<= 10; c++ ))
      do 
      vert=$((100*$c ))

      echo " $vert $per"
      #vert=$(($RANDOM%5555 ))
      #per=`echo "scale=2;var=$(($RANDOM%101));var/=100;var" | bc`
      #p=$(($RANDOM%9999))
      #c=$(($RANDOM%99999))
      #per= "val=$(($RANDOM%101));val=/100.1" | bc 
      
      echo "  -> Gerar inputs$vert:"
      for (( i=1; i<=10; i++ ))
        do
        per=`echo "scale=2;var=$((($i*10)%101));var/=100;var" | bc`
        echo "    - Gerar input$c$per"
        python3 gen.py  $vert $per  $maxcapacaty $maxflow ./inputs/flow_$maxflow/capacaty_$maxcapacaty/$c$per.txt
        done
    done
    #criar lista de nomes dos ficheiros testes
    search_dir=./inputs/flow_$maxflow/capacaty_$maxcapacaty/$c$per.txt
    for entry in "$search_dir"/*.txt
      do
          files="$files$entry "
      done
    
  done
done
echo "$files " >> fileName.txt
