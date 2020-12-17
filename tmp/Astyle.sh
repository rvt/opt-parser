cd ../
astyle --options=./code.astylerc --recursive *.h
astyle --options=./code.astylerc --recursive *.c
astyle --options=./code.astylerc --recursive *.cpp
astyle --options=./code.astylerc --recursive *.hpp


find . -type f -name "*.orig" -exec rm {} \;
