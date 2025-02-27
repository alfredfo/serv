`default_nettype none
module serv_sleep
  #(
    parameter RESET_STRATEGY = "MINI"
  )
  (
    input wire      i_clk,
    input wire      i_rst,
    input wire 	    i_timer_irq,
    input wire      i_external_irq,
    input wire      i_sleep_request,

    output wire     o_clk_halt);


// Alternative version:
//    assign o_clk_halt = i_sleep_request & !(i_timer_irq | i_external_irq);   


    reg clk_halt;

    assign o_clk_halt = clk_halt & !(i_timer_irq | i_external_irq);

    always @(posedge i_clk) begin
      clk_halt <= i_sleep_request;

      if (i_rst)
      if (RESET_STRATEGY != "NONE") begin
        clk_halt <= 0;
      end
    end


endmodule
