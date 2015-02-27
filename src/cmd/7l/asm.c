// Inferno utils/5l/asm.c
// http://code.google.com/p/inferno-os/source/browse/utils/5l/asm.c
//
//	Copyright © 1994-1999 Lucent Technologies Inc.  All rights reserved.
//	Portions Copyright © 1995-1997 C H Forsyth (forsyth@terzarima.net)
//	Portions Copyright © 1997-1999 Vita Nuova Limited
//	Portions Copyright © 2000-2007 Vita Nuova Holdings Limited (www.vitanuova.com)
//	Portions Copyright © 2004,2006 Bruce Ellis
//	Portions Copyright © 2005-2007 C H Forsyth (forsyth@terzarima.net)
//	Revisions Copyright © 2000-2007 Lucent Technologies Inc. and others
//	Portions Copyright © 2009 The Go Authors.  All rights reserved.
//
// Permission is hereby granted, free of charge, to any person obtaining a copy
// of this software and associated documentation files (the "Software"), to deal
// in the Software without restriction, including without limitation the rights
// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
// copies of the Software, and to permit persons to whom the Software is
// furnished to do so, subject to the following conditions:
//
// The above copyright notice and this permission notice shall be included in
// all copies or substantial portions of the Software.
//
// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT.  IN NO EVENT SHALL THE
// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
// THE SOFTWARE.

// Writing object files.

#include	"l.h"
#include	"../ld/lib.h"
#include	"../ld/elf.h"
#include	"../ld/macho.h"
#include	"../ld/dwarf.h"


char linuxdynld[] = "/lib/ld-linux-aarch64.so.1";
char freebsddynld[] = "XXX";
char openbsddynld[] = "XXX";
char netbsddynld[] = "XXX";
char dragonflydynld[] = "XXX";
char solarisdynld[] = "XXX";

static int
needlib(char *name)
{
	char *p;
	LSym *s;

	if(*name == '\0')
		return 0;

	/* reuse hash code in symbol table */
	p = smprint(".dynlib.%s", name);
	s = linklookup(ctxt, p, 0);
	free(p);
	if(s->type == 0) {
		s->type = 100;	// avoid SDATA, etc.
		return 1;
	}
	return 0;
}

int	nelfsym = 1;

// Preserve highest 8 bits of a, and do addition to lower 24-bit
// of a and b; used to adjust ARM branch intruction's target
// static int32
// braddoff(int32 a, int32 b)
// {
// 	// TODO(aram): FIXME
// 	diag("braddoff: FIXME");
// 	return (((uint32)a) & 0xff000000U) | (0x00ffffffU & (uint32)(a + b));
// }

void
gentext(void)
{
}

void
adddynrela(LSym *rel, LSym *s, Reloc *r)
{
	USED(rel); USED(s); USED(r);
	// TODO(aram): FIXME
	diag("adddynrela: FIXME");
}

void
adddynrel(LSym *s, Reloc *r)
{
	LSym *targ;

	// TODO(aram): FIXME
	targ = r->sym;
	ctxt->cursym = s;
	diag("unsupported relocation for dynamic symbol %s (type=%d stype=%d)", targ->name, r->type, targ->type);
}

int
elfreloc1(Reloc *r, vlong sectoff)
{
	int elfsym;

	VPUT(sectoff);

	elfsym = r->xsym->elfsym;
	switch (r->type) {
	default:
		return -1;

	case R_ADDR:
		if(r->siz == 4)
			VPUT(R_AARCH64_ABS32 | (uint64)elfsym<<32);
		else if(r->siz == 8)
			VPUT(R_AARCH64_ABS64 | (uint64)elfsym<<32);
		else
			return -1;
		break;

	case R_CALLARM64:
		if(r->siz == 4)
			VPUT(R_AARCH64_CALL26 | (uint64)elfsym<<32);
		else
			return -1;
		break;
	}

	VPUT(r->xadd);
	return 0;
}

void
elfsetupplt(void)
{
	// TODO(aram): FIXME
	return;
}

