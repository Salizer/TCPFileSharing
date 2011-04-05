library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity AlarmWatch is
	port(CLK, SPEED, RESET, VIEW : in std_logic;
	AL_MIN_1, AL_MIN_10, AL_HOUR1, AL_HOUR10 : in unsigned(3 downto 0);
	SEG1, SEG10, SEG100, SEG1000, SEG10000, SEG100000 : out std_logic_vector(6 downto 0);
	ALARM : out std_logic);
end AlarmWatch;

architecture Alarmering of AlarmWatch is
--Signals
-- Limits
signal lim_al_min_1,lim_al_min_10,lim_al_hour_1,lim_al_hour_10 : std_logic_vector (3 downto 0);

-- s for sekunder, m for minutter, h for timer
signal s1,s10,m1,m10,h1,h10,m1a,m10a,h1a,h10a, s0a : std_logic_vector (6 downto 0) ;
signal bin_val : std_logic_vector(15 downto 0);

begin
	-- Limits for minutes
	lim_al_min_1 <= std_logic_vector(AL_MIN_1) when AL_MIN_1 <= 9 else "1001";
	lim_al_min_10 <= std_logic_vector(AL_MIN_10) when AL_MIN_10 <= 5 else "0101";
	
	-- Limits for hours
	lim_al_hour_1 <= std_logic_vector(AL_HOUR1) when AL_HOUR1 <= 9 and AL_HOUR10 < 2 else
					 std_logic_vector(AL_HOUR1) when AL_HOUR1 <= 3 and AL_HOUR10 = 2 else "0011";
					 
	lim_al_hour_10 <= std_logic_vector(AL_HOUR10) when AL_HOUR10 <= 2 else "0010";
		
		a0:			entity work.BinaryDecoder port map(V => "0000", HEX0 => s0a);
		a1:			entity work.BinaryDecoder port map(V => lim_al_min_1, HEX0 => m1a);
		a10:		entity work.BinaryDecoder port map(V => lim_al_min_10, HEX0 => m10a);
		a100:		entity work.BinaryDecoder port map(V => lim_al_hour_1, HEX0 => h1a);
		a1000:		entity work.BinaryDecoder port map(V => lim_al_hour_10, HEX0 => h10a);
		
		w: entity work.watch port map(CLK => CLK, SPEED => SPEED, RESET => RESET, SEG1 => s1, SEG10 => s10, SEG100 => m1, 
		SEG1000 => m10, SEG10000 => h1, SEG100000 => h10, BIN_VAL => bin_val);
		
		ALARM <= '1' when bin_val = lim_al_hour_10 & lim_al_hour_1 & lim_al_min_10 & lim_al_min_1 else '0';
		
		-- I forhold til opgavebeskrivelsen
		process (VIEW)
		begin
			if VIEW = '1' then
				SEG1 <= s0a;
				SEG10 <= s0a;
				SEG100 <= m1a;
				SEG1000 <= m10a;
				SEG10000 <= h1a;
				SEG100000 <= h10a;
			else
				SEG1 <= s1;
				SEG10 <= s10;
				SEG100 <= m1;
				SEG1000 <= m10;
				SEG10000 <= h1;
				SEG100000 <= h10;
			end if;
		end process;

end Alarmering;