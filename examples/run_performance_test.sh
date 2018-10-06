#!/bin/sh

set -e

me=$(basename $0)
rootdir=$(realpath $(dirname $0)/..)

timingfile=$rootdir/perfresults.txt
sizefile=$rootdir/sizeresults.txt
cat /dev/null > $timingfile
cat /dev/null > $sizefile



run_for_optlevel() {

builddir=$rootdir/build-perftests-$1

if [ -d "$builddir" ] ; then
  rm -rf "$builddir"
fi

mkdir -p $builddir
cd $builddir
CXX="ccache g++" CXXFLAGS="-O$1" cmake .. 
make -j8

#execute all performance tests


find . -type f -executable -name "performance_*" -print0 | \
sort --zero-terminated | \
xargs --null -n1 /usr/bin/time --append -o $timingfile --format="$1\t%C\t%e"

#measure the size of the binary
find . -type f -executable -name "performance_*" -print0 | \
sort --zero-terminated | \
xargs --null -n1 stat --printf "$1\t%n\t%s\n" >$sizefile
}


run_for_optlevel 0
run_for_optlevel g
run_for_optlevel 1
run_for_optlevel 2
run_for_optlevel 3
run_for_optlevel s

