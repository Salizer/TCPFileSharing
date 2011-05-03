library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.functionpackage.all -- ER DET IKKE DET HER, DER SKAL SKRIVES????

-- Entity with the functions: XOR, AND, OR
entity subprograms is
	port(A, B : in std_logic;
			XOR_OUT, AND_OUT, OR_OUT : out std_logic);
end subprograms;

begin

-- Call the functions
XOR_OUT <= func_xor(A, B);
proc_and_or(A, B, AND_OUT, OR_OUT);

end subbing;