#include "qemu/osdep.h"
#include "exec/exec-all.h"
#include "exec/helper-proto.h"
#include "qemu/log.h"

void helper_write_instruction_count(CPUArchState *cpu, target_ulong rs2) {
    for (int i = 0; i < 32; i++) {
        qemu_log("x%d = %lu\n", i, (unsigned long)cpu->gpr[i]);
    }
    qemu_log("%u\n", (unsigned int)cpu->gpr[rs2]);
    cpu->ic = cpu->gpr[rs2];
}

target_ulong helper_read_instruction_count(CPUArchState *cpu) {
  return cpu->ic;
}