int
machoreloc1(Reloc *r, vlong sectoff)
{
	uint32 v;
	LSym *rs;

	rs = r->xsym;

	if(rs->type == SHOSTOBJ || r->type == R_CALLARM64) {
		if(rs->dynid < 0) {
			diag("reloc %d to non-macho symbol %s type=%d", r->type, rs->name, rs->type);
			return -1;
		}
		v = rs->dynid;
		v |= 1<<27; // external relocation
	} else {
		v = rs->sect->extnum;
		if(v == 0) {
			diag("reloc %d to symbol %s in non-macho section %s type=%d", r->type, rs->name, rs->sect->name, rs->type);
			return -1;
		}
	}

	switch(r->type) {
	default:
		return -1;
	case R_ADDR:
		v |= MACHO_ARM64_RELOC_UNSIGNED<<28;
		break;
	case R_CALLARM:
		v |= 1<<24; // pc-relative bit
		v |= MACHO_ARM64_RELOC_BRANCH26<<28;
		break;
	}

	switch(r->siz) {
	default:
		return -1;
	case 1:
		v |= 0<<25;
		break;
	case 2:
		v |= 1<<25;
		break;
	case 4:
		v |= 2<<25;
		break;
	case 8:
		v |= 3<<25;
		break;
	}

	LPUT(sectoff);
	LPUT(v);
	return 0;
}

int
archreloc(Reloc *r, LSym *s, vlong *val)
{
	USED(s);

	if(linkmode == LinkExternal) {
		switch (r->type) {
		default:
			return -1;

		case R_CALLARM64:
			r->done = 0;
			r->xsym = r->sym;
			*val = (0xfc000000u & (uint32)r->add);
			r->xadd = ((~0xfc000000u) & ((uint32)r->add))*4;
			r->add = 0;

			// ld64 for arm seems to want the symbol table to contain offset
			// into the section rather than pseudo virtual address that contains
			// the section load address.
			// we need to compensate that by removing the instruction's address
			// from addend.
			if(HEADTYPE == Hdarwin)
				r->xadd -= symaddr(s) + r->off;
			return 0;
		}
	}
	switch(r->type) {
	case R_CONST:
		*val = r->add;
		return 0;
	case R_GOTOFF:
		*val = symaddr(r->sym) + r->add - symaddr(linklookup(ctxt, ".got", 0));
		return 0;
	case R_CALLARM64: // bl XXXXXX or b YYYYYY
		*val = (0xfc000000u & (uint32)r->add) | (uint32)((symaddr(r->sym) + ((uint32)r->add) * 4 - (s->value + r->off)) / 4);
		return 0;
	}
	return -1;
}

vlong
archrelocvariant(Reloc *r, LSym *s, vlong t)
{
	USED(r);
	USED(s);
	sysfatal("unexpected relocation variant");
	return t;
}

void
adddynsym(Link *ctxt, LSym *s)
{
	USED(ctxt); USED(s);
	// TODO(aram): FIXME
	return;
}

void
adddynlib(char *lib)
{
	LSym *s;
	
	if(!needlib(lib))
		return;
	
	if(iself) {
		s = linklookup(ctxt, ".dynstr", 0);
		if(s->size == 0)
			addstring(s, "");
		elfwritedynent(linklookup(ctxt, ".dynamic", 0), DT_NEEDED, addstring(s, lib));
	} else {
		diag("adddynlib: unsupported binary format");
	}
}

