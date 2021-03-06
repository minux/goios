// Copyright 2012 The Go Authors.  All rights reserved.
// Use of this source code is governed by a BSD-style
// license that can be found in the LICENSE file.

package main

import (
	"bytes"
	"fmt"
	"strings"
)

/*
 * Helpers for building cmd/gc.
 */

// gcopnames creates opnames.h from go.h.
// It finds the OXXX enum, pulls out all the constants
// from OXXX to OEND, and writes a table mapping
// op to string.
func gcopnames(dir, file string) {
	var out bytes.Buffer
	fmt.Fprintf(&out, "// auto generated by go tool dist\n")
	fmt.Fprintf(&out, "static char *opnames[] = {\n")

	in := readfile(pathf("%s/go.h", dir))
	lines := splitlines(in)
	i := 0
	for i < len(lines) && !strings.Contains(lines[i], "OXXX") {
		i++
	}
	for _, line := range lines[i:] {
		if i := strings.Index(line, "//"); i >= 0 {
			line = line[:i]
		}
		for _, field := range splitfields(line) {
			field = strings.TrimPrefix(field, "O")
			field = strings.TrimSuffix(field, ",")
			fmt.Fprintf(&out, "\t[O%s] = \"%s\",\n", field, field)
		}
		if strings.Contains(line, "OEND") {
			break
		}
	}
	fmt.Fprintf(&out, "};\n")

	writefile(out.String(), file, 0)
}

// mkanames reads [5689].out.h and writes anames[5689].c
// The format is much the same as the Go opcodes above.
// It also writes out cnames array for C_* constants.
func mkanames(dir, file string) {
	ch := file[len(file)-3]
	targ := pathf("%s/../cmd/%cl/%c.out.h", dir, ch, ch)
	in := readfile(pathf("%s/../../include/link.h", dir)) + readfile(targ)
	lines := splitlines(in)

	// Include link.h so that the extern declaration there is
	// checked against the non-extern declaration we are generating.
	var out bytes.Buffer
	fmt.Fprintf(&out, "// auto generated by go tool dist\n")
	fmt.Fprintf(&out, "#include <u.h>\n")
	fmt.Fprintf(&out, "#include <libc.h>\n")
	fmt.Fprintf(&out, "#include <bio.h>\n")
	fmt.Fprintf(&out, "#include <link.h>\n")
	fmt.Fprintf(&out, "#include \"../cmd/%cl/%c.out.h\"\n", ch, ch)
	fmt.Fprintf(&out, "\n")

	fmt.Fprintf(&out, "char*	anames%c[] = {\n", ch)
	for _, line := range lines {
		// Use all A names found in the headers,
		// except don't use A_ARCHSPECIFIC (left to arch to define),
		// and don't use any aliases (= A...),
		// except do use the arch-defined alias for A_ARCHSPECIFIC.
		if strings.Contains(line, ";") {
			continue
		}
		if strings.HasPrefix(line, "\tA") && !strings.Contains(line, "\tA_") && (!strings.Contains(line, "= A") || strings.Contains(line, "= A_ARCHSPECIFIC")) {
			if i := strings.Index(line, ","); i >= 0 {
				line = line[:i]
			}
			if i := strings.Index(line, "="); i >= 0 {
				line = line[:i]
			}
			if i := strings.Index(line, "\n"); i >= 0 {
				line = line[:i]
			}
			line = line[2:]
			fmt.Fprintf(&out, "\t\"%s\",\n", strings.TrimSpace(line))
		}
	}
	fmt.Fprintf(&out, "};\n")

	j := 0
	var out2 bytes.Buffer
	fmt.Fprintf(&out2, "char*	cnames%c[] = {\n", ch)
	for _, line := range lines {
		if strings.HasPrefix(line, "\tC_") {
			if i := strings.Index(line, ","); i >= 0 {
				line = line[:i]
			}
			if i := strings.Index(line, "\n"); i >= 0 {
				line = line[:i]
			}
			line = line[3:]
			fmt.Fprintf(&out2, "\t\"%s\",\n", line)
			j++
		}
	}
	fmt.Fprintf(&out2, "};\n")
	if j > 0 {
		out.Write(out2.Bytes())
	}

	writefile(out.String(), file, 0)
}
