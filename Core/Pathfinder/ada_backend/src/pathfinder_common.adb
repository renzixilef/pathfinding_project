with Ada.Numerics.Long_Elementary_Functions;
use Ada.Numerics.Long_Elementary_Functions;

package body Pathfinder_Common is

   function Octile_Distance (A, B : Grid_Coord) return Safe_Cost
   with SPARK_Mode => On
   is
      DX, DY : Long_Float;
      Val    : Long_Float;
   begin
      DX := Long_Float (A.x) - Long_Float (B.x);
      DY := Long_Float (A.y) - Long_Float (B.y);
      if DX < 0.0 then
         DX := -DX;
      end if;
      if DY < 0.0 then
         DY := -DY;
      end if;

      -- octile: min*sqrt(2) + abs(dx-dy)
      if DX > DY then
         Val := Long_Float (DY) * Sqrt2 + Long_Float (DX - DY);
      else
         Val := Long_Float (DX) * Sqrt2 + Long_Float (DY - DX);
      end if;

      -- clamp to Safe_Cost'Last
      if Val > Long_Float (Safe_Cost'Last) then
         Val := Long_Float (Safe_Cost'Last);
      end if;
      return Safe_Cost (Val);
   end Octile_Distance;

   function Absolute_Distance (A, B : Grid_Coord) return Safe_Cost
   with SPARK_Mode => On
   is
      DX, DY : Long_Float;
      Val    : Long_Float;
   begin
      DX := Long_Float (A.x) - Long_Float (B.x);
      DY := Long_Float (A.y) - Long_Float (B.y);

      Val := Sqrt (Long_Float (DX * DX + DY * DY));

      -- clamp to Safe_Cost'Last
      if Val > Long_Float (Safe_Cost'Last) then
         Val := Long_Float (Safe_Cost'Last);
      end if;

      return Safe_Cost (Val);
   end Absolute_Distance;

end Pathfinder_Common;
