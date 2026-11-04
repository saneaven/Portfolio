PRG=gnu.exe
all:
	latex -interaction=nonstopmode -halt-on-error report.tex
	if test -f "report.dvi"; then dvipdf report.dvi; fi
0:
	file report.pdf | grep "PDF document" > studentout$@
	@echo "lines after the next are mismatches with master output -- see out$@"
	diff out$@ studentout$@ $(DIFFLAGS)
clean:
	rm -f report.aux report.dvi report.pdf report.log studentout* difference*
