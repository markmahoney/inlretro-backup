install: venv inlretro

venv: venv/bin/activate

venv/bin/activate: requirements.txt
	test -d venv || virtualenv venv
	venv/bin/pip install -Ur requirements.txt
	touch venv/bin/activate

inlretro:
	cd vendor/inlretro/host && $(MAKE) unix

clean:
	cd vendor/inlretro/host && $(MAKE) clean
