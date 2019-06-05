#!/bin/bash

### call by doing something like ./keywordReplacer.sh replaceKeyword "${editingDir}" "${textToReplace}" "${replacementText}"
### see result with echo $?

replaceKeyword()
{

	if [ "$#" != 3 ]; then
		echo "not enough inputs! Inputs must be \"editingDir\" \"textToReplace\" \"replacementText\""
		echo "  \"editingDir\" is the top level directory searched through to find all files with the keyword to be replaced"
		echo "  \"textToReplace\" and \"replacementText\" can only have \"\\\" characters if they look like \"\\\$\", since \"/\" chars will be replaced with \"\\/\" chars and \"=\" chars will be replaced by \"\\=\" chars in order for \"sed\" to work correctly to replace text. \"\$\" chars would be replaced with \"\\\$\" chars, but there are major problems with this because bash uses \"\$\" chars by themselves to determine when the string is actually a variable, hence why that is a char type exception."
		return 1
	fi

	editingDir="${1}"
	textToReplace="${2}"
	replacementText="${3}"

	## first make sure the input editing directory exists
	if [ ! -d "${editingDir}" ]; then
		echo "!!!"${editingDir}" does not exist!!!"
		return 1
	fi

	## now break the input editingDir up into the different path parts, first make sure the input editingDir is within the WRF-WindNinja-FarsiteScript repo, then make sure it is not at the top level or within the include dir, or within the .git dir, or within the utility_scripts dir
	## set values to know if these directories are found
	repoIdx=-9999
	includeIdx=-9999
	gitIdx=-9999
	utilitiesIdx=-9999
	## set starting loop variables
	currentDirName="$(basename $editingDir)"
	currentDirPath="$(dirname $editingDir)"
	whileLoopCounter=1
	while true; do
		## check and see if a given directory has been found
		if [ "${currentDirName}" == "WRF-WindNinja-FarsiteScript" ]; then
			repoIdx=$whileLoopCounter
		fi
		if [ "${currentDirName}" == "include" ]; then
			includeIdx=$whileLoopCounter
		fi
		if [ "${currentDirName}" == ".git" ]; then
			gitIdx=$whileLoopCounter
		fi
		if [ "${currentDirName}" == "utility_scripts" ]; then
			utilitiesIdx=$whileLoopCounter
		fi
		if [[ "${currentDirPath}" == "." || "${currentDirPath}" == "/" || "${currentDirName}" == "WRF-WindNinja-FarsiteScript" ]]; then
			break;  	  ## stop if doesn't contain repo name or at the repo name
		fi
		## update variables for next loop
		currentDirName="$(basename $currentDirPath)"
		currentDirPath="$(dirname $currentDirPath)"
		whileLoopCounter=$whileLoopCounter+1
	done

	## the looping above should make it so that no other indices can be the same or greater than repoIdx, so first thing is to make sure that repoIdx is not the preset value or 1
	if [ $repoIdx == -9999 ]; then
		echo "!!! input editingDir is not within the WRF-WindNinja-FarsiteScript repo !!!"
		return 1
	elif [ $repoIdx == 1 ]; then
		echo "!!! input editingDir is within the WRF-WindNinja-FarsiteScript repo, but is at the top level which is not allowed !!!"
		return 1
	else
		if [ $includeIdx != -9999 ]; then
			echo "!!! input editingDir is within the WRF-WindNinja-FarsiteScript repo and not the top level, but is inside the include directory which is not allowed !!!"
			return 1
		fi
		if [ $gitIdx != -9999 ]; then
			echo "!!! input editingDir is within the WRF-WindNinja-FarsiteScript repo and not the top level, but is inside the .git directory which is not allowed !!!"
			return 1
		fi
		if [ $utilitiesIdx != -9999 ]; then
			echo "!!! input editingDir is within the WRF-WindNinja-FarsiteScript repo and not the top level, but is inside the utility_scripts directory which is not allowed !!!"
			return 1
		fi
	fi
	

	## now make sure textToReplace only has "\" chars followed by "$" chars
	nChars=${#textToReplace}
	for (( charIdx=0; charIdx<${nChars}; charIdx++ ));
	do
		currentChar=${textToReplace:charIdx:charIdx+1}
		if [ $currentChar == "\\" ]; then
			if [ $charIdx == $(( $nChars-1 )) ]; then
				echo "!!! input textToReplace has a \"\\\" char without being followed by a \"\$\" char !!!"
				return 1
			else
				nextChar=${textToReplace:charIdx+1:charIdx+2}
				if [ $nextChar != "\$" ]; then
					echo "!!! input textToReplace has a \"\\\" char without being followed by a \"\$\" char !!!"
					return 1
				fi
			fi
		fi
	done

	## now make sure replacementText only has "\" chars followed by "$" chars
	nChars=${#replacementText}
	for (( charIdx=0; charIdx<${nChars}; charIdx++ ));
	do
		currentChar=${replacementText:charIdx:charIdx+1}
		if [ $currentChar == "\\" ]; then
			if [ $charIdx == $(( $nChars-1 )) ]; then
				echo "!!! input replacementText has a \"\\\" char without being followed by a \"\$\" char !!!"
				return 1
			else
				nextChar=${replacementText:charIdx+1:charIdx+2}
				if [ $nextChar != "\$" ]; then
					echo "!!! input replacementText has a \"\\\" char without being followed by a \"\$\" char !!!"
					return 1
				fi
			fi
		fi
	done

	## now enter the editingDir and replace the text. If an error occurs because of something other than nothing to replace, probably an input symbol that needs the "\" char before it but is not yet processed was probably introduced, so should warn this script may need modified
	echo "" # want a nice clean line
	echo "entering editingDir ${editingDir}"
	cd "${editingDir}"
	success=$?
	if [ $success != 0 ]; then
		echo "!!! could not execute cd command !!!"
		return 1
	fi
	echo "finding all cases of \"${textToReplace}\" in ${editingDir} and replacing them with \"${replacementText}\""
	## prepare the text by replacing all "/" chars with "\/" chars
	preppedTextToReplace=$(sed 's/\//\\\//g' <<<"$textToReplace")
	preppedReplacementText=$(sed 's/\//\\\//g' <<<"$replacementText")
	## prepare the text by replacing all "=" chars with "\=" chars
	preppedTextToReplace=$(sed 's/\=/\\\=/g' <<<"$preppedTextToReplace")
	preppedReplacementText=$(sed 's/\=/\\\=/g' <<<"$preppedReplacementText")
	grep -rl "${preppedTextToReplace}" "${editingDir}" --exclude-dir=.git --exclude-dir=include --exclude-dir=utility_scripts --exclude-dir=WRF-WindNinja-FarsiteScript --exclude=readme | xargs sed -i 's/'"${preppedTextToReplace}"'/'"${preppedReplacementText}"'/g'
	success=$?
	if [ $success != 0 ]; then
		if [ $success == 123 ]; then
			echo "!warning, no stuff to replace!"
			success=0
		else
			echo "!!! failed during replace path text process !!!"
			return 1
		fi
	else
		echo "finished replacing text"
	fi
	
	## if it gets to this point, it didn't break with a return 1
	return 0
}

"$@"

