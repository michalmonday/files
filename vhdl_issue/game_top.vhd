library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.vga_graphics.all;

-- this could be "stacker arcade" like game

entity game is
port (
	i_Switch_1, i_Switch_2, i_Switch_3, i_Switch_4 : in std_logic;
	o_LED_1, o_LED_2,o_LED_3, o_LED_4 : out std_logic;

	o_Segment1_A, o_Segment1_B : out std_logic;

	io_PMOD_1, io_PMOD_2, io_PMOD_3, io_PMOD_4 : inout std_logic;
	io_PMOD_7, io_PMOD_8, io_PMOD_9, io_PMOD_10 : inout std_logic;

	o_VGA_HSync, o_VGA_VSync : out std_logic;
    o_VGA_Red_0, o_VGA_Red_1, o_VGA_Red_2 : out std_logic;
    o_VGA_Grn_0, o_VGA_Grn_1, o_VGA_Grn_2 : out std_logic;
    o_VGA_Blu_0, o_VGA_Blu_1, o_VGA_Blu_2 : out std_logic;

	i_Clk : in std_logic
);
	
end entity game;

architecture RTL of game is
--signal pixel_clock : std_logic;
--signal hcount, vcount : unsigned (10 downto 0);
--signal blank : std_logic;

-- vga_graphics record contains information needed to determine if 
-- some shape should be drawn or not, it can be easily passed to
-- procedures or entities. (it contains rgb, hcount, vcount, blank)
-- vga_graphics is defined in vga_graphics_pkg.vhd
signal graphics : vga_graphics;
signal debounced_user_input : std_logic := '0';
begin
	-- Nandland "Go Board FPGA" has 25MHz clock
	graphics.pixel_clk <= i_Clk;

	-- Basys3 board has 50MHz clock
	--clock_div : entity work.clock_divider 
	--	generic map (divisor => 2) 
	--	port map (i_clk => i_Clk, o_clk => pixel_clock);

	--clock_div_2 : entity work.clock_divider 
	--	generic map (divisor => 10000) 
	--	port map (i_clk => pixel_clock, o_clk => divided_clock);

	vga_timing : entity work.vga_timing_25MHz 
		port map (
			i_clk => graphics.pixel_clk, 
			o_hsync => o_VGA_HSync, 
			o_vsync => o_VGA_VSync,
			o_blank => graphics.blank, 
			o_hcount => graphics.hcount,
			o_vcount => graphics.vcount
			--o_graphics => graphics
			);

	game_logic : entity work.game_logic
		port map (
			i_increase_level_clk => debounced_user_input,
			io_graphics => graphics,
			rect_ypos(0) => io_PMOD_8,
			rect_ypos(1) => io_PMOD_9
			);

	-- 50ms debounce
	debounce : entity work.debounce 
		generic map (ticks_duration => 1250000) -- 25000000MHz / 1000ms * 50ms
		port map (
			i_clk => graphics.pixel_clk,
			i_switch => i_Switch_2,
			o_switch => debounced_user_input
			);

	graphics.frame_clk <= '1' when (graphics.hcount = 0 and graphics.vcount = 0) else '0';
	graphics.line_clk <= '1' when graphics.hcount = 0 else '0';

    -- connections to output pins (for logic analyser)
	io_PMOD_1 <= i_Switch_2;
	io_PMOD_2 <= debounced_user_input;
	io_PMOD_3 <= o_VGA_VSync;
	io_PMOD_4 <= o_VGA_Red_0;
	io_PMOD_7 <= o_VGA_Red_1;
	io_PMOD_10 <= graphics.frame_clk;

	-- Nandland board has 3 signals per color 
	(o_VGA_Red_0, o_VGA_Red_1, o_VGA_Red_2) <= graphics.rgb(11 downto 9);
	(o_VGA_Grn_0, o_VGA_Grn_1, o_VGA_Grn_2) <= graphics.rgb(7 downto 5);
	(o_VGA_Blu_0, o_VGA_Blu_1, o_VGA_Blu_2) <= graphics.rgb(3 downto 1);

	-- for Basys3 I could use:
	-- (coma-separated-color-signals) <= graphics.rgb;


	

	--process (i_Clk)
	--begin

	--end process;
end architecture RTL;