with Ada.Numerics.Long_Elementary_Functions; use Ada.Numerics.Long_Elementary_Functions;

package body Pathfinder_Common is

   function Octile_Distance (A, B : Grid_Coord) return C_Double is
      DX, DY : Integer;
      MinXY  : Integer;
      Diff   : Integer;
      Val    : Long_Float;
   begin
      DX := Integer (A.x) - Integer (B.x);
      if DX < 0 then DX := -DX; end if;
      DY := Integer (A.y) - Integer (B.y);
      if DY < 0 then DY := -DY; end if;
      
      -- octile: min*sqrt(2) + abs(dx-dy)
      if DX > DY then
         Val := Long_Float (DY) * Sqrt2 + Long_Float (DX-DY);
      else
         Val := Long_Float (DX) * Sqrt2 + Long_Float (DY-DX);
      end if; 
      return C_Double (Val);
    end Octile_Distance;

    function Absolute_Distance (A, B : Grid_Coord) return C_Double is
      DX, DY : Integer;
      MinXY  : Integer;
      Diff   : Integer;
      Val    : Long_Float;
   begin
      DX := Integer (A.x) - Integer (B.x);
      DY := Integer (A.y) - Integer (B.y);
      
      return C_Double (Sqrt(Long_Float (DX * DX + DY * DY)));
    end Absolute_Distance;

end Pathfinder_Common;
 
 