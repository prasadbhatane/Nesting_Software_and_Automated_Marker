from Point import Point
from Rectangle import Rectangle
from Sheet import Sheet
from helping_functions import show_graphics, insert_rectangles

sh = Sheet(500, 400)

sh.get_info()

n =  int(input("Enter the number of rectangles : "))
input_rect = []
for i in range(n):
	s = input("rect {} length breadth : ".format(i+1)).split()
	input_rect.append((int(s[0]), int(s[1])))


#input_rect = [(100,30), (20,50), (200,70), (70, 50), (300, 120), (90,90), (40, 120), (50, 150)]

insert_rectangles(input_rect, sh)

sh.get_info()

show_graphics(sh.rectangles_tl_br , sh.l, sh.b)