#!/bin/bash
# Isto foi feito para ajudar o pessoal a correr testes de compiladores automaticamente
# Qualquer coisa agradeçam ao Miguel Ventura, ou então deixem lá isso

# Mudar o caminho da variavel DIR para o directorio onde estao os testes
DIR="testing/tests-xpl-daily-201701121739/"
MYT="testing/mytests/"


#--------------------------------------------------------------------------------------#

COUNTER=1

cd xpl
make clean
make
cd ..

cd testing/tests-xpl-daily-201701121739/
rm -r images
mkdir images
rm -r dotfiles
mkdir dotfiles
rm *.xml
cd ../../

cd testing/mytests/
rm -r images
mkdir images
rm -r dotfiles
mkdir dotfiles
rm *.xml
cd ../../

echo
echo ">> Chegou a hora, vamos gerar o xml:"
echo 

#--------------------------------------------------------------------------------------#
for file in ${DIR}*.xpl
do
  # detecta numero do teste
  FILENAME=$(basename $file)
  NUM=`echo "$FILENAME" | cut -d'-' -f3`
  
  # Se foi fornecido um intervalo
  if [[ -n "$1" && -n "$2" ]]; then
    # limite inferior
    if [ "$NUM" -lt "$1" ]; then
      continue
    fi
    
    #limite superior
    if [ "$NUM" -gt "$2" ]; then
      break
    fi
  fi
  
  # apenas 1 argumento, numero de testes a correr
  if [[ -n "$1" && -z "$2" ]]; then
    if [ "$COUNTER" -gt "$1" ]; then
        break
    fi
  fi
  
  # comando a ser executado
  NAME=`echo "$file" | cut -d'.' -f1`
  TEST=`echo "$FILENAME" | cut -d'.' -f1`
  echo $TEST
  # { ./xpl "$file" --target xml; } >& /dev/null
  { "./xpl/xpl" "$file" --target xml; } &> $DIR/$TEST".xml" # se este nao estiver usa o de cima
  
  # cenas pra converter pra dot e depois pra imagem (neste caso, .ps porque a maquina virtual nao tem nada melhor...)
  { "./xml2dot.py" $DIR/$TEST".xml";   } &> $DIR/"dotfiles"/$TEST".dot"
  { "dot" "-Tps" $DIR/"dotfiles"/$TEST".dot"; } &> $DIR/"images"/$TEST".ps"
  echo $FILENAME" : xml done"
  
  let COUNTER=COUNTER+1
done
#--------------------------------------------------------------------------------------#

#--------------------------------------------------------------------------------------#
for file in ${MYT}*.xpl
do
  # detecta numero do teste
  FILENAME=$(basename $file)
  NUM=`echo "$FILENAME" | cut -d'-' -f3`
  
  # Se foi fornecido um intervalo
  if [[ -n "$1" && -n "$2" ]]; then
    # limite inferior
    if [ "$NUM" -lt "$1" ]; then
      continue
    fi
    
    #limite superior
    if [ "$NUM" -gt "$2" ]; then
      break
    fi
  fi
  
  # apenas 1 argumento, numero de testes a correr
  if [[ -n "$1" && -z "$2" ]]; then
    if [ "$COUNTER" -gt "$1" ]; then
        break
    fi
  fi
  
  # comando a ser executado
  NAME=`echo "$file" | cut -d'.' -f1`
  TEST=`echo "$FILENAME" | cut -d'.' -f1`
  echo $TEST
  # { ./xpl "$file" --target xml; } >& /dev/null
  { "./xpl/xpl" "$file" --target xml; } &> $MYT/$TEST".xml" # se este nao estiver usa o de cima
  
  # cenas pra converter pra dot e depois pra imagem (neste caso, .ps porque a maquina virtual nao tem nada melhor...)
  { "./xml2dot.py" $MYT/$TEST".xml";   } &> $MYT/"dotfiles"/$TEST".dot"
  { "dot" "-Tps" $MYT/"dotfiles"/$TEST".dot"; } &> $MYT/"images"/$TEST".ps"
  echo $FILENAME" : xml done"
  
  let COUNTER=COUNTER+1
done
#--------------------------------------------------------------------------------------#

echo
echo ">> Já está tudo xmlizado!"
echo
echo $(($COUNTER - 1)) " ficheiros xmlizados."
echo "Agora vai lá ver os .xml para ver se não houve porcaria."
echo

cd xpl 
make clean
cd ..


echo
echo "Verificar erros nos xml/dot/ps"
echo

grep -r "error" testing
grep -r "syntax" testing
grep -r "segmentation" testing

echo
echo "bye"
echo
