from sheet import Sheet
from utils import insertRectangles, showNestedDiagram

n = int(input("Enter the number of rectangles : "))
input_rect = []
sheetLength, sheetBreadth = 0, 0
print('Enter length and breadth of each rectangle separated by space eg, : 100 20')
for i in range(n):
    s = input("rect {} length breadth : ".format(i + 1)).split()
    sheetLength += max((int(s[0]), int(s[1])))
    sheetBreadth = sheetLength
    input_rect.append((int(s[0]), int(s[1])))


sh = Sheet(sheetLength, sheetBreadth)

insertRectangles(input_rect, sh)

showNestedDiagram(sh.rectangleSet, sh.length, sh.breadth)
