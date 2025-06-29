
library IEEE;
use IEEE.STD_LOGIC_1164.ALL;
use ieee.numeric_std.all;

entity firhp3 is
   port(clk,r,en :in std_logic;
             xin : in  signed(11 downto 0);
            yout1 : out signed(15 downto 0);
            yout2 : out signed(15 downto 0));
end firhp3;

architecture behavioral of firhp3 is

--component PWM is
--     generic(n:integer:=48);
--	   port(clk,sck,reset               :in std_logic;
--	        periodin,pwmin_a,pwmin_b:in signed(n-1 downto 0);
--                Str                     :in std_logic;
--                flag                    :buffer std_logic;
--	        outa,outb                 :out std_logic);
--end component;

  type impulse is array(0 to 62) of signed(11 downto 0);
  constant coeffs:impulse:=
    (b"000000000010",b"000000000010",b"000000000010",b"000000000010",b"000000000011",b"000000000011",b"000000000011",b"000000000100",
b"000000000100",b"000000000011",b"000000000010",b"000000000001",b"111111111110",b"111111111011",b"111111110111",b"111111110011",b"111111101110",
b"111111101000",b"111111100001",b"111111011001",b"111111010010",b"111111001001",b"111111000001",b"111110111000",b"111110110000",b"111110101001",
b"111110100010",b"111110011100",b"111110010111",b"111110010011",b"111110010001",b"011111111111",b"111110010001",b"111110010011",b"111110010111",
b"111110011100",b"111110100010",b"111110101001",b"111110110000",b"111110111000",b"111111000001",b"111111001001",b"111111010010",b"111111011001",
b"111111100001",b"111111101000",b"111111101110",b"111111110011",b"111111110111",b"111111111011",b"111111111110",b"000000000001",b"000000000010",
b"000000000011",b"000000000100",b"000000000100",b"000000000011",b"000000000011",b"000000000011",b"000000000010",b"000000000010",b"000000000010",
b"000000000010");--------------------------------------| 
  
   type siginput is array(0 to 62) of signed(11 downto 0);
   signal xinp    : siginput:=(others=>"000000000000");
   signal xbuff           :signed(11 downto 0):=(others=>'0');
   signal ycon            :signed(23 downto 0):=(others=>'0');--Accumulator
   signal yconbuf         :signed(23 downto 0):=(others=>'0');
   signal yscale1,yscale2 :signed(47 downto 0):=(others=>'0'); --Downsize Accumulator
  -- signal period          :signed(47 downto 0):=(others=>'0'); --pwm period

begin

xbuff<=(xin-200);
yout1(15)<=yscale1(15);yout1(14)<=yscale1(14); yout1(13)<=yscale1(13); yout1(12)<=yscale1(12); yout1(11)<=yscale1(11); yout1(10)<=yscale1(10); yout1(9)<=yscale1(9); yout1(8)<=yscale1(8); yout1(7)<=yscale1(7);
 yout1(6)<=yscale1(6); yout1(5)<=yscale1(5); yout1(4)<=yscale1(4); yout1(3)<=yscale1(3); yout1(2)<=yscale1(2); yout1(1)<=yscale1(1); yout1(0)<=yscale1(0);
yout2(15)<=yscale2(15);yout2(14)<=yscale2(14); yout2(13)<=yscale2(13); yout2(12)<=yscale2(12); yout2(11)<=yscale2(11); yout2(10)<=yscale2(10); yout2(9)<=yscale2(9); yout2(8)<=yscale2(8); yout2(7)<=yscale2(7);
 yout2(6)<=yscale2(6); yout2(5)<=yscale2(5); yout2(4)<=yscale2(4); yout2(3)<=yscale2(3); yout2(2)<=yscale2(2); yout2(1)<=yscale2(1); yout2(0)<=yscale2(0);
