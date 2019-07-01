# author: snikons

BUILD_TYPE=Debug

if [ ! -z $1 ]; then
  BUILD_TYPE="$1"
fi

rm -rf _build
mkdir _build
cd _build
cmake -DCMAKE_BUILD_TYPE="$BUILD_TYPE" ..
make