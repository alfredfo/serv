#include <fcntl.h>
#include <stdint.h>
#include <signal.h>

#include "verilated_vcd_c.h"
#include "Vdecoder_sim.h"

#include <ctime>

using namespace std;

static bool done;

vluint64_t main_time = 0;

void INThandler(int signal) {
  printf("\nCaught ctrl-c\n");
  done = true;
}

void test_instruction(Vdecoder_sim *, const char *, uint32_t);
void cycle(Vdecoder_sim *);

int main(int argc, char **argv, char **env) {
  Verilated::commandArgs(argc, argv);

  Vdecoder_sim* top = new Vdecoder_sim;

  VerilatedVcdC * tfp = 0;
  const char *vcd = Verilated::commandArgsPlusMatch("vcd=");
  if (vcd[0]) {
    Verilated::traceEverOn(true);
    tfp = new VerilatedVcdC;
    top->trace (tfp, 99);
    tfp->open ("trace.vcd");
  }

  signal(SIGINT, INThandler);

  vluint64_t timeout = 0;
  const char *arg_timeout = Verilated::commandArgsPlusMatch("timeout=");
  if (arg_timeout[0])
    timeout = atoi(arg_timeout+9);

  vluint64_t vcd_start = 0;
  const char *arg_vcd_start = Verilated::commandArgsPlusMatch("vcd_start=");
  if (arg_vcd_start[0])
    vcd_start = atoi(arg_vcd_start+11);

  int cur_cycle = 0;
  int last_cycle = 0;
  std::time_t last_time = std::time(nullptr);

  top->wb_clk = 0;
  top->wb_rdt = 0;
  top->wb_en = 0;

  printf("testing decoder\n");
  int clock = 0;
  while (!(done || Verilated::gotFinish())) {
    clock++;
    top->eval();
    if (tfp) {
      tfp->dump(main_time);
    }
    if (top->wb_clk) {
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      test_instruction(top, "jalr", 0x0000006F);
      test_instruction(top, "mret", 0x30200073);
      test_instruction(top, "ebreak", 0x00100073);
      test_instruction(top, "wfi", 0x10500073);
    }
    if (timeout && (main_time >= timeout)) {
      printf("Timeout: Exiting at time %lu\n", main_time);
      done = true;
    }
    top->wb_clk = !top->wb_clk;
    main_time+=31.25;
  }

  
  if (tfp) {
    tfp->close();
  }
  exit(0);
}

void cycle(Vdecoder_sim *top) {
  top->eval();
  top->wb_clk = 0;
  top->eval();
  top->wb_clk = 1;
  top->eval();
}

void test_instruction(Vdecoder_sim *top, const char *instr_name, uint32_t instr) {
  printf("testing %s\n", instr_name);
  top->wb_rdt = instr >> 2;
  top->wb_en = 1;
  cycle(top);
  printf("jal_or_jalr: %d\n", top->jal_or_jalr);
  printf("ebreak: %d\n", top->ebreak);
  printf("mret: %d\n", top->mret);
  printf("wfi: %d\n", top->wfi);
  printf("sh_right: %d\n", top->sh_right);
  printf("bne_or_bge: %d\n", top->bne_or_bge);
  printf("cond_branch: %d\n", top->cond_branch);
  printf("e_op: %d\n", top->e_op);
  printf("branch_op: %d\n", top->branch_op);
  printf("shift_op: %d\n", top->shift_op);
  printf("slt_or_branch: %d\n", top->slt_or_branch);
  printf("rd_op: %d\n", top->rd_op);
  printf("two_stage_op: %d\n", top->two_stage_op);
  printf("dbus_en: %d\n", top->dbus_en);
  printf("mdu_op: %d\n", top->mdu_op);
  printf("ext_funct3: %d\n", top->ext_funct3);
  printf("bufreg_rs1_en: %d\n", top->bufreg_rs1_en);
  printf("bufreg_imm_en: %d\n", top->bufreg_imm_en);
  printf("bufreg_clr_lsb: %d\n", top->bufreg_clr_lsb);
  printf("bufreg_sh_signed: %d\n", top->bufreg_sh_signed);
  printf("ctrl_utype: %d\n", top->ctrl_utype);
  printf("ctrl_pc_rel: %d\n", top->ctrl_pc_rel);
  printf("alu_sub: %d\n", top->alu_sub);
  printf("alu_bool_op: %d\n", top->alu_bool_op);
  printf("alu_cmp_eq: %d\n", top->alu_cmp_eq);
  printf("alu_cmp_sig: %d\n", top->alu_cmp_sig);
  printf("alu_rd_sel: %d\n", top->alu_rd_sel);
  printf("mem_signed: %d\n", top->mem_signed);
  printf("mem_word: %d\n", top->mem_word);
  printf("mem_half: %d\n", top->mem_half);
  printf("mem_cmd: %d\n", top->mem_cmd);
  printf("csr_en: %d\n", top->csr_en);
  printf("csr_addr: %d\n", top->csr_addr);
  printf("csr_mstatus_en: %d\n", top->csr_mstatus_en);
  printf("csr_mie_en: %d\n", top->csr_mie_en);
  printf("csr_mcause_en: %d\n", top->csr_mcause_en);
  printf("csr_source: %d\n", top->csr_source);
  printf("csr_d_sel: %d\n", top->csr_d_sel);
  printf("csr_imm_en: %d\n", top->csr_imm_en);
  printf("mtval_pc: %d\n", top->mtval_pc);
  printf("immdec_ctrl: %d\n", top->immdec_ctrl);
  printf("immdec_en: %d\n", top->immdec_en);
  printf("op_b_source: %d\n", top->op_b_source);
  printf("rd_mem_en: %d\n", top->rd_mem_en);
  printf("rd_csr_en: %d\n", top->rd_csr_en);
  printf("rd_alu_en: %d\n", top->rd_alu_en);
  top->wb_en = 0;
  cycle(top);
  printf("\n");
}
