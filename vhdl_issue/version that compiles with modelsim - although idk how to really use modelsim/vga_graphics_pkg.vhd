library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;

-- This file contains package with vga_graphics record.
-- Why is it created?
-- To avoid passing many parameters to procedures or entities.
-- Instead of passing a lot of individual signals, or few vectors 
-- (containing colors, hcount, vcount) I will be able to pass 
-- a single "vga_graphics" (I hope)

-- About records: https://www.fpgapro.com/blog/using-records-in-vhdl/

package vga_graphics is
	type vga_graphics is record

		-- Nandland Go Board has 3 signals per color
		-- Basys3 board has 4 signals per color (so it's set to 12 bits)
		rgb : std_logic_vector (11 downto 0); 
		hcount : unsigned (10 downto 0);
		vcount : unsigned (10 downto 0);
		blank : std_logic;

		-- these are not necessary but I added them to make other
		-- code simpler (at expense of having additional wires)
		line_clk : std_logic;	
		frame_clk : std_logic; 
		pixel_clk : std_logic;
	end record;
end package;