library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity watch is
	port(CLK, SPEED, RESET : in std_logic;
			SEG1, SEG10, SEG100, SEG1000, SEG10000, SEG100000 : out std_logic_vector(6 downto 0);
			COUT : out std_logic;
			BIN_VAL : out std_logic_vector(15 downto 0));
end watch;

architecture watching of watch is
-- Signals
signal i_clk : std_logic;
signal i_cout1 : std_logic;
signal i_cout10 : std_logic;
signal i_cout100 : std_logic;
signal i_cout1000 : std_logic;
signal i_cout10000 : std_logic;
signal logicBinVal1, logicBinVal2 : std_logic_vector(3 downto 0); -- For feedback(When to reset)
signal resetSignal : std_logic;

begin
process(CLK)
begin
	if((not RESET) = '1') then
		resetSignal <= '0';
	elsif((logicBinVal1 = "0100") and (logicBinVal2 = "0010")) then -- 24:00:00
		resetSignal <= '0';
	else
		resetSignal <= '1';
	end if;
end process;

-- Connect the entities
clocker : entity work.clockGen port map(CLK => CLK, SPEED => SPEED, RESET => RESET, CLK_OUT => i_clk);
counterS1 : entity work.multiCounter port map(CLK => i_clk, MODE => "00", RESET => resetSignal, SEG => SEG1, 
	COUT => i_cout1, BIN_VAL => open); 
counterS10 : entity work.multiCounter port map(CLK => i_cout1, MODE => "01", RESET => resetSignal, SEG => SEG10, 
	COUT => i_cout10, BIN_VAL => open); 
counterM1 : entity work.multiCounter port map(CLK => i_cout10, MODE => "00", RESET => resetSignal, SEG => SEG100, 
	COUT => i_cout100, BIN_VAL => BIN_VAL(3 downto 0));
counterM10 : entity work.multiCounter port map(CLK => i_cout100, MODE => "01", RESET => resetSignal, SEG => SEG1000, 
	COUT => i_cout1000, BIN_VAL => BIN_VAL(7 downto 4));   
counterH1 : entity work.multiCounter port map(CLK => i_cout1000, MODE => "00", RESET => resetSignal, SEG => SEG10000, 
	COUT => i_cout10000, BIN_VAL => logicBinVal1);
counterH10 : entity work.multiCounter port map(CLK => i_cout10000, MODE => "11", RESET => resetSignal, SEG => SEG100000, 
	COUT => open, BIN_VAL => logicBinVal2);   
	
	BIN_VAL(11 downto 8) <= logicBinVal1;
	BIN_VAL(15 downto 12) <= logicBinVal2;
end watching;