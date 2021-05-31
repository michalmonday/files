library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-- vga_timing_single circuit can be reused to implement horizontal
-- and vertical timing circuit by supplying different generic
-- values when instantiating the "vga_timing_single" entity

-- At the bottom of the file there's vga_timing_25MHz entity.
-- It instantiates both, horizontal and vertical "vga_timing_single"
-- entities, it can be used by the parent class like:

-- architecture top_level_architecture of game is
-- begin
--   vga_timing : entity work.vga_timing_25MHz 
--	    port map (i_clk => PIXEL_CLOCK_TO_SUPPLY, 
--		  o_blank => VGA_BLANK_EXTERNAL_WIRE, 
--		  o_hsync => VGA_HSYNC_EXTERNAL_WIRE, 
--		  o_vsync => VGA_VSYNC_EXTERNAL_WIRE);
-- end architecture


entity vga_timing_single is
generic (
	front_porch_start  : unsigned (10 downto 0);
	sync_signal_start  : unsigned (10 downto 0);
	back_porch_start   : unsigned (10 downto 0);
	visible_area_start : unsigned (10 downto 0)
);
port (
	i_clk : in std_logic;
	o_sync  : out std_logic; 
	o_blank : out std_logic;
	o_count : out unsigned (10 downto 0)
);
end entity vga_timing_single;

architecture RTL of vga_timing_single is
begin
	process(i_clk)
	begin
		if rising_edge(i_clk) then
			o_count <= o_count + 1;
			case o_count is
				when front_porch_start  =>  o_blank <= '1';
				when sync_signal_start  =>  o_sync  <= '0';
				when back_porch_start   =>  o_sync  <= '1';
				when visible_area_start =>  o_blank <= '0'; 
					o_count <= to_unsigned(0,11);
				when others => null;
			end case;
		end if;
	end process;
end architecture RTL;


-- idk what are the exact rules to put 2 entities/architectures 
-- in a single file, but without these 2 repeated lines below there 
-- are errors like:
--     vga_timing.vhd":68:21:68:21|No identifier "std_logic" in scope
--     vga_timing.vhd":84:26:84:26|No identifier "i_clk" in scope
library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

entity vga_timing_25Mhz is
port(
	i_clk : in std_logic;
	o_hsync : out std_logic;
	o_vsync : out std_logic;
	o_blank : out std_logic;
	o_hcount : out unsigned (10 downto 0);
	o_vcount : out unsigned (10 downto 0)
	--o_graphics : out vga_graphics
	);
end entity;

architecture RTL of vga_timing_25Mhz is
signal h_blank, v_blank : std_logic;
begin
	-- 25MHz pixel clock setup such as:
	-- http://tinyvga.com/vga-timing/640x480@60Hz
	hor_timing : entity work.vga_timing_single 
		generic map (
			front_porch_start => to_unsigned(640,11),
			sync_signal_start => to_unsigned(656,11),
			back_porch_start => to_unsigned(752,11),
			visible_area_start => to_unsigned(800,11))
		port map (i_clk => i_clk, o_sync => o_hsync, o_blank => h_blank, o_count => o_hcount);

	ver_timing : entity work.vga_timing_single 
		generic map (
			front_porch_start => to_unsigned(480,11),
			sync_signal_start => to_unsigned(490,11),
			back_porch_start => to_unsigned(492,11),
			visible_area_start => to_unsigned(525,11))
		port map (i_clk => h_blank, o_sync => o_vsync, o_blank => v_blank, o_count => o_vcount);

	o_blank <= h_blank or v_blank;
end architecture RTL;