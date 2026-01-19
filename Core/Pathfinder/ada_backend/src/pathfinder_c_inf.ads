with Interfaces;
with Interfaces.C;
with System;

package Pathfinder_C_Inf is
   subtype Grid_Handle is System.Address;
   subtype Cell_Queue_Handle is System.Address;

   -- Basic integer/byte aliases
   subtype C_Long is Interfaces.C.long;
   subtype C_Double is Interfaces.C.double;

   -- C-types
   type Grid_Coord is record
      x : C_Long;
      y : C_Long;
   end record
   with Convention => C;

   type GridCoordArray is array (0 .. 7) of Grid_Coord with Convention => C;

   type Neighbors_Array is record
      coords : GridCoordArray;
   end record
   with Convention => C;

   type Cell_Cost is record
      gCost : C_Double;
      hCost : C_Double;
   end record
   with Convention => C;

   -- C-style state Enum
   type Cell_State is
     (CELL_OBSTACLE, CELL_OPEN, CELL_VISITED, CELL_CLOSED, CELL_PATH)
   with Convention => C;

   -- Imported functions
   function get_neighbors_from_coord
     (grid : Grid_Handle; coordX : C_Long; coordY: C_Long) return Neighbors_Array
   with Import => True, Convention => C;

   function get_cell_cost
     (grid : Grid_Handle; coordX : C_Long; coordY: C_Long) return Cell_Cost
   with Import => True, Convention => C;

   function get_cell_state
     (grid : Grid_Handle; coordX : C_Long; coordY: C_Long) return Cell_State
   with Import => True, Convention => C;

   procedure increment_visited_cell_count (grid : Grid_Handle)
   with Import => True, Convention => C;

   procedure mark_cell_visited (grid : Grid_Handle; coordX : C_Long; coordY: C_Long)
   with Import => True, Convention => C;

   procedure set_cell_costs
     (grid : Grid_Handle; coordX : C_Long; coordY: C_Long; gCost: C_Double; hCost: C_Double)
   with Import => True, Convention => C;

   procedure set_parent_cell_pointer
     (grid : Grid_Handle; coordX : C_Long; coordY: C_Long; parentCoordX : C_Long; parentCoordY: C_Long)
   with Import => True, Convention => C;

   procedure push_to_cell_queue (queue : Cell_Queue_Handle; coordX : C_Long; coordY: C_Long)
   with Import => True, Convention => C;

end Pathfinder_C_Inf;
