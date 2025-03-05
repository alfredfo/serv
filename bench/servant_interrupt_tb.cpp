#include <fcntl.h>
#include <stdint.h>
#include <signal.h>

#include "verilated_vcd_c.h"
#include "Vservant_interrupt_sim.h"

#include <ctime>

using namespace std;

static bool done;

#define TIMER_CLK_DIV 3

vluint64_t main_time = 0;       // Current simulation time
// This is a 64-bit integer to reduce wrap over issues and
// allow modulus.  You can also use a double, if you wish.

double sc_time_stamp () {       // Called by $time in Verilog
  return main_time;           // converts to double, to match
  // what SystemC does
}

void INThandler(int signal) {
  printf("\nCaught ctrl-c\n");
  done = true;
}

int reset(Vservant_interrupt_sim *, VerilatedVcd *)
int timer_test(Vservant_interrupt_sim *, VerilatedVcdC *, vluint64_t);
int external_test(Vservant_interrupt_sim *, VerilatedVcdC *, vluint64_t);

int main(int argc, char **argv, char **env) {
  Verilated::commandArgs(argc, argv);

  Vservant_interrupt_sim* top = new Vservant_interrupt_sim;

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

  top->wb_clk = 1;
  top->timer_clk = 1;
  bool q = top->q;
  int clock = 0;
  timer_test(top, tfp, timeout);
  reset(top, tfp);
  timer_test(top, tfp, timeout);
  if (tfp) {
    tfp->close();
  }
  exit(0);
}


int reset(Vservant_interrupt_sim *top, VerilatedVcd *tfp) {
    top->wb_rst = 1;
    for (int i = 0; i < 10; i++) {
        top->wb_clk = !top->wb_clk;
        top->eval();
        if (tfp) {
            tfp->dump(main_time);
        }
    }
    top->wb_rst = 0;
    return 0;
}

int external_test(Vservant_interrupt_sim *top, VerilatedVcdC *tfp, vluint64_t timeout) {
  int clock = 0;
}

int timer_test(Vservant_interrupt_sim *top, VerilatedVcdC *tfp, vluint64_t timeout) {
  int clock = 0;
  while (!(done || Verilated::gotFinish())) {
    clock++;
    top->wb_rst = main_time < 1000;
    top->eval();
    if (tfp) {
      tfp->dump(main_time);
    }
    if (top->wb_clk && top->pc_vld && top->pc_adr) {
      std::this_thread::sleep_for(std::chrono::milliseconds(500));
      printf("%d | %d\n", clock, top->pc_adr);
    }
    if (timeout && (main_time >= timeout)) {
      printf("Timeout: Exiting at time %lu\n", main_time);
      done = true;
    }
    top->wb_clk = !top->wb_clk;
    top->timer_clk = ((clock % TIMER_CLK_DIV) <= (TIMER_CLK_DIV / 2));

    main_time+=31.25;
  }
}
