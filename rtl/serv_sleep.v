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
    input wire      i_cnt_done,

    output wire     o_clk_halt);
    
    wire irq;

    reg sleep_pending;
    reg sleep_set;
    reg sleep_reset;

    assign irq = i_timer_irq | i_external_irq;
    assign o_clk_halt = sleep_set & !sleep_reset;


    always @(negedge i_clk) begin
      sleep_set <= sleep_pending;
    end



    always @(posedge (i_clk | irq | i_rst)) begin
      sleep_pending <= i_sleep_request & i_cnt_done;

      sleep_reset <= irq;

      if (i_rst)
      if (RESET_STRATEGY != "NONE") begin
        sleep_reset <= 1;
      end

    end

endmodule
