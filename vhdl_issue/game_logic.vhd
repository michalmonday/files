library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use work.vga_graphics.all;

entity game_logic is
port(
	i_increase_level_clk : in std_logic;
	io_graphics : inout vga_graphics;
	rect_xpos, rect_ypos : out unsigned (2 downto 0)
	);
end entity game_logic;


architecture RTL of game_logic is
	signal game_clk : std_logic;

	signal rect_color: std_logic_vector(11 downto 0);

	type rect_grid is array (1 downto 0, 1 downto 0) of std_logic;
	signal tower_grid : rect_grid := ("00", "11");

	type level is record 
		pos : integer range -3 to 8;
		length : integer range 1 to 3;
	end record;

	type level_array is array (0 to 7) of level;

	signal levels : level_array := (
		-- pos, length
		(4,1),
		(4,1),
		(4,1),
		(4,2),
		(4,2),
		(3,3),
		(3,3),
		(3,3));

	signal current_level_index : integer range 0 to 7 := 6;
	

	-- signals to track which rectangle is currently being painted
	--signal rect_xpos, rect_ypos : unsigned (2 downto 0);

--type rect_grid :
begin

	rect_pos : entity work.rect_pos
		generic map(
			rect_width => 80,
			rect_height => 60
			)
		port map(
			i_graphics => io_graphics,
			o_xpos => rect_xpos,
			o_ypos => rect_ypos
			);

	game_clock : entity work.clock_divider
		generic map(
			divisor => 60
			)
		port map(
			i_rst => '0', 
			i_clk => io_graphics.frame_clk,
			o_clk => game_clk
			);

	--io_graphics.rgb <= (others => '0') when io_graphics.blank = '1' else					   
	--					std_logic_vector(rect_xpos & "0") & "0000" & std_logic_vector(rect_ypos & "0");

						----------------------------------------------------------"111100001111" when (io_graphics.hcount > 580 and io_graphics.vcount > 420) else
					   -----------------------------------------------------------"111100000000" when io_graphics.vcount > 420 else 
					   -----------------------------------------------------------"000000001111" when io_graphics.hcount > 580 else


    process (i_increase_level_clk)
    begin
    	if rising_edge(i_increase_level_clk) then
    		current_level_index <= current_level_index + 1;
    	end if;
    end process;

	process (game_clk)		
		variable reverse_dir : std_logic := '0';
		--variable current_level : level;
	begin
		--current_level := levels(current_level_index);
		if rising_edge(game_clk) then
			if levels(current_level_index).pos > 7 then
				reverse_dir := '1';
			elsif levels(current_level_index).pos < 1-levels(current_level_index).length then
				reverse_dir := '0';
			end if;

			if reverse_dir = '0' then
				levels(current_level_index).pos <= levels(current_level_index).pos + 1;
			else 
				levels(current_level_index).pos <= levels(current_level_index).pos - 1;
			end if;

			--for i in levels'low to levels'high loop
				
			--end loop;
		end if;
	end process;


	process (io_graphics.frame_clk, io_graphics.vcount, io_graphics.hcount)
		variable x, y: integer range 0 to 7;  -- for readability
	begin
		x := to_integer(rect_xpos);
		y := to_integer(rect_ypos);



		--if io_graphics.hcount > 500 then
		--	io_graphics.rgb(3 downto 0) <= "1111";
		--else 
		--	io_graphics.rgb(3 downto 0) <= "0000";
		--end if;

		--if io_graphics.vcount > 300 then
		--	io_graphics.rgb(11 downto 8) <= "1111";
		--else 
		--	io_graphics.rgb(11 downto 8) <= "0000";
		--end if;

		io_graphics.rgb <= (others => '0');

		-- if tower level should be painted
		if x >= levels(y).pos and x < (levels(y).pos + levels(y).length) then

		 	-- rect_xpos is used to make the adjacent rects have distinct color
			--io_graphics.rgb <= std_logic_vector(rect_xpos & "0") & std_logic_vector(rect_ypos & "0") & "1100";
			io_graphics.rgb <= "000011001111";
		elsif y = current_level_index then
			io_graphics.rgb <= "111111011101";
		end if;

		if io_graphics.blank = '1' then
			-- it's the last assignment to ensure that it has priority (over previous ones)
			-- because signals change state at the end of the process
			io_graphics.rgb <= (others => '0');
		end if;

	end process;
end architecture RTL;