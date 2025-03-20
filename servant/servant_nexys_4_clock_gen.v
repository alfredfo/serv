`default_nettype none
module servant_nexys_4_clock_gen
  (input wire  i_clk,
   input wire  i_rst,
   output wire o_clk,
   output reg  o_rst);

   wire   clkfb;
   wire   locked;
   wire   pll_rst;
   reg 	  locked_r;

   assign pll_rst = !i_rst;

   MMCME2_BASE
     #(.CLKIN1_PERIOD   (10), //100MHz

       /* Set VCO frequency to 100*8.0=800 MHz
	Allowed values are 2.0 to 64.0. Resulting VCO freq
	needs to be 600-1200MHz */
       .CLKFBOUT_MULT_F (8.000),

       .CLKOUT0_DIVIDE_F (50.000)) // 800/50 = 16 MHz
   pll
     (.CLKIN1   (i_clk),
      .RST      (pll_rst),
      .CLKOUT0  (o_clk),
      .LOCKED   (locked),
      .CLKFBOUT (clkfb),
      .CLKFBIN  (clkfb));

   always @(posedge o_clk) begin
      locked_r <= locked;
      o_rst  <= !locked_r;
   end

endmodule
`default_nettype wire
