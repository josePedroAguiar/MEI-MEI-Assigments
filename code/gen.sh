#!/usr/bin/bash
for (( j=1; j<=10; j++ ))
do
maxflow=$((9999*$j))
for (( j=1; j<=10; j++ ))
do
maxcapacaty=$((9999*$j))
  for (( c=1; c<=10; c++ ))
      do 
      vert=$((2500*$c ))

      echo " $vert $per"
      #vert=$(($RANDOM%5555 ))
      #per=`echo "scale=2;var=$(($RANDOM%101));var/=100;var" | bc`
      #p=$(($RANDOM%9999))
      #c=$(($RANDOM%99999))
      #per= "val=$(($RANDOM%101));val=/100.1" | bc 
      echo "gerar input$c:"
      for (( i=1; i<=20; i++ ))
        do

        per=`echo "scale=2;var=$((($i*5)%101));var/=100;var" | bc`
        echo "  gerar input$c$per"
        python3 gen.py  $vert $per  $maxcapacaty $maxflow ./inputs/input$maxflow$maxcapacaty$c$per.txt
        done
    done
  done
done


#criar lista de nomes dos ficheiros testes
search_dir=./inputs
files=""
echo > fileName.txt
for entry in "$search_dir"/*.txt
do
    files="$files$entry "
done
echo "$files " >> fileName.txt