void
asmb(void)
{
	uint32 symo, dwarfoff, machlink;
	Section *sect;
	LSym *sym;
	int i;

	if(debug['v'])
		Bprint(&bso, "%5.2f asmb\n", cputime());
	Bflush(&bso);

	if(iself)
		asmbelfsetup();

	sect = segtext.sect;
	cseek(sect->vaddr - segtext.vaddr + segtext.fileoff);
	codeblk(sect->vaddr, sect->len);
	for(sect = sect->next; sect != nil; sect = sect->next) {
		cseek(sect->vaddr - segtext.vaddr + segtext.fileoff);
		datblk(sect->vaddr, sect->len);
	}

	if(segrodata.filelen > 0) {
		if(debug['v'])
			Bprint(&bso, "%5.2f rodatblk\n", cputime());
		Bflush(&bso);

		cseek(segrodata.fileoff);
		datblk(segrodata.vaddr, segrodata.filelen);
	}

	if(debug['v'])
		Bprint(&bso, "%5.2f datblk\n", cputime());
	Bflush(&bso);

	cseek(segdata.fileoff);
	datblk(segdata.vaddr, segdata.filelen);

	machlink = 0;
	if(HEADTYPE == Hdarwin) {
		if(debug['v'])
			Bprint(&bso, "%5.2f dwarf\n", cputime());

		if(!debug['w']) { // TODO(minux): enable DWARF Support
			dwarfoff = rnd(HEADR+segtext.len, INITRND) + rnd(segdata.filelen, INITRND);
			cseek(dwarfoff);

			segdwarf.fileoff = cpos();
			dwarfemitdebugsections();
			segdwarf.filelen = cpos() - segdwarf.fileoff;
		}
		machlink = domacholink();
	}

	/* output symbol table */
	symsize = 0;
	lcsize = 0;
	symo = 0;
	if(!debug['s']) {
		// TODO: rationalize
		if(debug['v'])
			Bprint(&bso, "%5.2f sym\n", cputime());
		Bflush(&bso);
		switch(HEADTYPE) {
		default:
			if(iself)
				goto ElfSym;
		case Hplan9:
			symo = segdata.fileoff+segdata.filelen;
			break;
		case Hdarwin:
			symo = rnd(HEADR+segtext.filelen, INITRND)+rnd(segdata.filelen, INITRND)+machlink;
			break;
		ElfSym:
			symo = segdata.fileoff+segdata.filelen;
			symo = rnd(symo, INITRND);
			break;
		}
		cseek(symo);
		switch(HEADTYPE) {
		default:
			if(iself) {
				if(debug['v'])
					Bprint(&bso, "%5.2f elfsym\n", cputime());
				asmelfsym();
				cflush();
				cwrite(elfstrdat, elfstrsize);
	
				if(debug['v'])
					Bprint(&bso, "%5.2f dwarf\n", cputime());
				dwarfemitdebugsections();
				
				if(linkmode == LinkExternal)
					elfemitreloc();
			}
			break;
		case Hplan9:
			asmplan9sym();
			cflush();

			sym = linklookup(ctxt, "pclntab", 0);
			if(sym != nil) {
				lcsize = sym->np;
				for(i=0; i < lcsize; i++)
					cput(sym->p[i]);

				cflush();
			}
			break;
		case Hdarwin:
			if(linkmode == LinkExternal)
				machoemitreloc();
			break;
		}
	}

	ctxt->cursym = nil;
	if(debug['v'])
		Bprint(&bso, "%5.2f header\n", cputime());
	Bflush(&bso);
	cseek(0L);
	switch(HEADTYPE) {
	default:
	case Hplan9:	/* plan 9 */
		LPUT(0x647);			/* magic */
		LPUT(segtext.filelen);			/* sizes */
		LPUT(segdata.filelen);
		LPUT(segdata.len - segdata.filelen);
		LPUT(symsize);			/* nsyms */
		LPUT(entryvalue());		/* va of entry */
		LPUT(0L);
		LPUT(lcsize);
		break;
	case Hlinux:
	case Hfreebsd:
	case Hnetbsd:
	case Hopenbsd:
	case Hnacl:
		asmbelf(symo);
		break;
	case Hdarwin:
		asmbmacho();
		break;
	}
	cflush();
	if(debug['c']){
		print("textsize=%ulld\n", segtext.filelen);
		print("datsize=%ulld\n", segdata.filelen);
		print("bsssize=%ulld\n", segdata.len - segdata.filelen);
		print("symsize=%d\n", symsize);
		print("lcsize=%d\n", lcsize);
		print("total=%lld\n", segtext.filelen+segdata.len+symsize+lcsize);
	}
}

vlong
rnd(vlong v, int32 r)
{
	vlong c;

	if(r <= 0)
		return v;
	v += r - 1;
	c = v % r;
	if(c < 0)
		c += r;
	v -= c;
	return v;
}
