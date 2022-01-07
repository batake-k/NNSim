import sys

class Board :

    def __init__(self, file):
        self.squares = []

        f = open(file)
        num_squares = int(f.readline())
        for i in range(num_squares):
            line = f.readline()
            sp = line.split(',')
            self.squares.append([int(sp[0]), int(sp[1])])

    def isExist(self, square):
        for s in self.squares:
            if s == square:
                return True

        return False

    def calcComplexity1(self): # 周の長さ
        length = 0

        for square in self.squares:
            if not self.isExist([square[0] + 1, square[1] + 0]):
                length += 1
            if not self.isExist([square[0] - 1, square[1] + 0]):
                length += 1
            if not self.isExist([square[0] + 0, square[1] + 1]):
                length += 1
            if not self.isExist([square[0] + 0, square[1] - 1]):
                length += 1

        return length

    def calcComplexity2(self): # 周囲8マスが埋まっているかどうかの平均
        count = 0

        for square in self.squares:
            if self.isExist([square[0] + 1, square[1] + 0]):
                count += 1
            if self.isExist([square[0] + 1, square[1] + 1]):
                count += 1
            if self.isExist([square[0] + 0, square[1] + 1]):
                count += 1
            if self.isExist([square[0] - 1, square[1] + 1]):
                count += 1
            if self.isExist([square[0] - 1, square[1] + 0]):
                count += 1
            if self.isExist([square[0] - 1, square[1] - 1]):
                count += 1
            if self.isExist([square[0] + 0, square[1] - 1]):
                count += 1
            if self.isExist([square[0] + 1, square[1] - 1]):
                count += 1

        return float(count) / len(self.squares)

    def calcComplexity(self):
        complexity1 = self.calcComplexity1()
        complexity2 = self.calcComplexity2()
        print(str(complexity1) + "," + str(complexity2))


file = sys.argv[1]

board = Board(file)
board.calcComplexity()