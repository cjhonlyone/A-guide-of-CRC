readme:
	@echo  generate PDF with Refs
	pdflatex *.tex
	bibtex *.aux
	pdflatex *.tex
	pdflatex *.tex
	@echo delete temp files after the generating work done
	rm *.bib *.blg *.bbl *.aux *.log *.brf *.nlo *.out *.dvi *.ps *.lof *.toc \
		*.fls *.fdb_latexmk *.pdfsync *.synctex*.gz *.ind *.ilg *.idx

clean:
	rm *.pdf
	rm *.bib *.blg *.bbl *.aux *.log *.brf *.nlo *.out *.dvi *.ps *.lof *.toc \
		*.fls *.fdb_latexmk *.pdfsync *.synctex*.gz *.ind *.ilg *.idx
