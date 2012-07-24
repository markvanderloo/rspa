# Makefile to use knitr for package vignettes 
# Copied shamelessly and gratefully from Yihui: https://github.com/yihui/knitr/blob/master/inst/doc/Makefile

# put all PDF targets here, separated by spaces
PDFS= getting_started.pdf

all: $(PDFS) 

clean:
	rm -f *.tex *.bbl *.blg *.aux *.out *.log *.spl 

%.pdf: %.Rnw
	R -e "library(knitr); knit2pdf('$*.Rnw')"

