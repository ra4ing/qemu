static bool trans_sjalr(DisasContext* ctx, arg_sjalr* a) {

    CPUState *cpu = ctx->cs;
    RISCVCPU *rv_cpu = RISCV_CPU(cpu);
    CPUArchState *env = &rv_cpu->env;

    /* env->ic = env->gpr[a->rs2]; */
    helper_write_instruction_count(env, a->rs2);

    // 处理跳转
    TCGv target_pc = tcg_temp_new();
    TCGv succ_pc = dest_gpr(ctx, a->rd);
    tcg_gen_mov_tl(target_pc, get_gpr(ctx, a->rs1, EXT_NONE));
    tcg_gen_andi_tl(target_pc, target_pc, (target_ulong)-2);

    if (get_xl(ctx) == MXL_RV32) {
        tcg_gen_ext32s_tl(target_pc, target_pc);
    }

    TCGLabel* misaligned = NULL;
    if (!has_ext(ctx, RVC) && !ctx->cfg_ptr->ext_zca) {
        TCGv t0 = tcg_temp_new();
        misaligned = gen_new_label();
        tcg_gen_andi_tl(t0, target_pc, 0x2);
        tcg_gen_brcondi_tl(TCG_COND_NE, t0, 0x0, misaligned);
    }

    gen_pc_plus_diff(succ_pc, ctx, ctx->cur_insn_len);
    gen_set_gpr(ctx, a->rd, succ_pc);
    tcg_gen_mov_tl(cpu_pc, target_pc);
    lookup_and_goto_ptr(ctx);

    if (misaligned) {
        gen_set_label(misaligned);
        gen_exception_inst_addr_mis(ctx, target_pc);
    }

    ctx->base.is_jmp = DISAS_NORETURN;

    return true;
}

static bool trans_sict(DisasContext* ctx, arg_sict* a) {
    gen_set_gpr(ctx, a->rd, get_gpr(ctx, a->rs1, EXT_NONE));
    return true;
}

