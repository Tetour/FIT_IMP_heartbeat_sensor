PIO = /home/adam/.platformio/penv/bin/platformio

# LaTeX documentation
LATEX_DIR = doc/documentation_latex
LATEX_MAIN = $(LATEX_DIR)/main.tex
PDF_OUTPUT = $(LATEX_DIR)/documentation.pdf

all: flash autosave

flash: build upload

build:
	$(PIO) run

upload:
	$(PIO) run -t upload

monitor:
	$(PIO) device monitor -b 115200

autosave:
	python3 scripts/auto_save_listener.py

plot: plot-clean
	./scripts/plot_sensor_data.py data/measurements
	./scripts/plot_sensor_data.py data/examples

plot-clean:
	rm -f data/**/*.png

venv:
	python3 -m venv venv
	./venv/bin/pip install pandas matplotlib

latex: $(PDF_OUTPUT)

$(PDF_OUTPUT): $(LATEX_MAIN)
	cd $(LATEX_DIR) && pdflatex -interaction=nonstopmode -jobname documentation main.tex
	cd $(LATEX_DIR) && pdflatex -interaction=nonstopmode -jobname documentation main.tex  # Run twice for references

latex-clean:
	rm -f $(LATEX_DIR)/*.aux $(LATEX_DIR)/*.log $(LATEX_DIR)/*.out $(LATEX_DIR)/*.toc $(LATEX_DIR)/*.lof $(LATEX_DIR)/*.lot $(LATEX_DIR)/main.pdf

zip: xvrskaa00.zip

xvrskaa00.zip:
	zip -r xvrskaa00.zip . \
		--exclude=".pio/*" \
		--exclude=".vscode/*" \
		--exclude="venv/*" \
		--exclude=".git/*" \
		--exclude="data/**/*.png" \
		--exclude="data/measurements/*" \
		--exclude="*.aux" \
		--exclude="*.log" \
		--exclude="*.out" \
		--exclude="*.toc" \
		--exclude="*.lof" \
		--exclude="*.lot" \
		--exclude="*.fls" \
		--exclude="*.fdb_latexmk" \
		--exclude="*.synctex.gz" \
		--exclude="*.bbl" \
		--exclude="*.blg" \
		--exclude="*.idx" \
		--exclude="*.ind" \
		--exclude="*.ilg" \
		--exclude="doc/documentation_latex/*.pdf" \
		--exclude="xvrskaa00.zip"

clean: plot-clean latex-clean
	$(PIO) run -t clean
	rm -f xvrskaa00.zip

.PHONY: all build upload flash monitor clean venv plot autosave latex latex-clean
