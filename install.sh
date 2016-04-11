cd src/
if [ ! -d "./boost_1_60_0/" ]; then
  wget http://netix.dl.sourceforge.net/project/boost/boost/1.60.0/boost_1_60_0.tar.bz2
  tar xvjf boost_1_60_0.tar.bz2
  rm boost_1_60_0.tar.bz2
fi
cd boost_1_60_0/
ls -la
./bootstrap.sh
./b2
cd ../../
mkdir build
cd build 
cmake -DCMAKE_BUILD_TYPE="Release" ..
make
