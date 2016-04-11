rm src/boost/ -rf
cd src/
wget http://netix.dl.sourceforge.net/project/boost/boost/1.55.0/boost_1_55_0.tar.bz2
tar xvjf boost_1_55_0.tar.bz2
rm boost_1_55_0.tar.bz2
cd boost_1_55_0/
./boostrap.sh
./b2
cd ../../
mkdir build
cd build 
cmake -DCMAKE_BUILD_TYPE="Release" ..
make
