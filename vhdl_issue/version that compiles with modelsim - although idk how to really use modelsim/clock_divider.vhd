library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity clock_divider is
generic (
	divisor : integer
);
port(
	i_rst, i_clk : in std_logic;
	o_clk : out std_logic
	);
end entity clock_divider;

architecture RTL of clock_divider is
constant half_divisor : integer := divisor/2;
--signal count : integer;
begin
	process (i_clk, i_rst)
		variable count : integer := 0;
	begin
		if i_rst = '1' then
			count := 0;
			o_clk <= '1';
		elsif rising_edge(i_clk) then
			count := count + 1;
			if count = half_divisor then
				count := 0;
				o_clk <= not o_clk;
			end if;
		end if;
	end process;
end architecture RTL;

