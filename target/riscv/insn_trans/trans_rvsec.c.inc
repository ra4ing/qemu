static bool trans_sict(DisasContext* ctx, arg_sict* a) {
    TCGv ica = dest_gpr(ctx, a->rd);
    TCGv offset_ica = get_gpr(ctx, a->rs1, EXT_NONE);
    TCGv ic = get_gpr(ctx, a->rs2, EXT_NONE);
    tcg_gen_add_tl(ica, ica, offset_ica);

    gen_set_ic(ctx, ica, ic);

    CPUState* cpu = ctx->cs;
    CPUArchState* env = cpu_env(cpu);
    helper_write_instruction_count(env, a->rs2);
    return true;
}

static bool trans_sjalr(DisasContext* ctx, arg_sjalr* a) {
    TCGLabel* misaligned = NULL;
    TCGv target_pc = tcg_temp_new();
    TCGv succ_pc = dest_gpr(ctx, a->rd);

    tcg_gen_addi_tl(target_pc, get_gpr(ctx, a->rs1, EXT_NONE), a->imm);
    tcg_gen_andi_tl(target_pc, target_pc, (target_ulong)-2);

    if (get_xl(ctx) == MXL_RV32) {
        tcg_gen_ext32s_tl(target_pc, target_pc);
    }

    if (!has_ext(ctx, RVC) && !ctx->cfg_ptr->ext_zca) {
        TCGv t0 = tcg_temp_new();
        misaligned = gen_new_label();
        tcg_gen_andi_tl(t0, target_pc, 0x2);
        tcg_gen_brcondi_tl(TCG_COND_NE, t0, 0x0, misaligned);
    }

    gen_pc_plus_diff(succ_pc, ctx, ctx->cur_insn_len);
    gen_set_gpr(ctx, a->rd, succ_pc);

    gen_check_reta(ctx, target_pc);

    tcg_gen_mov_tl(cpu_pc, target_pc);
    lookup_and_goto_ptr(ctx);

    if (misaligned) {
        gen_set_label(misaligned);
        gen_exception_inst_addr_mis(ctx, target_pc);
    }
    ctx->base.is_jmp = DISAS_NORETURN;

    return true;
}

static bool trans_ssra(DisasContext* ctx, arg_ssra* a) {
    TCGLabel* misaligned = NULL;
    TCGv target_pc = dest_gpr(ctx, a->rd);
    TCGv offset_pc = get_gpr(ctx, a->rs1, EXT_NONE);


    tcg_gen_add_tl(target_pc, target_pc, offset_pc);
    tcg_gen_andi_tl(target_pc, target_pc, (target_ulong)-2);

    if (get_xl(ctx) == MXL_RV32) {
        tcg_gen_ext32s_tl(target_pc, target_pc);
    }

    if (!has_ext(ctx, RVC) && !ctx->cfg_ptr->ext_zca) {
        TCGv t0 = tcg_temp_new();
        misaligned = gen_new_label();
        tcg_gen_andi_tl(t0, target_pc, 0x2);
        tcg_gen_brcondi_tl(TCG_COND_NE, t0, 0x0, misaligned);
    }

    // gen_set_gpr(ctx, a->rd, target_pc);
    gen_set_reta(ctx, target_pc);

    if (misaligned) {
        gen_set_label(misaligned);
        gen_exception_inst_addr_mis(ctx, target_pc);
    }

    return true;
}


static bool trans_sjal(DisasContext* ctx, arg_sjal* a)
{
    gen_check_jmpa(ctx, a->rd, a->imm);
    gen_jal(ctx, a->rd, a->imm);
    return true;
}

static bool trans_ssja(DisasContext* ctx, arg_ssja* a)
{
    TCGLabel* misaligned = NULL;
    TCGv target_pc = dest_gpr(ctx, a->rd);
    TCGv offset_pc = get_gpr(ctx, a->rs1, EXT_NONE);


    tcg_gen_add_tl(target_pc, target_pc, offset_pc);
    tcg_gen_andi_tl(target_pc, target_pc, (target_ulong)-2);

    if (get_xl(ctx) == MXL_RV32) {
        tcg_gen_ext32s_tl(target_pc, target_pc);
    }

    if (!has_ext(ctx, RVC) && !ctx->cfg_ptr->ext_zca) {
        TCGv t0 = tcg_temp_new();

        misaligned = gen_new_label();
        tcg_gen_andi_tl(t0, target_pc, 0x2);
        tcg_gen_brcondi_tl(TCG_COND_NE, t0, 0x0, misaligned);
    }

    // gen_set_gpr(ctx, a->rd, target_pc);
    gen_set_jmpa(ctx, target_pc);

    if (misaligned) {
        gen_set_label(misaligned);
        gen_exception_inst_addr_mis(ctx, target_pc);
    }

    return true;
}
