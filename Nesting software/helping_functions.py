import matplotlib.pyplot as plt
from Point import Point
from Rectangle import Rectangle
from Sheet import Sheet

def show_graphics(rect_tl_br_arr, sheet_l, sheet_b):
    plt.axes()
    sheet = plt.Rectangle((0,0),(sheet_l), (sheet_b), fc='gray',ec="black")
    plt.gca().add_patch(sheet)
    
    for rect in rect_tl_br_arr:
        rtl = rect[0]
        rbr = rect[1]
        rectangle = plt.Rectangle((rtl.x, rtl.y),(rbr.x-rtl.x), (rbr.y-rtl.y), fc='blue',ec="red")
        plt.gca().add_patch(rectangle)
    plt.axis('scaled')
    plt.show()


def insert_rectangles(input_rect, sh):
    for i in range(len(input_rect)):
        if input_rect[i][0] < input_rect[i][1]:
            input_rect[i] = (input_rect[i][1], input_rect[i][0])
    
    input_rect = sorted(input_rect, reverse=True, key=lambda x:(x[0]*x[1], x[0], x[1]))
    for rt in input_rect:
        rect_obj = Rectangle(rt[0], rt[1])
        sh.add_rect(rect_obj)