
for (( c=1; c<=30; c++ ))
do 
  per=`echo "scale=2;var=$(($RANDOM%101));var/=100;var" | bc`
  a=$(($RANDOM%5555 ))
  b=$(($RANDOM%9999))
  c=$(($RANDOM%99999))
  #per= "val=$(($RANDOM%101));val=/100.1" | bc 
    echo "gerar input$c"
    for (( c=1; c<=30; c++ ))
    do
    python gen.py  $a $per  $(($RANDOM%99999)) ./inputs/input$c.txt
    done
   
done


#criar lista de nomes dos ficheiros testes
search_dir=.
files=""
echo > fileName.txt
for entry in "$search_dir"/*.txt
do
    files="$files$entry "
done
echo "$files " >> fileName.txt