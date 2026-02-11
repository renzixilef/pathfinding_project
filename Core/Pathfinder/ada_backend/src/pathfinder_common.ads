with Pathfinder_C_Inf; use Pathfinder_C_Inf;
with Interfaces.C;
use type Interfaces.C.double;

package Pathfinder_Common is
   Sqrt2 : constant Long_Float := 1.4142135623730951;

   function Octile_Distance (A, B : Grid_Coord) return Safe_Cost
   with
     SPARK_Mode => On;

   function Absolute_Distance (A, B : Grid_Coord) return Safe_Cost
   with
     SPARK_Mode => On;
end Pathfinder_Common;
