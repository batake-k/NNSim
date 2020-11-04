# NNSim

## 必要ライブラリ等
gcc  
boost

## 結合重み、バイアス値ファイルの生成
NQueen問題に関しては`data/nqueen`下に別スクリプトとして記述。
```
$ cd NNsim/data/nqueen
$ g++ -std=c++11 -o nqueen nqueen.cpp
$ ./nqueen/nqueen [N] [A] [B] [C] [D]
```
N=5, 100の場合に関しては事前に作成済み。

## Simulatorの実行
N=100のNqueen問題を解く場合は
```
$ cd NNsim
$ make -j
$ ./nnsim -w data/nqueen/nqueen_weights.N100.A1.B1.C1.D1 -b data/nqueen/nqueen_bias.N100.A1.B1.C1.D1 -o output
```
オプションの詳細は
```
$ ./nnsim
```
で確認可能
