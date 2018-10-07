#!/bin/sh
# 
# For analyzing data from the performance test. 
#
# License:
# dual license, pick your choice. Either Boost license 1.0, or GPL(v2 or later,
# at your option).
# By Paul Dreik 20181007

set -e

me=$(basename $0)
rootdir=$(realpath $(dirname $0)/..)

timingfile=$rootdir/perfresults.txt
sizefile=$rootdir/sizeresults.txt

tabfile=$rootdir/perfresults_summary.tsv


#make a tab separated file with the results. sort
#echo "optlevel\tmethod\ttypeA\ttypeB\truntime" >$tabfile
sed -e 's@./examples/performance_@@g' <$timingfile | \
tr '_' '\t' | \
sort -k1 -k2 -k3 -k4 >$tabfile

#rearrange so the three methods (plain, throw, correct) are pasted
#column wise
cols=$( cut -f2 $tabfile |sort |uniq)
for col in $cols ; do
  grep $col $tabfile >${col}
done
paste $cols >all.tsv

#keep only the interesting
echo "optlevel\tmethod\ttype1\ttype2" >interesting.tsv
cut -f1-5,7,10,12,15 all.tsv >>interesting.tsv

echo done, have a look at the file interesting.tsv
