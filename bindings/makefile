all: install

install: package
	python3 setup.py install --user


package:
	python3 setup.py build


clean:
	rm -rf build dist
	rm flus_wrap.cpp flus.py
	rm -rf *.egg-info

