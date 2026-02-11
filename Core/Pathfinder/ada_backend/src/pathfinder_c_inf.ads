with Interfaces.C;     use type Interfaces.C.double;
with Interfaces.C;     use type Interfaces.C.long;
with System;

package Pathfinder_C_Inf is
   subtype Grid_Handle is System.Address;
   subtype Cell_Queue_Handle is System.Address;

   -- Basic integer/byte aliases
   subtype C_Long is Interfaces.C.long;
   subtype C_Double is Interfaces.C.double;
   -- Grid indices are implemented in the backend as uint32_t and not long
   subtype Grid_Idx is C_Long range 0 .. 2**32-1;
   subtype Grid_Idx_Dev is C_Long range -2**32-1 .. 2**32-1;
   subtype Safe_Cost is C_Double range 0.0 .. 6.1E9;

   -- C-types
   type Grid_Coord is record
      x : Grid_Idx;
      y : Grid_Idx;
   end record
   with Convention => C;

   type GridCoordArray is array (0 .. 7) of Grid_Coord with Convention => C;

   type Neighbors_Array is record
      coords : GridCoordArray;
   end record
   with Convention => C;

   type Cell_Cost is record
      gCost : Safe_Cost;
      hCost : Safe_Cost;
   end record
   with Convention => C;

   -- C-style state Enum
   type Cell_State is
     (CELL_OBSTACLE, CELL_OPEN, CELL_VISITED, CELL_CLOSED, CELL_PATH)
   with Convention => C;

   -- Imported functions
   function get_neighbors_from_coord
     (grid : Grid_Handle; coordX : Grid_Idx; coordY : Grid_Idx)
      return Neighbors_Array
   with Import => True, Convention => C, Global => null;

   function get_cell_cost
     (grid : Grid_Handle; coordX : Grid_Idx; coordY : Grid_Idx) return Cell_Cost
   with
     Import     => True,
     Convention => C,
     Global     => null;

   function get_cell_state
     (grid : Grid_Handle; coordX : Grid_Idx; coordY : Grid_Idx) return Cell_State
   with Import => True, Convention => C, Global => null;

   procedure increment_visited_cell_count (grid : Grid_Handle)
   with Import => True, Convention => C, Global => null, Always_Terminates;

   procedure mark_cell_visited
     (grid : Grid_Handle; coordX : Grid_Idx; coordY : Grid_Idx)
   with Import => True, Convention => C, Global => null, Always_Terminates;

   procedure set_cell_costs
     (grid   : Grid_Handle;
      coordX : Grid_Idx;
      coordY : Grid_Idx;
      gCost  : C_Double;
      hCost  : C_Double)
   with Import => True, Convention => C, Global => null, Always_Terminates;

   procedure set_parent_cell_pointer
     (grid         : Grid_Handle;
      coordX       : Grid_Idx;
      coordY       : Grid_Idx;
      parentCoordX : Grid_Idx;
      parentCoordY : Grid_Idx)
   with Import => True, Convention => C, Global => null, Always_Terminates;

   procedure push_to_cell_queue
     (queue : Cell_Queue_Handle; coordX : Grid_Idx; coordY : Grid_Idx)
   with Import => True, Convention => C, Global => null, Always_Terminates;

end Pathfinder_C_Inf;
