// Copyright 2014 The Go Authors.  All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

#include <u.h>
#include <libc.h>
#include "gg.h"
#include "opt.h"

enum {
	LeftRdwr = LeftRead | LeftWrite,
	RightRdwr = RightRead | RightWrite,
};

// This table gives the basic information about instruction
// generated by the compiler and processed in the optimizer.
// See opt.h for bit definitions.
//
// Instructions not generated need not be listed.
// As an exception to that rule, we typically write down all the
// size variants of an operation even if we just use a subset.
//
// The table is formatted for 8-space tabs.
static ProgInfo progtable[ALAST] = {
	[ATYPE]=	{Pseudo | Skip},
	[ATEXT]=	{Pseudo},
	[AFUNCDATA]=	{Pseudo},
	[APCDATA]=	{Pseudo},
	[AUNDEF]=	{Break},
	[AUSEFIELD]=	{OK},
	[ACHECKNIL]=	{LeftRead},
	[AVARDEF]=	{Pseudo | RightWrite},
	[AVARKILL]=	{Pseudo | RightWrite},

	// NOP is an internal no-op that also stands
	// for USED and SET annotations, not the Power opcode.
	[ANOP]=		{LeftRead | RightWrite},
	[AHINT]=	{OK},

	// Integer
	[AADD]=		{SizeQ | LeftRead | RegRead | RightWrite},
	[ASUB]=		{SizeQ | LeftRead | RegRead | RightWrite},
	[ANEG]=		{SizeQ | LeftRead | RegRead | RightWrite},
	[AAND]=		{SizeQ | LeftRead | RegRead | RightWrite},
	[AORR]=		{SizeQ | LeftRead | RegRead | RightWrite},
	[AEOR]=		{SizeQ | LeftRead | RegRead | RightWrite},
	[ASMULL]=	{SizeQ | LeftRead | RegRead | RightWrite},
	[AUMULL]=	{SizeQ | LeftRead | RegRead | RightWrite},
	[ASMULH]=	{SizeL | LeftRead | RegRead | RightWrite},
	[AUMULH]=	{SizeL | LeftRead | RegRead | RightWrite},
	[ASDIV]=	{SizeQ | LeftRead | RegRead | RightWrite},
	[AUDIV]=	{SizeQ | LeftRead | RegRead | RightWrite},
	[ALSL]=		{SizeQ | LeftRead | RegRead | RightWrite},
	[ALSR]=		{SizeQ | LeftRead | RegRead | RightWrite},
	[AASR]=	{SizeQ | LeftRead | RegRead | RightWrite},
	[ACMP]=		{SizeQ | LeftRead | RightRead},

	// Floating point.
	[AFADDD]=	{SizeD | LeftRead | RegRead | RightWrite},
	[AFADDS]=	{SizeF | LeftRead | RegRead | RightWrite},
	[AFSUBD]=	{SizeD | LeftRead | RegRead | RightWrite},
	[AFSUBS]=	{SizeF | LeftRead | RegRead | RightWrite},
	[AFMULD]=	{SizeD | LeftRead | RegRead | RightWrite},
	[AFMULS]=	{SizeF | LeftRead | RegRead | RightWrite},
	[AFDIVD]=	{SizeD | LeftRead | RegRead | RightWrite},
	[AFDIVS]=	{SizeF | LeftRead | RegRead | RightWrite},
	[AFCVTZSD]=	{SizeD | LeftRead | RegRead | RightWrite},
	[ASCVTFS]=	{SizeF | LeftRead | RegRead | RightWrite},
	[AFCMPD]=	{SizeD | LeftRead | RightRead},
	[AFCMPS]=	{SizeF | LeftRead | RightRead},
	[AFCVTSD]=	{SizeD | LeftRead | RightWrite | Conv},
	[AFCVTDS]=	{SizeD | LeftRead | RightWrite | Conv},

	// Moves
	// TODO(aram): remove duplicates here.
	[AMOVB]=	{SizeB | LeftRead | RightWrite | Move | Conv},
	[AMOVBU]=	{SizeB | LeftRead | RightWrite | Move | Conv | PostInc},
//	[AMOVBU]=	{SizeB | LeftRead | RightWrite | Move | Conv},
	[AMOVH]=	{SizeW | LeftRead | RightWrite | Move | Conv},
	[AMOVHU]=	{SizeW | LeftRead | RightWrite | Move | Conv | PostInc},
//	[AMOVHU]=	{SizeW | LeftRead | RightWrite | Move | Conv},
	[AMOVW]=	{SizeL | LeftRead | RightWrite | Move | Conv},
	// there is no AMOVWU.
	[AMOVWU]=	{SizeL | LeftRead | RightWrite | Move | Conv | PostInc},
//	[AMOVWU]=	{SizeL | LeftRead | RightWrite | Move | Conv},
	[AMOV]=	{SizeQ | LeftRead | RightWrite | Move},
//	[AMOV]=	{SizeQ | LeftRead | RightWrite | Move | PostInc},
	[AFMOVS]=	{SizeF | LeftRead | RightWrite | Move | Conv},
	[AFMOVD]=	{SizeD | LeftRead | RightWrite | Move},

	// Jumps
	[AB]=		{Jump | Break},
	[ABL]=		{Call},
	[ABEQ]=		{Cjmp},
	[ABNE]=		{Cjmp},
	[ABGE]=		{Cjmp},
	[ABLT]=		{Cjmp},
	[ABGT]=		{Cjmp},
	[ABLE]=		{Cjmp},
	[ARETURN]=	{Break},

	[ADUFFZERO]=	{Call},
	[ADUFFCOPY]=	{Call},
};

void
proginfo(ProgInfo *info, Prog *p)
{
	//TODO(aram): re-enable this.
//	initproginfo();

	*info = progtable[p->as];
	if(info->flags == 0) {
		*info = progtable[AADD];
		fatal("proginfo: unknown instruction %P", p);
	}

	if((info->flags & RegRead) && p->reg == NREG) {
		info->flags &= ~RegRead;
		info->flags |= /*CanRegRead |*/ RightRead;
	}

	if((p->from.type == D_OREG || p->from.type == D_CONST) && p->from.reg != NREG) {
		info->regindex |= RtoB(p->from.reg);
		if(info->flags & PostInc) {
			info->regset |= RtoB(p->from.reg);
		}
	}
	if((p->to.type == D_OREG || p->to.type == D_CONST) && p->to.reg != NREG) {
		info->regindex |= RtoB(p->to.reg);
		if(info->flags & PostInc) {
			info->regset |= RtoB(p->to.reg);
		}
	}

	if(p->from.type == D_CONST && p->from.sym != nil && (info->flags & LeftRead)) {
		info->flags &= ~LeftRead;
		info->flags |= LeftAddr;
	}

	if(p->as == ADUFFZERO) {
		info->reguse |= (1<<D_R0) | RtoB(3);
		info->regset |= RtoB(3);
	}
	if(p->as == ADUFFCOPY) {
		// TODO(austin) Revisit when duffcopy is implemented
		info->reguse |= RtoB(3) | RtoB(4) | RtoB(5);
		info->regset |= RtoB(3) | RtoB(4);
	}
}
