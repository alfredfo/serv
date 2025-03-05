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

      printf("testing jalr\n");
      uint32_t jalr_instr = 0x0000006F;// 0x00008067;
      top->wb_rdt = jalr_instr >> 2;
      top->wb_en = 1;
      cycle(top);
      printf("jal_or_jalr: %d\n", top->jal_or_jalr);
      printf("mret: %d\n", top->mret);
      printf("ebreak %d\n", top->ebreak);	    
      cycle(top);
      printf("\n");

      printf("testing mret\n");
      uint32_t mret_instr = 0x30200073;
      top->wb_rdt = mret_instr >> 2;
      top->wb_en = 1;
      cycle(top);
      printf("jal_or_jalr: %d\n", top->jal_or_jalr);
      printf("mret: %d\n", top->mret);
      printf("ebreak %d\n", top->ebreak);	    
      cycle(top);
      printf("\n");


      printf("testing ebreak\n");
      uint32_t ebreak_instr = 0x00100073;
      top->wb_rdt = ebreak_instr >> 2;
      top->wb_en = 1;
      cycle(top);
      printf("jal_or_jalr: %d\n", top->jal_or_jalr);
      printf("mret: %d\n", top->mret);
      printf("ebreak %d\n", top->ebreak);	    
      cycle(top);
      printf("\n");
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
