with Pathfinder_C_Inf; use Pathfinder_C_Inf;
package dijkstra is
   procedure nextStep(grid : Grid_Handle; current_coord_x : Pathfinder_C_Inf.C_Long;
        current_coord_y: Pathfinder_C_Inf.C_Long; queue : Cell_Queue_Handle)
      with 
         Convention => C,
         SPARK_Mode => On,
         Export => True,
         External_Name => "ada_dijkstra_next_step";
   
end dijkstra;
