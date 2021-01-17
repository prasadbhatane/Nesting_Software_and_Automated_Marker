from point import Point
from rectangle import Rectangle
from utils import areOverlapping


class Sheet:
    def __init__(self, length, breadth):
        self.length = length
        self.breadth = breadth

        self.area = length * breadth

        self.cornerPoints = set()  # contains tuples (x, y)
        self.cornerPoints.add((0, 0))

        self.rectangleSet = set()

        self.x_set = set()
        self.y_set = set()

    def getInfo(self):
        print("sheet dimensions are :", self.length, self.breadth)
        print("corner points : ", self.cornerPoints)
        print("rectangles_tl_br : ", self.rectangleSet)

    def isSheetEmpty(self):
        return len(self.cornerPoints) == 1

    def addRectangle(self, rectangle):
        # get greedy position for given rectangle in sheet
        xl, yl, reverse = self.getGreedyPosition(rectangle)

        rl = rectangle.length
        rb = rectangle.breadth
        if reverse:
            rl, rb = rb, rl

        l1 = Point(xl, yl)
        r1 = Point(xl + rl, yl + rb)

        # add the rectangle with coordinates in rectangleSet
        self.rectangleSet.add((l1, r1))

        # add all 4 corner points of rectangle in cornerPoints
        self.cornerPoints.add((xl, yl))
        self.cornerPoints.add((xl, yl + rb))
        self.cornerPoints.add((xl + rl, yl))
        self.cornerPoints.add((xl + rl, yl + rb))

        # add x_set points
        self.x_set.add(xl)
        self.x_set.add(xl + rl)

        # add y_set points
        self.y_set.add(yl)
        self.y_set.add(yl + rb)

    def getGreedyPosition(self, rectangle):
        if self.isSheetEmpty():
            return 0, 0, False
        else:
            xyInscribedArea = []

            # without reversing rectangle
            for p in self.cornerPoints:
                tl = Point(p[0], p[1])
                br = Point(p[0] + rectangle.length, p[1] + rectangle.breadth)

                overlapFlag = False

                # checking overlap with every rectangle ...
                for old_tl_br in self.rectangleSet:
                    old_tl = old_tl_br[0]
                    old_br = old_tl_br[1]
                    if areOverlapping(old_tl, old_br, tl, br):
                        overlapFlag = True
                        break

                if not overlapFlag:
                    if ((p[0] + rectangle.length) <= self.length) and ((p[1] + rectangle.breadth) <= self.breadth):
                        m_x = max(p[0] + rectangle.length, max(self.x_set))
                        m_y = max(p[1] + rectangle.breadth, max(self.y_set))
                        m_area_inscribed = m_x * m_y
                        xyInscribedArea.append((p[0], p[1], m_area_inscribed, False))
                    else:
                        pass

            # after reversing rectangle
            for p in self.cornerPoints:
                tl = Point(p[0], p[1])
                br = Point(p[0] + rectangle.breadth, p[1] + rectangle.length)

                overlapFlag = False
                # checking overlap with every rectangle ...
                for old_tl_br in self.rectangleSet:
                    old_tl = old_tl_br[0]
                    old_br = old_tl_br[1]
                    if areOverlapping(old_tl, old_br, tl, br):
                        overlapFlag = True
                        break

                if not overlapFlag:
                    if ((p[0] + rectangle.breadth) <= self.length) and ((p[1] + rectangle.length) <= self.breadth):
                        m_x = max(p[0] + rectangle.breadth, max(self.x_set))
                        m_y = max(p[1] + rectangle.length, max(self.y_set))
                        m_area_inscribed = m_x * m_y
                        xyInscribedArea.append((p[0], p[1], m_area_inscribed, True))

            xyInscribedArea = sorted(xyInscribedArea, key=lambda x: x[2])
            return xyInscribedArea[0][0], xyInscribedArea[0][1], xyInscribedArea[0][3]
