library ieee;
use ieee.std_logic_1164.all;
use ieee.std_logic_unsigned.all;
use ieee.numeric_std.all;
use work.types.all;
use work.constants.all;

package Utils is

  function sha (str : std_logic_vector) return std_logic_vector;

  function padding(inp : std_logic_vector) return std_logic_vector;

  function parsing(inp : std_logic_vector) return blocks;

  function compression (inp : blocks) return std_logic_vector;

  function expansion(inp : std_logic_vector) return W;

  function permutation(inp : std_logic_vector(31 downto 0)) return std_logic_vector;

  function multiply(inp :std_logic_vector; int : INTEGER) return std_logic_vector;

  function sigma0(x : std_logic_vector) return std_logic_vector;

  function sigma1(x : std_logic_vector) return std_logic_vector;

  function bigSigma0(x : std_logic_vector) return std_logic_vector;

  function bigSigma1(x : std_logic_vector) return std_logic_vector;

  function bigSigma2(x : std_logic_vector) return std_logic_vector;

  function ch(x, y, z : std_logic_vector) return std_logic_vector;

  function maj(x, y, z :std_logic_vector) return std_logic_vector;

  function concatVectors(inp : std_logic_vector;
                         int : integer) return std_logic_vector;

  function reverse_any_vector (a: in std_logic_vector) return std_logic_vector;

end package;

package body Utils is
--sha
    function sha (str : std_logic_vector) return std_logic_vector is
      begin
        return compression(parsing(padding(str)));
    end function;
