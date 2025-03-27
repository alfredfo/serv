`default_nettype none
module servant_sleep_dummy
  (
   input wire  i_clk,
   input wire  i_rst,
   input wire  ext_irq,
   output wire q);

   parameter   memfile = "zephyr_hello.hex";
   parameter   memsize = 8192;

   wire	       main_clk;
   wire	       wb_clk;
   wire	       wb_rst;

   wire	       sleep_req;
   wire	       wakeup_req;

   reg	       sleep;

   always @(posedge main_clk) begin
      if (sleep_req)
        sleep <= 0;
      if (wakeup_req)
        sleep <= 1;
      if (wb_rst)
        sleep <= 1;
   end

   assign wb_clk = (i_clk & (!sleep));

   servant
     #(.memfile (memfile),
       .memsize (memsize))
   servant
     (.wb_clk (wb_clk),
      .wb_rst (wb_rst),
      .q      (q),
      .o_sleep_req  (sleep_req),
      .o_wakeup_req (wakeup_req));

endmodule
`default_nettype wire
