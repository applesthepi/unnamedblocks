autoload zmv

zmv '*[[:upper:]]*' \
'${(M)f##*/}${(L)${${f##*/}//(#b)([[:upper:]])/_$match[1]}#_}'
