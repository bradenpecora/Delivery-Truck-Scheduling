# icpc -o amazon_demo main.cc
g++ -o amazon_demo.out main.cc
./amazon_demo.out
python3 showRoutes.py --arrows routes/greedy.txt & python3 showRoutes.py --arrows routes/nongreedy.txt
python3 showRoutes.py routes/Greedy\ 150.txt & python3 showRoutes.py routes/\'Opt2\'\ 150.txt
python3 showRoutes.py routes/pre_multopt2_1.txt routes/pre_multopt2_2.txt & python3 showRoutes.py routes/post_multopt2_1.txt routes/post_multopt2_2.txt
python3 showRoutes.py routes/noOpt1.txt routes/noOpt2.txt & python3 showRoutes.py routes/noPrimeSmall1.txt routes/noPrimeSmall2.txt & python3 showRoutes.py routes/PrimeSmall1.txt routes/PrimeSmall2.txt
python3 showRoutes.py routes/noPrimeLarge1.txt routes/noPrimeLarge2.txt & python3 showRoutes.py routes/withPrimeLarge1.txt routes/withPrimeLarge2.txt