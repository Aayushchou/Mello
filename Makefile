build-logic: 
	cmake --build build
	cp -R "build/src/Mello_artefacts/AU/Mello.component" ~/Library/Audio/Plug-Ins/Components/
	osascript -e 'quit app "Logic Pro X"' ; open -a "Logic Pro X"
compile:
	cmake -S . -B build