#!/bin/sh
set -x
DIR=`dirname $0` &&
cd $DIR &&
#--- Compilation LaTex
rm -f acan.pdf ref.* acan.ilg acan.ind &&
rm -f acan.aux acan.idx acan.lof acan.lot acan.toc &&
rm -f acan.log acan.out acan.synctex.gz &&
#--- First pass
PDF_LATEX=`which xelatex` &&
#MAKE_INDEX=`which makeindex` &&
$PDF_LATEX --file-line-error --shell-escape acan.tex &&
touch ref.idx &&
touch ref.lof &&
touch ref.lot &&
touch ref.toc &&
iteration=0 &&
while [ `cmp -s ref.toc acan.toc ; echo $?` -ne 0 ]
#    || [ `cmp -s ref.lot acan.lot ; echo $?` -ne 0 ] \
#    || [ `cmp -s ref.toc acan.toc ; echo $?` -ne 0 ] \
#    || [ `cmp -s ref.idx acan.idx ; echo $?` -ne 0 ]
do
#  cp acan.idx ref.idx &&
#  cp acan.lof ref.lof &&
#  cp acan.lot ref.lot &&
  cp acan.toc ref.toc &&
#  $MAKE_INDEX -s $DIR/acan-latex-inclusions/style-indexes.ist acan.idx &&
  $PDF_LATEX --file-line-error --shell-escape acan.tex &&
  iteration=$((iteration+=1))
done &&
rm -f acan.aux acan.idx acan.lof acan.lot acan.toc &&
rm -f acan.log acan.ilg acan.ind acan.out acan.synctex.gz &&
echo "---------------- SUCCES $iteration iterations"
