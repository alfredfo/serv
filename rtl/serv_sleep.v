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
    input wire      i_wfi,
    input wire      i_cnt_done,

    output wire     o_sleep_req,
    output wire     o_wakeup_req);
    

    assign o_sleep_req = i_wfi & i_cnt_done;
    assign o_wakeup_req = i_timer_irq | i_external_irq;
    

endmodule
