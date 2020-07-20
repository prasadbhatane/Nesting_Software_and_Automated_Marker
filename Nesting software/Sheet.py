from Point import Point

def do_Overlap(l1, r1, l2, r2):
    if l1.x == l2.x and l1.y == l2.y:
        return True
    
    # atleast one point among 4 points of rect2 should be in rect1 for overlapping
    p1 = Point(l2.x, l2.y)
    p2 = Point(l2.x, r2.y)
    p3 = Point(r2.x, l2.y)
    p4 = Point(r2.x, r2.y)
    
    if (p1.x < r1.x and p1.x > l1.x) and (p1.y > r1.y and p1.y < l1.y):
        return True
    elif (p2.x < r1.x and p2.x > l1.x) and (p2.y > r1.y and p2.y < l1.y):
        return True
    elif (p3.x < r1.x and p3.x > l1.x) and (p3.y > r1.y and p3.y < l1.y):
        return True
    elif (p4.x < r1.x and p4.x > l1.x) and (p4.y > r1.y and p4.y < l1.y):
        return True
    else:
        return False

class Sheet:
    def __init__(self, l, b):
        self.l = l
        self.b = b
        self.area = l*b
        self.corner_points = set()
        self.rectangles_tl_br = set()              #----------> {(l1,r1)}
        self.x_set = set()
        self.y_set = set()
        
    def get_info(self):
        print("sheet dimensions are :", self.l, self.b)
        print("corner points : ", self.corner_points)
        print("rectangles_tl_br : ", self.rectangles_tl_br)
        print("x_set : ", self.x_set)
        print("y_set : ", self.y_set)
        
        
    def get_area(self):
        return self.area
    
    def get_length(self):
        return self.length
    
    def get_breadth(self):
        return self.breadth
    
    def is_sheet_ampty(self):
        return len(self.corner_points) == 1
    
    def add_rect(self, rect):
        xl,yl,orientation = self.greedy_position(rect)
        rl = rect.l
        rb = rect.b
        
        if orientation == 'rotate':
            rl, rb = rb, rl
        
        l1 = Point(xl, yl)
        r1 = Point(xl+rl, yl+rb)
        
        self.rectangles_tl_br.add((l1,r1))
        
        self.corner_points.add((xl,yl))
        self.corner_points.add((xl,yl+rb))
        self.corner_points.add((xl+rl,yl))
        self.corner_points.add((xl+rl,yl+rb))
        
        # add x_set points
        self.x_set.add(xl)
        self.x_set.add(xl+rl)
        
        # add y_set points
        self.y_set.add(yl)
        self.y_set.add(yl+rb)
        
        
    def greedy_position(self, rect):
        if len(self.rectangles_tl_br) == 0:
            return 0,0,'same'
        else:
            ### rect -----> rect.l  , rect.b
            tlx_tly_inscribed_area = []
            
            ##########################################################################################
            # for same orientation
            for p in self.corner_points:
                tl = Point(p[0], p[1])
                br = Point(p[0] + rect.l, p[1] + rect.b)
                
                overlap_flag = False
                # checking overlap with every rectangle ... 
                # if there's an overlap.. then come out of inner for loop and see the next point
                # or if there isn't then add point and area to the tlx_tly_inscribed_area
                for old_tl_br in self.rectangles_tl_br:
                    old_tl = old_tl_br[0]
                    old_br = old_tl_br[1]
                    if do_Overlap(old_tl, old_br, tl, br):
                        overlap_flag = True
                        break
                        
                if (not overlap_flag) and ((p[0] + rect.l) <= self.l) and ((p[1] + rect.b) <= self.b):
                    m_x = max(p[0] + rect.l, max(self.x_set))
                    m_y = max(p[1] + rect.b, max(self.y_set))
                    m_area_inscribed = m_x*m_y
                    tlx_tly_inscribed_area.append((p[0], p[1], m_area_inscribed, "same"))
                    
            ###########################################################################################        
            # for rotate orientation
            for p in self.corner_points:
                tl = Point(p[0], p[1])
                br = Point(p[0] + rect.b, p[1] + rect.l)
                
                overlap_flag = False
                # checking overlap with every rectangle ... 
                # if there's an overlap.. then come out of inner for loop and see the next point
                # or if there isn't then add point and area to the tlx_tly_inscribed_area
                for old_tl_br in self.rectangles_tl_br:
                    old_tl = old_tl_br[0]
                    old_br = old_tl_br[1]
                    if do_Overlap(old_tl, old_br, tl, br):
                        overlap_flag = True
                        break
                        
                if (not overlap_flag) and ((p[0] + rect.b) <= self.l) and ((p[1] + rect.l) <= self.b):
                    m_x = max(p[0] + rect.b, max(self.x_set))
                    m_y = max(p[1] + rect.l, max(self.y_set))
                    m_area_inscribed = m_x*m_y
                    tlx_tly_inscribed_area.append((p[0], p[1], m_area_inscribed, 'rotate'))
            ###########################################################################################
            
            tlx_tly_inscribed_area = sorted(tlx_tly_inscribed_area, key=lambda x:x[2])
            return tlx_tly_inscribed_area[0][0], tlx_tly_inscribed_area[0][1], tlx_tly_inscribed_area[0][3]
                
                
            