PNV=$PROJECTNAME-$PROJECTVERSION
TMPDIR=/tmp/$PNV
TARFILE=$PNV.tar

echo "creating source package"

mkdir -p $TMPDIR/doc/conf
cp README Makefile dep.def shlib.def $TMPDIR
cp -R inc $TMPDIR
cp -R src $TMPDIR
cp -R scripts $TMPDIR
cp ./doc/conf/$PROJECTNAME.conf $TMPDIR/doc/conf
cd /tmp
tar -cvf $TARFILE ./$PNV/README ./$PNV/Makefile ./$PNV/dep.def ./$PNV/shlib.def ./$PNV/doc >/dev/null
tar -rf $TARFILE `find ./$PNV/inc -name "*.h"`
tar -rf $TARFILE `find ./$PNV/src -name "*.cpp"`
tar -rf $TARFILE `find ./$PNV/src -name "Makefile"`
tar -rf $TARFILE `find ./$PNV/scripts -name "*.h"`
bzip2 $TARFILE
cd - >/dev/null
mv /tmp/$TARFILE.bz2 .
rm -rf $TMPDIR

echo "-> $TARFILE.bz2"
