#!/bin/sh

set -e

me=$(basename $0)
rootdir=$(realpath $(dirname $0)/..)

timingfile=$rootdir/perfresults.txt
sizefile=$rootdir/sizeresults.txt

tabfile=$rootdir/perfresults_summary.tsv


#make a tab separated file with the results
echo "optlevel\tmethod\ttypeA\ttypeB\truntime" >$tabfile
sed -e 's@./examples/performance_@@g' <$timingfile | \
tr '_' '\t' >>$tabfile

libreoffice $tabfile

