#!/bin/sh
#
# This script tries to make an amalgated header by using gcc dependency
# generation to find out which files depend on which. Then it uses
# tsort to find out in which order they need to be include. After that,
# it extracts standard includes and put them at the top. The output
# is an amalgated source file which is verified to make sense using
# gcc.
# The result can be used to create a single file include for installation
# or perhaps copy pasting into some online IDE.
#
# Invoke with *all* header files, each individual file
# as a command line argument.
# 
# Paul Dreik 20181007

set -e

#we expect to be executed in the include/ folder.
if [ ! -d safe_compare ] ; then
  echo "expected to be executed in the include/ folder"
  exit 1
fi
IPATH=.

#strip this path from the given includes
pathtostrip=$(pwd)/include/

#given a file $1, lists which non-system header it depends on
#into the deps file, on a format recognized by gnu tsort.
depends_on() {
strippedfilename=$(echo $1 | sed -e "s@$pathtostrip\(.*\)@\1@g")
echo "#include \"$strippedfilename\"" | \
g++ -MM -I $IPATH -E - | \
tr -d '\\' | \
tr '\n' ' ' | \
xargs -n1 echo $strippedfilename >>deps 
}

cat /dev/null >deps
while [ $# -gt 0 ] ; do
  depends_on $1
  shift
done

tsort deps | \
tac | \
grep -v ':' >deps.tmp

cat /dev/null >amalgated.hpp
for f in $(cat deps.tmp) ; do
   echo "// begin include of file $f" >>amalgated.hpp
   grep -v '#include "' $f |grep -v "#pragma once" >>amalgated.hpp
   echo "// end include of file $f" >>amalgated.hpp
done

#make sure the file is ok, by compiling it
g++ -c amalgated.hpp -o /dev/null

#assemble the complete file
mv amalgated.hpp amalgated.hpp.orig

echo '//amalgated source of lib safe_compare' > amalgated.hpp
echo "#pragma once" >>amalgated.hpp
grep "#include <" amalgated.hpp.orig |sort |uniq >>amalgated.hpp
grep -v "#include <" amalgated.hpp.orig >>amalgated.hpp

#clean up the garbage
rm deps deps.tmp amalgated.hpp.orig
