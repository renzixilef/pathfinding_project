with Pathfinder_C_Inf;  use Pathfinder_C_Inf;
with Pathfinder_Common; use Pathfinder_Common;
with Interfaces.C;      use Interfaces.C;
with Interfaces;        use Interfaces;
with Ada.Text_IO;       use Ada.Text_IO;

package body a_star is
   procedure nextStep
     (grid            : Grid_Handle;
      current_coord_x : Pathfinder_C_Inf.C_Long;
      current_coord_y : Pathfinder_C_Inf.C_Long;
      end_coord_x     : Pathfinder_C_Inf.C_Long;
      end_coord_y     : Pathfinder_C_Inf.C_Long;
      queue           : Cell_Queue_Handle)
   with SPARK_Mode => On
   is
      current_coord           : Pathfinder_C_Inf.Grid_Coord :=
        Pathfinder_C_Inf.Grid_Coord'(current_coord_x, current_coord_y);
      end_coord               : Pathfinder_C_Inf.Grid_Coord :=
        Pathfinder_C_Inf.Grid_Coord'(end_coord_x, end_coord_y);
      neighbors               : Neighbors_Array :=
        get_neighbors_from_coord (grid, current_coord.x, current_coord.y);
      current_costs           : Cell_Cost :=
        get_cell_cost (grid, current_coord.x, current_coord.y);
      current_g               : C_Double := current_costs.gCost;
      neighbor_coord          : Grid_Coord;
      neighbor_costs          : Cell_Cost;
      neighbor_state          : Cell_State;
      neighbor_g_from_current : C_Double;
   begin
      for I in GridCoordArray'Range loop
         neighbor_coord := neighbors.coords (I);

         -- skip empty sentinel entries (wrapper pads remaining slots with U32_MAX)
         exit when neighbor_coord.x = -1 and neighbor_coord.y = -1;

         neighbor_g_from_current :=
           current_g + Octile_Distance (neighbor_coord, current_coord);

         neighbor_state :=
           get_cell_state (grid, neighbor_coord.x, neighbor_coord.y);

         if neighbor_state = CELL_OPEN then
            set_cell_costs
              (grid,
               neighbor_coord.x,
               neighbor_coord.y,
               neighbor_g_from_current,
               Absolute_Distance (neighbor_coord, end_coord));
            set_parent_cell_pointer (grid, neighbor_coord.x, neighbor_coord.y, current_coord.x, current_coord.y);
            mark_cell_visited (grid, neighbor_coord.x, neighbor_coord.y);
            push_to_cell_queue (queue, neighbor_coord.x, neighbor_coord.y);
            increment_visited_cell_count (grid);

         elsif neighbor_state = CELL_VISITED then
            neighbor_costs := get_cell_cost (grid, neighbor_coord.x, neighbor_coord.y);
            if neighbor_costs.gCost > neighbor_g_from_current then
               -- preserve previous h-cost
               set_cell_costs
                 (grid,
                  neighbor_coord.x, neighbor_coord.y,
                  neighbor_g_from_current, neighbor_costs.hCost);
               set_parent_cell_pointer (grid, neighbor_coord.x, neighbor_coord.y, current_coord.x, current_coord.y);
               push_to_cell_queue (queue, neighbor_coord.x, neighbor_coord.y);
               increment_visited_cell_count (grid);
            end if;
         end if;
      end loop;
   end nextStep;

end a_star;
