with Pathfinder_C_Inf; use Pathfinder_C_Inf;

package Pathfinder_Common is
    Sqrt2 : constant Long_Float := 1.4142135623730951;

    function Octile_Distance (A, B : Grid_Coord) return C_Double;
    function Absolute_Distance (A, B : Grid_Coord) return C_Double;
end Pathfinder_Common;