`default_nettype none
module servant_timer_sim
  (input wire         wb_clk,
   input wire         wb_rst,
   input wire         timer_clk,
   output wire [31:0] pc_adr,
   output wire        pc_vld,
   output wire        q,
   output wire        timer_irq
   );

   parameter          memfile = "";
   parameter          memsize = 8192;
   parameter          width = 1;
   parameter          with_csr = 1;
   parameter          compressed = 0;
   parameter          align = compressed;

   wire               sleep;
   wire               ext_irq;

   reg [1023:0]       firmware_file;
   initial
     if ($value$plusargs("firmware=%s", firmware_file)) begin
        $display("Loading RAM from %0s", firmware_file);
        $readmemh(firmware_file, dut.servant.ram.mem);
     end

   servant_sleep_dummy
     #(.memfile  (memfile),
       .memsize  (memsize),
       .width    (width),
       .debug    (1'b1),
       .sim      (1),
       .with_csr (with_csr),
       .compress (compressed[0:0]),
       .align    (align[0:0]))
   dut(wb_clk, wb_rst, ext_irq, q);

   assign pc_adr = dut.servant.wb_mem_adr;
   assign pc_vld = dut.servant.wb_mem_ack;
   assign timer_irq = dut.servant.timer_irq;
endmodule
