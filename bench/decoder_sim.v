`default_nettype none
module decoder_sim
  (input wire        wb_clk,
   input wire [31:2] wb_rdt,
   input wire	     wb_en,
   output wire	     ebreak,
   output wire	     jal_or_jalr,
   output wire	     mret,
   output wire	     wfi
);

   // Instantiate the DUT
   serv_decode
     #(.PRE_REGISTER (1),
       .MDU (0))
   dut (
       .clk(wb_clk),
       .i_wb_rdt(wb_rdt),
       .i_wb_en(wb_en),
       // Outputs
       .o_sh_right(),
       .o_bne_or_bge(),
       .o_cond_branch(),
       .o_e_op(),
       .o_ebreak(),
       .o_branch_op(),
       .o_shift_op(),
       .o_slt_or_branch(),
       .o_rd_op(),
       .o_two_stage_op(),
       .o_dbus_en(),
       .o_mdu_op(),
       .o_ext_funct3(),
       .o_bufreg_rs1_en(),
       .o_bufreg_imm_en(),
       .o_bufreg_clr_lsb(),
       .o_bufreg_sh_signed(),
       .o_ctrl_jal_or_jalr(),
       .o_ctrl_utype(),
       .o_ctrl_pc_rel(),
       .o_ctrl_mret(),
       .o_alu_sub(),
       .o_alu_bool_op(),
       .o_alu_cmp_eq(),
       .o_alu_cmp_sig(),
       .o_alu_rd_sel(),
       .o_mem_signed(),
       .o_mem_word(),
       .o_mem_half(),
       .o_mem_cmd(),
       .o_csr_en(),
       .o_csr_addr(),
       .o_csr_mstatus_en(),
       .o_csr_mie_en(),
       .o_csr_mcause_en(),
       .o_csr_source(),
       .o_csr_d_sel(),
       .o_csr_imm_en(),
       .o_mtval_pc(),
       .o_immdec_ctrl(),
       .o_immdec_en(),
       .o_op_b_source(),
       .o_rd_mem_en(),
       .o_rd_csr_en(),
       .o_rd_alu_en()
   );

   assign jal_or_jalr = dut.o_ctrl_jal_or_jalr;
   assign ebreak = dut.o_ebreak;
   assign mret = dut.o_ctrl_mret;
   assign wfi = dut.o_wfi;

endmodule