--padding
    function padding(inp : std_logic_vector) return std_logic_vector is
        variable temp : std_logic_vector (inp'length downto 0);
        variable zeroCount : integer;
        variable inpSize : integer := inp'length;
        begin
            temp(0) := '1';
            temp (temp'length - 1 downto 1) := inp;
            zC : for i in 0 to 200 loop
                if ((512 * i) + 448 - temp'length > 0) then
                    zeroCount := (512 * i) + 448 - temp'length;
                    exit;
                end if;
            end loop;
            return concatVectors(temp,zeroCount);
    end function padding;
--parsing
     function parsing(inp : std_logic_vector) return blocks is
         variable numberOfBlocks : INTEGER;
         variable temp : std_logic_vector(511 downto 0);
         variable outp : blocks ((inp'length / 512) - 1 downto 0);
         begin
            numberOfBlocks := inp'length / 512;
            parse : for i in 1 to ((inp'length / 512)) loop
              temp := inp((i * 512) - 1 downto (i - 1) * 512);
              outp(i - 1) := temp;
            end loop;
         return outp;
     end function;
--compression
   function compression (inp : blocks) return std_logic_vector is
     variable blockSize : integer := inp'length;
     variable a,b,c,d,e1,f,g,h : std_logic_vector(31 downto 0);
     variable t1,t2 : std_logic_vector(31 downto 0);
     variable p_a,p_b,p_c,p_d,p_e,p_f,p_g,p_h : std_logic_vector(31 downto 0);
     variable w : W;
     variable outp : std_logic_vector(255 downto 0);
     begin
       p_a := pre_a;
       p_b := pre_b;
       p_c := pre_c;
       p_d := pre_d;
       p_e := pre_e;
       p_f := pre_f;
       p_g := pre_g;
       p_h := pre_h;

       for_loop : for i in 0 to blockSize - 1 loop
         a := p_a;
         b := p_b;
         c := p_c;
         d := p_d;
         e1 := p_e;
         f := p_f;
         g := p_g;
         h := p_h;
         w := expansion(inp(i));

         we : for i in 0 to 63 loop
           --t2 := h + bigSigma1(e1) + ch(e1,f,g) + k_values(i) + w(i);
           --t1 := bigSigma0(a) + maj(a,b,c) + bigSigma2(c + d);
           --h := g;
           --f := e1;
           --d := c;
           --b := a;
           --g := f;
           --e1 := d + t1;
           --c := b;
           --a := multiply(t1,3) - t2;
           
           --standard :
           t1 := h + bigSigma1(e1) + ch(e1,f,g) + k_values(i) + w(i);
           t2 := bigSigma0(a) + maj(a,b,c);
           h := g;
           g := f;
           f := e1;
           e1 := d + t1;
           d := c;
           c := b;
           b := a;
           a := t1 + t2;
         end loop;

         p_a := a + p_a;
         p_b := b + p_b;
         p_c := c + p_c;
         p_d := d + p_d;
         p_e := e1 + p_e;
         p_f := f + p_f;
         p_g := g + p_g;
         p_h := h + p_h;
       end loop;
       a := p_a;
       b := p_b;
       c := p_c;
       d := p_d;
       e1 := p_e;
       f := p_f;
       g := p_g;
       h := p_h;

       outp := a & b & c & d & e1 & f & g & h;
       return outp;
   end function;
-- expansion
   function expansion(inp : std_logic_vector) return W is
       variable temp : std_logic_vector (31 downto 0);
       variable tempOut : W;
       variable outp : W;
       begin
           for i in 0 to 15 loop
               temp := inp(512 - (i * 32) - 1 downto 512 - ((i + 1) * 32));
               tempOut(i) := temp;
           end loop;
           for i in 16 to 63 loop
               --temp := sigma1(tempOut(i - 1)) + tempOut(i - 6) + sigma0(tempOut(i - 12)) + tempOut(i - 15);
               --standard :
               temp := sigma1(tempOut(i - 2)) + tempOut(i - 7) + sigma0(tempOut(i - 15)) + tempOut(i - 16);
               
               tempOut(i) := temp;
           end loop;
           --for i in 0 to 63 loop
               --outp(i) := permutation(tempOut(i));
           --end loop;
           
           --standard:
           for i in 0 to 63 loop
             outp(i) := tempOut(i);
          end loop;
           return outp;
   end function;
--permutation
    function permutation(inp : std_logic_vector(31 downto 0)) return std_logic_vector is
      variable Dreverse : std_logic_vector (7 downto 0);
      variable ABreverse : std_logic_vector (15 downto 0);
      variable outp : std_logic_vector (31 downto 0);
      begin
        Dreverse := reverse_any_vector(inp (7 downto 0));
        ABreverse := reverse_any_vector(inp (31 downto 16));
        outp(31 downto 24) := Dreverse;
        outp(23 downto 16) := inp (15 downto 8);
        outp(15 downto 0) := ABreverse;
        return outp;
    end function;
--multiply
     function multiply(inp :std_logic_vector; int : INTEGER) return std_logic_vector is
       variable res : std_logic_vector (31 downto 0);
       begin
         res := inp;
         multi : for i in 1 to (int - 1) loop
           res := inp + res;
         end loop;
         return res;
     end function;
--sigma0
    function sigma0 (x: std_logic_vector) return std_logic_vector is
        begin
            --return std_logic_vector(rotate_right(unsigned(x), 17) xor rotate_right(unsigned(x),14) xor shift_right(unsigned(x),12));
            --standard:
            return std_logic_vector(rotate_right(unsigned(x), 7) xor rotate_right(unsigned(x),18) xor shift_right(unsigned(x),3));
    end function sigma0;
--sigma1
    function sigma1 (x: std_logic_vector) return std_logic_vector is
        begin
            --return std_logic_vector(rotate_right(unsigned(x), 9) xor rotate_right(unsigned(x),19) xor shift_right(unsigned(x),9));
            --standard:
            return std_logic_vector(rotate_right(unsigned(x), 17) xor rotate_right(unsigned(x),19) xor shift_right(unsigned(x),10));
    end function sigma1;
--bigSigma0
    function bigSigma0 (x: std_logic_vector) return std_logic_vector is
        begin
           --return std_logic_vector(rotate_right(unsigned(x), 2) xor rotate_right(unsigned(x),13) xor rotate_right(unsigned(x),22) xor shift_right(unsigned(x),7));
           --standard:
           return std_logic_vector(rotate_right(unsigned(x), 2) xor rotate_right(unsigned(x),13) xor rotate_right(unsigned(x),22)); 
    end function bigSigma0;
--bigSigma1
    function bigSigma1 (x: std_logic_vector) return std_logic_vector is
      begin
        return std_logic_vector(rotate_right(unsigned(x), 6) xor rotate_right(unsigned(x),11) xor rotate_right(unsigned(x),25) );
    end function bigSigma1;
--bigSigma2
    function bigSigma2 (x: std_logic_vector) return std_logic_vector is
    begin
      return std_logic_vector(rotate_right(unsigned(x), 2) xor rotate_right(unsigned(x),3) xor rotate_right(unsigned(x),15) xor shift_right(unsigned(x),5));
    end function bigSigma2 ;
--ch
    function ch(x, y, z : std_logic_vector) return std_logic_vector is
      begin
        --return (x and y) xor ((not y) and z) xor ((not x) and z);
        -- standard:
        return (x and y) xor ((not x) and z);
    end function ch;
--maj
    function maj(x, y, z : std_logic_vector) return std_logic_vector is
        begin
            return (x and z) xor (x and y ) xor ( y and z);
    end function maj;
--concatVectors
    function concatVectors(inp : std_logic_vector;
                           int : integer) return std_logic_vector is
        variable zero : std_logic_vector (int - 1 downto 0);
        variable number : std_logic_vector (63 downto 0);
        variable outp : std_logic_vector (inp'length + zero'length + number'length - 1 downto 0);
        begin
            for i in 0 to zero'length - 1 loop
                zero(i) := '0';
            end loop;
            number := std_logic_vector(to_unsigned(inp'length - 1, number'length));
            outp := inp & zero & number;
            return outp;
    end function;
-- function reverse_any_vector
    function reverse_any_vector (a: in std_logic_vector) return std_logic_vector is
      variable result: std_logic_vector(a'RANGE);
      alias aa: std_logic_vector(a'REVERSE_RANGE) is a;
    begin
      for i in aa'RANGE loop
        result(i) := aa(i);
      end loop;
      return result;
    end function;
 end package body;
