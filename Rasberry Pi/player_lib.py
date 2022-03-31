
class players:
    
    def __init__(self,foil = False, foil_button = False, left_chest = False, right_chest = False, belly = False, back = False):
        
        self.foil = foil
        self.foil_button = foil_button
        self.left_chest = left_chest
        self.right_chest = right_chest
        self.belly = belly
        self.back = back
        
    def set(self,foil, foil_button, left_chest, right_chest, belly, back):
        
        self.foil = foil
        self.foil_button = foil_button
        self.left_chest = left_chest
        self.right_chest = right_chest
        self.belly = belly
        self.back = back
        
    def print(self):
        
        return("foil: {},foil_button: {},left_chest: {},right_chest: {},belly: {},back: {}"
              .format(self.foil, self.foil_button, self.left_chest, self.right_chest, self.belly, self.back))