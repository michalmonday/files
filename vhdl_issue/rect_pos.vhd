library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.vga_graphics.all;


entity rect_pos is
generic(
	rect_width : unsigned (10 downto 0) := 80;
	rect_height : unsigned (10 downto 0) := 60
	);
port(
	i_graphics : in vga_graphics;
	o_xpos : out unsigned(2 downto 0) := 0;
	o_ypos : out unsigned(2 downto 0) := 0
	);
end entity;

architecture RTL of rect_pos is
	signal x_rect_clk, y_rect_clk : std_logic;
begin
	x_rect_clock : entity work.clock_divider 
		generic map(divisor => to_integer(rect_width)) 
		port map(i_rst => i_graphics.line_clk, i_clk => i_graphics.pixel_clk, o_clk => x_rect_clk);

	y_rect_clock : entity work.clock_divider 
		generic map(divisor => to_integer(rect_height)) 
		port map(i_rst => i_graphics.frame_clk, i_clk => i_graphics.line_clk, o_clk => y_rect_clk);

	process (x_rect_clk, i_graphics.line_clk, i_graphics.frame_clk)
	begin
		if i_graphics.line_clk = '1' or i_graphics.frame_clk = '1' then
			o_xpos <= 0;
		elsif rising_edge(x_rect_clk) then
			o_xpos <= o_xpos + 1;
		end if;
	end process;

	process (y_rect_clk, i_graphics.frame_clk)
	begin
		if i_graphics.frame_clk = '1' then
			o_ypos <= 0;
		elsif rising_edge(y_rect_clk) then
			o_ypos <= o_ypos + 1;
		end if;
	end process;

	--process (i_graphics.frame_clk)
	--begin 
	--	if rising_edge(i_graphics.frame_clk) then
	--		--o_ypos <= 0;
	--	end if;
	--end process;

	--process (i_graphics.line_clk)
	--begin 
	--	if rising_edge(i_graphics.line_clk) then
	--		--o_xpos <= 0;
	--	end if;
	--end process;
end architecture RTL;
