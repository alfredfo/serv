`default_nettype none
module servant_sleep_dummy
  (
   input wire  i_clk,
   input wire  i_rst,
   input wire  ext_irq,
   output wire q);

   parameter   memfile = "zephyr_hello.hex";
   parameter   memsize = 8192;
   parameter   reset_strategy = "MINI";
   parameter   width = 1;
   parameter   sim = 0;
   parameter [0:0] debug = 1'b0;
   parameter       with_csr = 1;
   parameter [0:0] compress = 0;
   parameter [0:0] align = compress;

   wire            wb_clk;
   wire            sleep_req;
   wire            wakeup_req;


   reg         sleep_req_prev;
   wire        sleep_pulse;
   reg         sleep;

      always @(posedge i_clk) begin
      if (i_rst) begin
         sleep_req_prev <= 1'b0;
         sleep <= 1'b0;
      end else begin
         sleep_req_prev <= sleep_req;
         if (sleep_req & ~sleep_req_prev & ~wakeup_req)
            sleep <= 1'b1;
         if (wakeup_req)
            sleep <= 1'b0;
      end
   end


   assign sleep_pulse = sleep_req & ~sleep_req_prev;

   assign wb_clk = (i_clk & (!sleep));

   servant #(
             .memfile(memfile),
             .memsize(memsize),
             .reset_strategy(reset_strategy),
             .width(width),
             .sim(sim),
             .debug(debug),
             .with_csr(with_csr),
             .compress(compress),
             .align(align)
             )
   servant (
            .wb_clk (wb_clk),
            .timer_clk (i_clk),
            .wb_rst (i_rst),
            .q      (q),
            .o_sleep_req  (sleep_req),
            .o_wakeup_req (wakeup_req));

endmodule
`default_nettype wire
