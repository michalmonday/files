library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-- This code is to some extent based on the following guide:
-- https://www.nandland.com/goboard/debounce-switch-project.html

-- However, that guide used debouncing method which ignored the pressed/released
-- button until sufficient clock cycles with changed state occured.
-- I think it is not the best of debouncing for this game because 
-- this game is all about timing precision. So my method is ignoring 
-- glitches themselves instead after initial state change. 
-- The drawback of my method is that it may recognize glitch as a button press.

entity debounce is
generic (
	ticks_duration : natural
);
port(
	i_clk, i_switch : in std_logic;
	o_switch : out std_logic := '0'
	);
end entity debounce;

architecture RTL of debounce is
begin
	process (i_clk)
		variable count : natural range 0 to ticks_duration := 0;
	begin
		if rising_edge(i_clk) then
			if count = 0 and i_switch /= o_switch then
				o_switch <= i_switch;
				-- set count to 1, which initiates actual counting
				count := 1;
			elsif count = ticks_duration then
				count := 0;
			elsif count /= 0 then
				-- count only if it was initiated
				count := count + 1;
				-- ignore i_switch changes, to avoid recognizing glitches as valid changes
			end if;
		end if;
	end process;
end architecture RTL;


