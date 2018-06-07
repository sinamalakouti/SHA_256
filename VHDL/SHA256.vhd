library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;
use work.types.all;
use work.constants.all;
use work.Utils.all;

entity SHA256 is
end SHA256;

architecture Behavioral of SHA256 is 
    signal x : std_logic_vector (31 downto 0) := "01100001011000100110001101100100";
    signal res : std_logic_vector (255 downto 0); 
    begin
        res <= sha(x);
end architecture;