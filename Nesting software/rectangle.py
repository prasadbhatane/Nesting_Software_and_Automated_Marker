class Rectangle:
    def __init__(self, l, b):
        self.length = l
        self.breadth = b
        self.area = l * b

    def get_area(self):
        return self.area

    def get_length(self):
        return self.length

    def get_breadth(self):
        return self.breadth
