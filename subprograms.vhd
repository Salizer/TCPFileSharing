library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
work.functionpackage.all -- ER DET IKKE DET HER, DER SKAL SKRIVES????

-- Entity with the functions: XOR, AND, OR
entity subprograms is
	port(A, B : in std_logic;
			XOR_OUT, AND_OUT, OR_OUT : out std_logic);
end subprograms;

-- VED AT ANVENDE WORK.FUNCTIONSPACKAGE.ALL, SÅ BEHØVER VI VEL IKKE ALT UNDERSTÅENDE INDTIL BEGIN?

-- Architecture for the functions implemented
architecture subbing of subprograms is
-- Functions and procedures
-- Returns 'in1' and 'in2' xor´ed
function func_xor (signal in1, in2 : in std_logic) return std_logic is
begin
	return(in1 XOR in2);
end func_xor;

-- Returns(In the 2 signals) the result for AND and OR
procedure proc_and_or (signal in1, in2 : in std_logic; signal OAnd, OOr : out std_logic) is
begin
	OAnd <= in1 AND in2;
	OOr <= in1 OR in2;
end proc_and_or;

begin

-- Call the functions
XOR_OUT <= func_xor(A, B);
proc_and_or(A, B, AND_OUT, OR_OUT);

end subbing;