process(clk,r,en) --- Input samplew array shift process---
 begin
    if r='0'then
      xinp(0)<=(others=>'0');
     else
       if en='0' then
       xinp(0)<=(others=>'0');
      else
        if rising_edge(clk) then
         xinp(0)<=xbuff; xinp(1)<=xinp(0); xinp(2)<=xinp(1); xinp(3)<=xinp(2); xinp(4)<=xinp(3); xinp(5)<=xinp(4); xinp(6)<=xinp(5); xinp(7)<=xinp(6); xinp(8)<=xinp(7);xinp(9)<=xinp(8);
         xinp(10)<=xinp(9); xinp(11)<=xinp(10); xinp(12)<=xinp(11); xinp(13)<=xinp(12); xinp(14)<=xinp(13); xinp(15)<=xinp(14); xinp(16)<=xinp(15); xinp(17)<=xinp(16); xinp(18)<=xinp(17); xinp(19)<=xinp(18);
         xinp(20)<=xinp(19); xinp(21)<=xinp(20); xinp(22)<=xinp(21); xinp(23)<=xinp(22); xinp(24)<=xinp(23); xinp(25)<=xinp(24); xinp(26)<=xinp(25); xinp(27)<=xinp(26); xinp(28)<=xinp(27); xinp(29)<=xinp(28);
         xinp(30)<=xinp(29); xinp(31)<=xinp(30); xinp(32)<=xinp(31); xinp(33)<=xinp(32); xinp(34)<=xinp(33); xinp(35)<=xinp(34); xinp(36)<=xinp(35); xinp(37)<=xinp(36); xinp(38)<=xinp(37); xinp(39)<=xinp(38);
         xinp(40)<=xinp(39); xinp(41)<=xinp(40); xinp(42)<=xinp(41); xinp(43)<=xinp(42); xinp(44)<=xinp(43); xinp(45)<=xinp(44); xinp(46)<=xinp(45); xinp(47)<=xinp(46); xinp(48)<=xinp(47); xinp(49)<=xinp(48);
         xinp(50)<=xinp(49); xinp(51)<=xinp(50); xinp(52)<=xinp(51); xinp(53)<=xinp(52); xinp(54)<=xinp(53); xinp(55)<=xinp(54); xinp(56)<=xinp(55); xinp(57)<=xinp(56); xinp(58)<=xinp(57); xinp(59)<=xinp(58);
         xinp(60)<=xinp(59); xinp(61)<=xinp(60); xinp(62)<=xinp(61);

     end if;
     end if;
    end if;
end process;

    process(clk,r,en)---Convolution Process- 
       begin
        if r='0' then
           ycon<=(others=>'0');
        else 
          if en='0' then
            ycon<=(others=>'0');
          else
          if rising_edge(clk) then
           ycon<= ((xinp(0)*coeffs(0))+(xinp(1)*coeffs(1))+(xinp(2)*coeffs(2))+(xinp(3)*coeffs(3))+(xinp(4)*coeffs(4))+(xinp(5)*coeffs(5))+(xinp(6)*coeffs(6))+(xinp(7)*coeffs(7))+(xinp(8)*coeffs(8))+(xinp(9)*coeffs(9))+
                    (xinp(10)*coeffs(10))+(xinp(11)*coeffs(11))+(xinp(12)*coeffs(12))+(xinp(13)*coeffs(13))+(xinp(14)*coeffs(14))+(xinp(15)*coeffs(15))+(xinp(16)*coeffs(16))+(xinp(17)*coeffs(17))+(xinp(18)*coeffs(18))+(xinp(19)*coeffs(19))+
                    (xinp(20)*coeffs(20))+(xinp(21)*coeffs(21))+(xinp(22)*coeffs(22))+(xinp(23)*coeffs(23))+(xinp(24)*coeffs(24))+(xinp(25)*coeffs(25))+(xinp(26)*coeffs(26))+(xinp(27)*coeffs(27))+(xinp(28)*coeffs(28))+(xinp(29)*coeffs(29))+
                     (xinp(30)*coeffs(30))+(xinp(31)*coeffs(31))+(xinp(32)*coeffs(32))+(xinp(33)*coeffs(33))+(xinp(34)*coeffs(34))+(xinp(35)*coeffs(35))+(xinp(36)*coeffs(36))+(xinp(37)*coeffs(37))+(xinp(38)*coeffs(38))+(xinp(39)*coeffs(39))+
                     (xinp(40)*coeffs(40))+(xinp(41)*coeffs(41))+(xinp(42)*coeffs(42))+(xinp(43)*coeffs(43))+(xinp(44)*coeffs(44))+(xinp(45)*coeffs(45))+(xinp(46)*coeffs(46))+(xinp(47)*coeffs(47))+(xinp(48)*coeffs(48))+(xinp(49)*coeffs(49))+
                    (xinp(50)*coeffs(50))+(xinp(51)*coeffs(51))+(xinp(52)*coeffs(52))+(xinp(53)*coeffs(53))+(xinp(54)*coeffs(54))+(xinp(55)*coeffs(55))+(xinp(56)*coeffs(56))+(xinp(57)*coeffs(57))+(xinp(58)*coeffs(58))+(xinp(59)*coeffs(59))+
                    (xinp(60)*coeffs(60))+(xinp(61)*coeffs(61))+(xinp(62)*coeffs(62)));
             else
           ycon<=ycon;
          end if;
       end if;
      end if;     
     end process;




  process(clk,r,en)---seperation process

    begin
      if r='1' then
         if en='1'then
            if rising_edge(clk) then    
             if ycon>=signed'("0000000000000000000000000") then
                    yscale1<=((ycon*12)/24);
                    yscale2<=signed'("000000000000000000000000000000000000000000000000");
             elsif ycon<signed'("0000000000000000000000000") then         
                yconbuf<= (not ycon);
                yscale2<=((yconbuf*12)/24);
                yscale1<=signed'("000000000000000000000000000000000000000000000000");
            end if;
          end if;
        end if;
       end if;
 end process;

  --  period<="";
  --  pwmcomponent: PWM generic map(n=>n)
  --	      port map(clk,r,period,yscale1,scale2,St,f,yout1,yout2);

end behavioral;   