from rectangle import Rectangle
import matplotlib.pyplot as plt


def areOverlapping(l1, r1, l2, r2):
    # if one rectangle is on the left of the other
    if (l1.x >= r2.x) or (l2.x >= r1.x):
        return False

    # if one rectangle is on the top of the other
    if (r1.y <= l2.y) or (r2.y <= l1.y):
        return False

    return True


def insertRectangles(input_rect, sh):
    for i in range(len(input_rect)):
        if input_rect[i][0] < input_rect[i][1]:
            input_rect[i] = (input_rect[i][1], input_rect[i][0])

    input_rect = sorted(input_rect, reverse=True, key=lambda x: (x[0], x[1]))
    for rt in input_rect:
        rect_obj = Rectangle(rt[0], rt[1])
        sh.addRectangle(rect_obj)


def showNestedDiagram(rect_tl_br_arr, sheet_l, sheet_b):
    plt.axes()
    sheet = plt.Rectangle((0, 0), sheet_l, sheet_b, fc='gray', ec="black")
    plt.gca().add_patch(sheet)

    for rect in rect_tl_br_arr:
        rtl = rect[0]
        rbr = rect[1]
        rectangle = plt.Rectangle((rtl.x, rtl.y), (rbr.x - rtl.x), (rbr.y - rtl.y), fc='blue', ec="red")
        plt.gca().add_patch(rectangle)
    plt.axis('scaled')
    plt.show()
