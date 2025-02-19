library ieee;
use ieee.std_logic_1164.all;
use ieee.numeric_std.all;
use ieee.math_real.all;	-- Needed for the `ceil()` function

-- ===============================================
--  For synchronisation, pixels are ordered as
--    Visible, Front, H/V Pulse, Back, [-repeat-]
-- ===============================================
entity Image_generator is
	Generic (
		-- ===============================
		--  Horizontal timing (in pixels)
		-- ===============================
		H_VISIBLE	: natural := 640;
		H_FRONT		: natural := 16;
		H_PULSE		: natural := 64;
		H_BACK		: natural := 120;

		-- =============================
		--  Vertical timing (in pixels)
		-- =============================
		V_VISIBLE	: natural := 480;
		V_FRONT		: natural := 1;
		V_PULSE		: natural := 3;
		V_BACK		: natural := 16);
		
    Port (
		CLK		: in  std_logic;
		RESETN	: in  std_logic;

		-- ========
		--  Colors
		-- ========
		RED		: out std_logic_vector(4 downto 0);
		GREEN	: out std_logic_vector(5 downto 0);
		BLUE	: out std_logic_vector(4 downto 0);

		-- =======================================
		--  Data Enable (signals visible pixels)
		--  Unrelated to VGA. Required by HDMI.
		--  The HDMI transmitter can generate it.
		-- =======================================
	    DE		: out std_logic;

		-- =======================================================
		--  Synchronization signals (polarization is unnecessary)
		-- =======================================================
		HSYNC	: out std_logic;
		VSYNC	: out std_logic);
end Image_generator;

architecture arch of Image_generator is
	-- ====================================
	--  Calculate required bits for number
	-- ====================================
	function bit_width (num: natural) return natural is
	begin
		if num = 0 then
			return 1;
		else
			return integer(ceil(log2(real(num + 1))));
		end if;
	end function bit_width;

	-- =========
	--  Aliases
	-- =========
	constant H_TOTAL		: natural := H_VISIBLE + H_FRONT + H_PULSE + H_BACK;
	constant V_TOTAL		: natural := V_VISIBLE + V_FRONT + V_PULSE + V_BACK;
	-- ==
	constant HSYNC_START	: natural := H_VISIBLE + H_FRONT;
	constant HSYNC_STOP		: natural := H_VISIBLE + H_FRONT + H_PULSE - 1;
	-- ==
	constant VSYNC_START	: natural := V_VISIBLE + V_FRONT;
	constant VSYNC_STOP		: natural := V_VISIBLE + V_FRONT + V_PULSE - 1;

	-- ===========
	--  Variables
	-- ===========
	signal h_pos	: natural range 0 to 2**bit_width(H_TOTAL) - 1;
	signal v_pos	: natural range 0 to 2**bit_width(V_TOTAL) - 1;
begin
	HSYNC	<= '1' when
		h_pos >= HSYNC_START and
		h_pos <= HSYNC_STOP else '0';
	VSYNC	<= '1' when
		v_pos >= VSYNC_START and
		v_pos <= VSYNC_STOP else '0';

	-- ===============================================
	--  (Pixel ordering: Visible, Front, Pulse, Back)
	-- ===============================================
	DE		<= '1' when h_pos < H_VISIBLE and v_pos < V_VISIBLE else '0';

	-- ========
	--  Output
	-- ========
	RED		<= std_logic_vector(to_unsigned(16#19#, RED'length));
	GREEN	<= std_logic_vector(to_unsigned(16#19#, GREEN'length));
	BLUE	<= std_logic_vector(to_unsigned(16#70#, BLUE'length));

	-- ================
	--  Advance line
	--  Advance column
	-- ================
	process (CLK, RESETN) is
	begin
		if rising_edge(CLK) then
			-- =======
			--  Reset
			-- =======
			if RESETN = '0' then
				h_pos	<= 0;
				v_pos	<= 0;
			else
				if h_pos >= H_TOTAL - 1 then
					h_pos	<= 0;
					-- ===============
					--  Restart frame
					-- ===============
					if v_pos >= V_TOTAL - 1 then
						v_pos	<= 0;
					-- ======================
					--  Advance to next line
					-- ======================
					else
						v_pos	<= v_pos + 1;
					end if;
				-- ========================
				--  Advance to next column
				-- ========================
				else
					h_pos	<= h_pos + 1;
				end if;
			end if;
		end if;
	end process;
end arch;
