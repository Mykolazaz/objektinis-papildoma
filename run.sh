mkdir -p ./build
cd ./build

cp ../vilnius-wiki.txt .
cp ../tlds-alpha-by-domain.txt .

cmake ..

make

./fileAnalyzer
