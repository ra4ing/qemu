#include "qemu/osdep.h"
#include "exec/exec-all.h"
#include "exec/helper-proto.h"
#include "qemu/log.h"

void helper_write_instruction_count(CPUArchState *cpu, target_ulong rs2) {

    // cpu->sreg[0] = 5;
    // qemu_log("####################################################################################################################################\n\n");

    // for (int i = 0; i < 32; i += 8) {
    //     qemu_log("x%-2d= 0x%-9x x%-2d= 0x%-9x x%-2d= 0x%-9x x%-2d= 0x%-9x x%-2d= 0x%-9x x%-2d= 0x%-9x x%-2d= 0x%-9x x%-2d= 0x%-9x\n",
    //              i, (unsigned int)cpu->gpr[i], 
    //              i+1, (unsigned int)cpu->gpr[i+1],
    //              i+2, (unsigned int)cpu->gpr[i+2], 
    //              i+3, (unsigned int)cpu->gpr[i+3],
    //              i+4, (unsigned int)cpu->gpr[i+4], 
    //              i+5, (unsigned int)cpu->gpr[i+5],
    //              i+6, (unsigned int)cpu->gpr[i+6], 
    //              i+7, (unsigned int)cpu->gpr[i+7]);
    // }
    qemu_log("ic = %-9d ica = 0x%-9x pc = 0x%-9x\n", (unsigned int)cpu->sreg[3], (unsigned int)cpu->sreg[2], (unsigned int)cpu->pc);

}

target_ulong helper_read_instruction_count(CPUArchState *cpu) {
  return cpu->gpr[31];
}


