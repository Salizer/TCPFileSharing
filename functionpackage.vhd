library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

package functionpackage is
	-- Returns 'in1' and 'in2' xor´ed
	function func_xor (signal in1, in2 : in std_logic) return std_logic;
	
	-- Returns(In the 2 signals) the result for AND and OR
	procedure proc_and_or (signal in1, in2 : in std_logic; signal OAnd, OOr : out std_logic);
end;

package body functionpackage is

function func_xor (signal in1, in2 : in std_logic) return std_logic is
begin
	return(in1 XOR in2);
end func_xor;

procedure proc_and_or (signal in1, in2 : in std_logic; signal OAnd, OOr : out std_logic) is
begin
	OAnd <= in1 AND in2;
	OOr <= in1 OR in2;
end proc_and_or;

end package body;